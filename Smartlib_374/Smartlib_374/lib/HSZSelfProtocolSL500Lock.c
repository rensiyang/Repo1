/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZSelfProtocolSL500Lock.c
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2019年03月05日
  最近修改   :
  功能描述   : 处理SL500智能门锁协议的Data数据
  函数列表   :
  修改历史   :
  1.日    期   : 2019年03月05日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件
******************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include "HSZCommon.h"
#include "cJSON.h"
#include "HSZMessage.h"
#include "HSZCommandId.h"
#include "HSZClusterId.h"
#include "HSZAttributeId.h"
#include "HSZAttributeType.h"
#include "HSZDeviceData.h"
#include "HSZSelfProtocolSL500Lock.h"
#include "HSZSmartLockMessage.h"


#define DEBUG_ON

#define ARRAY_SIZE 512
#define STRING_SIZE 32

#ifdef DEBUG_ON
        #define HIS_DEBUG(format,...) printf(format,__VA_ARGS__)
#else
        #define HIS_DEBUG(format,...)
#endif


unsigned char lock_sl500_send_msg_buf[512] = {0};



/*****************************************************************************
 函 数 名  : my_tolower
 功能描述  : 大写字母转换为小写字母
 输入参数  : 
                      
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年9月25日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int SL500_my_tolower(int c)  
{  
          if (c >= 'A' && c <= 'Z')  
          {  
              	return c + 'a' - 'A';
          }  
          else  
          {  
              	return c;  
          }  
}  

void SL500_my_toUppercase(char *src,int len)
{
        int distance='a'-'A';
        int i=0;
        if(src==NULL)
                return ;
        for(i=0;i<len;i++)
        {
		        if((src[i]>='a')&&(src[i]<='z'))
		            {
		                	src[i]-=distance;
		            }
        }
}


int SL500_my_htoi(char s[])
{
		int i;
		int n = 0;

		if (s[0] == '0' && (s[1]=='x' || s[1]=='X'))
		{
				i = 2;
		}
		else
		{
				i = 0;
		}

		for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i)
		{
				if (SL500_my_tolower(s[i]) > '9')
				{
		    			n = 16 * n + (10 + SL500_my_tolower(s[i]) - 'a');
				}
				else
				{
		    			n = 16 * n + (SL500_my_tolower(s[i]) - '0');
				}
		}

		return n;

}

//检查校验位（即传输指令长度）
/*****************************************************************************
 函 数 名  : HSZCalculateSelfProtocolLockDataCheckSum
 功能描述  : 计算传入数据的Check fcs数值
*****************************************************************************/

int HSZCalculateSelfProtocolSL500LockDataCheckSum(int sof,int len,unsigned char *data_buf)
{
        int index = 0;
        int sum = 0x00;

        printf("HSZCalculateSmartLockDataCheckSum:\n");

        printf("sof:%02x \n",sof);
        printf("len:%02x \n",len);

        for(index = 0;index < len;index++)
        {
                printf("%d:  0x%02x\n ",index,data_buf[index]&0xFF);
        }

        printf("\n");


        if((sof>0xFF)||(len>PRIVATE_DATA_LEN))
        {
                HSZDeviceError(HSZ_ERR_SMARTLOCK_MSG_TYPE,"Send smartlock message type error.",NULL);
                return -1;
        }

        sum = (sum + sof)&0xFF;                               

        sum = (sum + len)&0xFF;                                

        for(index = 0;index<len;index++)                
        {
                sum = (sum + data_buf[index])&0xFF;
        }

        printf("sum:%02x \n",sum);

        return sum;
}

