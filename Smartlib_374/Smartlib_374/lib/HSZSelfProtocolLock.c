
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
#include "HSZSelfProtocolLock.h"
#include "HSZSmartLockMessage.h"


#define DEBUG_ON

#ifdef DEBUG_ON
        #define HIS_DEBUG(format,...) printf(format,__VA_ARGS__)
#else
        #define HIS_DEBUG(format,...)
#endif


unsigned char lock_send_msg_buf[512] = {0};



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
int my_tolower(int c)  
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

void my_toUppercase(char *src,int len)
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


int my_htoi(char s[])
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
        if (my_tolower(s[i]) > '9')
        {
            n = 16 * n + (10 + my_tolower(s[i]) - 'a');
        }
        else
        {
            n = 16 * n + (my_tolower(s[i]) - '0');
        }
    }

    return n;

}

/*****************************************************************************
 函 数 名  : HSZSendSelfProtocolLockData
 功能描述  : 封装智能门锁控制指令数据
*****************************************************************************/


int HSZSendSelfProtocolLockData(char* dev_mac,char *send_buf)
{
        int rc = -1;
        int i = 0;
        int data_len = 0;
        char cmd_buf[4] = {0};
        int  cmd_type;

        char data_buf[128]={0};
        char msg_buf[128]={0};
        
        memset(lock_send_msg_buf, 0, 512);

        //if(strncmp(command_buf,SELF_PROTOCOL_SMARTLOCK_COMMAND,4)==0)
        {
                memcpy(data_buf, send_buf + 4, strlen(send_buf)-3);        //11acdddddddd
                        
                strncpy(cmd_buf,data_buf,2);

                cmd_type = strtol(cmd_buf,NULL,16);

                printf("gw_mac:%s,device_mac:%s,cmd:%s\n",HSZGatewayMac(),dev_mac,data_buf);

                printf("cmd_buf:%02x.\n",cmd_type);

                strcpy(msg_buf,data_buf+2);                                               //acdddddddd

                printf("msg_buf:%s.\n",msg_buf);
                        
                switch(cmd_type)
                {
                        case APP_REMOTR_OPEN_DOOR_COMMAND:                     
                        {
                                data_len = HSZSelfProtocolLockCmdOpenDoor(msg_buf);
                        }
                        break;
                        
                        case APP_QUERY_SMARTLOCK_TIME:              
                        {
                                data_len = HSZSelfProtocolLockCmdQuerryTime(msg_buf);
                        }
                        break;
                        
                        case SMARTLOCK_TIME_SYNCHRONIZATION_COMMAND:              
                        {
                                data_len = HSZSelfProtocolLockCmdSynchronizeTime(msg_buf);
                        }
                        break;
                        
                        case SMARTLOCK_PASSWD_AUTHEN_COMMAND:                        
                        {
                                data_len = HSZSelfProtocolLockCmdPasswdCheck(msg_buf);
                        }
                        break;

                        case SMARTLOCK_ADD_NEW_TEMUSER_COMMAND:                         /*添加新用户*/
                        {
                                data_len = HSZSelfProtocolLockCmdAddUser(msg_buf);
                        }
                        break;

                        case APP_DELETE_TEMUSER_COMMAND:                         /*删除用户*/
                        {
                                data_len = HSZSelfProtocolDeleteUser(msg_buf);
                        }
                        break;

                        case SMARTLOCK_QUERY_TEMUSER_INFO_COMMAND:                         /*查询临时用户信息*/
                        {
                                data_len =  HSZSelfProtocolLockQueryUser(TEM_USER);
                        }
                        break;

                        case SMARTLOCK_QUERY_NORMALUSER_INFO_COMMAND:                         /*查询普通用户信息*/
                        {
                                data_len =  HSZSelfProtocolLockQueryUser(NORMAL_USER);
                        }
                        break;

                        case SMARTLOCK_QUERY_ADMINUSER_INFO_COMMAND:                         /*查询管理员用户信息*/
                        {
                                data_len =  HSZSelfProtocolLockQueryUser(ADMIN_USER);
                        }
                        break;

                        case SMARTLOCK_QUERY_OPENDOOR_HISTORY_COMMAND:                         /*查询开锁历史记录*/
                        {
                                data_len =  HSZSelfProtocolLockOpenDoorHistory();
                         
                        }
                        break;

                        case SMARTLOCK_QUERY_LOCK_INFO_COMMAND:                         /*查询锁具信息*/
                        {
                                data_len =  HSZSelfProtocolLockQueryLockInfo(msg_buf);
                        }
                        break;

                        case SMARTLOCK_RESET:                         //reset
                        {
                                data_len =  HSZSelfProtocolLockReset();
                        }
                        break;

			   			case SMARTLOCK_OPRN_HISTORY:                        
                        {
                                data_len =  HSZSelfProtocolLockHistory(msg_buf);
                        }
                        break;
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
                
                //CommandSendDataToSmartLock(HSZGatewayMac(),dev_mac,controlCode);
                CommandSendDataToSmartLockSL920(HSZGatewayMac(),dev_mac,controlCode);
        }
        
        return rc;
}

//检查校验位（即传输指令长度）
/*****************************************************************************
 函 数 名  : HSZCalculateSelfProtocolLockDataCheckSum
 功能描述  : 计算传入数据的Check fcs数值
*****************************************************************************/
int HSZCalculateSelfProtocolLockDataCheckSum(int sof,int len,unsigned char *data_buf)
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
 函 数 名  : HSZAnalysisSelfProtocolLockData
 功能描述  : 解析智能门锁私有数据
*****************************************************************************/
int HSZAnalysisSelfProtocolLockData(unsigned char *data,char* gw_mac,char* dev_mac)
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
          strncpy(response,SL920_QUERY_TIME,4);
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

		 printf("RSY msg_buffer is %s \n",msg_buffer);
     
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
                                printf("868Error info report...\n");
                                HSZSelfProtocolLockReturnDataForErrorCode(msg_buffer,gw_mac,dev_mac);
                        }
                        break;
                        
                        case CMD2_OPEN_DOOR_INFO_RESPONSE:             
                        {
                                printf("Unlock info report...\n");
                                HSZSelfProtocolLockReturnForUnlockInfo(msg_buffer,gw_mac,dev_mac);
                        }
                        break;
                        
                        case CMD2_OPEN_DOOR_HISTORY_RESPONSE:        
                        {
                                printf("Unlock history report...\n");
                                HSZSelfProtocolLockReturnForHistory(msg_buffer,gw_mac,dev_mac);
                        }
                        break;
                        
                        case CMD2_DOOR_TIME_RESPONSE:                   
                        {
                                printf("Lock local time report...\n");
                                HSZSelfProtocolLockReturnForSyncTime(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_DOOR_REQUEST_TIME_SYNC:           
                        {
                                printf("lock requir time...\n");
                                HSZSelfProtocolLockReturnForQueryTime(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_ADMIN_PASSWD_AUTH_OK:                
                        {
                                printf("Admin passwd auth succeed...\n");
                                HSZSelfProtocolLockReturnAdminPasswdAuthSuccess(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_DOOR_EVENT_REPORT:                    
                        {
                                printf("Lock event report...\n");
                                HSZReturnDataForLockEvent(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_REMOTE_ADD_USER_OK_REPORT:     
                        {
                                printf("Remote add user succeed result report...\n");
                                HSZSelfProtocolLockReturnDataForAddTemUser(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_RREMOTE_DEL_USER_OK_REPORT:  
                        {
                                printf("Remote del user succeed result report...\n");
                                HSZSelfProtocolLockReturnDataForDelUser(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_USER_QUERY_RESULT_REPORT:        
                        {
                                printf("Query user result report...\n");
                                HSZSelfProtocolLockReturnDataForUserInfo(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_DOOR_INFO_REPORT:                      
                        {
                               HSZSelfProtocolLockReturnForLockVersionInfo(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_LOCAL_NEW_USER_REPORT:              
                        {
                                printf("Local add user result report...\n");
                                HSZSelfProtocolLockReturnDataForLocalAddUser(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_LOCAL_DEL_USER_REPORT:              
                        {
                               printf("Local del user result report...\n");
                               HSZSelfProtocolLockReturnDataForLocalDelUser(msg_buffer,gw_mac,dev_mac);
                        }
                        break;

                        case CMD2_LOCAL_MOD_USER_REPORT:            
                        {
                               printf("Local del user result report...\n");
                               HSZSelfProtocolLockReturnDataForLocalModUser(msg_buffer,gw_mac,dev_mac);
                        }
                        break;
                        
                        default:
                        break;  
                 }
         }   
 
         return 0;
}          


int HSZSelfProtocolDeleteUser(char *data_buf)
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
                     

int HSZSelfProtocolLockCmdAddUser(char *data_buf)
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

int HSZSelfProtocolLockCmdOpenDoor(char *data_buf)
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


int HSZSelfProtocolLockCmdQuerryTime(char *data_buf)
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


int HSZSelfProtocolLockCmdSynchronizeTime(char *data_buf)
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
 函 数 名  : HSZSelfProtocolLockCmdPasswdCheck
 功能描述  : 管理员密码认证
*****************************************************************************/

int HSZSelfProtocolLockCmdPasswdCheck(char *data_buf)
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


int HSZSelfProtocolLockQueryUser(int user_type)
{
        int sum = 0x00;
        int index = 0;
        
        memset(lock_send_msg_buf,0x00,512);
                
        lock_send_msg_buf[0] = 0xFA;                                                                
                        
        lock_send_msg_buf[1] = 0x04;                                                                
                        
        lock_send_msg_buf[2] = 0x13;                                                                  
                        
        lock_send_msg_buf[3] = 0x07;                                                                

        lock_send_msg_buf[4] = 0x01;                                                                

        if (user_type == ADMIN_USER)
        {
               lock_send_msg_buf[5] = SMARTLOCK_ADMIN_USER;   
        }
        else if (user_type == NORMAL_USER)
        {
               lock_send_msg_buf[5] = SMARTLOCK_NORMAL_USER;
        }
        else if (user_type == TEM_USER)
        {
               lock_send_msg_buf[5] = SMARTLOCK_TEP_USER;
        }
                          
       for(index = 0;index<6;index++)         
        {
                sum = (sum + lock_send_msg_buf[index])&0xFF;
        }
        
        lock_send_msg_buf[6] = sum;

        return 7;
}


int HSZSelfProtocolLockQueryLockInfo(char *data_buf)
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


int HSZSelfProtocolLockOpenDoorHistory(void)
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

int HSZSelfProtocolLockReset(void)
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

int HSZSelfProtocolLockHistory(char *data_buf)
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

int HSZSelfProtocolLockReturnDataForControl(char *data_buf,char* gw_mac,char* dev_mac)
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


int HSZSelfProtocolLockReturnDataForErrorCode(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[30] = {0};

        if(strncmp(SYNC_TIME_FAILED_ERRORCODE,data_buf,4)==0)				//校时失败
        {
                strncpy(response,LOCK_RESERT,4);
        }
        else if(strncmp(PASSWD_ERROR_ERRORCODE,data_buf,4)==0)
        {
                strncpy(response,OPENDOOR_PASSWD_ERROR,4);
        }
        else if(strncmp(ADMIN_PASSWD_AUTH_FAILED_ERRORCODE,data_buf,4)==0)
        {
                strncpy(response,ADMIN_PASSWD_AUTH_FAILED,4);
        }
        else if(strncmp(REMOTE_OPENDOOR_PASSWD_ERROR_ERRORCODE,data_buf,4)==0)
        {
                strncpy(response,OPENDOOR_PASSWD_ERROR,4);
        }
        else if(strncmp(OPENDOOR_FAILED_ERRORCODE,data_buf,4)==0)
        {
                strncpy(response,OPENDOOR_FAILED,4);
        }
        else if(strncmp(REMOTE_OPENDOOR_FORBID_ERRORCODE,data_buf,4)==0)
        {
                strncpy(response,REMOTE_OPENDOOR_FORBID,4);
        }
        else if(strncmp(ADD_PASSWD_DUPLICATE_ERRORCODE,data_buf,4)==0)
        {
                strncpy(response,ADD_PASSWD_DUPLICATE,4);
        }
        else if(strncmp(ADD_USER_FAILED_ERRORCODE,data_buf,4)==0)
        {
                strncpy(response,ADD_USER_FAILED,4);
        }
        else if(strncmp(DEL_USER_FAILED_ERRORCODE,data_buf,4)==0)
        {
                strncpy(response,DEL_USER_FAILED,4);
        }
        /*else if(22 == strlen(data_buf))
        {
                if(strncmp(SMARTLOCK_OPEN_DOOR,data_buf+2,6)==0)
                {
                        strncpy(open_id,data_buf+14,2);
                        strncpy(open_type,data_buf+16,2);
                        strncpy(user_type,data_buf+20,2);
                        strncpy(response,LOCK_OPEN_DOOR,4);
                        strcat(response,user_type);
                        strcat(response,open_id);
                        strcat(response,open_type);
                }
                else
                {
                        return rc;
                }
        }*/
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


int HSZReturnDataForLockEvent(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[30] = {0};

        printf("data_buf = %s\n",data_buf);

        my_toUppercase(data_buf,strlen(data_buf));

        printf("data_buf = %s\n",data_buf);

        if(strcmp(SMARTLOCK_LOCK_CLOSED,data_buf)==0)
        {
               printf("lock closed...\n");
               strncpy(response,LOCAL_REPORT_LOCK,4);
        }
        else if(strcmp(SMARTLOCK_PASSWD_VALIDATION_ERROR_3TIMES,data_buf)==0)
        {
                strncpy(response,PASSWD_VALIDATION_ERROR_3TIMES,4);
        }
        else if(strcmp(SMARTLOCK_FINGERPRINT_VALIDATION_ERROR_3TIMES,data_buf)==0)
        {
                strncpy(response,FINGERPRINT_VALIDATION_ERROR_3TIMES,4);
        }
        else if(strcmp(SMARTLOCK_CARD_VALIDATION_ERROR_3TIMES,data_buf)==0)
        {
                strncpy(response,CARD_VALIDATION_ERROR_3TIMES,4);
        }
        else if(strcmp(SMARTLOCK_DEMOLITION_ALARM,data_buf)==0)
        {
                strncpy(response,DEMOLITION_ALARM,4);
        }
        else if(strcmp(SMARTLOCK_LOW_BATTERY_ALARM,data_buf)==0)
        {
                strncpy(response,LOW_BATTERY_ALARM,4);
        }
        else if(strcmp(SMARTLOCK_DOUBLE_LOCK1,data_buf)==0)
        {
                strncpy(response,DOUBLE_LOCK,4);
        }
        else if(strcmp(SMARTLOCK_RELEASE_LOCK,data_buf)==0)
        {
                strncpy(response,RELEASE_LOCK,4);
        }
        else if(strcmp(SMARTLOCK_LOCAL_SETTING_REMOTE_UNLOK_PERMIT,data_buf)==0)
        {
                strncpy(response,LOCAL_SETTING_REMOTE_UNLOK_PERMIT,4);
        }
        else if(strcmp(SMARTLOCK_LOCAL_SETTING_REMOTE_UNLOK_FORBID,data_buf)==0)
        {
                strncpy(response,LOCAL_SETTING_REMOTE_UNLOK_FORBID,4);
        }
        else if(strcmp(SMARTLOCK_FACTORY_RESET,data_buf)==0)
        {
                strncpy(response,FACTORY_RESET,4);
        }
        else if(strcmp(SMARTLOCK_RELEASE_SYSTEM_LOCK_IN,data_buf)==0)
        {
                strncpy(response,RELEASE_SYSTEM_LOCK_IN,4);
        }
        else if(strcmp(SMARTLOCK_UNLATCHED_DOOR,data_buf)==0)
        {
                strncpy(response,UNLATCHED_DOOR,4);
        }
        else if(strcmp(SMARTLOCK_DOORBELL,data_buf)==0)
        {
                printf("door bell ring...");
                strncpy(response,DOORBELL,4);
        }
	 else if(strcmp(SMARTLOCK_OUTMODE,data_buf)==0)
        {
                strncpy(response,OUTMODE_DOOR,4);
        }
        else
        {
                return rc;
        }

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        printf("response = %s\n",response);
        return rc;
}


int HSZSelfProtocolLockReturnSyncTimeFail(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};

        //if(strcmp(data_buf,)==0)
        {
                strncpy(response,SYNC_TIME_FAIL,4);
        }      
        //else
        //{
                //return rc;
        //}

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}


int HSZSelfProtocolLockReturnForUnlockInfo(char *data_buf,char* gw_mac,char* dev_mac)
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


int HSZSelfProtocolLockReturnForHistory(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};
        char code[64] = {0};
        int codelength = 0;

        strncpy(response,"0809",4);

        strncpy(code,data_buf,4);                           //历史记录编号
        codelength = strtol(code,NULL,16);
        strncat(response,code,4); 

        strncat(response,data_buf+4,2);            //客户属性

        memset(code,0,64);
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

        return rc;
}


int HSZSelfProtocolLockReturnForSyncTime(char *data_buf,char* gw_mac,char* dev_mac)
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


int HSZSelfProtocolLockReturnForQueryTime(char *data_buf,char* gw_mac,char* dev_mac)
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

int HSZSelfProtocolLockReturnAdminPasswdAuthSuccess(char *data_buf,char* gw_mac,char* dev_mac)
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


int HSZSelfProtocolLockReturnDataForAddTemUser(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[30] = {0};
        char result[128] = {0};
        int codelength = 0;


        /*if(strlen(data_buf)==4)
        {
                if(strncmp(SMARTLOCK_ADD_TEMCODE_FAIL,data_buf+2,2)==0)
                {
                        strncpy(response,ADD_TEMCODE_FAIL,4);
                }
                else if(strncmp(SMARTLOCK_ADD_TEMCODE_REPEAT,data_buf+2,2)==0)
                {
                        strncpy(response,ADD_TEMCODE_REPEAT,4);
                }
                else
                {
                        return rc;
                }
        }
        else*/
        {
                strncat(response,"080E",4);  
                strncat(response,"02",2);                                     //临时用户

                strncpy(result,data_buf+2,4);
                //codelength = strtol(codelen,NULL,16);
                strncat(response,result,4);                                //用户ID

                memset(result,0,30);
                strncpy(result,data_buf+8,2);
                //codelength = strtol(codelen,NULL,16);
                strncat(response,result,2);                               //密码长度

                codelength = my_htoi(result);                          //密码长度转为整型

                memset(result,0,30);
                strncpy(result,data_buf+10,codelength*2);
                //codelength = strtol(codelen,NULL,16);
                strncat(response,result,codelength*2);              //密码内容

                memset(result,0,30);
                strncpy(result,data_buf+10+codelength*2,20);      //密码起始截止时间
                strncat(response,result,20);  

               /*time =  my_htoi(result);    
                Hal_Time_SecondToDate(time,TimeYear,TimeMonth,TimeDate,TimeHour,TimeMinute,TimeSecond);
                strcat(response,TimeYear);
                strcat(response,TimeMonth);
                strcat(response,TimeDate);
                strcat(response,TimeHour);
                strcat(response,TimeMinute);
                strcat(response,TimeSecond);*/

                //memset(result,0,30);
                //strncpy(result,data_buf+20+codelength*2,10);      //密码结束时间
                //strncat(response,result,10);  

                /*time =  my_htoi(result);    
                Hal_Time_SecondToDate(time,TimeYear,TimeMonth,TimeDate,TimeHour,TimeMinute,TimeSecond);
                strcat(response,TimeYear);
                strcat(response,TimeMonth);
                strcat(response,TimeDate);
                strcat(response,TimeHour);
                strcat(response,TimeMinute);
                strcat(response,TimeSecond);*/
        }

        printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

int HSZSelfProtocolLockReturnDataForUserInfo(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[64] = {0};
        char result[64] = {0};
        int codelength = 0;
        char user_type[8] = {0};
        char data[8] = {0};
        char codelen[8] = {0};

        strncpy(user_type,data_buf,2);
        strncpy(data,data_buf+2,2);
        printf("data = %s\n",data);

        if (0 == strncmp(data,SL920_DATA_IS_NULL,2) )
        {
                 if(0 == strncmp(user_type,SL920_ADMIN_USER,2))
                 {
                        strcpy(response,ADMIN_USER_IS_NULL);
                 }
                 else if(0 == strncmp(user_type,SL920_NORMAL_USER,2))
                 {
                        strcpy(response,NORMAL_USER_IS_NULL);
                 }
                 else if(0 == strncmp(user_type,SL920_TEMP_USER,2))
                 {
                        strcpy(response,TEMP_USER_IS_NULL);
                 }
        }
        else
        {
                 if(0 == strncmp(user_type,QUERY_ADMIN_USER_TYPE,2))
                 {
                        strcpy(response,"0805");
                        memset(result,0,64);
                        strncpy(result,data_buf+2,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result,2);                                //当前用户总数

                        memset(result,0,64);
                        strncpy(result,data_buf+4,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result,2);                               //当前用户顺序

                        memset(result,0,64);
                        strncpy(result,data_buf+6,4);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result,4);                               //当前用户ID

                        memset(result,0,64);
                        strncpy(result,data_buf+10,10);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result,10);                               //用户名称

                        memset(result,0,64);
                        strncpy(result,data_buf+20,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result+1,1);                               //密码个数

                        memset(result,0,64);
                        strncpy(result,data_buf+22,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result+1,1);                               //指纹个数

                        memset(result,0,64);
                        strncpy(result,data_buf+24,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result+1,1);                               //卡个数

                 }
                 else if(0 == strncmp(user_type,QUERY_NORMAL_USER_TYPE,2))
                 {
                        strcpy(response,"0806");

                        memset(result,0,64);
                        strncpy(result,data_buf+2,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result,2);                                //当前用户总数

                        memset(result,0,64);
                        strncpy(result,data_buf+4,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result,2);                               //当前用户顺序

                        memset(result,0,64);
                        strncpy(result,data_buf+6,4);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result,4);                               //当前用户ID

                        memset(result,0,64);
                        strncpy(result,data_buf+10,10);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result,10);                               //用户名称

                        memset(result,0,64);
                        strncpy(result,data_buf+20,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result+1,1);                               //密码个数

                        memset(result,0,64);
                        strncpy(result,data_buf+22,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result+1,1);                               //指纹个数

                        memset(result,0,64);
                        strncpy(result,data_buf+24,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result+1,1);                               //卡个数
                 }
                 else if(0 == strncmp(user_type,QUERY_TEMP_USER_TYPE,2))
                 {
                        strcpy(response,"0807");

                        memset(result,0,64);
                        strncpy(result,data_buf+2,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result,2);                                //当前用户总数

                        memset(result,0,64);
                        strncpy(result,data_buf+4,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result,2);                               //当前用户顺序

                        memset(result,0,64);
                        strncpy(result,data_buf+6,4);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result,4);                               //当前用户ID

                        memset(result,0,64);
                        strncpy(result,data_buf,2);
                        //codelength = strtol(codelen,NULL,16);
                        strncat(response,result,2);                                //用户类型

                        memset(result,0,64);
                        strncpy(result,data_buf+12,2);
                        strncpy(codelen,data_buf+12,2);
                        strncat(response,result,2);                               //密码长度

                        memset(result,0,64);
                        codelength = strtol(codelen,NULL,10);
                        strncpy(result,data_buf+14,codelength * 2);
                        strncat(response,result,codelength * 2);        //密码内容

                        memset(result,0,64);
                        strncpy(result,data_buf+14+codelength*2,10);      //密码起始时间
                        strncat(response,result,10); 

                        /*time = my_htoi(result);    
                        Hal_Time_SecondToDate(time,TimeYear,TimeMonth,TimeDate,TimeHour,TimeMinute,TimeSecond);
                        strcat(response,TimeYear);
                        strcat(response,TimeMonth);
                        strcat(response,TimeDate);
                        strcat(response,TimeHour);
                        strcat(response,TimeMinute);
                        strcat(response,TimeSecond);*/

                        memset(result,0,64);
                        strncpy(result,data_buf+24+codelength*2,10);      //密码结束时间
                        strncat(response,result,10); 

                        /*time = my_htoi(result);    
                        Hal_Time_SecondToDate(time,TimeYear,TimeMonth,TimeDate,TimeHour,TimeMinute,TimeSecond);
                        strcat(response,TimeYear);
                        strcat(response,TimeMonth);
                        strcat(response,TimeDate);
                        strcat(response,TimeHour);
                        strcat(response,TimeMinute);
                        strcat(response,TimeSecond);*/
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


int HSZSelfProtocolLockReturnForLockVersionInfo(char *data_buf,char* gw_mac,char* dev_mac)
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


int HSZSelfProtocolLockReturnDataForLocalAddUser(char *data_buf,char* gw_mac,char* dev_mac)
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

//解析反馈本地修改用户
int HSZSelfProtocolLockReturnDataForLocalModUser(char *data_buf,char* gw_mac,char* dev_mac)
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


//解析反馈本地删除用户
int HSZSelfProtocolLockReturnDataForLocalDelUser(char *data_buf,char* gw_mac,char* dev_mac)
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

//解析反馈删除用户
int HSZSelfProtocolLockReturnDataForDelUser(char *data_buf,char* gw_mac,char* dev_mac)
{
        int rc = -1;
        char response[30] = {0};
        char result[30] = {0};

       strncpy(response,"080F",4);  

       strncpy(result,data_buf,6);
       strncat(response,result,6);
       
       //memset(result,0,30);
       //strncpy(result,data_buf+2,4);                                 
       //strncat(response,result,4);

       printf("response = %s\n",response);

        if (hsbk_devicedata!=NULL)
        {
                rc = 0x00;
                (*hsbk_devicedata)(gw_mac,dev_mac,response);
        }

        return rc;
}

/*****************************************************************************
 函 数 名  : HSZSendSmartLockData
 功能描述  : 封装智能门锁控制指令数据
*****************************************************************************/
int HSZSendSelfProtocolSL868LockData(char* dev_mac,char *send_buf)
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
            case APP_REMOTR_OPEN_DOOR_COMMAND:                     
                        {
                                data_len = HSZSelfProtocolLockCmdOpenDoor(msg_buf);
                        }
                        break;
                        
                        case APP_QUERY_SMARTLOCK_TIME:              
                        {
                                data_len = HSZSelfProtocolLockCmdQuerryTime(msg_buf);
                        }
                        break;
                        
                        case SMARTLOCK_TIME_SYNCHRONIZATION_COMMAND:              
                        {
                                data_len = HSZSelfProtocolLockCmdSynchronizeTime(msg_buf);
                        }
                        break;
                        
                        case SMARTLOCK_PASSWD_AUTHEN_COMMAND:                        
                        {
                                data_len = HSZSelfProtocolLockCmdPasswdCheck(msg_buf);
                        }
                        break;

                        case SMARTLOCK_ADD_NEW_TEMUSER_COMMAND:                        		 /*添加新用户*/
                        {
                                data_len = HSZSelfProtocolLockCmdAddUser(msg_buf);
                        }
                        break;

                        case APP_DELETE_TEMUSER_COMMAND:                         			 /*删除用户*/
                        {
                                data_len = HSZSelfProtocolDeleteUser(msg_buf);
                        }
                        break;

                        case SMARTLOCK_QUERY_TEMUSER_INFO_COMMAND:                         	 /*查询临时用户信息*/
                        {
                                data_len =  HSZSelfProtocolLockQueryUser(TEM_USER);
                        }
                        break;

                        case SMARTLOCK_QUERY_NORMALUSER_INFO_COMMAND:                        /*查询普通用户信息*/
                        {
                                data_len =  HSZSelfProtocolLockQueryUser(NORMAL_USER);
                        }
                        break;

                        case SMARTLOCK_QUERY_ADMINUSER_INFO_COMMAND:                         /*查询管理员用户信息*/
                        {
                                data_len =  HSZSelfProtocolLockQueryUser(ADMIN_USER);
                        }
                        break;

                        case SMARTLOCK_QUERY_OPENDOOR_HISTORY_COMMAND:                       /*查询开锁历史记录*/
                        {
                                data_len =  HSZSelfProtocolLockOpenDoorHistory();
                         
                        }
                        break;

                        case SMARTLOCK_QUERY_LOCK_INFO_COMMAND:                         	 /*查询锁具信息*/
                        {
                                data_len =  HSZSelfProtocolLockQueryLockInfo(msg_buf);
                        }
                        break;

                        case SMARTLOCK_RESET:                         						 //reset
                        {
                                data_len =  HSZSelfProtocolLockReset();
                        }
                        break;

			   			case SMARTLOCK_OPRN_HISTORY:                        
                        {
                                data_len =  HSZSelfProtocolLockHistory(msg_buf);
                        }
                        break;
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
      CommandSendDataToSmartLockSL868(HSZGatewayMac(),dev_mac,controlCode);
      return rc;
}
