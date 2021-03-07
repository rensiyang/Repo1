/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZSmartLockMessage.c
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2017年12月13日
  最近修改   :
  功能描述   : 处理私有智能门锁协议的 Data数据
  函数列表   :
  修改历史   :
  1.日    期   : 2018年3月29日
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
#include "HSZSmartLockMessage.h"


unsigned char send_msg_buf[512] = {0};
int DAYS_TAB[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*****************************************************************************
 函 数 名  : HSZSendSmartLockData
 功能描述  : 封装智能门锁控制指令数据
 输入参数  : 
			int stx：
			int cmd：
			int len:
			char *data_buf:
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZSendSmartLockData(char* dev_mac,char *send_buf)
{
		int rc = -1;
		int i = 0;
		int data_len = 0;
		char command_buf[8] = {0};
		char cmd_buf[4] = {0};
		int  cmd_type;

		char data_buf[128]={0};
		char msg_buf[128]={0};
		
		memset(send_msg_buf, 0, 512);
		memset(command_buf, 0, 8);
		memcpy(command_buf, send_buf, 4);

		if(strncmp(command_buf,SMARTLOCK_COMMAND,4)==0)
		{
			memcpy(data_buf, send_buf+4, strlen(send_buf)-3);
				
			/*获取commmad cmd type value*/	
			strncpy(cmd_buf,data_buf,2);

			cmd_type = strtol(cmd_buf,NULL,16);
			DEBUG_ZONE(LOG_ZONE_TEST,"HSZSendSmartLockData.\n");

			DEBUG_ZONE(LOG_ZONE_MY,"gw_mac:%s,device_mac:%s,cmd:%s\n",HSZGatewayMac(),dev_mac,data_buf);

			DEBUG_ZONE(LOG_ZONE_TEST,"cmd_buf:%d.\n",cmd_type);

			/*获取send_msg*/	
			strcpy(msg_buf,data_buf+2);

			DEBUG_ZONE(LOG_ZONE_TEST,"msg_buf:%s.\n",msg_buf);
				
			switch(cmd_type)
			{
					case SMARTLOCK_TIME_SYNCHRONIZATION_COMMAND:  		/*时钟同步*/
					{
							data_len = HSZSmartLockTimeSynchronize(msg_buf);
					}
					break;
					
					case SMARTLOCK_PASSWD_AUTHEN_COMMAND:				/*管理员密码认证*/
					{
							data_len = HSZSmartLockPasswdCheck(msg_buf);
					}
					break;

					case SMARTLOCK_QUERY_TEMUSER_INFO_COMMAND:			/*查询锁具临时用户信息*/
					{
							data_len = HSZQueryTemUser();
					}
					break;

					case SMARTLOCK_ADD_NEW_TEMUSER_COMMAND:				/*添加新用户*/
					{
							data_len = HSZAddTemUser(msg_buf);
					}
					break;

					case APP_DELETE_TEMUSER_COMMAND:					/*APP删除用户*/	
					{
							data_len = HSZDeleteTemUser(msg_buf);
					}
					break;

					case APP_REMOTR_OPEN_DOOR_COMMAND:					/*APP开锁*/
					{
							data_len = HSZSmartLockRemoteOpenDoor(msg_buf);
					}
					break;

					case APP_QUERY_SMARTLOCK_TIME:						/*APP查询锁具时间*/
					{
							data_len = HSZQueryLockTime();
					}
					break;
					
					
					default:
					break;
			}

#if 1
			printf("data_len:%d\n",data_len);
			printf("send_msg_buf: ");
			for(i=0;i<data_len;i++)
			{
				printf("%02X ",send_msg_buf[i]);
			}
			printf("\n");	
#endif 
			char controlCode[512] = {0};
			HSZConvertUnCharToStr(controlCode,send_msg_buf,data_len);
			DEBUG_ZONE(LOG_ZONE_MY,"controlCode:%s\n",controlCode);
			CommandSendDataToSmartLock(HSZGatewayMac(),dev_mac,controlCode);
		}
		return rc;
}

/*****************************************************************************
 函 数 名  : HSZSmartLockTimeSynchronize
 功能描述  : 时钟同步
 输入参数  : 
			char *data_buf:下发数据
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZSmartLockTimeSynchronize(char *data_buf)
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
		
		memset(send_msg_buf,0x00,512);
			
		send_msg_buf[0] = 0xFA;									/*sof*/
				
		send_msg_buf[1] = 0x0B;									/*len*/
				
		send_msg_buf[2] = 0x14;									/*cmd1*/
				
		send_msg_buf[3] = 0x52;									/*cmd2*/

		send_msg_buf[4] = 0xFF;									/*src_high*/

		send_msg_buf[5] = 0xFF;									/*src_low*/

		send_msg_buf[6] = 0xFF;									/*dest_high*/

		send_msg_buf[7] = 0xFF;									/*dest_high*/

		send_msg_buf[8] = 0x20;									/*device_type*/

		send_msg_buf[9] = ((time&0xFF000000)>>24);				/*time_sync*/

		send_msg_buf[10] = ((time&0x00FF0000)>>16);				/*time_sync*/

		send_msg_buf[11] = ((time&0x0000FF00)>>8);				/*time_sync*/

		send_msg_buf[12] = (time&0x000000FF);					/*time_sync*/
			        		
		for(index = 0;index<13;index++)		
		{
			sum = (sum + send_msg_buf[index])&0xFF;
		}
		
		send_msg_buf[13] = sum;

		return 0x0E;
}