/*****************************************************************************
 函 数 名  : HSZAnalysisSelfProtocolSL500LockData
 功能描述  : 解析智能门锁私有数据
*****************************************************************************/
int HSZAnalysisSelfProtocolSL500LockData(unsigned char *data,char* gw_mac,char* dev_mac)
{
         struct smartlock_self_protocol_info smartlock_self_protocol_info_data;
         memset(&smartlock_self_protocol_info_data,0,sizeof(struct smartlock_self_protocol_info));
         
         int rc = -1;
         int sum = -1; 		 
         int i;
		 char buf_sof[16] = {0};
		 char buf_len[16] = {0};
		 char buf_cmd1[16] = {0};
		 char buf_cmd2[16] = {0};
		 char buf_data_len[16] = {0};
		 char buf_fcs[16] = {0};
		 char data_buf[16] = {0};
		 char data_value[16] = {0};
		 
         smartlock_self_protocol_info_data.sof = data[0];                                                                           
         
         smartlock_self_protocol_info_data.len = data[1];                                                                          

         smartlock_self_protocol_info_data.cmd1 = data[2];                                                                      

         smartlock_self_protocol_info_data.cmd2 = data[3];                                                                    

         smartlock_self_protocol_info_data.data_len = data[4];                                                                

         memset(smartlock_self_protocol_info_data.data, 0, ARRAY_SIZE*2);
         memcpy(smartlock_self_protocol_info_data.data, &data[2], smartlock_self_protocol_info_data.len);       //CMD1开始算起
         
         sum = HSZCalculateSelfProtocolSL500LockDataCheckSum(smartlock_self_protocol_info_data.sof,smartlock_self_protocol_info_data.len,smartlock_self_protocol_info_data.data);
         
         if(sum != data[smartlock_self_protocol_info_data.len + 0x02])                     
         {
                printf("Receive smartlock message checksum error.\n");
                
                return rc;
         }
         //////////////////////////////////////////////////////////////////////////
         char msg_time[1024]={0};  
         char response[64] = {0};
         
         HSZConvertUnCharToStr(msg_time,data,smartlock_self_protocol_info_data.len+2);    
         printf(" msg_time = %s\n", msg_time);
         printf(" strncmp(QUERY_TIME,msg_time,28) = %d\n", strncmp(QUERY_TIME,msg_time,28));
         if(strncmp(QUERY_TIME,msg_time,28)==0)
         {
          strncpy(response,REQUST_TIME,4);
         if (hsbk_devicedata!=NULL)
         {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
         }
         printf(" response = %s\n", response);
         return rc;
         }
         //////////////////////////////////////////////////////////////////////////
         
         memset(smartlock_self_protocol_info_data.data, 0, ARRAY_SIZE*2);
         memcpy(smartlock_self_protocol_info_data.data, &data[5], smartlock_self_protocol_info_data.data_len);       //DATA数据段

         //////////////////////////////////////////////////////////////////////////
         
         printf(" smartlock_self_protocol_info_data.data_len = %d\n", smartlock_self_protocol_info_data.data_len);


         char msg_buffer[1024]={0};  
         
         HSZConvertUnCharToStr(msg_buffer,smartlock_self_protocol_info_data.data,smartlock_self_protocol_info_data.data_len);    
              
         for(i = 0;i < (smartlock_self_protocol_info_data.data_len);i++)		//data_len=ff                                                                                        
         {
                printf("0x%02x ",smartlock_self_protocol_info_data.data[i]&0xFF);
         }
        
         printf("\n");
        
 		sprintf(buf_sof,"%02x",smartlock_self_protocol_info_data.sof);		
 		sprintf(buf_len,"%02x",smartlock_self_protocol_info_data.len);		
 		sprintf(buf_cmd1,"%02x",smartlock_self_protocol_info_data.cmd1);		
 		sprintf(buf_cmd2,"%02x",smartlock_self_protocol_info_data.cmd2);
		sprintf(buf_data_len,"%02x",smartlock_self_protocol_info_data.data_len);
		
		memset(data_buf,0,sizeof(data_buf));
		
		for(i=5;i<13;i++)
		{
			data_value[i]=data[i];
			sprintf(data_value,"%02x",data[i]);
			strncat(data_buf,data_value,2);
		}
		
		smartlock_self_protocol_info_data.fcs = data[13];
		
 		sprintf(buf_fcs,"%02x",smartlock_self_protocol_info_data.fcs);
		
         if(CMD1_LOCK_RESPONSE == smartlock_self_protocol_info_data.cmd1)
         {		 
		 		if(0==strncasecmp("Fa",buf_sof,2) && 0==strncasecmp("0B",buf_len,2) &&
				    0==strncasecmp("14",buf_cmd1,2) && 0==strncasecmp("51",buf_cmd2,2) &&
				    0==strncasecmp("ff",buf_data_len,2) && 0==strncasecmp("ffffff20FFFFFFFF",data_buf,16) &&
				    0==strncasecmp("82",buf_fcs,2))		
		 		{
						printf("Lock request sync time activity... \n");
						HSZSelfProtocolSL500LockReturnActiveSyncTime(msg_buffer,gw_mac,dev_mac);
				}
				else
				{
						switch(smartlock_self_protocol_info_data.cmd2)							
						{
								   //printf("RSY smartlock_self_protocol_info_data.cmd2 = %d\n", smartlock_self_protocol_info_data.cmd2);
								   
								   case CMD2_ERROR_INFO_RESPONSE:						   
								   {
										   printf("500Error info report...\n");
										   HSZSelfProtocolSL500LockReturnDataForErrorCode(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
								   
								   case CMD2_OPEN_DOOR_INFO_RESPONSE:			  
								   {
										   printf("Unlock info report...\n");
										   HSZSelfProtocolSL500LockReturnForUnlockInfo(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
								   
								   case CMD2_OPEN_DOOR_HISTORY_RESPONSE:		
								   {
										   printf("Unlock history report...\n");
										   HSZSelfProtocolSL500LockReturnForHistory(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
								   
								   case CMD2_DOOR_TIME_RESPONSE:		  //0x33		 
								   {
										   printf("Lock local time report...\n");
										   HSZSelfProtocolSL500LockReturnForSyncTime(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_DOOR_REQUEST_TIME_SYNC:		   
								   {
										   printf("lock requir time...\n");
										   HSZSelfProtocolSL500LockReturnForQueryTime(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_ADMIN_PASSWD_AUTH_OK:				  
								   {
										   printf("Admin passwd auth succeed...\n");
										   HSZSelfProtocolSL500LockReturnAdminPasswdAuthSuccess(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_DOOR_EVENT_REPORT: 				   
								   {
										   printf("Lock event report...\n");
										   HSZReturnDataForSL500LockEvent(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_REMOTE_ADD_USER_OK_REPORT: 	
								   {
										   printf("Remote add user succeed result report...\n");
										   HSZSelfProtocolSL500LockReturnDataForAddTemUser(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_RREMOTE_DEL_USER_OK_REPORT:  
								   {
										   printf("Remote del user succeed result report...\n");
										   HSZSelfProtocolSL500LockReturnDataForDelUser(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_USER_QUERY_RESULT_REPORT:			   //39
								   {
										   printf("Query user result report...\n");
										   HSZSelfProtocolSL500LockReturnDataForUserInfo(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_DOOR_INFO_REPORT:						
								   {
										  HSZSelfProtocolSL500LockReturnForLockVersionInfo(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_LOCAL_NEW_USER_REPORT: 			 
								   {
										   printf("Local add user result report...\n");
										   HSZSelfProtocolSL500LockReturnDataForLocalAddUser(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_LOCAL_DEL_USER_REPORT: 			 
								   {
										  printf("Local del user result report...\n");
										  HSZSelfProtocolSL500LockReturnDataForLocalDelUser(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_LOCAL_MOD_USER_REPORT: 		   
								   {
										  printf("Local del user result report...\n");
										  HSZSelfProtocolSL500LockReturnDataForLocalModUser(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_MOD_OPEN_LOCK_MODE:
								   {
										  printf("Modify unlocking mode result report...\n");
										  HSZSelfProtocolLockReturnDataForModOpenMode(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_MOD_CLOSE_LOCK_MODE:
								   {
										  printf("Modify unlocking mode result report...\n");
										  HSZSelfProtocolLockReturnDataForModCloseMode(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_REMOTE_ADD_ADMIN_OK_REPORT:
								   {
										  printf("Modify unlocking mode result report...\n");
										  HSZSelfProtocolLockReturnDataForRemoteAddAdminUser(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_REMOTE_ADD_NORMAL_OK_REPORT:
								   {
										  printf("Modify unlocking mode result report...\n");
										  HSZSelfProtocolLockReturnDataForRemoteAddNormalUser(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
							
								   case CMD2_USER_PASSWD_AUTHEN_OK_REPORT:
								   {
										  printf("User password authentication result report...\n");
										  HSZSelfProtocolLockReturnDataForUserPasswdAuthen(msg_buffer,gw_mac,dev_mac);
								   }
								   break;
								   
								   default:
								   break;  
						}
				}
         }   
 
         return 0;
}          

/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockCmdAddUser
 功能描述  : 无线删除用户
*****************************************************************************/
int HSZSelfProtocolSL500DeleteUser(char *data_buf)
{
        int sum = 0x00;
        int index = 0;
        int code_temp = 0;
        
        char pincode[4] = {0};

        memset(lock_sl500_send_msg_buf,0x00,ARRAY_SIZE);
                
        lock_sl500_send_msg_buf[0] = 0xFA;                                                                                       
        lock_sl500_send_msg_buf[1] = 0x06;                                                                                      
        lock_sl500_send_msg_buf[2] = 0x13;                                                                                        
        lock_sl500_send_msg_buf[3] = 0x06;                                                                
        lock_sl500_send_msg_buf[4] = 0x03;
		
        strncpy(pincode,data_buf,1);                
        code_temp = strtol(pincode,NULL,16);
        lock_sl500_send_msg_buf[5] = (code_temp&0xFF);  
                                                          
        strncpy(pincode,data_buf+1,2);                
        code_temp = strtol(pincode,NULL,16);       
        lock_sl500_send_msg_buf[6] = (code_temp&0xFF);  

        strncpy(pincode,data_buf+3,2);                
        code_temp = strtol(pincode,NULL,16);       
        lock_sl500_send_msg_buf[7] = (code_temp&0xFF);  

        for(index = 0;index<8;index++)           
        {
                sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
        }
        
        lock_sl500_send_msg_buf[8] = sum;

        return 9;
}
                     
/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockCmdAddUser
 功能描述  : 无线添加临时用户
*****************************************************************************/
int HSZSelfProtocolSL500LockCmdAddUser(char *data_buf)							//1.增加6临时卡片用户；2.修改密码长度修改为2字节，'dd'
{
        int sum = 0x00;
        int index = 0;
        int code_length = 0;
        int code_temp = 0;
        int i;
        int user_type = 0;
        
        char codelen[8] = {0};
        char pincode[8] = {0};
        char usertype[8] = {0};

        strncpy(usertype,data_buf,1);
        user_type = strtol(usertype,NULL,10);

        strncpy(codelen,data_buf+1,2);
        code_length = strtol(codelen,NULL,16);  

        memset(lock_sl500_send_msg_buf,0x00,ARRAY_SIZE);
                
        lock_sl500_send_msg_buf[0] = 0xFA;                                                                                                              
        lock_sl500_send_msg_buf[2] = 0x13;                                                                                        
        lock_sl500_send_msg_buf[3] = 0x05;

        switch(user_type)		
        {
              case 0:
              {
                     printf("add temp admin user.\n");
              }
              break;

              case 1:
              {
                     printf("add temp normal user.\n");
              }
              break;

              case 2:
              {
                     printf("add temp passwd user.\n");

					 lock_sl500_send_msg_buf[1] = ((code_length+1+1+5+5+3)&0xFF);
					 lock_sl500_send_msg_buf[4] = ((code_length+1+1+5+5)&0xFF); 
                     lock_sl500_send_msg_buf[5] = user_type & 0xFF ; 					 
                     lock_sl500_send_msg_buf[6] = (code_length)&0xFF;

	 				for(i=0;i<code_length;i++)                                                                                              
                    {
                              //strncpy(pincode,data_buf+2+i,1);
                              strncpy(pincode,data_buf+1+2+i,1);             
                              code_temp = strtol(pincode,NULL,10);												//密码10进制           
                              lock_sl500_send_msg_buf[7+i] = code_temp & 0xFF ; 
                    }

					for(i=0;i<5;i++)																		  
					{
						   strncpy(pincode,data_buf+code_length+1+2+i*2,2); 
						   code_temp = strtol(pincode,NULL,16);
						   lock_sl500_send_msg_buf[7+code_length+i] = code_temp & 0xFF ;
					}

					for(i=0;i<5;i++)																		   
					{
						   strncpy(pincode,data_buf+code_length+1+2+10+i*2,2);			  
						   code_temp = strtol(pincode,NULL,16);
						   lock_sl500_send_msg_buf[7+code_length+5+i] = code_temp & 0xFF ; 
					}

					for(index = 0;index<(5+lock_sl500_send_msg_buf[4]);index++)		   
					{
						   sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
					}

					lock_sl500_send_msg_buf[5+lock_sl500_send_msg_buf[4]] = sum;						//checksum
              }
			  break;

			  case 6:	//不是密码形式，而是cmd中两位变FA中一位，密码长度除2。																
			  {
					printf("add temp card user.\n");
					
					lock_sl500_send_msg_buf[1] = ((code_length/2+1+1+5+5+3)&0xFF);
					lock_sl500_send_msg_buf[4] = (((code_length/2)+1+1+5+5)&0xFF);
					lock_sl500_send_msg_buf[5] = 0x06;
					lock_sl500_send_msg_buf[6] = (code_length/2)&0xFF;

					 for(i=0;i<(code_length/2);i++)                                                                                              
                     {
                              strncpy(pincode,data_buf+1+2+i*2,2);         
                              code_temp = strtol(pincode,NULL,16);												//卡片id 16进制
                              lock_sl500_send_msg_buf[7+i] = code_temp & 0xFF ; 
                     }

					 for(i=0;i<5;i++)																		  
					{
						   strncpy(pincode,data_buf+code_length+1+2+i*2,2); 
						   code_temp = strtol(pincode,NULL,16);
						   lock_sl500_send_msg_buf[7+(code_length/2)+i] = code_temp & 0xFF ;
					}

					for(i=0;i<5;i++)																		   
					{
						   strncpy(pincode,data_buf+code_length+1+2+10+i*2,2);			  
						   code_temp = strtol(pincode,NULL,16);
						   lock_sl500_send_msg_buf[7+(code_length/2)+5+i] = code_temp & 0xFF ; 
					}

					for(index = 0;index<(5+lock_sl500_send_msg_buf[4]);index++)		   
					{
						   sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
					}

					lock_sl500_send_msg_buf[5+lock_sl500_send_msg_buf[4]] = sum;						//checksum
			  }
			  break;

			  default:
			  break;
        }

		return (5+lock_sl500_send_msg_buf[4]+1);        
}                     


int HSZSelfProtocolSL500LockCmdOpenDoor(char *data_buf)										//data_buf = accdddddd
{
        int sum = 0x00;
        int index = 0;
        int code_length = 0;
        int i = 0;
        char codelen[4] = {0};
        int code_temp = 0;
        char pincode[4] = {0};
        char opendoor_way[4] = {0};
        int opendoor_type = 0;

        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;
        char TIME_year[4] = {0};
        char TIME_month[4] = {0};
        char TIME_day[4] = {0};
        char TIME_hour[4] = {0};
        char TIME_minute[4] = {0};
        char TIME_second[4] = {0};
        
        unsigned long int time;

        char timeunix_hex[32] = {0};
        int timeunix_hex_val = 0;

		strncpy(codelen,data_buf+1,2);
        code_length = strtol(codelen,NULL,16);

        strncpy(opendoor_way,data_buf,1);
        opendoor_type = strtol(opendoor_way,NULL,10);
        
        memset(lock_sl500_send_msg_buf,0x00,ARRAY_SIZE);
                
        lock_sl500_send_msg_buf[0] = 0xFA;                                                                                           
        lock_sl500_send_msg_buf[1] = (code_length+2+3)&0xFF;                         
        lock_sl500_send_msg_buf[2] = 0x13;                                                                                           
        lock_sl500_send_msg_buf[3] = 0x01;                                                                 

        switch(opendoor_type)
        {
                case 1:                                                                                                                    
                {
                        printf("admin password open door...\n");
                        lock_sl500_send_msg_buf[4] = ((code_length+2)&0xFF);

                        printf("code_length = %d\n",code_length);
                        printf("lock_sl500_send_msg_buf[4] = %02x\n",lock_sl500_send_msg_buf[4]);

                        lock_sl500_send_msg_buf[5] = 0x01;                                                                     
                        lock_sl500_send_msg_buf[6] = code_length & 0xFF;                                             

                        for(i=0;i<code_length;i++)                                                              
                        {
                                 strncpy(pincode,data_buf+1+2+i,1);               
                                 code_temp = strtol(pincode,NULL,10);                
                                 lock_sl500_send_msg_buf[7+i] = code_temp & 0xFF ; 
                        }
                }
                break;
                
                case 2:                                                                                                                     
                {
                        printf("password open door...\n");
                        lock_sl500_send_msg_buf[4] = ((code_length+2)&0xFF);
						
                        printf("code_length = %d\n",code_length);
                        printf("lock_sl500_send_msg_buf[4] = %02x\n",lock_sl500_send_msg_buf[4]);

                        lock_sl500_send_msg_buf[5] = 0x02;                                                                     
                        lock_sl500_send_msg_buf[6] = code_length & 0xFF;                                            

                        for(i=0;i<code_length;i++)                                                              
                        {
                                 strncpy(pincode,data_buf+1+2+i,1);                
                                 code_temp = strtol(pincode,NULL,10);                
                                 lock_sl500_send_msg_buf[7+i] = code_temp & 0xFF ; 
                        }
                }
                break;
                
                case 3:                                                                                                                    
                {
                        printf("force open door...\n");

                        lock_sl500_send_msg_buf[1] = 0x09;     

                        lock_sl500_send_msg_buf[4] = 0x06;                                     
                        printf("code_length = %d\n",code_length);
                        printf("lock_sl500_send_msg_buf[4] = %02x\n",lock_sl500_send_msg_buf[4]);

                        lock_sl500_send_msg_buf[5] = 0x03;                                                                                                             
                        lock_sl500_send_msg_buf[6] = 0x04;                                                                  

                       data_buf = data_buf + 3;					//1+2 'c'->'cc'

                       printf("data_buf  = %s\n",data_buf );

                       strncpy(TIME_year,data_buf,2);
                       year = strtol(TIME_year,NULL,16);

                       strncpy(TIME_month,data_buf+2,2);
                       month = strtol(TIME_month,NULL,16);

                       strncpy(TIME_day,data_buf+4,2);
                       day = strtol(TIME_day,NULL,16);
                   
                       strncpy(TIME_hour,data_buf+6,2);
                       hour = strtol(TIME_hour,NULL,16);
                 
                       strncpy(TIME_minute,data_buf+8,2);
                       minute = strtol(TIME_minute,NULL,16);
   
                       strncpy(TIME_second,data_buf+10,2);
                       second = strtol(TIME_second,NULL,16);   

                       time = Hal_Time_DateToSecond(year,month,day,hour,minute,second);

//                     printf("time = %d\n",(int)time);

                       sprintf(timeunix_hex,"%x",(unsigned int)time);

                       SL500_my_toUppercase(timeunix_hex,strlen(timeunix_hex));

                       printf("timeunix_hex_upper = %s\n",timeunix_hex);

                       timeunix_hex_val = strtol(timeunix_hex,NULL,16);

                       lock_sl500_send_msg_buf[7] = (((timeunix_hex_val)&0xFF000000)>>24);                         
                       lock_sl500_send_msg_buf[8] = (((timeunix_hex_val)&0x00FF0000)>>16);                         
                       lock_sl500_send_msg_buf[9] = (((timeunix_hex_val)&0x0000FF00)>>8);                          
                       lock_sl500_send_msg_buf[10] = ((timeunix_hex_val)&0x000000FF);                                 
                }
                break;
                
                default:               
                break;
        }
        
        for(index = 0;index<(5+lock_sl500_send_msg_buf[4]);index++)           
        {
                sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
        }
        
        lock_sl500_send_msg_buf[5+lock_sl500_send_msg_buf[4]] = sum;                                   

        return (5+lock_sl500_send_msg_buf[4]+1);
}


int HSZSelfProtocolSL500LockCmdQuerryTime(char *data_buf)
{
        
        memset(lock_sl500_send_msg_buf,0x00,ARRAY_SIZE);
        
        lock_sl500_send_msg_buf[0] = 0xFA;                                                                                          
        lock_sl500_send_msg_buf[1] = 0x03;                                                                                    
        lock_sl500_send_msg_buf[2] = 0x13;                                                                                          
        lock_sl500_send_msg_buf[3] = 0x02;                                                                           
        lock_sl500_send_msg_buf[4] = 0x00;                                                                            
        lock_sl500_send_msg_buf[5] = 0x12;                                                                   
        
        return 0x06;
}


int HSZSelfProtocolSL500LockCmdSynchronizeTime(char *data_buf)
{
        int sum = 0x00;
        int index = 0;
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;
        char TIME_year[4] = {0};
        char TIME_month[4] = {0};
        char TIME_day[4] = {0};
        char TIME_hour[4] = {0};
        char TIME_minute[4] = {0};
        char TIME_second[4] = {0};
        
        unsigned long int time;

        strncpy(TIME_year,data_buf,2);
        year = strtol(TIME_year,NULL,16);

        strncpy(TIME_month,data_buf+2,2);
        month = strtol(TIME_month,NULL,16);

        strncpy(TIME_day,data_buf+4,2);
        day = strtol(TIME_day,NULL,16);
    
        strncpy(TIME_hour,data_buf+6,2);
        hour = strtol(TIME_hour,NULL,16);
  
        strncpy(TIME_minute,data_buf+8,2);
        minute = strtol(TIME_minute,NULL,16);
   
        strncpy(TIME_second,data_buf+10,2);
        second = strtol(TIME_second,NULL,16);   

        time = Hal_Time_DateToSecond(year,month,day,hour,minute,second);
        memset(lock_sl500_send_msg_buf,0x00,ARRAY_SIZE);
                
        lock_sl500_send_msg_buf[0] = 0xFA;                                                                                         
        lock_sl500_send_msg_buf[1] = 0x07;                                                                                           
        lock_sl500_send_msg_buf[2] = 0x13;                                                                                           
        lock_sl500_send_msg_buf[3] = 0x03;                                                                   
        lock_sl500_send_msg_buf[4] = 0x04;                                                                   
        lock_sl500_send_msg_buf[5] = ((time&0xFF000000)>>24);                        
        lock_sl500_send_msg_buf[6] = ((time&0x00FF0000)>>16);                       
        lock_sl500_send_msg_buf[7] = ((time&0x0000FF00)>>8);                         
        lock_sl500_send_msg_buf[8] = (time&0x000000FF);                                       
                                        
        for(index = 0;index<(5+lock_sl500_send_msg_buf[4]);index++)           
        {
                sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
        }
        lock_sl500_send_msg_buf[9] = sum;

        return 0x0A;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolLockCmdPasswdCheck
 功能描述  : 管理员密码认证
*****************************************************************************/
int HSZSelfProtocolSL500LockCmdPasswdCheck(char *data_buf)						//密码长度' '->'dd'
{
        int sum = 0x00;
        int index = 0;
        int code_length = 0;
        int code_temp = 0;
        int i;
        
        char codelen[4] = {0};
        char pincode[4] = {0};

		strncpy(codelen,data_buf,2);
        code_length = strtol(codelen,NULL,16);  
        memset(lock_sl500_send_msg_buf,0x00,ARRAY_SIZE);
		
        lock_sl500_send_msg_buf[0] = 0xFA;                                                                                        
		lock_sl500_send_msg_buf[1] = (code_length+1+3)&0xFF;                        
        lock_sl500_send_msg_buf[2] = 0x13;                                                                                         
        lock_sl500_send_msg_buf[3] = 0x04;                                                                
        lock_sl500_send_msg_buf[4] = (code_length+1)&0xFF; 
		lock_sl500_send_msg_buf[5] = code_length & 0xFF;					//dd

        for(i=0;i<code_length;i++)                                                                      
        {
                strncpy(pincode,data_buf+2+i,1);                
                code_temp = strtol(pincode,NULL,10);                
                lock_sl500_send_msg_buf[6+i] = (code_temp&0xFF);  
        }
                                
        for(index = 0;index<(5+lock_sl500_send_msg_buf[4]);index++)           
        {
                sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
        }
        
        lock_sl500_send_msg_buf[5+lock_sl500_send_msg_buf[4]] = sum;

        return (5+lock_sl500_send_msg_buf[4]+1);
}

/****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockQueryUser
 功能描述  : 私有协议SL500门锁解析查询管理员、普通和临时用户信息
*****************************************************************************/
int HSZSelfProtocolSL500LockQueryUser(int user_type)   
{
        int sum = 0x00;
        int index = 0;
        
        memset(lock_sl500_send_msg_buf,0x00,ARRAY_SIZE);
                
        lock_sl500_send_msg_buf[0] = 0xFA;                                                                                        
        lock_sl500_send_msg_buf[1] = 0x04;                                                                                        
        lock_sl500_send_msg_buf[2] = 0x13;                                                                                          
        lock_sl500_send_msg_buf[3] = 0x07;                                                                
        lock_sl500_send_msg_buf[4] = 0x01;                                                                

        if (user_type == ADMIN_USER)
        {
               lock_sl500_send_msg_buf[5] = SMARTLOCK_ADMIN_USER;   
        }
        else if (user_type == NORMAL_USER)
        {
               lock_sl500_send_msg_buf[5] = SMARTLOCK_NORMAL_USER;
        }
        else if (user_type == TEM_USER)
        {
               lock_sl500_send_msg_buf[5] = SMARTLOCK_TEP_USER;
        }
                          
        for(index = 0;index<6;index++)         
        {
                sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
        }
        
        lock_sl500_send_msg_buf[6] = sum;

        return 7;
}

/****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockQueryLockInfo
 功能描述  : 私有协议SL500门锁解析查询锁具信息
*****************************************************************************/
int HSZSelfProtocolSL500LockQueryLockInfo(char *data_buf)							//表格添加02开锁方式，03关锁方式，04固件版本号。得到[5]位的‘aa’，不知在哪修改
{
        int sum = 0x00;
        int index = 0;
	 	int code_temp = 0;
        
        char pincode[4] = {0};

        
        memset(lock_sl500_send_msg_buf,0x00,ARRAY_SIZE);
                
        lock_sl500_send_msg_buf[0] = 0xFA;                                                                                         
        lock_sl500_send_msg_buf[1] = 0x04;                                                                                        
        lock_sl500_send_msg_buf[2] = 0x13;                                                                                          
        lock_sl500_send_msg_buf[3] = 0x08;                                                                 
        lock_sl500_send_msg_buf[4] = 0x01;   
		
	 	strncpy(pincode,data_buf,2);                
        code_temp = strtol(pincode,NULL,16);       
        lock_sl500_send_msg_buf[5] = (code_temp&0xFF);  
       
        for(index = 0;index<6;index++)         
        {
                sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
        }
        
        lock_sl500_send_msg_buf[6] = sum;

        return 7;
}

/****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockOpenDoorHistory
 功能描述  : 私有协议SL500门锁解析开锁历史查询
*****************************************************************************/
int HSZSelfProtocolSL500LockOpenDoorHistory(void)
{       
        memset(lock_sl500_send_msg_buf,0x00,ARRAY_SIZE);                
        lock_sl500_send_msg_buf[0] = 0xFA;                                                                                       
        lock_sl500_send_msg_buf[1] = 0x03;                                                                
        lock_sl500_send_msg_buf[2] = 0x13;                                                                                         
        lock_sl500_send_msg_buf[3] = 0x09;                                                                
        lock_sl500_send_msg_buf[4] = 0x00; 
        lock_sl500_send_msg_buf[5] = 0x19;

        return 6;
}


/****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockReset
 功能描述  : 私有协议SL500门锁解析恢复出厂设置状态确认
*****************************************************************************/
int HSZSelfProtocolSL500LockReset(void)
{
        memset(lock_sl500_send_msg_buf,0x00,ARRAY_SIZE);
        lock_sl500_send_msg_buf[0] = 0xFA;                                                               
        lock_sl500_send_msg_buf[1] = 0x03;                                                                
        lock_sl500_send_msg_buf[2] = 0x13;                                                                 
        lock_sl500_send_msg_buf[3] = 0x0B;                                                                
        lock_sl500_send_msg_buf[4] = 0x00; 
        lock_sl500_send_msg_buf[5] = 0x1B;
		
        return 6;
}

/****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockHistory
 功能描述  : 私有协议SL500门锁解析查询开锁历史记录
*****************************************************************************/
int HSZSelfProtocolSL500LockHistory(char *data_buf)
{
 	 	int sum = 0x00;
        int index = 0;
        int code_temp = 0;
        
        char pincode[4] = {0};

        memset(lock_sl500_send_msg_buf,0x00,ARRAY_SIZE);
                
        lock_sl500_send_msg_buf[0] = 0xFA;                                                                                        
        lock_sl500_send_msg_buf[1] = 0x05;                                                                                      
        lock_sl500_send_msg_buf[2] = 0x13;                                                                                        
        lock_sl500_send_msg_buf[3] = 0x0A;                                                                
        lock_sl500_send_msg_buf[4] = 0x02;                                                                 
                                                                  
        strncpy(pincode,data_buf,2);               
        code_temp = strtol(pincode,NULL,16);       
        lock_sl500_send_msg_buf[5] = (code_temp&0xFF);  

        strncpy(pincode,data_buf+2,2);                
        code_temp = strtol(pincode,NULL,16);
        lock_sl500_send_msg_buf[6] = (code_temp&0xFF);  

        for(index = 0;index<7;index++)           
        {
                sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
        }
        
        lock_sl500_send_msg_buf[7] = sum;

        return 8;
}

/****************************************************RSY*******************************************************/
/****************************************************************************
 函 数 名  : HSZSelfProtocolQueryTemCardUserInfo
 功能描述  : 私有协议SL500门锁解析查询临时卡片用户信息（类似查询临时密码用户，已经根据cmd_type（33or37）进入不同分支）
*****************************************************************************/
int HSZSelfProtocolQueryTemCardUserInfo(int user_type)
{
		int sum = 0x00;
        int index = 0;
        
        memset(lock_sl500_send_msg_buf,0x00,ARRAY_SIZE);
                
        lock_sl500_send_msg_buf[0] = 0xFA;                                                                                        
        lock_sl500_send_msg_buf[1] = 0x04;                                                                                        
        lock_sl500_send_msg_buf[2] = 0x13;                                                                                          
        lock_sl500_send_msg_buf[3] = 0x07;               					//pdf中缺少CMD2字段？？？                                                 
        lock_sl500_send_msg_buf[4] = 0x01;

		switch(user_type)
		{
				case 0:
				{
						lock_sl500_send_msg_buf[5] = SMARTLOCK_ADMIN_USER;  
				}
				break;
				
				case 1:
				{
						lock_sl500_send_msg_buf[5] = SMARTLOCK_NORMAL_USER;
				}
				break;
				
				case 2:
				{
						lock_sl500_send_msg_buf[5] = SMARTLOCK_TEP_USER;
				}
				break;
				
				case 6:
				{
						lock_sl500_send_msg_buf[5] = SMARTLOCK_TEP_CARD_USER;
				}
				break;

				default:
				break;
		}
                         
		for(index = 0;index<6;index++)         
		{
		    sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
		}
        
        lock_sl500_send_msg_buf[6] = sum;

        return 7;	
}

/****************************************************************************
 函 数 名  : HSZSelfProtocolSL500ModOpenLockMode
 功能描述  : 私有协议SL500门锁解析修改开锁方式
*****************************************************************************/
int HSZSelfProtocolModOpenLockMode(char *data_buf)
{
		int sum = 0x00;
		int index = 0;
		int code_temp = 0;
		
		char pincode[4] = {0};

        lock_sl500_send_msg_buf[0] = 0xFA;         					//帧头FA                                                                               
        lock_sl500_send_msg_buf[1] = 0x04;         					//帧长                                                                 
        lock_sl500_send_msg_buf[2] = 0x13;        					//CMD1：0x13(zigbee->锁)                                                                         
        lock_sl500_send_msg_buf[3] = 0x0C;         					//CMD2：0x0C(修改开锁方式)                              
        lock_sl500_send_msg_buf[4] = 0x01; 		   					//数据段长度

		strncpy(pincode,data_buf,2);                
        code_temp = strtol(pincode,NULL,16);       
        lock_sl500_send_msg_buf[5] = (code_temp&0xFF);			   //数据段内容，根据传过来的data_buf="aa"第[5]位

        for(index = 0;index<6;index++)         
        {
                sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
        }
        
        lock_sl500_send_msg_buf[6] = sum;

        return 7;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500ModCloseLockMode
 功能描述  : 私有协议SL500门锁解析修改关锁方式
*****************************************************************************/
	int HSZSelfProtocolModCloseLockMode(char *data_buf)
	{
		int sum = 0x00;
		int index = 0;
		int code_temp = 0;
		
		char pincode[4] = {0};

		lock_sl500_send_msg_buf[0] = 0xFA;		   																				
		lock_sl500_send_msg_buf[1] = 0x04;		   															
		lock_sl500_send_msg_buf[2] = 0x13;		   					//CMD1：0x13(zigbee->锁)												  						
		lock_sl500_send_msg_buf[3] = 0x0D;		   					//CMD2：0x0D(修改关锁方式)							  
		lock_sl500_send_msg_buf[4] = 0x01;		  					//数据段长度

		strncpy(pincode,data_buf,2);				
		code_temp = strtol(pincode,NULL,16);	   
		lock_sl500_send_msg_buf[5] = (code_temp&0xFF);			   //数据段内容，根据传过来的data_buf="aa"第几位

		for(index = 0;index<6;index++)		   
		{
				sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
		}
		
		lock_sl500_send_msg_buf[6] = sum;

		return 7;	
	}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolAddAdminOrNormalUser
 功能描述  : 私有协议SL500门锁解析添加管理员、普通用户
*****************************************************************************/
int HSZSelfProtocolAddAdminOrNormalUser(char *data_buf)
	{
		int sum = 0x00;
		int index = 0;
		int code_temp = 0;
		int id_temp = 0;
		int code_length = 0;
		int i = 0;
		
		char pincode[16] = {0};
        char codelen[4] = {0};
		char front_pincode[16] = {0};

		strncpy(codelen,data_buf+18,2);									//密码长度或卡片id长度
        code_length = strtol(codelen,NULL,16); 
		
		lock_sl500_send_msg_buf[0] = 0xFA;
		lock_sl500_send_msg_buf[2] = 0x13;
		
		strncpy(pincode,data_buf,2);
		code_temp = strtol(pincode,NULL,16);	   						//先取数据的前两位“aa”，根据是管理员（0）还是普通用户（1）分支

		switch(code_temp)
		{
			case 0:
			{										
				   	lock_sl500_send_msg_buf[3] = 0x0E;					//CMD2:无线添加管理员用户
			}
			break;

			case 1:
			{								
					lock_sl500_send_msg_buf[3] = 0x0F;					//CMD2:无线添加普通用户
			}
			break;

			default:
			break;
		}				
	
		strncpy(pincode,data_buf+2,2);									
		code_temp = strtol(pincode,NULL,16);
		lock_sl500_send_msg_buf[5] = (code_temp&0xFF);					//5、6位是用户id

		strncpy(pincode,data_buf+4,2);									
		code_temp = strtol(pincode,NULL,16);
		lock_sl500_send_msg_buf[6] = (code_temp&0xFF);

		for(i=0;i<5;i++)
		{
				memset(front_pincode,0,16);
				memset(pincode,0,16);
				strncpy(pincode,data_buf+2+4+i*2,2); 
				strncat(front_pincode,pincode,2);
				id_temp = strtol(front_pincode,NULL,16);	
				lock_sl500_send_msg_buf[7+i] = (id_temp&0xFF);
		}
		
		strncpy(pincode,data_buf+2+4+10,2);							//hh [12]
		code_temp = strtol(pincode,NULL,16);			
		lock_sl500_send_msg_buf[7+5] = (code_temp&0xFF);

		switch(code_temp)
		{
				case 0:
				{
						for(i=0;i<code_length;i++)                                                                      
				        {				        		
				        		lock_sl500_send_msg_buf[1] = (lock_sl500_send_msg_buf[4]+3)&0xFF;	//密码用户长度就为密码个数				        		
								lock_sl500_send_msg_buf[4] = (code_length+2+5+1+1)&0xFF;
								
				        		memset(front_pincode,0,4);
				                strncpy(pincode,data_buf+2+4+10+2+2+i,1); 
								strncat(front_pincode,pincode,1);
				                code_temp = strtol(front_pincode,NULL,10);						//密码用户转10进制
				                lock_sl500_send_msg_buf[7+5+1+1+i] = (code_temp&0xFF);
								lock_sl500_send_msg_buf[13] = (code_length&0xFF);			//ii [13]
				        }
				}
				break;
				
				case 2:
				{
						for(i=0;i<(code_length/2);i++)                                                                      
				        {				        		
				        		lock_sl500_send_msg_buf[1] = (lock_sl500_send_msg_buf[4]+3)&0xFF;	//密码用户长度就为密码个数				        		
				        		lock_sl500_send_msg_buf[4] = ((code_length/2)+2+5+1+1)&0xFF;		//数据段长度/2
				        		
				        		memset(front_pincode,0,4);								
				                strncpy(pincode,data_buf+2+4+10+2+2+i*2,2); 
								strncat(front_pincode,pincode,2);
				                code_temp = strtol(front_pincode,NULL,16);						//卡片用户转16进制
				                lock_sl500_send_msg_buf[7+5+1+1+i] = (code_temp&0xFF);
								lock_sl500_send_msg_buf[13] = ((code_length/2)&0xFF);			//ii [13]
				        }
				}
				break;

				default:
				break;
		}

		for(index = 0;index<(5+lock_sl500_send_msg_buf[4]);index++)           
        {
                sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
        }
        
        lock_sl500_send_msg_buf[5+lock_sl500_send_msg_buf[4]] = sum; 

        return (5+lock_sl500_send_msg_buf[4]+1);
	}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolUserPasswdCheck
 功能描述  : 私有协议SL500门锁解析用户密码验证
*****************************************************************************/
int HSZSelfProtocolUserPasswdCheck(char *data_buf)
{
		int sum = 0x00;
        int index = 0;
        int code_length = 0;
        int code_temp = 0;
        int i;
        
        char codelen[4] = {0};
        char pincode[4] = {0};

		strncpy(codelen,data_buf,2);
        code_length = strtol(codelen,NULL,16);

		lock_sl500_send_msg_buf[0] = 0xFA;
		lock_sl500_send_msg_buf[1] = (code_length+1+3)&0xFF;
		lock_sl500_send_msg_buf[2] = 0x13;
		lock_sl500_send_msg_buf[3] = 0x10;
		lock_sl500_send_msg_buf[4] = (code_length+1)&0xFF;
		lock_sl500_send_msg_buf[5] = code_length&0xFF;

		for(i=0;i<code_length;i++)                                                                      
        {
                strncpy(pincode,data_buf+2+i,1);               
                code_temp = strtol(pincode,NULL,10);                
                lock_sl500_send_msg_buf[6+i] = (code_temp&0xFF);  
        }

		for(index = 0;index<(lock_sl500_send_msg_buf[4]+5);index++)           
        {
                sum = (sum + lock_sl500_send_msg_buf[index])&0xFF;
        }
        
        lock_sl500_send_msg_buf[lock_sl500_send_msg_buf[4]+5] = sum;

        return (5+lock_sl500_send_msg_buf[4]+1);
}
/****************************************************RSY*******************************************************/

/*****************************************************************************
 函 数 名  : HSZSelfProtocolLockReturnDataForErrorCode
 功能描述  : 私有协议SL500门锁解析错误密码返回数据
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnDataForErrorCode(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[30] = {0};
		int data_type = 0;
		
		data_type = strtol(data_buf,NULL,16);

		switch(data_type)
		{
				case SYNC_TIME_FAILED_ERRORCODE:
				{
						 strncpy(response,SYNC_TIME_FAILED,4);					//0A02
				}
				break;

				case PASSWD_ERROR_ERRORCODE:
				{
						 strncpy(response,ADMIN_PASSWD_AUTH_FAILED,4);			//0A11
				}
				break;

				case ADMIN_PASSWD_AUTH_FAILED_ERRORCODE:
				{
						 strncpy(response,USER_AUTH_FAILED,4);					//0A00
				}
				break;				

				case REMOTE_OPENDOOR_PASSWD_ERROR_ERRORCODE:
				{
						 strncpy(response,OPENDOOR_PASSWD_ERROR,4);				//0A10
				}
				break;

				case OPENDOOR_FAILED_ERRORCODE:
				{
						 strncpy(response,OPENDOOR_FAILED,4);					//0A01
				}
				break;

				case REMOTE_OPENDOOR_FORBID_ERRORCODE:
				{
						 strncpy(response,REMOTE_OPENDOOR_FORBID,4);			//0301
				}
				break;

				case ADD_TEMPPASSWD_DUPLICATE_ERRORCODE:
				{
						 strncpy(response,ADD_TEMPPASSWD_DUPLICATE,4);			//0A12
				}
				break;
				
				case ADD_USER_FAILED_ERRORCODE:
				{
						 strncpy(response,ADD_USER_FAILED,4);					//0A03
				}
				break;

				case DEL_USER_FAILED_ERRORCODE:
				{
						 strncpy(response,DEL_USER_FAILED,4);					//0A04
				}
				break;

				case PASSWD_AUTH_FAILED_ERRORCODE:
				{
						 strncpy(response,PASSWD_AUTHEN_FAILED,4);				//0A05
				}
				break;
				
				case MOD_OPENLOCK_MODE_FAILED_ERRORCODE:
				{
						 strncpy(response,MOD_OPENLOCK_MODE_FAILED,4);			//0A06
				}
				break;

				case MOD_CLOSELOCK_MODE_FAILED_ERRORCODE:
				{
						 strncpy(response,MOD_CLOSELOCK_MODE_FAILED,4);			//0A07
				}
				break;

				case ADD_USER_CARD_DUPLICATE:
				{
						 strncpy(response,ADD_TEMPCARD_DUPLICATE,4);			//0A08
				}
				break;			

				default:
				break;
				
		}

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZReturnDataForSL500LockEvent
 功能描述  : 私有协议SL500门锁事件上报(主动返回)
*****************************************************************************/
int HSZReturnDataForSL500LockEvent(char *data_buf,char* gw_mac,char* dev_mac)					//(01,gw_mac,dev_mac)
{
        int rc = -1;
		int data_type = 0;
        char response[64] = {0};

        printf("data_buf=%s	",data_buf);

        SL500_my_toUppercase(data_buf,strlen(data_buf));

		data_type = strtol(data_buf,NULL,16);

		switch(data_type)
		{
				case SMARTLOCK_LOCK_CLOSED:
				{
						 strncpy(response,LOCAL_REPORT_LOCK,4);
				}
				break;

				case SMARTLOCK_PASSWD_VALIDATION_ERROR_3TIMES:
				{
						 strncpy(response,PASSWD_VALIDATION_ERROR_3TIMES,4);
				}
				break;

				case SMARTLOCK_FINGERPRINT_VALIDATION_ERROR_3TIMES:
				{
						 strncpy(response,FINGERPRINT_VALIDATION_ERROR_3TIMES,4);
				}
				break;

				case SMARTLOCK_CARD_VALIDATION_ERROR_3TIMES:
				{
						 strncpy(response,CARD_VALIDATION_ERROR_3TIMES,4);
				}
				break;

				case SMARTLOCK_DEMOLITION_ALARM:
				{
						 strncpy(response,DEMOLITION_ALARM,4);
				}
				break;

				case SMARTLOCK_LOW_BATTERY_ALARM:
				{
						 strncpy(response,LOW_BATTERY_ALARM,4);
				}
				break;

				case SMARTLOCK_DOUBLE_LOCK1:
				{
						 strncpy(response,DOUBLE_LOCK,4);
				}
				break;

				case SMARTLOCK_RELEASE_LOCK:
				{
						 strncpy(response,RELEASE_LOCK,4);
				}
				break;

				case SMARTLOCK_LOCAL_SETTING_REMOTE_UNLOK_PERMIT:
				{
						 strncpy(response,LOCAL_SETTING_REMOTE_UNLOK_PERMIT,4);
				}
				break;

				case SMARTLOCK_LOCAL_SETTING_REMOTE_UNLOK_FORBID:
				{
						 strncpy(response,LOCAL_SETTING_REMOTE_UNLOK_FORBID,4);
				}
				break;

				case SMARTLOCK_FACTORY_RESET:
				{
						 strncpy(response,FACTORY_RESET,4);
				}
				break;

				case SMARTLOCK_RELEASE_SYSTEM_LOCK_IN:
				{
						 strncpy(response,RELEASE_SYSTEM_LOCK_IN,4);
				}
				break;

				case SMARTLOCK_UNLATCHED_DOOR:
				{
						 strncpy(response,UNLATCHED_DOOR,4);
				}
				break;

				case SMARTLOCK_DOORBELL:
				{
						 strncpy(response,DOORBELL,4);
				}
				break;

				case SMARTLOCK_OUTMODE:
				{
						 strncpy(response,OUTMODE_DOOR,4);
				}
				break;

				default:
				break;

		}

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        printf("response = %s\n",response);
        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockReturnActiveSyncTime
 功能描述  : 私有协议SL500门锁主动时间同步
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnActiveSyncTime(char *data_buf,char* gw_mac,char* dev_mac)
{		
//回复0502给SmartHome
		int rc = -1;
		char response[64] = {0};

		strncpy(response,SL500_ACTIVE_GET_TIME,4);

		printf("response = %s\n",response);

		if (hsbk_devicedata!=NULL)
		{
				rc = 0x00;
				(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}

		return rc;

}
/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockReturnSyncTimeFail
 功能描述  : 私有协议SL500门锁时间同步失败
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnSyncTimeFail(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};

        strncpy(response,SYNC_TIME_FAIL,4);

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockReturnForUnlockInfo
 功能描述  : 私有协议SL500门锁无线密码验证开锁(主动返回)
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnForUnlockInfo(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};
        char code[64] = {0};
        int codelength = 0;

        strncpy(response,"0808",4);

        strncat(response,data_buf,2);                     //客户属性

        strncpy(code,data_buf+2,4);
        codelength = strtol(code,NULL,16);

        strncat(response,code,4);                         //用户id
        strncat(response,data_buf+6,2);             //开锁方式
        strncat(response,data_buf+8,2);            //开锁方式id

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockReturnForHistory
 功能描述  : 私有协议SL500门锁查询开锁历史记录(主动返回)
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnForHistory(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};
        char code[64] = {0};
        int codelength = 0;
		int data_len = 0;
		int i = 0;

		char historyID[64] = {0};

        strncpy(response,"0809",4);

        strncpy(code,data_buf,4);                           //历史记录编号
        codelength = strtol(code,NULL,16);
        strncat(response,code,4); 
		strncat(historyID,code,4);

        strncat(response,data_buf+4,2);            //客户属性

        //memset(code,0,STRING_SIZE);
        strncpy(code,data_buf+6,4);                   //用户id
        codelength = strtol(code,NULL,16);
        strncat(response,code,4);            
           
        strncat(response,data_buf+10,2);        //开锁方式
        strncat(response,data_buf+12,8);       //时间戳

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

//RSY Test open door history!
/*
		printf("RSY Continue check history!	historyID=%s\n",historyID);
		data_len = HSZSelfProtocolSL500LockHistory(historyID);	//开锁历史记录确认回复

		if(8 == data_len)
		{
				printf("data_len:%d\n",data_len);
				printf("lock_sl500_send_msg_buf: ");
				for(i=0;i<data_len;i++)
				{
						//此处要改，这是确认历史这个函数的！
				       printf("%02X ",lock_sl500_send_msg_buf[i]);
				}
				printf("\n");  

				char controlCode[512] = {0};
				HSZConvertUnCharToStr(controlCode,lock_sl500_send_msg_buf,data_len);
				printf("controlCode:%s\n",controlCode);
				CommandSendDataToSmartLockSL500(HSZGatewayMac(),dev_mac,controlCode);
				return rc;
		}
		else
		{
				printf("RSY ERROR!\n");
		}
*/
//////////////////////////////////////////////////////////////////////

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockReturnForSyncTime
 功能描述  : 私有协议SL500门锁查询锁端时间(主动返回)
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnForSyncTime(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};
        char result[64] = {0};
        unsigned long int time;
        char TimeYear[4] = {0};
        char TimeMonth[4] = {0};
        char TimeDate[4] = {0};
        char TimeHour[4] = {0};
        char TimeMinute[4] = {0};
        char TimeSecond[4] = {0};

        strncpy(response,"080A",8);

        strncpy(result,data_buf,8);

        time =  SL500_my_htoi(result);    
        Hal_Time_SecondToDate(time,TimeYear,TimeMonth,TimeDate,TimeHour,TimeMinute,TimeSecond);
        strcat(response,TimeYear);
        strcat(response,TimeMonth);
        strcat(response,TimeDate);
        strcat(response,TimeHour);
        strcat(response,TimeMinute);
        strcat(response,TimeSecond);

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockReturnForQueryTime
 功能描述  : 解析锁具主动获取时间返回数据
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnForQueryTime(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};

        strncpy(response,REQUST_TIME,4);

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolLockReturnAdminPasswdAuthSuccess
 功能描述  : 解析管理员密码认证返回数据
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnAdminPasswdAuthSuccess(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};

        strncpy(response,ADMIN_AUTH_SUCCESS,4);

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }
		
        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolLockReturnDataForAddTemUser
 功能描述  : 解析添加临时用户成功结果上报
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnDataForAddTemUser(char *data_buf,char* gw_mac,char* dev_mac)			
{
        int rc = -1;
        char response[64] = {0};
        char result[64] = {0};
        int codelength = 0;
		int user_type = 0;

        strncat(response,"080E",4);  								  //data_buf是pdf中的数据段的str""即字符串形式                                     

		//memset(result,0,STRING_SIZE);
		strncpy(result,data_buf,2);
		user_type = SL500_my_htoi(result);
		switch(user_type)												//RSY 先判断用户属性，再分支
        {
				case 2:
				{
					strncat(response,"02",2);							//临时用户
				}
				break;

				case 6:
				{
					strncat(response,"06",2);
				}
				break;

				default:
				break;
		}

		memset(result,0,STRING_SIZE);
        strncpy(result,data_buf+2,4);
        strncat(response,result,4);                                //用户ID

        memset(result,0,STRING_SIZE);
        strncpy(result,data_buf+8,2);
        strncat(response,result,2);                               //密码长度

        codelength = SL500_my_htoi(result);                          //密码或卡片id长度转为整型

        memset(result,0,STRING_SIZE);
        strncpy(result,data_buf+10,codelength*2);
        strncat(response,result,codelength*2);              	//密码或卡片id内容

        memset(result,0,STRING_SIZE);
        strncpy(result,data_buf+10+codelength*2,20);      		//密码或卡片id起始截止时间
        strncat(response,result,20);  

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolLockReturnDataForUserInfo
 功能描述  : 解析查询管理员、普通和临时密码用户信息成功结果上报
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnDataForUserInfo(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};
        char result[64] = {0};
        int codelength = 0;
        int user_type= 0;
        char data[64] = {0};
        char codelen[8] = {0};
		char user_buf[4] = {0};

		printf("RSY data_buf=%s \n",data_buf);
		
        strncpy(user_buf,data_buf,2);
		user_type = strtol(user_buf,NULL,16),
		printf("user_type:%02x \n",user_type);
		
        strncpy(data,data_buf+2,strlen(data_buf)-2);		//为后面判断数据段data是否为空准备
        printf("data = %s\n",data);

        if (0 == strncmp(data,SL500_DATA_IS_NULL,strlen(data_buf)-2) )
        {
				switch(user_type)
				{
						case SL500_ADMIN_USER:
						{
								strcpy(response,ADMIN_USER_IS_NULL);
						}
						break;

						case SL500_NORMAL_USER:
						{
								strcpy(response,NORMAL_USER_IS_NULL);
						}
						break;

						case SL500_TEMP_PASSWD_USER:
						{
								strcpy(response,TEMP_USER_IS_NULL);
						}
						break;

						case SL500_TEMP_CARD_USER:
						{
								strcpy(response,TEMP_CARD_USER_IS_NULL);
						}
						break;

						default:
						break;							
				}
        }
        else												//不同用户不同字段
        {

				switch(user_type)
				{
						case 0:
						{
								strcpy(response,"0805");
			                    memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2,4);							   //用户总数为2个字节，4个字符
		                        strncat(response,result,4);                                //当前用户总数

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+4,4);							  //用户顺序为2个字节，4个字符
		                        strncat(response,result,4);                               //当前用户顺序

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+4+4,4);
		                        strncat(response,result,4);                               //当前用户ID

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+4+4+4,10);
		                        strncat(response,result,10);                               //用户名称

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+12+10,2);
		                        strncat(response,result+1,1);                               //密码个数

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+12+10+2,2);
		                        strncat(response,result+1,1);                               //指纹个数

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+12+10+2+2,2);
		                        strncat(response,result+1,1);                               //卡个数
						}
						break;
						
						case 1:
						{
								strcpy(response,"0806");

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2,4);
		                        strncat(response,result,4);                                //当前用户总数

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+4,4);
		                        strncat(response,result,4);                               //当前用户顺序

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+4+4,4);
		                        strncat(response,result,4);                               //当前用户ID

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+12,10);
		                        strncat(response,result,10);                               //用户名称

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+12+10,2);
		                        strncat(response,result+1,1);                               //密码个数

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+12+10+2,2);
		                        strncat(response,result+1,1);                               //指纹个数

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+12+10+2+2,2);
		                        strncat(response,result+1,1);                               //卡个数
						}
						break;

						case 2:
						{
								strcpy(response,"0807");
								
								memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2,4);
		                        strncat(response,result,4);                                //当前用户总数

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+4,4);
		                        strncat(response,result,4);                               //当前用户顺序

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+4+4,4);
		                        strncat(response,result,4);                               //当前用户ID

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf,2);
		                        strncat(response,result,2);                                //用户类型或卡片id

		                        memset(result,0,STRING_SIZE);
								strncpy(result,data_buf+2+12+2,2);
								strncpy(codelen,data_buf+2+12+2,2);						  //另将密码长度或卡片id长度信息保存
		                        strncat(response,result,2);                               //密码或卡片id长度

		                        memset(result,0,STRING_SIZE);
		                        codelength = strtol(codelen,NULL,10);
		                        strncpy(result,data_buf+2+12+4,codelength * 2);
		                        strncat(response,result,codelength * 2);        			//密码或卡片id内容

		                        memset(result,0,STRING_SIZE);     			
		                        strncpy(result,data_buf+2+12+4+codelength*2,10);			//密码或卡片id起始时间
		                        strncat(response,result,10); 

		                        memset(result,0,STRING_SIZE);      		
		                        strncpy(result,data_buf+2+12+4+10+codelength*2,10);			//密码结束时间
		                        strncat(response,result,10);
						}
						break;

						case 6:
						{
								strcpy(response,"0804");
								
								memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2,4);
		                        strncat(response,result,4);                                //当前用户总数

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+4,4);
		                        strncat(response,result,4);                               //当前用户顺序

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf+2+4+4,4);
		                        strncat(response,result,4);                               //当前用户ID

		                        memset(result,0,STRING_SIZE);
		                        strncpy(result,data_buf,2);
		                        strncat(response,result,2);                                //用户类型或卡片id

		                        memset(result,0,STRING_SIZE);
								strncpy(result,data_buf+2+12+2,2);
								strncpy(codelen,data_buf+2+12+2,2);						  //另将密码长度或卡片id长度信息保存
		                        strncat(response,result,2);                               //密码或卡片id长度

		                        memset(result,0,STRING_SIZE);
		                        codelength = strtol(codelen,NULL,10);
		                        strncpy(result,data_buf+2+12+4,codelength * 2);
		                        strncat(response,result,codelength * 2);        			//密码或卡片id内容

		                        memset(result,0,STRING_SIZE);     			
		                        strncpy(result,data_buf+2+12+4+codelength*2,10);			//密码或卡片id起始时间
		                        strncat(response,result,10); 

		                        memset(result,0,STRING_SIZE);      		
		                        strncpy(result,data_buf+2+12+4+10+codelength*2,10);			//密码结束时间
		                        strncat(response,result,10); 
						}
						break;
						
						default:
						break;						
				}

        }

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolLockReturnForLockVersionInfo
 功能描述  : 解析查询锁具信息成功结果上报
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnForLockVersionInfo(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};
		char result[64] = {0};
		int info_type = 0;

        strncpy(response,"080C",4);
 
		strncpy(result,data_buf,2);								//取查询信息类型aa
		info_type = strtol(result,NULL,10);	

		switch(info_type)
		{
				case 1:
				{
						strncpy(result,data_buf,16);
						strncat(response,data_buf,16); 
				}
				break;
				
				case 2:
				{
						strncpy(result,data_buf,4);
						strncat(response,data_buf,4);
				}
				break;
				
				case 3:
				{
						strncpy(result,data_buf,4);
						strncat(response,data_buf,4);

				}
				break;
				
				case 4:
				{
						strncpy(result,data_buf,6);
						strncat(response,data_buf,6);
				}
				break;

				default:
				break;
		}    

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockReturnDataForLocalAddUser
 功能描述  : 解析本地新增用户结果上报
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnDataForLocalAddUser(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[STRING_SIZE] = {0};
        char result[STRING_SIZE] = {0};

        strncpy(response,"0801",4);
    
        strncpy(result,data_buf,16);
        strncat(response,result,16);
    
        memset(result,0,STRING_SIZE);
        strncpy(result,data_buf+16,2);
        strncat(response,result+1,1);
    
        memset(result,0,STRING_SIZE);
        strncpy(result,data_buf+18,2);
        strncat(response,result+1,1);
    
        memset(result,0,STRING_SIZE);
        strncpy(result,data_buf+20,2);
        strncat(response,result+1,1);

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockReturnDataForLocalModUser
 功能描述  : 解析本地修改用户结果上报
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnDataForLocalModUser(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[STRING_SIZE] = {0};
        char result[STRING_SIZE] = {0};

       strncpy(response,"0803",4);  

       memset(result,0,STRING_SIZE);
       strncpy(result,data_buf,16);
       strncat(response,result,16);
       
       memset(result,0,STRING_SIZE);
       strncpy(result,data_buf+16,2);                                 
       strncat(response,result+1,1);

       memset(result,0,STRING_SIZE);
       strncpy(result,data_buf+18,2);                                 
       strncat(response,result+1,1);

       memset(result,0,STRING_SIZE);
       strncpy(result,data_buf+20,2);                                 
       strncat(response,result+1,1);

       printf("response = %s\n",response);

		if (hsbk_devicedata!=NULL)
		{
				rc = 0x00;
				(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}

        return rc;
}


/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockReturnDataForLocalDelUser
 功能描述  : 解析本地删除用户结果上报
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnDataForLocalDelUser(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[STRING_SIZE] = {0};
        char result[STRING_SIZE] = {0};

       strncpy(response,"0802",4);  

       strncpy(result,data_buf,2);
       strncat(response,result,2);
       
       bzero(result,0);
       strncpy(result,data_buf+2,4);                                 
       strncat(response,result,4);

       printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolSL500LockReturnDataForDelUser
 功能描述  : 解析无线删除用户结果上报
*****************************************************************************/
int HSZSelfProtocolSL500LockReturnDataForDelUser(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
		char response[STRING_SIZE] = {0};
		char result[STRING_SIZE] = {0};
		int user_type = 0;

		strncpy(response,"080F",4);  

		strncpy(result,data_buf,2);
		user_type = SL500_my_htoi(result);
		switch(user_type)											   //RSY 先判断用户属性，再分支
		{
			       case 0:
				   {
						   strncat(response,"00",2);
				   }
				   break;
				   
					   case 1:
				   {
						   strncat(response,"01",2);
				   }
				   break;
				   
				   case 2:
				   {
						   strncat(response,"02",2);
				   }
				   break;
			   
				   case 6:
				   {
						   strncat(response,"06",2);
				   }
				   break;

				   default:
				   break;
		}

		strncpy(result,data_buf+2,4);
		strncat(response,result,4);

		printf("response = %s\n",response);

		if (hsbk_devicedata!=NULL)
		{
		        rc = 0x00;
		        (*hsbk_devicedata)(gw_mac,dev_mac,response);
		}

		return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolLockReturnDataForModOpenMode
 功能描述  : 解析修改开锁方式成功返回数据
*****************************************************************************/
int HSZSelfProtocolLockReturnDataForModOpenMode(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};

        strncpy(response,SL500_MOD_OPEN_MODE,4);

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolLockReturnDataForModCloseMode
 功能描述  : 解析修改关锁方式成功返回数据
*****************************************************************************/
int HSZSelfProtocolLockReturnDataForModCloseMode(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};

        strncpy(response,SL500_MOD_CLOSE_MODE,4);

        printf("response = %s\n",response);
        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }
        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolLockReturnDataForRemoteAddAdminUser
 功能描述  : 解析无线添加管理员用户成功返回数据
*****************************************************************************/
int HSZSelfProtocolLockReturnDataForRemoteAddAdminUser(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
        char response[STRING_SIZE] = {0};
        char result[STRING_SIZE] = {0};

		strncat(response,"080B00",6);
		
		strncpy(result,data_buf,4);
		strncat(response,result,4);

		memset(result,0,STRING_SIZE);
		strncpy(result,data_buf+4,10);
		strncat(response,result,10);

		memset(result,0,STRING_SIZE);
		strncpy(result,data_buf+4+10,2);
		strncat(response,result+1,1);

		memset(result,0,STRING_SIZE);
		strncpy(result,data_buf+4+10+2,2);
		strncat(response,result+1,1);

		memset(result,0,STRING_SIZE);
		strncpy(result,data_buf+4+10+2+2,2);
		strncat(response,result+1,1);

		printf("response = %s\n",response);
        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }
        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolLockReturnDataForRemoteAddNormalUser
 功能描述  : 解析无线添加普通用户成功返回数据
*****************************************************************************/
int HSZSelfProtocolLockReturnDataForRemoteAddNormalUser(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
		char response[STRING_SIZE] = {0};
		char result[STRING_SIZE] = {0};
		
		strncat(response,"080B01",6);
		
		strncpy(result,data_buf,4);
		strncat(response,result,4);
		
		memset(result,0,STRING_SIZE);
		strncpy(result,data_buf+4,10);
		strncat(response,result,10);
		
		memset(result,0,STRING_SIZE);
		strncpy(result,data_buf+4+10,2);
		strncat(response,result+1,1);
		
		memset(result,0,STRING_SIZE);
		strncpy(result,data_buf+4+10+2,2);
		strncat(response,result+1,1);
		
		memset(result,0,STRING_SIZE);
		strncpy(result,data_buf+4+10+2+2,2);
		strncat(response,result+1,1);
		
		printf("response = %s\n",response);
		if (hsbk_devicedata!=NULL)
		{
				rc = 0x00;
				(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}
		return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocolLockReturnDataForUserPasswdAuthen
 功能描述  : 解析用户密码认证返回数据
*****************************************************************************/
int HSZSelfProtocolLockReturnDataForUserPasswdAuthen(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};

        strncpy(response,SL500_USER_PASSWD_AUTH,4);

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSendSelfProtocolSL500LockData
 功能描述  : 封装智能门锁控制指令数据
*****************************************************************************/
int HSZSendSelfProtocolSL500LockData(char* dev_mac,char *send_buf)
{
		int rc = -1;
		int i = 0;
		int data_len = 0;
		char cmd_buf[4] = {0};
		int  cmd_type;
		char data_buf[128]={0};
		char msg_buf[128]={0};

		memset(lock_sl500_send_msg_buf, 0, ARRAY_SIZE);
		memcpy(data_buf, send_buf + 4, strlen(send_buf)-4);         //11acdddddddd -> 11accdddddddd 'cc'表示密码长度（在此发送06-FF）
		strncpy(cmd_buf,data_buf,2);
		cmd_type = strtol(cmd_buf,NULL,16);
		printf("gw_mac:%s,device_mac:%s,cmd:%s\n",HSZGatewayMac(),dev_mac,data_buf);
		printf("cmd_buf:%02x \n",cmd_type);
		strcpy(msg_buf,data_buf+2);                                 //acdddddddd -> accdddddd
		printf("msg_buf:%s\n",msg_buf);
		  
		switch(cmd_type)
		{
   				case APP_REMOTR_OPEN_DOOR_COMMAND:                     
                {
                        data_len = HSZSelfProtocolSL500LockCmdOpenDoor(msg_buf);
                }
                break;
                
                case APP_QUERY_SMARTLOCK_TIME:              
                {
                        data_len = HSZSelfProtocolSL500LockCmdQuerryTime(msg_buf);
                }
                break;
                
                case SMARTLOCK_TIME_SYNCHRONIZATION_COMMAND:              
                {
                        data_len = HSZSelfProtocolSL500LockCmdSynchronizeTime(msg_buf);
                }
                break;
                
                case SMARTLOCK_PASSWD_AUTHEN_COMMAND:                        
                {
                        data_len = HSZSelfProtocolSL500LockCmdPasswdCheck(msg_buf);
                }
                break;

                case SMARTLOCK_ADD_NEW_TEMUSER_COMMAND:                         			/*无线添加临时用户21*/
                {
                        data_len = HSZSelfProtocolSL500LockCmdAddUser(msg_buf);
                }
                break;

                case APP_DELETE_TEMUSER_COMMAND:                         					/*删除用户22*/
                {
                        data_len = HSZSelfProtocolSL500DeleteUser(msg_buf);
                }
                break;

                case SMARTLOCK_QUERY_TEMUSER_INFO_COMMAND:                          		/*查询临时用户信息33*/
                {
                        data_len =  HSZSelfProtocolSL500LockQueryUser(TEM_USER);
                }
                break;

                case SMARTLOCK_QUERY_NORMALUSER_INFO_COMMAND:                       		/*查询普通用户信息32*/
                {
                        data_len =  HSZSelfProtocolSL500LockQueryUser(NORMAL_USER);
                }
                break;

                case SMARTLOCK_QUERY_ADMINUSER_INFO_COMMAND:                        		/*查询管理员用户信息31*/
                {
                        data_len =  HSZSelfProtocolSL500LockQueryUser(ADMIN_USER);
                }
                break;

                case SMARTLOCK_QUERY_OPENDOOR_HISTORY_COMMAND:                      		/*查询开锁历史记录*/
                {
                        data_len =  HSZSelfProtocolSL500LockOpenDoorHistory();
                 
                }
                break;

                case SMARTLOCK_QUERY_LOCK_INFO_COMMAND:                         			/*查询锁具信息*/
                {
                        data_len =  HSZSelfProtocolSL500LockQueryLockInfo(msg_buf);
                }
                break;

                case SMARTLOCK_RESET:                         //reset
                {
                        data_len =  HSZSelfProtocolSL500LockReset();
                }
                break;

	    		case SMARTLOCK_OPRN_HISTORY:                        
                {
                        data_len =  HSZSelfProtocolSL500LockHistory(msg_buf);
                }
				break;

				case SELFPROTOCOLSL500LOCK_QUERY_TEMCARDUSER_INFO:							 //查询临时卡片用户37
				{
						data_len = HSZSelfProtocolQueryTemCardUserInfo(TEM_CARD_USER);  	 //临时卡片用户06 
				}
				break;
				case SELFPROTOCOLSL500LOCK_MOD_OPEN_MODE:
				{
						data_len = HSZSelfProtocolModOpenLockMode(msg_buf);
				}
				break;
				case SELFPROTOCOLSL500LOCK_MOD_CLOSE_MODE:
				{
						data_len = HSZSelfProtocolModCloseLockMode(msg_buf);			
				}
				break;
				case SELFPROTOCOLSL500LOCK_ADD_ADMIN_OR_NORMAL_USER:
				{
						data_len = HSZSelfProtocolAddAdminOrNormalUser(msg_buf);
				}
				break;
				case SELFPROTOCOLSL500LOCK_USER_PASSWD_AUTHEN:
				{
						data_len = HSZSelfProtocolUserPasswdCheck(msg_buf);
				}
        		break;

                default:
                break;
		}
		printf("data_len:%d\n",data_len);
		printf("lock_sl500_send_msg_buf: ");
		for(i=0;i<data_len;i++)
		{
		       printf("%02X ",lock_sl500_send_msg_buf[i]);
		}
		printf("\n");  

		char controlCode[512] = {0};
		HSZConvertUnCharToStr(controlCode,lock_sl500_send_msg_buf,data_len);
		printf("controlCode:%s\n",controlCode);
		CommandSendDataToSmartLockSL500(HSZGatewayMac(),dev_mac,controlCode);
		return rc;
}


/*****************************************************************************
 函 数 名  : HSZSendSelfProtocolSL500LockData
 功能描述  : 封装智能门锁控制指令数据的上报
*****************************************************************************/
/***********************test return*************************************
int HSZReturnSelfProtocolSL500LockData(char* dev_mac,char *send_buf)							//传入的send_buf要带有CMD2命令:310100040201
{
		int CMD2_type = 0;
		int data_len = 0;
		char CMD2_buf[4] = {0};
		char data_buf[64] = {0};
		
		strncpy(CMD2_buf,send_buf,2);
		printf("CMD2_buf=%s ",CMD2_buf);
		CMD2_type = strtol(CMD2_buf,NULL,16);
		printf("CMD2_type=%02x \n",CMD2_type);

		strncpy(data_buf,send_buf+2,strlen(send_buf)-2);										//data_buf=0100040201
		printf("data_buf_last=%s \n",data_buf);

		switch(CMD2_type)
		{
				case SL500LOCK_RETURN_FOR_ACTIVE_SYSC_TIME:				//51
				{
						data_len = HSZSelfProtocolSL500LockReturnActiveSyncTime(data_buf,AppGatewayMac(),dev_mac);  
				}
				break;
					
				case SL500LOCK_RETURN_FOR_ERROR_CODE:					//30
				{
						data_len = HSZSelfProtocolSL500LockReturnDataForErrorCode(data_buf,AppGatewayMac(),dev_mac);  
				}
				break;
				
				case SL500LOCK_RETURN_FOR_OPENLOCK:						//31
				{
						data_len = HSZSelfProtocolSL500LockReturnForUnlockInfo(data_buf,AppGatewayMac(),dev_mac);              
				}
				break;

				case SL500LOCK_RETURN_QUERY_OPENLOCK_HISTORY:			//32
				{
						data_len = HSZSelfProtocolSL500LockReturnForHistory(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_RETURN_AUTO_LOCK:						//36
				{
						data_len = HSZReturnDataForSL500LockEvent(data_buf,AppGatewayMac(),dev_mac);		
				}
				break;

				case SL500LOCK_RETURN_ACTIVE_GET_TIME:					//34
				{	
						data_len = HSZSelfProtocolSL500LockReturnForQueryTime(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_RETURN_QUERY_LOCK_TIME:					//33
				{
						data_len = HSZSelfProtocolSL500LockReturnForSyncTime(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_RETURN_ADMIN_PASSWD_AUTHEN:				//35
				{
						data_len = HSZSelfProtocolSL500LockReturnAdminPasswdAuthSuccess(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_RETURN_WIRELESS_ADD_TEMP:				//37
				{
						data_len = HSZSelfProtocolSL500LockReturnDataForAddTemUser(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_RETURN_WIRELESS_DEL_USER:				//38
				{
						data_len = HSZSelfProtocolSL500LockReturnDataForDelUser(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_RETURN_USER_INFO:						//39
				{
						data_len = HSZSelfProtocolSL500LockReturnDataForUserInfo(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_QUERY_LOCK_INFO:							//3A
				{
						data_len = HSZSelfProtocolSL500LockReturnForLockVersionInfo(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_LOCAL_ADD_USER:							//3B
				{
						data_len = HSZSelfProtocolSL500LockReturnDataForLocalAddUser(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_LOCAL_DEL_USER:							//3C
				{
						data_len = HSZSelfProtocolSL500LockReturnDataForLocalDelUser(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_LOCAL_MOD_USER:							//3D
				{
						data_len = HSZSelfProtocolSL500LockReturnDataForLocalModUser(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_MOD_OPEN_MODE:							//3E
				{
						data_len = HSZSelfProtocolLockReturnDataForModOpenMode(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_MOD_CLOSE_MODE:							//3F
				{
						data_len = HSZSelfProtocolLockReturnDataForModCloseMode(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_WIRELESS_ADD_ADMIN:
				{
						data_len = HSZSelfProtocolLockReturnDataForRemoteAddAdminUser(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_WIRELESS_ADD_NORMAL:
				{
						data_len = HSZSelfProtocolLockReturnDataForRemoteAddNormalUser(data_buf,AppGatewayMac(),dev_mac);
				}
				break;

				case SL500LOCK_USER_PASSWD_AUTHEN:
				{
						data_len = HSZSelfProtocolLockReturnDataForUserPasswdAuthen(data_buf,AppGatewayMac(),dev_mac);
				}
				break;
				
				default:
						printf("Nothing \n");
				break;
		}
}
*********************test return*********************************/
