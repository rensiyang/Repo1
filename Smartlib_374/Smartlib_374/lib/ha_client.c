#include "ha_client.h"

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h> 
#include <netinet/tcp.h>
#include <netdb.h>
#include <string.h>

#include "ringbuffer.h"

//pipe for msg
static int pipe_ha_msg_fds[2];

//pthread id
static pthread_t ha_client_thread_id;

//read buf
#define READ_BUF_LEN 512
static char read_buf[READ_BUF_LEN];

//message buf as a ring buf
static ring_buffer_t ha_client_ring_buf = {0};
static char client_buf[1024*256];

//encode or decode buf
#define HA_CLIENT_BUF_LEN 1024*256
static char ha_client_buf[HA_CLIENT_BUF_LEN];

//ha client thread function
static void* ha_client_thread_func(void* arg);

static volatile int ha_client_connected = 0;
//rw lock for ha_client_connected
static pthread_rwlock_t ha_client_connected_rwlock = PTHREAD_RWLOCK_INITIALIZER;

//start the ha client thread, 0 for ok, <0 for err
int create_ha_client_thread(recv_ha_client_msg precv_func)
{
	//create thread for ha client
	printf("ha_client.c: create thread for ha client\n");
	
	pthread_rwlock_wrlock(&ha_client_connected_rwlock);
	ha_client_connected = 0;
	pthread_rwlock_unlock(&ha_client_connected_rwlock);

	//init pipe
	printf("ha_client.c: create pipe for IPC\n");

	if( pipe(pipe_ha_msg_fds) < 0)
	{
		printf("ha_client.c: create pipe_ha_msg_fds error\n");
		return -1;
	}
	
	//init ringbuffer
	Ring_Buffer_Init(&ha_client_ring_buf, client_buf, 1024*256);
	
	//start ha client thread
	if( 0 != pthread_create(&ha_client_thread_id, NULL, ha_client_thread_func, precv_func) )
	{
		printf("ha_client.c: create thread for ha client error\n");
		return -1;
	}

	return 0;
}

//add a ha msg
int add_ha_client_msg(char *ptopic, char *pmsg)
{
	int ret;
	
	pthread_rwlock_rdlock(&ha_client_connected_rwlock);
	ret = ha_client_connected;
	pthread_rwlock_unlock(&ha_client_connected_rwlock);	

	if(ret < 1)
	{
		return -1;
	}

	int len = strlen(pmsg);
	char *send_msg = (char *)malloc(2+TOPIC_LEN+len+2+1);
	
	//header
	send_msg[0] = '@';
	send_msg[1] = '#';
	//topic
	memset(send_msg+2, 0, TOPIC_LEN);
	strcpy(send_msg+2, ptopic);
	//pmsg
	strcpy(send_msg+2+TOPIC_LEN, pmsg);
	//end
	send_msg[2+TOPIC_LEN+len+0] = '$';
	send_msg[2+TOPIC_LEN+len+1] = '%';
	send_msg[2+TOPIC_LEN+len+2] = '\0';
	
	char buf[16];
	//format the string for write pipe,end with \n
	memset(buf, 0, 16);
	sprintf(buf, "%u\n", (unsigned int)send_msg);
	
	//write the pipe
	return write(pipe_ha_msg_fds[1], buf, strlen(buf));
}

static int readline(int fd, char *buffer);