/*****************************************************************************
 函 数 名  : HSZAnalysisSmartLockData
 功能描述  :解析智能门锁私有数据
 输入参数  : 
			char *data:私有数据buf
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年3月29日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZAnalysisSmartLockData(unsigned char *data,char* gw_mac,char* dev_mac)
{
		 struct smartlock_protocol_info smartlock_info_data;
		 
		 int rc = -1;
		 int sum = -1; 

		 smartlock_info_data.sof = data[0];									/*sof*/
		 
		 smartlock_info_data.len = data[1];									/*len*/

		 smartlock_info_data.cmd1 = data[2];								/*cmd1*/

		 smartlock_info_data.cmd2 = data[3];								/*cmd2*/

		 memset(smartlock_info_data.data, 0, 1024);
		 memcpy(smartlock_info_data.data, &data[2], smartlock_info_data.len);
		 //strncpy(smartlock_info_data.data,&data[2],smartlock_info_data.len);	/*data*/
		 
		 sum = HSZCalculateSmartLockDataCheckSum(smartlock_info_data.sof,smartlock_info_data.len,smartlock_info_data.data);
		 
		 if(sum != data[smartlock_info_data.len + 0x02])	/*如果校验不正确，返回错误类型*/
		 {			
				HSZDeviceError(HSZ_ERR_SMARTLOCK_MSG_CHECKSUM,"Receive smartlock message checksum error.",NULL);
				
				DEBUG_ZONE(LOG_ZONE_MY,"Receive smartlock message checksum error.\n");
				
				return rc;
		 }

		 smartlock_info_data.src_low= data[5];

		 smartlock_info_data.src_high= data[4];

		 smartlock_info_data.dest_low= data[7];	

		 smartlock_info_data.dest_high= data[6];	

		 smartlock_info_data.deviceType = data[8];

		 if((RESERVED_BYTE!=smartlock_info_data.src_low)||(RESERVED_BYTE!=smartlock_info_data.src_high)
		 	||(RESERVED_BYTE!=smartlock_info_data.dest_low)||(RESERVED_BYTE!=smartlock_info_data.dest_high))
	     {
			DEBUG_ZONE(LOG_ZONE_MY,"Reserved byte error.\n");
			return rc;
		 }	 

		 memset(smartlock_info_data.msg, 0, 1024);
		 memcpy(smartlock_info_data.msg, &data[9], smartlock_info_data.len-7);
		 //strncpy(smartlock_info_data.msg,&data[9],smartlock_info_data.len-7);			/*除去devicetype后的msg*/
		 char msg_buffer[1024]={0};  
	     HSZConvertUnCharToStr(msg_buffer,smartlock_info_data.msg,smartlock_info_data.len-7);    
     
#if 1
		int i;
		for(i = 0;i < (smartlock_info_data.len-7);i++)											
		{
				printf("0x%02x ",smartlock_info_data.msg[i]&0xFF);
		}

		printf("\n");
		printf("%s\n", msg_buffer); 
