/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZSelfProtocolLock.h
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2018年8月124日
  最近修改   :
  功能描述   : 处理私有智能门锁协议的 Data数据
  函数列表   :
  修改历史   :
  1.日    期   : 2019年5月8日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

******************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
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
#include "HSZSelfProtocol374Lock.h"
#include "HSZSmartLockMessage.h"


#define DEBUG_ON

#ifdef DEBUG_ON
        #define HIS_DEBUG(format,...) printf(format,__VA_ARGS__)
#else
        #define HIS_DEBUG(format,...)
#endif

extern unsigned char lock_send_msg_buf[512];
extern int my_tolower(int c);
extern void my_toUppercase(char *src,int len);
extern int my_htoi(char s[]);
extern int HSZCalculateSelfProtocolLockDataCheckSum(int sof,int len,unsigned char *data_buf);


/*****************************************************************************
 函 数 名  : HSZAnalysisSelfProtocol374LockData
 功能描述  : 解析智能门锁私有数据
*****************************************************************************/
int HSZAnalysisSelfProtocol374LockData(unsigned char *data,char* gw_mac,char* dev_mac)
{
         struct smartlock_self_protocol_info smartlock_self_protocol_info_data;
         memset(&smartlock_self_protocol_info_data,0,sizeof(struct smartlock_self_protocol_info));
         
         int rc = -1;
         int sum = -1; 

         smartlock_self_protocol_info_data.sof = data[0];                                                                           
         
         smartlock_self_protocol_info_data.len = data[1];                                                                          

         smartlock_self_protocol_info_data.cmd1 = data[2];                                                                      

         smartlock_self_protocol_info_data.cmd2 = data[3];                                                                    

         smartlock_self_protocol_info_data.data_len = data[4];                                                                

         memset(smartlock_self_protocol_info_data.data, 0, 1024);
         memcpy(smartlock_self_protocol_info_data.data, &data[2], smartlock_self_protocol_info_data.len);       //CMD1开始算起
         
         sum = HSZCalculateSelfProtocolLockDataCheckSum(smartlock_self_protocol_info_data.sof,smartlock_self_protocol_info_data.len,smartlock_self_protocol_info_data.data);
         
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
	          strncpy(response,_374_QUERY_TIME,4);
	          if (hsbk_devicedata!=NULL)
	          {
	                rc = 0x00;
	                (*hsbk_devicedata)(gw_mac,dev_mac,response);
	          }
	          printf(" response = %s\n", response);
	          return rc;
         }
         //////////////////////////////////////////////////////////////////////////
         
         memset(smartlock_self_protocol_info_data.data, 0, 1024);
         memcpy(smartlock_self_protocol_info_data.data, &data[5], smartlock_self_protocol_info_data.data_len);       //DATA数据段

         //////////////////////////////////////////////////////////////////////////
         
         printf(" smartlock_self_protocol_info_data.data_len = %d\n", smartlock_self_protocol_info_data.data_len);


         char msg_buffer[1024]={0};  
         
         HSZConvertUnCharToStr(msg_buffer,smartlock_self_protocol_info_data.data,smartlock_self_protocol_info_data.data_len);    
     
         int i;
         
         for(i = 0;i < (smartlock_self_protocol_info_data.data_len);i++)                                                                                        
         {
                printf("0x%02x ",smartlock_self_protocol_info_data.data[i]&0xFF);
         }
        
         printf("\n");
        

         if(CMD1_LOCK_RESPONSE == smartlock_self_protocol_info_data.cmd1)
         {
                 switch(smartlock_self_protocol_info_data.cmd2)
                 {
                        case CMD2_ERROR_INFO_RESPONSE:                          
                        {
                                printf("374Error info report...\n");
                                HSZSelfProtocol374LockReturnDataForErrorCode(msg_buffer,gw_mac,dev_mac);
                        }
                        break;
                        
                        case CMD2_OPEN_DOOR_INFO_RESPONSE:             
                        {
                                printf("Unlock info report...\n");
                                HSZSelfProtocol374LockReturnForUnlockInfo(msg_buffer,gw_mac,dev_mac);
                        }
                        break;
                        
                        case CMD2_OPEN_DOOR_HISTORY_RESPONSE:        
                        {
                                printf("Unlock history report...\n");
                                HSZSelfProtocol374LockReturnForHistory(msg_buffer,gw_mac,dev_mac);
                        }
                        break;
                        
                        case CMD2_DOOR_TIME_RESPONSE:                   
                        {
                                printf("Lock local time report...\n");
                                HSZSelfProtocol374LockReturnForSyncTime(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_DOOR_REQUEST_TIME_SYNC:           
                        {
                                printf("lock requir time...\n");
                                HSZSelfProtocol374LockReturnForQueryTime(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_ADMIN_PASSWD_AUTH_OK:                
                        {
                                printf("Admin passwd auth succeed...\n");
                                HSZSelfProtocol374LockReturnAdminPasswdAuthSuccess(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_DOOR_EVENT_REPORT:                    
                        {
                                printf("Lock event report...\n");
                                HSZReturnDataFor374LockEvent(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_REMOTE_ADD_USER_OK_REPORT:     
                        {
                                printf("Remote add user succeed result report...\n");
                                HSZSelfProtocol374LockReturnDataForAddTemUser(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_RREMOTE_DEL_USER_OK_REPORT:  
                        {
                                printf("Remote del user succeed result report...\n");
                                HSZSelfProtocol374LockReturnDataForDelUser(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_USER_QUERY_RESULT_REPORT:        
                        {
                                printf("Query user result report...\n");
                                HSZSelfProtocol374LockReturnDataForUserInfo(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_DOOR_INFO_REPORT:                      
                        {
                               HSZSelfProtocol374LockReturnForLockVersionInfo(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_LOCAL_NEW_USER_REPORT:              
                        {
                                printf("Local add user result report...\n");
                                HSZSelfProtocol374LockReturnDataForLocalAddUser(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_LOCAL_DEL_USER_REPORT:              
                        {
                               printf("Local del user result report...\n");
                               HSZSelfProtocol374LockReturnDataForLocalDelUser(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_LOCAL_MOD_USER_REPORT:            
                        {
                               printf("Local del user result report...\n");
                               HSZSelfProtocol374LockReturnDataForLocalModUser(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

						case CMD2_QUERY_NORMAL_TIMELIMITED_REPORT:            
                        {
                               printf("Local del user result report...\n");
                               HSZSelfProtocol374LockReturnDataForQueryNorTimeLimited(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

						case CMD2_MOD_NORMAL_TIMELIMITED_REPORT:            
                        {
                               printf("Local del user result report...\n");
                               HSZSelfProtocol374LockReturnDataForModNorTimeLimited(msg_buffer,gw_mac,dev_mac);
                        }
                        break;
                        
                        default:
                        break;  
                 }
         }   
 
         return 0;
}          

/*****************************************************************************
 函 数 名  : HSZSelfProtocol374LockCmdAddUser
 功能描述  : 无线删除用户
*****************************************************************************/
int HSZSelfProtocol374DeleteUser(char *data_buf)
{
        int sum = 0x00;
        int index = 0;
        int code_temp = 0;
        
        char pincode[4] = {0};

        memset(lock_send_msg_buf,0x00,512);
                
        lock_send_msg_buf[0] = 0xFA;                                                                
                        
        lock_send_msg_buf[1] = 0x06;                                                              
                        
        lock_send_msg_buf[2] = 0x13;                                                                
                        
        lock_send_msg_buf[3] = 0x06;                                                                

        lock_send_msg_buf[4] = 0x03;                                                                 
        strncpy(pincode,data_buf,1);
                
        code_temp = strtol(pincode,NULL,16);

        lock_send_msg_buf[5] = (code_temp&0xFF);  
                                                          
        strncpy(pincode,data_buf+1,2);
                
        code_temp = strtol(pincode+1,NULL,16);
       
        lock_send_msg_buf[6] = (code_temp&0xFF);  

        strncpy(pincode,data_buf+3,2);
                
        code_temp = strtol(pincode,NULL,16);
       
        lock_send_msg_buf[7] = (code_temp&0xFF);  

        for(index = 0;index<8;index++)           
        {
                sum = (sum + lock_send_msg_buf[index])&0xFF;
        }
        
        lock_send_msg_buf[8] = sum;

        return 9;
}
                     
/*****************************************************************************
 函 数 名  : HSZSelfProtocol374LockCmdAddUser
 功能描述  : 无线添加临时用户
*****************************************************************************/
int HSZSelfProtocol374LockCmdAddUser(char *data_buf)
{
        int sum = 0x00;
        int index = 0;
        int code_length = 0;
        int code_temp = 0;
        int i;
        int  user_type = 0;
        
        char codelen[8] = {0};
        char pincode[8] = {0};
        char usertype[8] = {0};

        strncpy(usertype,data_buf,1);
        user_type = strtol(usertype,NULL,10);

        strncpy(codelen,data_buf+1,1);
        code_length = strtol(codelen,NULL,16);  

        memset(lock_send_msg_buf,0x00,512);
                
        lock_send_msg_buf[0] = 0xFA;                                                                
                        
        lock_send_msg_buf[1] = ((code_length+1+1+5+5+3)&0xFF);             
                        
        lock_send_msg_buf[2] = 0x13;                                                                
                        
        lock_send_msg_buf[3] = 0x05;                                                                

        switch(user_type)
        {
              case 0:
              {
                     printf("add admin user.\n");
              }
                 break;

              case 1:
              {
                     printf("add normal user.\n");
              }
                 break;

              case 2:
              {
                     printf("add temp user.\n");

                     lock_send_msg_buf[4] = ((code_length+1+1+5+5)&0xFF); ;  
                     lock_send_msg_buf[5] = 0x02;                                                             
                     lock_send_msg_buf[6] = (code_length)&0xFF;                                        

                     for(i=0;i<code_length;i++)                                                                                              
                     {
                              strncpy(pincode,data_buf+2+i,1);
             
                              code_temp = strtol(pincode,NULL,10);
             
                              lock_send_msg_buf[7+i] = code_temp & 0xFF ; 
                     }

                     //data_buf = data_buf + code_length;

                     for(i=0;i<5;i++)                                                                                
                     {
                              strncpy(pincode,data_buf+code_length+2+i*2,2);
             
                              code_temp = strtol(pincode,NULL,16);
             
                              lock_send_msg_buf[7+code_length+i] = code_temp & 0xFF ; 
                     }

                     for(i=0;i<5;i++)                                                                         
                     {
                              strncpy(pincode,data_buf+code_length+2+10+i*2,2);
             
                              code_temp = strtol(pincode,NULL,16);
             
                              lock_send_msg_buf[7+code_length+5+i] = code_temp & 0xFF ; 
                     }

                    for(index = 0;index<(5+lock_send_msg_buf[4]);index++)           
                    {
                            sum = (sum + lock_send_msg_buf[index])&0xFF;
                    }
                    
                    lock_send_msg_buf[5+lock_send_msg_buf[4]] = sum;                                    //checksum

                 }
                 break;

				 default:
				 break;
        }
		
		return (5+lock_send_msg_buf[4]+1);
}  

/*****************************************************************************
 函 数 名  : HSZSelfProtocol374LockCmdOpenDoor
 功能描述  : 开锁
*****************************************************************************/
int HSZSelfProtocol374LockCmdOpenDoor(char *data_buf)
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

        strncpy(codelen,data_buf+1,1);     
        code_length = strtol(codelen,NULL,16);

        strncpy(opendoor_way,data_buf,1);
        opendoor_type = strtol(opendoor_way,NULL,10);
        
        memset(lock_send_msg_buf,0x00,512);
                
        lock_send_msg_buf[0] = 0xFA;                                                                   
                        
        lock_send_msg_buf[1] = (code_length+2+3)&0xFF;                                
                        
        lock_send_msg_buf[2] = 0x13;                                                                   
                        
        lock_send_msg_buf[3] = 0x01;                                                                 

        switch(opendoor_type)
        {
                case 1:                                                                                                                    
                {
                        printf("admin password open door...\n");
                        lock_send_msg_buf[4] = ((code_length+2)&0xFF);                                    
                        printf("code_length = %d\n",code_length);
                        printf("lock_send_msg_buf[4] = %02x\n",lock_send_msg_buf[4]);

                        lock_send_msg_buf[5] = 0x01;                                                                     

                        lock_send_msg_buf[6] = code_length & 0xFF;                                             

                        for(i=0;i<code_length;i++)                                                              
                        {
                                 strncpy(pincode,data_buf+2+i,1);
                
                                 code_temp = strtol(pincode,NULL,10);
                
                                 lock_send_msg_buf[7+i] = code_temp & 0xFF ; 
                        }
                }
                break;
                
                case 2:                                                                                                                     
                {
                        printf("password open door...\n");
                        lock_send_msg_buf[4] = ((code_length+2)&0xFF);                                      
                        printf("code_length = %d\n",code_length);
                        printf("lock_send_msg_buf[4] = %02x\n",lock_send_msg_buf[4]);

                        lock_send_msg_buf[5] = 0x02;                                                                     

                        lock_send_msg_buf[6] = code_length & 0xFF;                                            

                        for(i=0;i<code_length;i++)                                                              
                        {
                                 strncpy(pincode,data_buf+2+i,1);
                
                                 code_temp = strtol(pincode,NULL,10);
                
                                 lock_send_msg_buf[7+i] = code_temp & 0xFF ; 
                        }
                }
                break;
                
                case 3:                                                                                                                    
                {
                        printf("force open door...\n");

                        lock_send_msg_buf[1] = 0x09;     

                        lock_send_msg_buf[4] = 0x06;                                     
                        printf("code_length = %d\n",code_length);
                        printf("lock_send_msg_buf[4] = %02x\n",lock_send_msg_buf[4]);

                        lock_send_msg_buf[5] = 0x03;                                                                     

                        //lock_send_msg_buf[6] = code_length & 0xFF;                                         

                        lock_send_msg_buf[6] = 0x04;                                                                  

                        /*for(i=0;i<code_length;i+=2)                                                              
                        {
                                 memset(pincode,0,4);
                                 strncpy(pincode,data_buf+2+i,2);

                                 strncat(result,pincode,2);
                
                                 //code_temp = strtol(pincode,NULL,10);
                
                                 //lock_send_msg_buf[7+i] = code_temp & 0xFF ; 
                        }*/

                       data_buf = data_buf + 2;					

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

                       printf("time = %ld\n",time);

                       sprintf(timeunix_hex,"%lx",time);

                       my_toUppercase(timeunix_hex,strlen(timeunix_hex));

                       printf("timeunix_hex_upper = %s\n",timeunix_hex);

                       timeunix_hex_val = strtol(timeunix_hex,NULL,16);

                       lock_send_msg_buf[7] = (((timeunix_hex_val)&0xFF000000)>>24);                         

                       lock_send_msg_buf[8] = (((timeunix_hex_val)&0x00FF0000)>>16);                         

                       lock_send_msg_buf[9] = (((timeunix_hex_val)&0x0000FF00)>>8);                          

                       lock_send_msg_buf[10] = ((timeunix_hex_val)&0x000000FF);                                 
                }
                break;
                
                default:
                
                break;
        }
        
        for(index = 0;index<(5+lock_send_msg_buf[4]);index++)           
        {
                sum = (sum + lock_send_msg_buf[index])&0xFF;
        }
        
        lock_send_msg_buf[5+lock_send_msg_buf[4]] = sum;                                   

        return (5+lock_send_msg_buf[4]+1);
}


int HSZSelfProtocol374LockCmdQuerryTime(char *data_buf)
{
        
        memset(lock_send_msg_buf,0x00,512);
        
        lock_send_msg_buf[0] = 0xFA;                                                                  
                        
        lock_send_msg_buf[1] = 0x03;                                                            
                        
        lock_send_msg_buf[2] = 0x13;                                                                  
                        
        lock_send_msg_buf[3] = 0x02;                                                                   
        
        lock_send_msg_buf[4] = 0x00;                                                                    
        
        lock_send_msg_buf[5] = 0x12;                                                                   
        
        return 0x06;
}


int HSZSelfProtocol374LockCmdSynchronizeTime(char *data_buf)
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
        memset(lock_send_msg_buf,0x00,512);
                
        lock_send_msg_buf[0] = 0xFA;                                                                 
                        
        lock_send_msg_buf[1] = 0x07;                                                                   
                        
        lock_send_msg_buf[2] = 0x13;                                                                   
                        
        lock_send_msg_buf[3] = 0x03;                                                                   

        lock_send_msg_buf[4] = 0x04;                                                                   

        lock_send_msg_buf[5] = ((time&0xFF000000)>>24);                        

        lock_send_msg_buf[6] = ((time&0x00FF0000)>>16);                       

        lock_send_msg_buf[7] = ((time&0x0000FF00)>>8);                         

        lock_send_msg_buf[8] = (time&0x000000FF);                                       
                                        
        //strncpy(lock_send_msg_buf[5],data_buf+3,data_buf[2]);                   //data
        for(index = 0;index<(5+lock_send_msg_buf[4]);index++)           
        {
                sum = (sum + lock_send_msg_buf[index])&0xFF;
        }
        lock_send_msg_buf[9] = sum;

        return 0x0A;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocol374LockCmdPasswdCheck
 功能描述  : 管理员密码认证
*****************************************************************************/
int HSZSelfProtocol374LockCmdPasswdCheck(char *data_buf)
{
        int sum = 0x00;
        int index = 0;
        int code_length = 0;
        int code_temp = 0;
        int i;
        
        char codelen[4] = {0};
        char pincode[4] = {0};

        strncpy(codelen,data_buf,1);
        code_length = strtol(codelen,NULL,16);  

        memset(lock_send_msg_buf,0x00,512);
                
        lock_send_msg_buf[0] = 0xFA;                                                                
                        
        lock_send_msg_buf[1] = (code_length+3)&0xFF;                                  
                        
        lock_send_msg_buf[2] = 0x13;                                                                 
                        
        lock_send_msg_buf[3] = 0x04;                                                                

        lock_send_msg_buf[4] = (code_length)&0xFF; 

        for(i=0;i<code_length;i++)                                                                      
        {
                strncpy(pincode,data_buf+1+i,1);
                
                code_temp = strtol(pincode,NULL,10);
                
                lock_send_msg_buf[5+i] = (code_temp&0xFF);  
        }
                                
        for(index = 0;index<(code_length+5);index++)           
        {
                sum = (sum + lock_send_msg_buf[index])&0xFF;
        }
        
        lock_send_msg_buf[code_length+5] = sum;

        return (code_length+5+1);
}

/****************************************************************************
 函 数 名  : HSZSelfProtocol374LockQueryUser
 功能描述  : 私有协议374门锁解析查询管理员、普通和临时用户信息
*****************************************************************************/
int HSZSelfProtocol374LockQueryUser(int user_type)
{
        int sum = 0x00;
        int index = 0;
        
        memset(lock_send_msg_buf,0x00,512);
                
        lock_send_msg_buf[0] = 0xFA;                                                                
                        
        lock_send_msg_buf[1] = 0x04;                                                                
                        
        lock_send_msg_buf[2] = 0x13;                                                                  
                        
        lock_send_msg_buf[3] = 0x07;                                                                

        lock_send_msg_buf[4] = 0x01;                                                                

		switch(user_type)
		{
				case 3:
				{
						lock_send_msg_buf[5] = SMARTLOCK_374_ADMIN_USER;   
				}
				break;

				case 2:
				{
						lock_send_msg_buf[5] = SMARTLOCK_374_NORMAL_USER;
				}
				break;

				case 1:
				{
						lock_send_msg_buf[5] = SMARTLOCK_374_TEP_USER;

				}
				break;
				
				default:
				break;
		}

/*
        if (user_type == ADMIN_USER)
        {
               lock_send_msg_buf[5] = SMARTLOCK_374_ADMIN_USER;   
        }
        else if (user_type == NORMAL_USER)
        {
               lock_send_msg_buf[5] = SMARTLOCK_374_NORMAL_USER;
        }
        else if (user_type == TEM_USER)
        {
               lock_send_msg_buf[5] = SMARTLOCK_374_TEP_USER;
        }
*/

       for(index = 0;index<6;index++)         
        {
                sum = (sum + lock_send_msg_buf[index])&0xFF;
        }
        
        lock_send_msg_buf[6] = sum;

        return 7;
}

/****************************************************************************
 函 数 名  : HSZSelfProtocol374LockQueryLockInfo
 功能描述  : 私有协议374门锁解析查询锁具信息
*****************************************************************************/
int HSZSelfProtocol374LockQueryLockInfo(char *data_buf)
{
        int sum = 0x00;
        int index = 0;
		int code_temp = 0;
        
        char pincode[4] = {0};

        
        memset(lock_send_msg_buf,0x00,512);
                
        lock_send_msg_buf[0] = 0xFA;                                                                 
                        
        lock_send_msg_buf[1] = 0x04;                                                                
                        
        lock_send_msg_buf[2] = 0x13;                                                                  
                        
        lock_send_msg_buf[3] = 0x08;                                                                 

        lock_send_msg_buf[4] = 0x01;   
		
	 strncpy(pincode,data_buf,2);
                
        code_temp = strtol(pincode,NULL,16);
       
        lock_send_msg_buf[5] = (code_temp&0xFF);  
       
        for(index = 0;index<6;index++)         
        {
                sum = (sum + lock_send_msg_buf[index])&0xFF;
        }
        
        lock_send_msg_buf[6] = sum;

        return 7;
}

/****************************************************************************
 函 数 名  : HSZSelfProtocol374LockOpenDoorHistory
 功能描述  : 私有协议374门锁解析开锁历史查询
*****************************************************************************/
int HSZSelfProtocol374LockOpenDoorHistory(void)
{
        
        memset(lock_send_msg_buf,0x00,512);
                
        lock_send_msg_buf[0] = 0xFA;                                                               
                        
        lock_send_msg_buf[1] = 0x03;                                                                
                        
        lock_send_msg_buf[2] = 0x13;                                                                 
                        
        lock_send_msg_buf[3] = 0x09;                                                                

        lock_send_msg_buf[4] = 0x00; 

        lock_send_msg_buf[5] = 0x19;   
       
        return 6;
}

/****************************************************************************
 函 数 名  : HSZSelfProtocol374LockReset
 功能描述  : 私有协议374门锁解析恢复出厂设置状态确认
*****************************************************************************/
int HSZSelfProtocol374LockReset(void)
{
        memset(lock_send_msg_buf,0x00,512);
        lock_send_msg_buf[0] = 0xFA;                                                               
        lock_send_msg_buf[1] = 0x03;                                                                
        lock_send_msg_buf[2] = 0x13;                                                                 
        lock_send_msg_buf[3] = 0x0B;                                                                

        lock_send_msg_buf[4] = 0x00; 
        lock_send_msg_buf[5] = 0x1B;   
        return 6;
}

/****************************************************************************
 函 数 名  : HSZSelfProtocol374LockHistory
 功能描述  : 私有协议374门锁解析查询开锁历史记录
*****************************************************************************/
int HSZSelfProtocol374LockHistory(char *data_buf)
{
		int sum = 0x00;
        int index = 0;
        int code_temp = 0;
        
        char pincode[4] = {0};

        memset(lock_send_msg_buf,0x00,512);
                
        lock_send_msg_buf[0] = 0xFA;                                                                
                        
        lock_send_msg_buf[1] = 0x05;                                                              
                        
        lock_send_msg_buf[2] = 0x13;                                                                
                        
        lock_send_msg_buf[3] = 0x0A;                                                                

        lock_send_msg_buf[4] = 0x02;                                                                 
                                                                  
        strncpy(pincode,data_buf,2);
                
        code_temp = strtol(pincode,NULL,16);
       
        lock_send_msg_buf[5] = (code_temp&0xFF);  

        strncpy(pincode,data_buf+2,2);
                
        code_temp = strtol(pincode,NULL,16);
       
        lock_send_msg_buf[6] = (code_temp&0xFF);  

        for(index = 0;index<7;index++)           
        {
                sum = (sum + lock_send_msg_buf[index])&0xFF;
        }
        
        lock_send_msg_buf[7] = sum;

        return 8;

}

/*****************************************RSY 374************************************/
/****************************************************************************
 函 数 名  : HSZSelfProtocol374QueryNormalTimeLimited
 功能描述  : 私有协议374门锁查询普通用户使用时限
*****************************************************************************/
int HSZSelfProtocol374QueryNormalTimeLimited(char *data_buf)
{
		int sum = 0x00;
		int index = 0;
		int code_temp = 0;
		char pincode[4] = {0};

		memset(lock_send_msg_buf,0x00,512);
		
		lock_send_msg_buf[0] = 0xFA;
		lock_send_msg_buf[1] = 0x05;
		lock_send_msg_buf[2] = 0x13;
		lock_send_msg_buf[3] = 0x11;
		lock_send_msg_buf[4] = 0x02;

		strncpy(pincode,data_buf,2);                
        code_temp = strtol(pincode,NULL,16);       
        lock_send_msg_buf[5] = (code_temp&0xFF);  

		strncpy(pincode,data_buf+2,2);                
        code_temp = strtol(pincode,NULL,16);       
        lock_send_msg_buf[6] = (code_temp&0xFF);  

		for(index = 0;index<7;index++)           
        {
                sum = (sum + lock_send_msg_buf[index])&0xFF;
        }
        
        lock_send_msg_buf[7] = sum;

        return 8;
}

/****************************************************************************
 函 数 名  : HSZSelfProtocol374ModNormalTimeLimited
 功能描述  : 私有协议374门锁修改普通用户使用时限
*****************************************************************************/
int HSZSelfProtocol374ModNormalTimeLimited(char *data_buf)
{
		int sum = 0x00;
        int index = 0;
        int time_length = 0;
        int time_temp = 0;
        int i;
        int user_type = 0;
        
        char codelen[8] = {0};
        char pincode[8] = {0};
        char usertype[8] = {0};

		memset(lock_send_msg_buf,0x00,512);
		
		lock_send_msg_buf[0] = 0xFA;
		lock_send_msg_buf[1] = 0x0F;
		lock_send_msg_buf[2] = 0x13;
		lock_send_msg_buf[3] = 0x12;
		lock_send_msg_buf[4] = 0x0C;

		strncpy(pincode,data_buf,2);                
        time_temp = strtol(pincode,NULL,16);       
        lock_send_msg_buf[5] = (time_temp&0xFF);  

		strncpy(pincode,data_buf+2,2);                
        time_temp = strtol(pincode,NULL,16);       
        lock_send_msg_buf[6] = (time_temp&0xFF); 

		for(i=0;i<5;i++)                                                                                
		{
				strncpy(pincode,data_buf+4+i*2,2);
				time_temp = strtol(pincode,NULL,16);
				lock_send_msg_buf[7+i] = time_temp & 0xFF ; 
		}

		for(i=0;i<5;i++)                                                                                
		{
				strncpy(pincode,data_buf+4+10+i*2,2);
				time_temp = strtol(pincode,NULL,16);
				lock_send_msg_buf[12+i] = time_temp & 0xFF ; 
		}

		for(index = 0;index<17;index++)           
        {
                sum = (sum + lock_send_msg_buf[index])&0xFF;
        }
        
        lock_send_msg_buf[17] = sum;

		return 18;

}
/*****************************************RSY 374************************************/



int HSZSelfProtocol374LockReturnDataForControl(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};

        if(strcmp(SMARTLOCK_SYNC_TIME_FAIL,data_buf)==0)
        {
                strncpy(response,SYNC_TIME_FAIL,4);
        }
        else if(strcmp(SMARTLOCK_OPEN_DOOR_FAIL,data_buf)==0)
        {
                strncpy(response,OPEN_DOOR_FAIL,4);
        }
        else if(strcmp(SMARTLOCK_OPEN_DOOR_PASSWORD_ERROR,data_buf)==0)
        {
                strncpy(response,OPEN_DOOR_PASSWORD_ERROR,4);
        }
        else if(strcmp(SMARTLOCK_FORBID_REMOTE_OPEN_DOOR,data_buf)==0)
        {
                strncpy(response,FORBID_REMOTE_OPEN_DOOR,4);
        }
        else
        {
                return rc;
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
 函 数 名  : HSZSelfProtocol374LockReturnDataForErrorCode
 功能描述  : 私有协议374门锁解析错误密码返回数据
*****************************************************************************/
int HSZSelfProtocol374LockReturnDataForErrorCode(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
		int data_code = 0;
        char response[30] = {0};

		data_code = strtol(data_buf,NULL,16);

		switch(data_code)
		{
				printf("374data_code is %d\n",data_code);
				case SYNC_TIME_FAILED_ERRORCODE:
				{
						strncpy(response,SYNC_TIME_FAIL,4);
				}
				break;

				case PASSWD_ERROR_ERRORCODE:
				{
						strncpy(response,OPENDOOR_PASSWD_ERROR,4);
				}
				break;

				case ADMIN_PASSWD_AUTH_FAILED_ERRORCODE:
				{
						strncpy(response,ADMIN_PASSWD_AUTH_FAILED,4);
				}
				break;

				case REMOTE_OPENDOOR_PASSWD_ERROR_ERRORCODE:
				{
						strncpy(response,OPENDOOR_PASSWD_ERROR,4);
				}
				break;

				case OPENDOOR_FAILED_ERRORCODE:
				{
						strncpy(response,OPENDOOR_FAILED,4);
				}
				break;

				case REMOTE_OPENDOOR_FORBID_ERRORCODE:
				{
						strncpy(response,REMOTE_OPENDOOR_FORBID,4);
				}
				break;

				case ADD_PASSWD_DUPLICATE_ERRORCODE:
				{
						strncpy(response,ADD_PASSWD_DUPLICATE,4);
				}
				break;

				case ADD_USER_FAILED_ERRORCODE:
				{
						strncpy(response,ADD_USER_FAILED,4);
				}
				break;

				case DEL_USER_FAILED_ERRORCODE:
				{
						strncpy(response,DEL_USER_FAILED,4);
				}
				break;

				case MOD_NORMALUSER_TIMELIMITED_FAILED:
				{
						strncpy(response,MOD_TIMELIMITED_FAILED,4);
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
 函 数 名  : HSZReturnDataFor374LockEvent
 功能描述  : 私有协议374门锁事件上报(主动返回)
*****************************************************************************/
int HSZReturnDataFor374LockEvent(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
		int data_type = 0;
        char response[30] = {0};

        printf("data_buf = %s\n",data_buf);

        my_toUppercase(data_buf,strlen(data_buf));

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
 函 数 名  : HSZSelfProtocol374LockReturnSyncTimeFail
 功能描述  : 私有协议374门锁时间同步失败
*****************************************************************************/
int HSZSelfProtocol374LockReturnSyncTimeFail(char *data_buf,char* gw_mac,char* dev_mac)
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
 函 数 名  : HSZSelfProtocol374LockReturnForUnlockInfo
 功能描述  : 私有协议374门锁无线密码验证开锁(主动返回)
*****************************************************************************/
int HSZSelfProtocol374LockReturnForUnlockInfo(char *data_buf,char* gw_mac,char* dev_mac)
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
        strncat(response,data_buf+6,2);             	  //开锁方式
        strncat(response,data_buf+8,2);            		  //开锁方式id

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocol374LockReturnForHistory
 功能描述  : 私有协议374门锁查询开锁历史记录(主动返回)
*****************************************************************************/
int HSZSelfProtocol374LockReturnForHistory(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};
        char code[64] = {0};
        int codelength = 0;

        strncpy(response,"0809",4);

        strncpy(code,data_buf,4);                           //历史记录编号
        codelength = strtol(code,NULL,16);
        strncat(response,code,4); 

        strncat(response,data_buf+4,2);            			//客户属性

        memset(code,0,64);
        strncpy(code,data_buf+6,4);                   		//用户id
        codelength = strtol(code,NULL,16);	
        strncat(response,code,4);            
           
        strncat(response,data_buf+10,2);        			//开锁方式
        strncat(response,data_buf+12,8);       				//时间戳

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocol374LockReturnActiveSyncTime
 功能描述  : 私有协议374门锁主动时间同步
*****************************************************************************/
int HSZSelfProtocol374LockReturnForSyncTime(char *data_buf,char* gw_mac,char* dev_mac)
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

        time =  my_htoi(result);    
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
 函 数 名  : HSZSelfProtocol374LockReturnForQueryTime
 功能描述  : 解析374锁具主动获取时间返回数据
*****************************************************************************/
int HSZSelfProtocol374LockReturnForQueryTime(char *data_buf,char* gw_mac,char* dev_mac)
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
 函 数 名  : HSZSelfProtocol374LockReturnAdminPasswdAuthSuccess
 功能描述  : 解析管理员密码认证返回数据
*****************************************************************************/
int HSZSelfProtocol374LockReturnAdminPasswdAuthSuccess(char *data_buf,char* gw_mac,char* dev_mac)
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
 函 数 名  : HSZSelfProtocol374LockReturnDataForAddTemUser
 功能描述  : 解析添加临时用户成功结果上报
*****************************************************************************/
int HSZSelfProtocol374LockReturnDataForAddTemUser(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[30] = {0};
        char result[128] = {0};
        int codelength = 0;

        strncat(response,"080E",4);  
        strncat(response,"02",2);                                  //临时用户

        strncpy(result,data_buf+2,4);
        strncat(response,result,4);                                //用户ID

        memset(result,0,30);
        strncpy(result,data_buf+8,2);
        strncat(response,result,2);                               //密码长度

        codelength = my_htoi(result);                          	  //密码长度转为整型

        memset(result,0,30);
        strncpy(result,data_buf+10,codelength*2);
        strncat(response,result,codelength*2);              	  //密码内容

        memset(result,0,30);
        strncpy(result,data_buf+10+codelength*2,20);      		  //密码起始截止时间
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
 函 数 名  : HSZSelfProtocol374LockReturnDataForUserInfo
 功能描述  : 解析查询管理员、普通和临时密码用户信息成功结果上报
*****************************************************************************/
int HSZSelfProtocol374LockReturnDataForUserInfo(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};
        char result[64] = {0};
        int codelength = 0;
		char user_buf[8] = {0};
        int user_type = 0;
        char data[8] = {0};
        char codelen[8] = {0};

        strncpy(user_buf,data_buf,2);
		user_type = strtol(user_buf,NULL,16);
        strncpy(data,data_buf+2,2);
        printf("data = %s\n",data);

        if(0 == strncmp(data,SL374_DATA_IS_NULL,2) )
        {
				switch(user_type)
				{
						case SL374_ADMIN_USER:
						{
								strcpy(response,ADMIN_USER_IS_NULL);
						}
						break;

						case SL374_NORMAL_USER:
						{
								strcpy(response,NORMAL_USER_IS_NULL);
						}
						break;

						case SL374_TEMP_USER:
						{
			                    strcpy(response,TEMP_USER_IS_NULL);
						}
						break;

						default:
						break;
				}
        }
        else
        {

				switch(user_type)
				{
						case 0:
						{
								strcpy(response,"0805");
								memset(result,0,64);
								strncpy(result,data_buf+2,2);
								strncat(response,result,2); 							   //当前用户总数
		
								memset(result,0,64);
								strncpy(result,data_buf+4,2);
								strncat(response,result,2); 							  //当前用户顺序
		
								memset(result,0,64);
								strncpy(result,data_buf+6,4);
								strncat(response,result,4); 							  //当前用户ID
		
								memset(result,0,64);
								strncpy(result,data_buf+10,10);
								strncat(response,result,10);							   //用户名称
		
								memset(result,0,64);
								strncpy(result,data_buf+20,2);
								strncat(response,result+1,1);								//密码个数
		
								memset(result,0,64);
								strncpy(result,data_buf+22,2);
								strncat(response,result+1,1);								//指纹个数
		
								memset(result,0,64);
								strncpy(result,data_buf+24,2);
								strncat(response,result+1,1);								//卡个数

						}
						break;

						
						case 1:
						{
								strcpy(response,"0806");
								
								memset(result,0,64);
								strncpy(result,data_buf+2,2);
								strncat(response,result,2); 							   //当前用户总数
								
								memset(result,0,64);
								strncpy(result,data_buf+4,2);
								strncat(response,result,2); 							  //当前用户顺序
								
								memset(result,0,64);
								strncpy(result,data_buf+6,4);
								strncat(response,result,4); 							  //当前用户ID
								
								memset(result,0,64);
								strncpy(result,data_buf+10,10);
								strncat(response,result,10);							   //用户名称
								
								memset(result,0,64);
								strncpy(result,data_buf+20,2);
								strncat(response,result+1,1);								//密码个数
								
								memset(result,0,64);
								strncpy(result,data_buf+22,2);
								strncat(response,result+1,1);								//指纹个数
								
								memset(result,0,64);
								strncpy(result,data_buf+24,2);
								strncat(response,result+1,1);								//卡个数

						}
						break;

						case 2:
						{
								strcpy(response,"0807");
								
								memset(result,0,64);
								strncpy(result,data_buf+2,2);
								strncat(response,result,2); 							   //当前用户总数
								
								memset(result,0,64);
								strncpy(result,data_buf+4,2);
								strncat(response,result,2); 							  //当前用户顺序
								
								memset(result,0,64);
								strncpy(result,data_buf+6,4);
								strncat(response,result,4); 							  //当前用户ID
								
								memset(result,0,64);
								strncpy(result,data_buf,2);
								strncat(response,result,2); 							   //用户类型
								
								memset(result,0,64);
								strncpy(result,data_buf+12,2);
								strncpy(codelen,data_buf+12,2);
								strncat(response,result,2); 							  //密码长度
								
								memset(result,0,64);
								codelength = strtol(codelen,NULL,10);
								strncpy(result,data_buf+14,codelength * 2);
								strncat(response,result,codelength * 2);		//密码内容
								
								memset(result,0,64);
								strncpy(result,data_buf+14+codelength*2,10);	  //密码起始时间
								strncat(response,result,10); 
								
								memset(result,0,64);
								strncpy(result,data_buf+24+codelength*2,10);	  //密码结束时间
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
 函 数 名  : HSZSelfProtocol374LockReturnForLockVersionInfo
 功能描述  : 解析查询锁具信息成功结果上报
*****************************************************************************/
int HSZSelfProtocol374LockReturnForLockVersionInfo(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};

        strncpy(response,"080C",4);

        strncat(response,data_buf,16);     

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSelfProtocol374LockReturnDataForLocalAddUser
 功能描述  : 解析本地新增用户结果上报
*****************************************************************************/
int HSZSelfProtocol374LockReturnDataForLocalAddUser(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[30] = {0};
        char result[30] = {0};

        strncpy(response,"0801",4);
    
        strncpy(result,data_buf,16);
        strncat(response,result,16);
    
        memset(result,0,30);
        strncpy(result,data_buf+16,2);
        strncat(response,result+1,1);
    
        memset(result,0,30);
        strncpy(result,data_buf+18,2);
        strncat(response,result+1,1);
    
        memset(result,0,30);
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
 函 数 名  : HSZSelfProtocol374LockReturnDataForLocalModUser
 功能描述  : 解析本地修改用户结果上报
*****************************************************************************/
int HSZSelfProtocol374LockReturnDataForLocalModUser(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
		char response[30] = {0};
		char result[30] = {0};

		strncpy(response,"0803",4);  

		memset(result,0,30);
		strncpy(result,data_buf,16);
		strncat(response,result,16);

		memset(result,0,30);
		strncpy(result,data_buf+16,2);                                 
		strncat(response,result+1,1);

		memset(result,0,30);
		strncpy(result,data_buf+18,2);                                 
		strncat(response,result+1,1);

		memset(result,0,30);
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
 函 数 名  : HSZSelfProtocol374LockReturnDataForLocalDelUser
 功能描述  : 解析本地删除用户结果上报
*****************************************************************************/
int HSZSelfProtocol374LockReturnDataForLocalDelUser(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[30] = {0};
        char result[30] = {0};

        strncpy(response,"0802",4);  
 
        strncpy(result,data_buf,2);
        strncat(response,result,2);
        
        memset(result,0,30);
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
 函 数 名  : HSZSelfProtocol374LockReturnDataForDelUser
 功能描述  : 解析无线删除用户结果上报
*****************************************************************************/
int HSZSelfProtocol374LockReturnDataForDelUser(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[30] = {0};
        char result[30] = {0};

		strncpy(response,"080F",4);  
		strncpy(result,data_buf,6);
		strncat(response,result,6);

		printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/****************************************RSY 374****************************************/
/*****************************************************************************
 函 数 名  : HSZSelfProtocol374LockReturnDataForModNorTimeLimited
 功能描述  : 解析普通用户使用时限查询结果上报
*****************************************************************************/
int HSZSelfProtocol374LockReturnDataForQueryNorTimeLimited(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
        char response[30] = {0};
        char result[30] = {0};

		strncpy(response,"080D",4);
		
		strncpy(result,data_buf,4);
        strncat(response,result,4);

		memset(result,0,30);
        strncpy(result,data_buf+4,20);                                 
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
 函 数 名  : HSZSelfProtocol374LockReturnDataForModNorTimeLimited
 功能描述  : 解析普通用户使用时限修改结果上报
*****************************************************************************/
int HSZSelfProtocol374LockReturnDataForModNorTimeLimited(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
        char response[30] = {0};
        char result[30] = {0};

		strncpy(response,"0810",4);		
		strncpy(result,data_buf,4);
        strncat(response,result,4);

		memset(result,0,30);
        strncpy(result,data_buf+4,20);                                 
        strncat(response,result,20);

		printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/****************************************RSY 374****************************************/
/*****************************************************************************
 函 数 名  : HSZSendSmartLockData
 功能描述  : 封装智能门锁控制指令数据
*****************************************************************************/
int HSZSendSelfProtocol374LockData(char* dev_mac,char *send_buf)
{
      int rc = -1;
      int i = 0;
      int data_len = 0;
      char cmd_buf[4] = {0};
      int  cmd_type;
      char data_buf[128]={0};
      char msg_buf[128]={0};
        
      memset(lock_send_msg_buf, 0, 512);
      memcpy(data_buf, send_buf + 4, strlen(send_buf)-3);        //11acdddddddd
      strncpy(cmd_buf,data_buf,2);
      cmd_type = strtol(cmd_buf,NULL,16);
      printf("gw_mac:%s,device_mac:%s,cmd:%s\n",HSZGatewayMac(),dev_mac,data_buf);
      printf("cmd_buf:%02x.\n",cmd_type);
      strcpy(msg_buf,data_buf+2);                                 //acdddddddd
      printf("msg_buf:%s.\n",msg_buf);
      switch(cmd_type)
      {
				case APP_374_REMOTR_OPENDOOR_CMD:                     
				{
				    	data_len = HSZSelfProtocol374LockCmdOpenDoor(msg_buf);
				}
				break;

				case APP_374_QUERY_SMARTLOCK_TIME:              
				{
				    	data_len = HSZSelfProtocol374LockCmdQuerryTime(msg_buf);
				}
				break;

				case SMARTLOCK_374_TIME_SYNC_CMD:              
				{
				    	data_len = HSZSelfProtocol374LockCmdSynchronizeTime(msg_buf);
				}
				break;

				case SMARTLOCK_374_PASSWD_AUTHEN_CMD:                        
				{
				    	data_len = HSZSelfProtocol374LockCmdPasswdCheck(msg_buf);
				}
				break;

				case SMARTLOCK_374_ADD_NEWTEMUSER_CMD:                        		 /*添加新用户*/
				{
				    	data_len = HSZSelfProtocol374LockCmdAddUser(msg_buf);
				}
				break;

				case APP_374_DELETE_TEMUSER_CMD:                         			 /*删除用户*/
				{
				    	data_len = HSZSelfProtocol374DeleteUser(msg_buf);
				}
				break;

				case SMARTLOCK_374_QUERY_TEMUSER_INFO_CMD:                         	 /*查询临时用户信息*/
				{
				    	data_len =  HSZSelfProtocol374LockQueryUser(TEM_USER);
				}
				break;

				case SMARTLOCK_374_QUERY_NORMALUSER_INFO_CMD:                        /*查询普通用户信息*/
				{
				    	data_len =  HSZSelfProtocol374LockQueryUser(NORMAL_USER);
				}
				break;

				case SMARTLOCK_374_QUERY_ADMINUSER_INFO_CMD:                         /*查询管理员用户信息*/
				{
				    	data_len =  HSZSelfProtocol374LockQueryUser(ADMIN_USER);
				}
				break;

				case SMARTLOCK_374_QUERY_OPENDOOR_HISTORY_CMD:                       /*查询开锁历史记录*/
				{
				    	data_len =  HSZSelfProtocol374LockOpenDoorHistory();

				}
				break;

				case SMARTLOCK_374_QUERY_LOCK_INFO_CMD:                         	 /*查询锁具信息*/
				{
				    	data_len =  HSZSelfProtocol374LockQueryLockInfo(msg_buf);
				}
				break;

				case SMARTLOCK_374_RESET:                         						 //reset
				{
				    	data_len =  HSZSelfProtocol374LockReset();
				}
				break;

				case SMARTLOCK_374_OPRN_HISTORY:                        
				{
				    	data_len =  HSZSelfProtocol374LockHistory(msg_buf);
				}
				break;

				/*********************************RSY 374************************************************/
				case SMARTLOCK_374_QUERY_NORMALUSER_TIMELIMITED:                        
				{
				    	data_len =  HSZSelfProtocol374QueryNormalTimeLimited(msg_buf);
				}
				break;

				case SMARTLOCK_374_MOD_NORMALUSER_TIMELIMITED:                        
				{
				    	data_len =  HSZSelfProtocol374ModNormalTimeLimited(msg_buf);
				}
				break;
				/*********************************RSY 374************************************************/
				
				default:
				break;
      }
      printf("data_len:%d\n",data_len);
      printf("lock_send_msg_buf: ");
	  
      for(i=0;i<data_len;i++)
      {
               printf("%02X ",lock_send_msg_buf[i]);
      }
      printf("\n");  

      char controlCode[512] = {0};
      HSZConvertUnCharToStr(controlCode,lock_send_msg_buf,data_len);
      printf("controlCode:%s\n",controlCode);
      CommandSendDataToSmartLock374(HSZGatewayMac(),dev_mac,controlCode);
      return rc;
}


/*****************************************************************************
 函 数 名  : HSZSendSelfProtocolSL500LockData
 功能描述  : 封装智能门锁控制指令数据的上报
*****************************************************************************/
/***********************test return*************************************
int HSZReturnSelfProtocol374LockData(char* dev_mac,char *send_buf)							//传入的send_buf要带有CMD2命令:310100040201
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
				case LOCK374_QUERY_NORUSER_TIMELIMITED:
				{
						data_len = HSZSelfProtocol374LockReturnDataForQueryNorTimeLimited(data_buf,AppGatewayMac(),dev_mac); 
				}
				break;

				case LOCK374_MOD_NORUSER_TIMELIMITED:
				{
						data_len = HSZSelfProtocol374LockReturnDataForModNorTimeLimited(data_buf,AppGatewayMac(),dev_mac); 
				}
				break;

				default:
				break;
		}

}
*/