//client msg function
static void* ha_client_thread_func(void* arg)
{
	printf("ha_client.c: ha_client_thread_func start\n");
	
	recv_ha_client_msg precv_func = (recv_ha_client_msg)arg;	

	//TCP socket for ha communication
	int ha_client_socket = 0;

	//connect to ha server
ha_client_connect:
	while(1)
	{
		printf("ha_client.c: start connect to ha server\n");
		
		pthread_rwlock_wrlock(&ha_client_connected_rwlock);
		ha_client_connected = 0;
		pthread_rwlock_unlock(&ha_client_connected_rwlock);
		
		if(ha_client_socket)
		{
			close(ha_client_socket);
		}
		
		//create socket to connect ha server
		if( (ha_client_socket = socket(AF_LOCAL, SOCK_STREAM, 0)) > 0 )
		{
			printf("ha_client.c: create ha_client_socket\n");
		}
		else
		{
			printf("ha_client.c: create ha_client_socket error\n");
			sleep(3);
			continue;
		}		

		struct sockaddr_un servaddr;
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sun_family = AF_LOCAL;
		strcpy(servaddr.sun_path, "/tmp/ha.str");

		//connect to the ha server
		if(0 == connect(ha_client_socket, (struct sockaddr *) &servaddr, sizeof(servaddr)))
		{
			printf("ha_client.c: ha_client_socket connected\n");
			break;
		}
		else
		{
			printf("ha_client.c: ha_client_socket connect error\n");
			sleep(3);
			continue;
		}
	}//end of while(1)
	
	//search for msg header and end
	int header_index = -1;
	int end_index = -1;
	int search_index = 0;

	Ring_Buffer_Init(&ha_client_ring_buf, client_buf, 1024*256);
	
	int max_fd = ha_client_socket;		
	max_fd = (max_fd > pipe_ha_msg_fds[0]) ?  max_fd : pipe_ha_msg_fds[0] ;
	
	pthread_rwlock_wrlock(&ha_client_connected_rwlock);
	ha_client_connected = 1;
	pthread_rwlock_unlock(&ha_client_connected_rwlock);
	
	//start proce msg
	while(1)
	{
		fd_set rset;
		FD_ZERO(&rset);
		FD_SET(ha_client_socket, &rset);
		FD_SET(pipe_ha_msg_fds[0], &rset);
		

		//wait for a msg
		int sret = select(max_fd+1, &rset, NULL, NULL, NULL);
		if( sret > 0 )
		{
			//if ha_client_socket
			if(FD_ISSET(ha_client_socket, &rset))
			{
				//calculate max space can store message
				int max_read = Ring_Buffer_Empty_Bytes_Cnt(&ha_client_ring_buf);
				max_read = (max_read > READ_BUF_LEN) ? READ_BUF_LEN : max_read ;
				
				//read socket
				int rret = read(ha_client_socket, read_buf, max_read);
				
				//read return value >0 ok, 0 server close, <0 error
				if(rret > 0)
				{	
					//copy read to ring buf
					Ring_Buffer_Queue_Arr(&ha_client_ring_buf, read_buf, rret);
					
					//search header @# and end $%	
 					int ring_buf_len = 0;
 					
 					while((ring_buf_len = Ring_Buffer_num_Items(&ha_client_ring_buf)) > 0)
 					{
						if(header_index < 0)
						{
							char header[2] = {0};
							int i = 0;
						
							for(i=search_index; i< ring_buf_len-1; i++)
							{
								Ring_Buffer_Peek(&ha_client_ring_buf, &header[0], i);
								Ring_Buffer_Peek(&ha_client_ring_buf, &header[1], i+1);
							
								if( ('@' == header[0]) && ('#' == header[1]) )
								{
									header_index = i;
									search_index = i+2;
									break;
								}
							
								search_index = i+1;
							} 
						}

					
						if( (header_index >= 0) && (end_index < 0) )
						{
							char end[2] = {0};
							int i;
						
							for(i=search_index; i< ring_buf_len-1; i++)
							{
								Ring_Buffer_Peek(&ha_client_ring_buf, &end[0], i);
								Ring_Buffer_Peek(&ha_client_ring_buf, &end[1], i+1);
							
								if( ('$' == end[0]) && ('%' == end[1]) )
								{
									end_index = i;
									break;
								}
							
								search_index = i+1;
							} 
						}

						if( (header_index < 0) || (end_index < 0) )
						{
							goto send_msg_to_server;
						}				

						//copy message to decode buf
						Ring_Buffer_Dequeue_Arr(&ha_client_ring_buf, ha_client_buf, end_index+2);
						ha_client_buf[end_index] = '\0';

						//decode
#if 0
						printf("ha_client.c: ha_client_thread_func get get topic:%s msg:%s \n",
												&ha_client_buf[header_index+2],
												&ha_client_buf[header_index+2+TOPIC_LEN]);
#endif
						
						if( NULL != precv_func )
						{
							precv_func(&ha_client_buf[header_index+2],
								   &ha_client_buf[header_index+2+TOPIC_LEN]);
						}
					
						header_index = -1;
						end_index = -1;
						search_index = 0;
					}
				}
				else
				{	
					printf("ha_client.c: close ha_client_socket in read ret 0\n");
					close(ha_client_socket);
					ha_client_socket = 0;
					
					Ring_Buffer_Init(&ha_client_ring_buf, client_buf, 1024*256);
				
					header_index = -1;
					end_index = -1;
					search_index = 0;
					
					goto ha_client_connect;	
				}
			}//end of if(FD_ISSET(ha_client_socket, &rset))
			
send_msg_to_server:
			//if pipe_ha_msg_fds[0]
			if( FD_ISSET(pipe_ha_msg_fds[0], &rset) )
			{
				char read_buf[16] = {0};
				if( readline(pipe_ha_msg_fds[0], read_buf)>0 )
				{
					char* pmsg = (char*)(strtoul(read_buf, 0, 0));
					
					if(0 != ha_client_socket)
					{
						if( write(ha_client_socket, pmsg, 2+TOPIC_LEN+strlen(pmsg+2+TOPIC_LEN)) <= 0 )
						{
							printf("ha_client.c: close ha_client_socket in write <= 0\n");
							close(ha_client_socket);
							ha_client_socket = 0;
							
							Ring_Buffer_Init(&ha_client_ring_buf, client_buf, 1024*256);
						
							header_index = -1;
							end_index = -1;
							search_index = 0;
							
							free(pmsg);
							
							goto ha_client_connect;
						}
					}
					
					free(pmsg);
				}
			}
		
		}
	}
	
}

static int readline(int fd, char *buffer) 
{ 
	int ret = -1; 
	int bytes_read = 0; 

	if (fd != -1) 
	{ 
		while ((ret = read(fd, &buffer[bytes_read], 1)) == 1)
	       	{ 
			if (buffer[bytes_read] == '\n') 
			{ 
				buffer[bytes_read+1] = '\0';  
				
				bytes_read += 1; 
				break; 
			} 
			bytes_read += 1; 
		} 
	} 
	if (ret == -1)
	{	
		return ret;
	}	
	else
	{	
		return bytes_read;
	} 
} 