#endif
	 
		 if(PROTOCOL_VERSION == smartlock_info_data.cmd1)
		 {
					 switch(smartlock_info_data.cmd2)
					 {
							 case REGULAR_DATA_REPORT:  				/*固定数据上报*/
							 {
									 if(SMARTLOCK_DEVICETYPE == smartlock_info_data.deviceType)
									 {
											DEBUG_ZONE(LOG_ZONE_MY,"HSZAnalysisSmartLockRegularData.\n");
										 	HSZAnalysisSmartLockRegularData(msg_buffer,gw_mac,dev_mac);
									 }								
							 }
							 break;

							 case TIME_DATA_REPORT:  					/*时钟同步*/
							 {								 	
									 if(SMARTLOCK_DEVICETYPE == smartlock_info_data.deviceType)
									 {
											DEBUG_ZONE(LOG_ZONE_MY,"HSZSmartLockTimeSync.\n");
										 	HSZSmartLockTimeSync(msg_buffer,gw_mac,dev_mac);
									 }
							 }
							 break;

							 case PRIVATE_DATA_REPORT:  				/*自定义数据上报*/
							 {
									 smartlock_info_data.clusterID_high = data[8];

									 smartlock_info_data.clusterID_low = data[9];

									 smartlock_info_data.attributeID_high = data[10];

									 smartlock_info_data.attributeID_low = data[11];

									 if((0xFD == smartlock_info_data.clusterID_high)&&(0x00 == smartlock_info_data.clusterID_low))
									 {
											smartlock_info_data.data_unit_len = data[12];

											memset(smartlock_info_data.data_unit_val, 0, 1024);
							 				memcpy(smartlock_info_data.data_unit_val, &data[13], smartlock_info_data.data_unit_len);
											//strncpy(smartlock_info_data.data_unit_val,&data[13],smartlock_info_data.data_unit_len);	

											char data_buffer[1024]={0};  
						    				HSZConvertUnCharToStr(data_buffer,smartlock_info_data.data_unit_val,smartlock_info_data.data_unit_len);    
						     

											#if 1
											int i;
											for(i = 0;i < (smartlock_info_data.data_unit_len);i++)											
											{
												printf("0x%02x ",smartlock_info_data.data_unit_val[i]&0xFF);
											}

											printf("\n");
											printf("%s\n", data_buffer); 
											#endif

											if(ADD_TMPPASSWORD == smartlock_info_data.attributeID_low)
											{
													DEBUG_ZONE(LOG_ZONE_MY,"HSZReturnDataForAddTemCode.\n");
													HSZReturnDataForAddTemCode(data_buffer,gw_mac,dev_mac);
											}
											else if(DELETE_TMPPASSWORD == smartlock_info_data.attributeID_low)
											{
													DEBUG_ZONE(LOG_ZONE_MY,"HSZReturnDataForDeleteTemCode.\n");
													HSZReturnDataForDeleteTemCode(data_buffer,gw_mac,dev_mac);
											}
											else if(QUERY_USER == smartlock_info_data.attributeID_low)
											{
													DEBUG_ZONE(LOG_ZONE_MY,"HSZReturnDataForQueryUser.\n");
													HSZReturnDataForQueryUser(data_buffer,gw_mac,dev_mac);
											}
											else if(ADMIN_AUTH == smartlock_info_data.attributeID_low)
											{
													DEBUG_ZONE(LOG_ZONE_MY,"HSZReturnDataForAdminAuth.\n");
													HSZReturnDataForAdminAuth(data_buffer,gw_mac,dev_mac);
											}
											else if(RESPONSE_CONTROL == smartlock_info_data.attributeID_low)
											{
													DEBUG_ZONE(LOG_ZONE_MY,"HSZReturnDataForControl.\n");
													HSZReturnDataForControl(data_buffer,gw_mac,dev_mac);
											}
											else
											{
													return rc;
											}
									 }
							 }
							 break;
							 
							 default:
							 break;
					 }
		 }	
		return 0;
}				

/*****************************************************************************
 函 数 名  : HSZCalculateSmartLockDataCheckSum
 功能描述  : 计算传入数据的Check fcs数值
 输入参数  : 
			int sof：起始字节
			int len:
			char *data_buf:
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年3月29日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZCalculateSmartLockDataCheckSum(int sof,int len,unsigned char *data_buf)
{
		int index = 0;
		int sum = 0x00;
		
		printf("HSZCalculateSmartLockDataCheckSum:\n");
		
	    printf("sof:%02x \n",sof);
		printf("len:%02x \n",len);
		
		for(index = 0;index < len;index++)											
		{
				printf("0x%02x ",data_buf[index]&0xFF);
		}
		
		printf("\n");
		
		
		if((sof>0xFF)||(len>PRIVATE_DATA_LEN))/*如果发送的数据不满足格式*/
		{
				HSZDeviceError(HSZ_ERR_SMARTLOCK_MSG_TYPE,"Send smartlock message type error.",NULL);
				return -1;
		}
		
		sum = (sum + sof)&0xFF;					/*sof*/
		
		sum = (sum + len)&0xFF;					/*len*/
		
		for(index = 0;index<len;index++)		/*data*/
		{
				sum = (sum + data_buf[index])&0xFF;
		}
		
		printf("sum:%02x \n",sum);
		
		return sum;
}	

