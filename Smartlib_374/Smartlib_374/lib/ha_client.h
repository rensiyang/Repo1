#ifndef __HA_CLIENT_H_
#define __HA_CLIENT_H_

#ifdef __cplusplus
	extern "C"
	{
#endif

#define TOPIC_LEN 64

//call back function type 
typedef void (*recv_ha_client_msg)(char *ptopic, char *pmsg);

//start the client thread, 0 for ok, <0 for err
int create_ha_client_thread(recv_ha_client_msg precv_func);

//send msg to server, return -1 for not connect, OK > 0
int add_ha_client_msg(char *ptopic, char *pmsg);

#ifdef __cplusplus
	}
#endif

#endif