/*****************************************************************************
 函 数 名  : HSZSmartLockTimeSync
 功能描述  : 时间同步请求
 输入参数  : 
			char *data:私有数据buf
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年3月29日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZSmartLockTimeSync(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
		char response[30] = {0};
		unsigned long int time;
		char TimeYear[4] = {0};
		char TimeMonth[4] = {0};
		char TimeDate[4] = {0};
		char TimeHour[4] = {0};
		char TimeMinute[4] = {0};
		char TimeSecond[4] = {0};

		printf("data_buf:%s \n",data_buf);
		
		if(strcmp(SMARTLOCK_GET_TIME,data_buf))
		{
				time = strtol(data_buf,NULL,16);
				Hal_Time_SecondToDate(time,TimeYear,TimeMonth,TimeDate,TimeHour,TimeMinute,TimeSecond);
				strcpy(response,LOCK_TIME);
				strcat(response,TimeYear);
				strcat(response,TimeMonth);
				strcat(response,TimeDate);
				strcat(response,TimeHour);
				strcat(response,TimeMinute);
				strcat(response,TimeSecond);
		}
		else
		{
				strncpy(response,REQUST_TIME,4);
		}

		if (hsbk_devicedata!=NULL)
		{
				rc = 0x00;
				(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}

		return rc;
}

/*****************************************************************************
 函 数 名  : HSZAnalysisSmartLockRegularData
 功能描述  : 解析门锁固定上报数据
 输入参数  : 
			char *data:私有数据buf
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年3月30日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZAnalysisSmartLockRegularData(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
		char response[30] = {0};
		char open_id[8] = {0};
		char open_type[4] = {0};
		char user_type[4] = {0};
		
		if(strncmp(SMARTLOCK_RESERT,data_buf,10)==0)
		{
				strncpy(response,LOCK_RESERT,4);
		}
		else if(strncmp(SMARTLOCK_LOCK,data_buf,8)==0)
		{
				strncpy(response,DOOR_LOCK,4);
		}
		else if(strncmp(SMARTLOCK_BATTERY_LOW,data_buf,10)==0)
		{
				strncpy(response,LOCK_BATTERY_LOW,4);
		}
		else if(strncmp(SMARTLOCK_PRY,data_buf,10)==0)
		{
				strncpy(response,LOCK_PRY,4);
		}
		else if(strncmp(SMARTLOCK_HIJACK,data_buf,10)==0)
		{
				strncpy(response,LOCK_HIJACK,4);
		}
		else if(strncmp(SMARTLOCK_PASSWORD_ALARM,data_buf,10)==0)
		{
				strncpy(response,LOCK_PASSWORD_ALARM,4);
		}
		else if(strncmp(SMARTLOCK_FINGPRINT_ALARM,data_buf,10)==0)
		{
				strncpy(response,LOCK_FINGPRINT_ALARM,4);
		}
		else if(strncmp(SMARTLOCK_CARD_ALARM,data_buf,10)==0)
		{
				strncpy(response,LOCK_CARD_ALARM,4);
		}
		else if(strncmp(SMARTLOCK_DOUBLE_LOCK,data_buf,8)==0)
		{
				strncpy(response,LOCK_DOUBLE_LOCK,4);
		}
		else if(22 == strlen(data_buf))
		{
				if(strncmp(SMARTLOCK_OPEN_DOOR,data_buf+2,6)==0)
				{
						strncpy(open_id,data_buf+12,4);
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

		return rc;
}

/*****************************************************************************
 函 数 名  : HSZReturnDataForAddTemCode
 功能描述  : 解析添加临时密码操作返回数据
 输入参数  : 
			char *data:私有数据buf
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月8日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZReturnDataForAddTemCode(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
		char response[30] = {0};
		char codelen[30] = {0};
		int codelength = 0;
		
		if(strlen(data_buf)==4)
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
		else
		{
				strncpy(codelen,data_buf+30,2);
				codelength = strtol(codelen,NULL,16);
			
				strncpy(response,ADD_TEMCODE_SUCCESS,6);
				strncat(response,data_buf+4,2);				/*用户编号*/
				strncat(response,data_buf+30,2);			/*密码长度*/
				strncat(response,data_buf+32,codelength*2);	/*密码内容*/

				strncat(response,data_buf+8,10);			/*开始时间*/
				strncat(response,data_buf+20,10);			/*结束时间*/
		}

		if (hsbk_devicedata!=NULL)
		{
				rc = 0x00;
				(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}

		return rc;
}

/*****************************************************************************
 函 数 名  : HSZReturnDataForDeleteTemCode
 功能描述  : 解析删除临时密码操作返回数据
 输入参数  : 
			char *data:私有数据buf
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月8日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZReturnDataForDeleteTemCode(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
		char id[4] = {0};
		char response[30] = {0};
		
		if(strncmp(SMARTLOCK_DELETE_TEMCODE_SUCCESS,data_buf+4,2)==0)
		{
				strncpy(response,DELETE_TEMCODE_SUCCESS,4);
				strncpy(id,data_buf+2,2);
				strcat(response,id);
		}
		else if(strncmp(SMARTLOCK_DELETE_TEMCODE_FAIL,data_buf+4,2)==0)
		{
				strncpy(response,DELETE_TEMCODE_FAIL,4);
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

		return rc;
}

/*****************************************************************************
 函 数 名  : HSZReturnDataForQueryUser
 功能描述  : 解析查询用户操作返回数据
 输入参数  : 
			char *data:私有数据buf
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZReturnDataForQueryUser(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
		char response[30] = {0};
		char codelen[30] = {0};
		int codelength = 0;
		
		if(strcmp(SMARTLOCK_NO_TEMP_USER,data_buf)==0)    /*临时用户为空*/
		{
				strcpy(response,LOCK_NO_TEMP_USER);
			
		}
		else
		{
				strncpy(codelen,data_buf+38,2);
				codelength = strtol(codelen,NULL,16);
			
				strncpy(response,QUERY_TEM_USER_SUCCESS,4);
				strncat(response,data_buf,2);				/*用户总数*/
				strncat(response,data_buf+2,2);				/*当前顺序*/
				strncat(response,data_buf+6,2);				/*用户编号*/
				strncat(response,data_buf+10,2);			/*用户类型*/
				strncat(response,data_buf+38,2);			/*密码长度*/		

				strncat(response,data_buf+40,codelength*2);	/*密码信息*/
				strncat(response,data_buf+16,10);			/*临时密码生效日期*/
				strncat(response,data_buf+28,10);			/*临时密码失效日期*/
		}

		if (hsbk_devicedata!=NULL)
		{
				rc = 0x00;
				(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}

		return rc;
}

/*****************************************************************************
 函 数 名  : HSZReturnDataForAdminAuth
 功能描述  : 解析管理员密码认证返回数据
 输入参数  : 
			char *data:私有数据buf
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月8日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZReturnDataForAdminAuth(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
		char response[30] = {0};
		
		if(strcmp(SMARTLOCK_ADMIN_AUTH_SUCCESS,data_buf)==0)
		{
				strncpy(response,ADMIN_AUTH_SUCCESS,4);		
		}
		else if(strcmp(SMARTLOCK_ADMIN_PASSWORD_ERROR,data_buf)==0)
		{
				strncpy(response,ADMIN_AUTH_PASSWORD_ERROR,4);
		}
		else if(strcmp(SMARTLOCK_ADMIN_AUTH_FAIL,data_buf)==0)
		{	
				strncpy(response,ADMIN_AUTH_FAIL,4);
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

		return rc;
}

/*****************************************************************************
 函 数 名  : HSZReturnDataForControl
 功能描述  : 解析门锁控制信息反馈数据
 输入参数  : 
			char *data:私有数据buf
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月8日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZReturnDataForControl(char *data_buf,char* gw_mac,char* dev_mac)
{
		int rc = -1;
		int data_type = 0;
		char response[30] = {0};


		data_type = strtol(data_buf,NULL,16);
		switch(data_type)
		{
				case SMARTLOCK_SYNC_TIME_FAIL:
				{
						strncpy(response,SYNC_TIME_FAIL,4);
				}
				break;

				case SMARTLOCK_OPEN_DOOR_FAIL:
				{
						strncpy(response,OPEN_DOOR_FAIL,4);
				}
				break;

				case SMARTLOCK_OPEN_DOOR_PASSWORD_ERROR:
				{
						strncpy(response,OPEN_DOOR_PASSWORD_ERROR,4);
				}
				break;

				case SMARTLOCK_FORBID_REMOTE_OPEN_DOOR:
				{
						strncpy(response,FORBID_REMOTE_OPEN_DOOR,4);
				}
				break;

				default:
				break;
		}

/*		
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
*/
		if (hsbk_devicedata!=NULL)
		{
				rc = 0x00;
				(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}

		return rc;
}

/***********************************************************************
**函数名:CheckLeapYear
**功能:判断是否是闰年
**参数:无
**返回值:无
**备注:
***********************************************************************/
int CheckLeapYear(unsigned int year)
{
		if(((year%4==0)&&(year%100!=0)) || (year%400==0))
		{
			return 1;
		}
		
		return 0;
}

/***********************************************************************
**函数名:Hal_Time_SecondToDate
**功能:时间戳换算为时间
**参数:second 相对时间，以秒为基础单位
**返回值:无
**备注:
***********************************************************************/
void Hal_Time_SecondToDate(unsigned long int second,char *TimeYear,char *TimeMonth,char *TimeDate,char *TimeHour,char *TimeMinute,char *TimeSecond)
{
		unsigned int date, i, day;
		unsigned long int tmp;
		int g_ucTimeYear, g_ucTimeMonth, g_ucTimeDate, g_ucTimeHour, g_ucTimeMinute, g_ucTimeSecond;

		second -= 946656000;

	  	day = (second/86400) + 1;

	  	for(i=2000;;i++)
	  	{
		    	tmp = 365 + CheckLeapYear(i);
		    	if(day <= tmp)
				{
						break;
				}
		    	else
				{
						day = day-tmp;
				}
	  	}

	  	DAYS_TAB[1] = 28;
	  	DAYS_TAB[1] += CheckLeapYear(i);

	  	date = i - 2000;
	  	g_ucTimeYear = 16*(date/10) + date%10;//年

	  	for(i = 0; i < 12; i++)
	  	{
		    	if(day <= DAYS_TAB[i])
				{
						break;
				}
		    	else
				{
						day = day - DAYS_TAB[i];
				}
	  	}

	  	date = i+1;
		g_ucTimeMonth = 16 * (date / 10) + date % 10; 		//月
		g_ucTimeDate = 16 * (day / 10) + day % 10; 			//日
		tmp = second % 86400;
		date = tmp / 3600;
		g_ucTimeHour = 16 * (date / 10) + date % 10; 		//时
		date = tmp % 3600 / 60;
		g_ucTimeMinute = 16 * (date / 10) + date % 10; 		//分
		date = tmp % 3600 % 60 % 60;
		g_ucTimeSecond = 16 * (date / 10) + date % 10; 		//秒

		sprintf(TimeYear,"%02x",g_ucTimeYear);
		sprintf(TimeMonth,"%02x",g_ucTimeMonth);
		sprintf(TimeDate,"%02x",g_ucTimeDate);
		sprintf(TimeHour,"%02x",g_ucTimeHour);
		sprintf(TimeMinute,"%02x",g_ucTimeMinute);
		sprintf(TimeSecond,"%02x",g_ucTimeSecond);
}

/***********************************************************************
**函数名:Hal_Time_DateToSecond
**功能:日期换算为时间戳
**参数:无
**返回值:相对时间，以秒为基础单位
**备注:
***********************************************************************/
unsigned long int Hal_Time_DateToSecond(const int g_ucTimeYear,const int g_ucTimeMonth,const int g_ucTimeDate,const int g_ucTimeHour,const int g_ucTimeMinute,const int g_ucTimeSecond)
{
		unsigned long int second, tmp;
		unsigned int i, day = 0;

		//累计以往各年的天数
		tmp = 2000 + 10*(g_ucTimeYear>>4) + (g_ucTimeYear&0x0f);
		for(i = 2000; i < tmp; i++)
		{
				day += 365 + CheckLeapYear(i);
		}

		DAYS_TAB[1] = 28;
	  	DAYS_TAB[1] += CheckLeapYear(i);

		//累计当年以往各月的天数
		tmp = 10*(g_ucTimeMonth>>4) + (g_ucTimeMonth&0x0f);
		for(i = 1; i < tmp; i++)
		{
				day += DAYS_TAB[i-1];
		}

		tmp = 10*(g_ucTimeDate>>4) + (g_ucTimeDate&0x0f);
		day += tmp -1;
	 	second = ((unsigned long)day)*24*3600;
		tmp = 10*(g_ucTimeHour>>4) + (g_ucTimeHour&0x0f);
		second += ((unsigned long)tmp)*3600;
		tmp = 10*(g_ucTimeMinute>>4) + (g_ucTimeMinute&0x0f);
		second += tmp*60;
		tmp = 10*(g_ucTimeSecond>>4) + (g_ucTimeSecond&0x0f);
		second += tmp;
		second += 946656000;
		
		return second;
}

/*****************************************************************************
 函 数 名  : HSZSmartLockRemoteOpenDoor
 功能描述  : 远程开锁
 输入参数  : 
			char *data_buf:下发数据
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZSmartLockRemoteOpenDoor(char *data_buf)
{
		int sum = 0x00;
		int index = 0;
		int code_length = 0;
		int open_type = 0;
		int code_temp = 0;
		int i;
		
		char codelen[4] = {0};
		char opentype[4] = {0};
		char pincode[4] = {0};

		strncpy(opentype,data_buf,1);
		open_type = strtol(opentype,NULL,10);

		strncpy(codelen,data_buf+1,1);
		code_length = strtol(codelen,NULL,16);	

		memset(send_msg_buf,0x00,512);
			
		send_msg_buf[0] = 0xFA;									/*sof*/
				
		send_msg_buf[1] = ((code_length+9)&0xFF);				/*len*/
				
		send_msg_buf[2] = 0x14;									/*cmd1*/
				
		send_msg_buf[3] = 0x53;									/*cmd2*/

		send_msg_buf[4] = 0xFF;									/*src_high*/

		send_msg_buf[5] = 0xFF;									/*src_low*/

		send_msg_buf[6] = 0xFF;									/*dest_high*/

		send_msg_buf[7] = 0xFF;									/*dest_high*/

		send_msg_buf[8] = 0x20;									/*device_type*/

		send_msg_buf[9] = (open_type&0xFF);						/*lock_state*/

		send_msg_buf[10] = (code_length&0xFF);					/*pin_len*/

		for(i=0;i<code_length;i++)								/*pin_data*/
		{
				strncpy(pincode,data_buf+2+i,1);
				
				code_temp = strtol(pincode,NULL,10);
				
				send_msg_buf[11+i] = (code_temp&0xFF);	
		}
		        		
		for(index = 0;index<(code_length+11);index++)		
		{
				sum = (sum + send_msg_buf[index])&0xFF;
		}
		
		send_msg_buf[code_length+11] = sum;

		return (code_length+12);
}

/*****************************************************************************
 函 数 名  : HSZQueryTemUser
 功能描述  : 查询临时用户信息
 输入参数  : 
			char *data_buf:下发数据
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZQueryTemUser()
{
		int sum = 0x00;
		int index = 0;
		
		memset(send_msg_buf,0x00,512);
			
		send_msg_buf[0] = 0xFA;									/*sof*/
				
		send_msg_buf[1] = 0x0F;									/*len*/
				
		send_msg_buf[2] = 0x14;									/*cmd1*/
				
		send_msg_buf[3] = 0x54;									/*cmd2*/

		send_msg_buf[4] = 0xFF;									/*src_high*/

		send_msg_buf[5] = 0xFF;									/*src_low*/

		send_msg_buf[6] = 0xFF;									/*dest_high*/

		send_msg_buf[7] = 0xFF;									/*dest_high*/

		send_msg_buf[8] = 0xFD;									/*clusterid_high*/

		send_msg_buf[9] = 0x00;									/*clusterid_low*/

		send_msg_buf[10] = 0x00;								/*attributeid_high*/

		send_msg_buf[11] = 0x03;								/*attributeid_low*/

		send_msg_buf[12] = 0x04;								/*data_len*/

		send_msg_buf[13] = 0xFF;								/*data_val*/
		
		send_msg_buf[14] = 0xFF;								/*data_val*/

		send_msg_buf[15] = 0x00;								/*data_val*/

		send_msg_buf[16] = 0x01;								/*data_val*/
		        		
		for(index = 0;index<17;index++)		
		{
				sum = (sum + send_msg_buf[index])&0xFF;
		}
		
		send_msg_buf[17] = sum;

		return 18;

}

/*****************************************************************************
 函 数 名  : HSZQueryLockTime
 功能描述  : 查询锁具内部时间
 输入参数  : 
			char *data_buf:下发数据
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZQueryLockTime()
{
		int sum = 0x00;
		int index = 0;
		
		memset(send_msg_buf,0x00,512);
			
		send_msg_buf[0] = 0xFA;									/*sof*/
				
		send_msg_buf[1] = 0x0B;									/*len*/
				
		send_msg_buf[2] = 0x14;									/*cmd1*/
				
		send_msg_buf[3] = 0x56;									/*cmd2*/

		send_msg_buf[4] = 0xFF;									/*src_high*/

		send_msg_buf[5] = 0xFF;									/*src_low*/

		send_msg_buf[6] = 0xFF;									/*dest_high*/

		send_msg_buf[7] = 0xFF;									/*dest_high*/

		send_msg_buf[8] = 0x20;									/*device_type*/

		send_msg_buf[9] = 0xFF;									/*time_query*/

		send_msg_buf[10] = 0xFF;								/*time_query*/

		send_msg_buf[11] = 0xFF;								/*time_query*/

		send_msg_buf[12] = 0xFF;								/*time_query*/

		        		
		for(index = 0;index<13;index++)		
		{
				sum = (sum + send_msg_buf[index])&0xFF;
		}
		
		send_msg_buf[13] = sum;

		return 14;

}

/*****************************************************************************
 函 数 名  : HSZDeleteTemUser
 功能描述  : 删除临时用户
 输入参数  : 
			char *data_buf:下发数据
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZDeleteTemUser(char *data_buf)
{
		int sum = 0x00;
		int index = 0;
		int useID =0;
		
		memset(send_msg_buf,0x00,512);
			
		send_msg_buf[0] = 0xFA;									/*sof*/
				
		send_msg_buf[1] = 0x0E;									/*len*/
				
		send_msg_buf[2] = 0x14;									/*cmd1*/
				
		send_msg_buf[3] = 0x54;									/*cmd2*/

		send_msg_buf[4] = 0xFF;									/*src_high*/

		send_msg_buf[5] = 0xFF;									/*src_low*/

		send_msg_buf[6] = 0xFF;									/*dest_high*/

		send_msg_buf[7] = 0xFF;									/*dest_high*/

		send_msg_buf[8] = 0xFD;									/*clusterid_high*/

		send_msg_buf[9] = 0x00;									/*clusterid_low*/

		send_msg_buf[10] = 0x00;								/*attributeid_high*/

		send_msg_buf[11] = 0x01;								/*attributeid_low*/

		send_msg_buf[12] = 0x03;								/*data_len*/

		send_msg_buf[13] = 0x02;								/*data_val*/
		
		send_msg_buf[14] = 0x00;								/*data_val*/

		useID = strtol(data_buf,NULL,10);

		send_msg_buf[15] = (useID&0xFF);						/*data_val*/
		        		
		for(index = 0;index<16;index++)		
		{
				sum = (sum + send_msg_buf[index])&0xFF;
		}
		
		send_msg_buf[16] = sum;

		return 17;

}

/*****************************************************************************
 函 数 名  : HSZSmartLockPasswdCheck
 功能描述  : 管理员密码认证
 输入参数  : 
			char *data_buf:下发数据
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZSmartLockPasswdCheck(char *data_buf)
{
		int sum = 0x00;
		int index = 0;
		int code_length = 0;
		int user_id = 0;
		int code_temp = 0;
		int i;
		
		char codelen[4] = {0};
		char userId[4] = {0};
		char pincode[4] = {0};

		strncpy(userId,data_buf+1,2);
		user_id = strtol(userId,NULL,10);

		strncpy(codelen,data_buf+3,1);
		code_length = strtol(codelen,NULL,16);	

		memset(send_msg_buf,0x00,512);
			
		send_msg_buf[0] = 0xFA;									/*sof*/
				
		send_msg_buf[1] = ((code_length+14)&0xFF);				/*len*/
				
		send_msg_buf[2] = 0x14;									/*cmd1*/
				
		send_msg_buf[3] = 0x54;									/*cmd2*/

		send_msg_buf[4] = 0xFF;									/*src_high*/

		send_msg_buf[5] = 0xFF;									/*src_low*/

		send_msg_buf[6] = 0xFF;									/*dest_high*/

		send_msg_buf[7] = 0xFF;									/*dest_high*/

		send_msg_buf[8] = 0xFD; 								/*clusterid_high*/
		
		send_msg_buf[9] = 0x00; 								/*clusterid_low*/
		
		send_msg_buf[10] = 0x00;								/*attributeid_high*/
		
		send_msg_buf[11] = 0x04;								/*attributeid_low*/
		
		send_msg_buf[12] = ((code_length+3)&0xFF);				/*data_len*/

		send_msg_buf[13] = 0x00;

		send_msg_buf[14] = (user_id&0xFF);						/*user_id*/

		send_msg_buf[15] = (code_length&0xFF);					/*pin_len*/

		for(i=0;i<code_length;i++)								/*pin_data*/
		{
				strncpy(pincode,data_buf+4+i,1);
				
				code_temp = strtol(pincode,NULL,10);
				
				send_msg_buf[16+i] = (code_temp&0xFF);	
		}
		        		
		for(index = 0;index<(code_length+16);index++)		
		{
				sum = (sum + send_msg_buf[index])&0xFF;
		}
		
		send_msg_buf[code_length+16] = sum;

		return (code_length+17);
}

/*****************************************************************************
 函 数 名  : HSZAddTemUser
 功能描述  : 添加临时用户
 输入参数  : 
			char *data_buf:下发数据
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年4月13日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZAddTemUser(char *data_buf)
{
		int sum = 0x00;
		int index = 0;
		int code_length = 0;
		int code_temp = 0;
		int i;
		
		char codelen[8] = {0};
		char pincode[8] = {0};
		char tmpcode[8] = {0};

		strncpy(codelen,data_buf+1,1);
		code_length = strtol(codelen,NULL,16);	

		memset(send_msg_buf,0x00,512);
			
		send_msg_buf[0] = 0xFA;									/*sof*/
				
		send_msg_buf[1] = ((code_length+25)&0xFF);				/*len*/
				
		send_msg_buf[2] = 0x14;									/*cmd1*/
				
		send_msg_buf[3] = 0x54;									/*cmd2*/

		send_msg_buf[4] = 0xFF;									/*src_high*/

		send_msg_buf[5] = 0xFF;									/*src_low*/

		send_msg_buf[6] = 0xFF;									/*dest_high*/

		send_msg_buf[7] = 0xFF;									/*dest_high*/

		send_msg_buf[8] = 0xFD; 								/*clusterid_high*/
		
		send_msg_buf[9] = 0x00; 								/*clusterid_low*/
		
		send_msg_buf[10] = 0x00;								/*attributeid_high*/
		
		send_msg_buf[11] = 0x00;								/*attributeid_low*/
		
		send_msg_buf[12] = ((code_length+14)&0xFF);				/*data_len*/

		send_msg_buf[13] = 0x02;								/*code_type*/

		send_msg_buf[14] = 0x20;								/*start_year*/

		for(i=0;i<5;i++)										/*start_time*/
		{
				strncpy(pincode,data_buf+code_length+2+i*2,2);
				
				code_temp = strtol(pincode,NULL,16);
				
				send_msg_buf[15+i] = (code_temp&0xFF);	
		}

		send_msg_buf[20] = 0x20;								/*end_year*/

		for(i=0;i<5;i++)										/*end_time*/
		{
				strncpy(pincode,data_buf+code_length+12+i*2,2);
				
				code_temp = strtol(pincode,NULL,16);
				
				send_msg_buf[21+i] = (code_temp&0xFF);	
		}

		
		send_msg_buf[26] = (code_length&0xFF);					/*pin_len*/

		for(i=0;i<code_length;i++)								/*pin_data*/
		{
				strncpy(tmpcode,data_buf+2+i,1);
				
				code_temp = strtol(tmpcode,NULL,10);
				
				send_msg_buf[27+i] = (code_temp&0xFF);	
		}
		        		
		for(index = 0;index<(code_length+27);index++)		
		{
				sum = (sum + send_msg_buf[index])&0xFF;
		}
		
		send_msg_buf[code_length+27] = sum;

		return (code_length+28);
}

/*****************************************************************************
 函 数 名  : HSZConvertUnCharToStr
 功能描述  : 数组转字符串
 输入参数  : 
			char* UnChar
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年5月4日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZConvertUnCharToStr(char* str,unsigned char* UnChar, int ucLen)  
{  
	    int i = 0;  
	    for(i = 0; i < ucLen; i++)  
	    {  
		        //格式化输str,每unsigned char 转换字符占两位置%x写输%X写输  
		        sprintf(str + i * 2, "%02x", UnChar[i]);  
	    }  
} 
