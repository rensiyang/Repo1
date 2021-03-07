/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZCommon.c
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2017年6月9日
  最近修改   :
  功能描述   : 通用处理接口函数定义
  函数列表   :
  修改历史   :
  1.日    期   : 2017年6月9日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

  2.日    期   : 2017年6月23日
    作    者   : Zigbee Project Team
    修改内容   : 修改文件架构

******************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "HSZCommon.h"
#include "cJSON.h"
#include "HSZMessage.h"
#include "HSZDeviceData.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ha_client.h"

#if !defined(WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif




#define MSG_BUFFER_SIZE         200*1024

#define HEATBEAT_TEST_TIME      5*6	
#define MODE                    (S_IRWXU|S_IRWXG|S_IRWXO)

time_t heatbeat_time;
time_t time_now;

struct mosquitto *mosq = NULL;
char *gatewayid  = NULL ;			/*解析获取网关MAC地址*/

char logmidwarename1[]="./log/MiddleWareLog1.log";
char logmidwarename2[]="./log/MiddleWareLog2.log";
char logmidwarename3[]="./log/MiddleWareLog3.log";
char logmidwarename4[]="./log/MiddleWareLog4.log";
char logmidwarename5[]="./log/MiddleWareLog5.log";

int log_file_index;

char value[512]={0x0};
CRITICAL_SECTION cs_log;

FILE *MiddleWareFp;

int received_join_time = 0;
int sum_join_time = 0;

extern int sub_topic;

static pthread_mutex_t heatbeatTimeLock;
/*****************************************************************************
 函 数 名  : Lock
 功能描述  : 写中间层log加锁
 输入参数  : CRITICAL_SECTION *l  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void Lock(CRITICAL_SECTION *l) 
{
    pthread_mutex_lock(l);
}

/*****************************************************************************
 函 数 名  : Unlock
 功能描述  : 写log解锁
 输入参数  : CRITICAL_SECTION *l  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void Unlock(CRITICAL_SECTION *l) 
{
    pthread_mutex_unlock(l);
}


/*****************************************************************************
 函 数 名  : HSZDebugZone
 功能描述  : 判断打印控制区域
 输入参数  : int zone  
 输出参数  : 无
 返 回 值  :  
 
 修改历史      :
  1.日    期   : 2017年6月21日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZDebugZone(int zone)
{
	if(-1 == HSZGetDebugCode(value))
	{
		return 0x00;
	}
	
	if(LOG_ZONE_ANY == HSZGetDebugCode(value)) /*打印所有的log*/
	{
		return 0x01;
	}
	
	if((zone & HSZGetDebugCode(value)) != 0x00) /*打印对应权限的log*/
	{
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}

/*****************************************************************************
 函 数 名  : DEBUG
 功能描述  : 写log函数
 输入参数  : int zone            
             const char *file    
             const char *func    
             int line            
             const char *pszFmt  
             ...                 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void DEBUG(int zone,const char *file,const char *func, int line,const char *pszFmt,...) 
{
	struct tm *now;
    struct timeb tb;
    va_list argp;
	char output[1024] = {0};
	char datestr[16];
	char timestr[16];
	
	if(HSZDebugZone(zone))
	{
		ftime(&tb);
		now=localtime(&tb.time);
		sprintf(datestr,"%04d-%02d-%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday);
		sprintf(timestr,"%02d:%02d:%02d",now->tm_hour     ,now->tm_min  ,now->tm_sec );
		
		Lock(&cs_log);
		va_start(argp,pszFmt);
		
		if(0x02 == HSZGetDebugOnOFF(value))
		{
			if (NULL!=MiddleWareFp) 
			{
				fprintf(MiddleWareFp,"%s %s",datestr,timestr);/*写日期+时间*/
				snprintf(output,1023," <%s> [%s] %05d: ",file,func,line);/*写文件名+函数名+行数*/
				fprintf(MiddleWareFp,"  %s",output);
				vfprintf(MiddleWareFp,pszFmt,argp);
				fflush(MiddleWareFp);
				
				HSZWriteLogFile();
 
			}
		}
	    else if(0x01 == HSZGetDebugOnOFF(value))
		{	
			printf("%s %s",datestr,timestr);/*打印日期+时间*/
			snprintf(output,1023," <%s> [%s] %05d: ",file,func,line);/*打印文件名+函数名+行数*/
			printf("%s",output);
			vprintf(pszFmt,argp);
		}
		else
		{
			;
		}
		
		va_end(argp);
		Unlock(&cs_log);
	}
}

/*****************************************************************************
 函 数 名  : HSZGatewayMac
 功能描述  : 获取网关MIC地址
 输入参数  : 无
 输出参数  : 网关MIC地址
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年6月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
char* HSZGatewayMac()
{
	if (NULL != gatewayid)
	{
		DEBUG_ZONE(LOG_ZONE_TEST,"The gatway id :%s.\n",gatewayid);
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_TEST,"The gatway id is NULL.\n");
	}
	
	return gatewayid;
}

/*****************************************************************************
 函 数 名  : mosquitto_publish_topic
 功能描述  : 发布消息
 输入参数  : 无
 输出参数  : const char *topic：发布消息的Topic
			 const char *payload：发布消息的Message
 返 回 值  : 发送失败返回-1，发送成功返回大于0的值
 
 修改历史      :
  1.日    期   : 2018年1月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int mosquitto_publish_topic(const char *topic,const char *payload)//payload="{\"commands\":[{\"command\":\"hswrattrdata 1 0xfd00 0x0003 {%s} {%s}\"}]}",dev_mac,cmd_data
{
	int rc = 0x00;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"Send Host Topic:%s,Message:%s\n",topic,payload);
	
	rc = add_ha_client_msg(topic,payload);
	
	return rc;
}
/*****************************************************************************
 函 数 名  : substr_c
 功能描述  : 截取字符串
 输入参数  : const char*str  要截取的字符串
             unsigned start  开始截取的起始位置
             unsigned end    停止截取的位置
 输出参数  : 截取的字符串内容
 返 回 值  :  
 
 修改历史      :
  1.日    期   : 2017年6月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
char* substr_c(const char*str,unsigned start,unsigned end)
{
	unsigned n = end - start;
	static char stbuf[256];
	
	if(n > 256)
	{
		return NULL;
	}
	
	strncpy(stbuf,str + start,n);
	stbuf[n] = 0x00;
	
	return stbuf;
}
/*****************************************************************************
 函 数 名  : strstr_r
 功能描述  : 匹配字符串查找
 输入参数  : const char *s1  
             const char *s2  
 输出参数  : 无
 返 回 值  : char
 
 修改历史      :
  1.日    期   : 2017年6月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
char *strstr_r(const char *s1,const char *s2)
{
    int len1 = strlen(s1);
    int len2 = strlen(s2);
	
	if (len1<len2)
	{
		return 0;
	}

    if(strncmp(s1+(len1-len2),s2,len2)==0)
	{
		return (char*)(s1 + (len1 - len2));
	}
	
    return(0);
}


/*****************************************************************************
 函 数 名  : HSZGetMessageType
 功能描述  : 获取订阅消息的消息类型，主要区分heartbeat
 输入参数  : const struct mosquitto_message *message  订阅到的消息
 输出参数  : 消息类型
 返 回 值  : 无
 
 修改历史      :
  1.日    期   : 2017年6月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZGetMessageType(const char *topic)
{
	int ntopictype = TYPE_TOPIC_ERR;
	char* chtopictype;
	
	chtopictype = strstr_r(topic,"/heartbeat");

	if (chtopictype != NULL)
	{
		ntopictype = TYPE_TOPIC_HEARTBEAT;/*消息类型:heartbeat */
	}

	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/devices");
		
		if (chtopictype != NULL)
		{
			ntopictype = TYPE_TOPIC_DEVICES;/*消息类型:devices */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/zclresponse");
		
		if (chtopictype != NULL)
		{
			ntopictype = TYPE_TOPIC_ZCLRESPONSE;/*消息类型:zclresponse */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/devicejoined");
		
		if (chtopictype != NULL)
		{
			ntopictype = TYPE_TOPIC_DEVICESJOINED;/*消息类型:devicejoined */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/deviceleft");
		
		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_DEVICELEFT;/*消息类型:deviceleft */
		}
	}

	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/devicestatechange");
		
		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_DECEICE_STATE_CHANGE;/*消息类型:devicestatechange */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/relays");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_RELAYS;/*消息类型:relays */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/settings");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_SETTINGS;/*消息类型:settings */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/executed");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_EXECUTED;/*消息类型:executed */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/otaevent");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_OTAEVENT;/*消息类型:otaevent */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/apsresponse");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_APSRESPONSE;/*消息类型:apsresponse */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/devlinestch");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_DEVLINESTCH;/*消息类型:apsresponse */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/zdoresponse");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_ZDO_RESPONSE;/*消息类型:zdoresponse */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/nodeidchanged");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_NODEID_CHANGE;/*消息类型:NodeID Change */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/ncpreset");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_NCP_RESET;/*消息类型:NCP 重新启动 */
		}
	}
	
		
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/pjoinres");
		
		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_PERMIT_JOIN_STATE;/*消息类型:允许入网状态反馈 */
		}
	}
	
		
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/ncpverres");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_NCP_VERSION;/*消息类型:反馈NCP版本*/
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/znetstatus");

		if (chtopictype!=NULL)
		{
		ntopictype = TYPE_TOPIC_ZIGBEE_NETWORK_STATE;/*消息类型:Zigbee 网络状态发生变化时topic*/
	}
	}
	
	DEBUG_ZONE(LOG_ZONE_TEST,"ntopictype:%d\n", ntopictype);
	
	return ntopictype;
}

/*****************************************************************************
 函 数 名  : HSZHeatbeatLockInit
 功能描述  : 初始化heartbeat锁
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年12月29日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
static void HSZHeatbeatLockInit(void)
{
	int status;
	status = pthread_mutex_init(&heatbeatTimeLock, NULL);
	
	if (status != 0) 
	{
		DEBUG_ZONE(LOG_ZONE_TEST,"pthread_mutex_init failed, status = 0x%X", status);
	}
}
/*****************************************************************************
 函 数 名  : HSZAcquireHeatbeatLock
 功能描述  : heartbeat锁
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年12月29日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
static void HSZAcquireHeatbeatLock(void)
{
  int status;
  
  /*Locking the mutex shouldn't fail, so we simply assert if it does*/
  status = pthread_mutex_lock(&heatbeatTimeLock);
  if (status != 0) 
  {
	DEBUG_ZONE(LOG_ZONE_TEST,"pthread_mutex_lock failed, status = 0x%X", status);
  }
}
/*****************************************************************************
 函 数 名  : HSZReleaseHeatbeatLock
 功能描述  : heartbeat解锁
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年12月29日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
static void HSZReleaseHeatbeatLock(void)
{
  int status;
  
  /*Unlocking the mutex shouldn't fail, so we simply assert if it does */
  status = pthread_mutex_unlock(&heatbeatTimeLock);
  
  if (status != 0) 
  {
	DEBUG_ZONE(LOG_ZONE_TEST,"pthread_mutex_lock failed, status = 0x%X", status);
  }
}

/*****************************************************************************
 函 数 名  : HSZGetMacAddrAndUpdateNetwrokStatus
 功能描述  : 获取网关地址,并且通知上层更新网络状态
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年1月22日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
static void HSZGetMacAddrAndUpdateNetwrokStatus(const char *topic)
{
	/*第一次获取heatbeat后，解析出beatheart,获取网关地址，存入数组*/
	gatewayid = substr_c(topic,3,19);
	DEBUG_ZONE(LOG_ZONE_MY,"Gateway MAC Address:%s.\n",gatewayid);
	
	/* 第一次获取Topic，截取Gateway MAC地址，请求Host获取入网设备列表，
	并存储，然后将初始化完成状态上报上层 */
	HSZGetDeviceList(gatewayid);
	
	if (hsbk_gatewaystate!=NULL)
	{
		hsbk_gatewaystate(HSZGatewayMac(),0x01);
	}		
}
/*****************************************************************************
 函 数 名  : HSZHandleMessage
 功能描述  : 根据消息类型处理消息
 输入参数  : const struct mosquitto_message *message  订阅到的消息
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年6月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZHandleMessage(const char *topic,const char *message)
{
	int ntopictype = TYPE_TOPIC_ERR;
	char *out;
	cJSON *jsonroot;
	char buf_revchar[MSG_BUFFER_SIZE];
	
	DEBUG_ZONE(LOG_ZONE_TEST,"Message Topic name:%s.\n", topic);
	DEBUG_ZONE(LOG_ZONE_TEST,"Message:%s.\n",message);
	
	if (NULL == gatewayid)
	{
		HSZGetMacAddrAndUpdateNetwrokStatus(topic);
	}
	
	ntopictype = HSZGetMessageType(topic);
	
	if(TYPE_TOPIC_HEARTBEAT == ntopictype)
	{
		/*更新获取hearbeat Time*/
		HSZAcquireHeatbeatLock();
		
		time(&heatbeat_time);
		
		// DEBUG_ZONE(LOG_ZONE_MY,"Heatbeat Message:%s.\n",message);
		
		// DEBUG_ZONE(LOG_ZONE_THREAD,"The heatbeat time is :%ld\n",heatbeat_time);
		
		HSZReleaseHeatbeatLock();
		
		/* 如果订阅了Heatbeat 上报消息内容*/
		if (hsbk_top!=NULL)
		{
			if(TYPE_TOPIC_HEARTBEAT == sub_topic)
			{
				(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
			}
		}

#if 0	    
		/*用于倒计时计时入网时间*/
		if(received_join_time!= 0x00)
		{
			sum_join_time = sum_join_time + 0x05;
			
		    if(sum_join_time>(received_join_time-1))/*倒计时时间到*/
			{
				if (hsbk_gatewaypermitjoinstate!=NULL)
				{
					hsbk_gatewaypermitjoinstate(HSZGatewayMac(),0x00);/*入网结束回调*/
				}
				
				sum_join_time = 0x00;
				received_join_time = 0x00;			
			}
		}

#endif
		
		/*Heartbeat Message 处理*/
		memset(buf_revchar,0,MSG_BUFFER_SIZE);
		
		if (strlen(message) < MSG_BUFFER_SIZE)
		{
			strncpy(buf_revchar,(char*)message,strlen(message));	
			
			jsonroot = cJSON_Parse(buf_revchar);
			
			if(jsonroot != NULL)
			{
				HSZHostHeartBeatMsgHandle(jsonroot);
			}
			
			cJSON_Delete(jsonroot);
		}
	}
	else
	{  
		{
			memset(buf_revchar,0,MSG_BUFFER_SIZE);

			if (strlen(message) < MSG_BUFFER_SIZE)
			{								
				strncpy(buf_revchar,(char*)message,strlen(message));
				
				jsonroot = cJSON_Parse(buf_revchar);
				
				if (jsonroot == NULL) 
				{
					HSZDeviceError(HSZ_ERR_MSG_TYPE,NULL,NULL);
				}
				else
				{
					out = cJSON_Print(jsonroot);
					
					DEBUG_ZONE(LOG_ZONE_TEST,"Json:%s.\n",out);

					switch (ntopictype)
					{
						case TYPE_TOPIC_DEVICES:
						{
							DEBUG_ZONE(LOG_ZONE_TEST,"HSZHandleDeviceList:\n");
							DEBUG_ZONE(LOG_ZONE_MY,"Device list: %s.\n",message);
							
							if(TYPE_TOPIC_DEVICES == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							
							HSZHandleDeviceList(jsonroot);
						}
						break;

						case TYPE_TOPIC_DEVICESJOINED:
						{
							DEBUG_ZONE(LOG_ZONE_TEST,"HSZHandleDeviceJoined:\n");
							DEBUG_ZONE(LOG_ZONE_MY,"Device Join:%s.\n",message);
							
							if(TYPE_TOPIC_DEVICESJOINED == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							HSZHandleDeviceJoined(jsonroot);
						}
						break;
																	
						case TYPE_TOPIC_DEVICELEFT:
						{
							DEBUG_ZONE(LOG_ZONE_TEST,"HSZHandleDeviceLeft:\n");
							DEBUG_ZONE(LOG_ZONE_MY,"Device Left:%s.\n",message);
							
							if(TYPE_TOPIC_DEVICELEFT == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							
							HSZHandleDeviceLeft(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_ZCLRESPONSE:
						{
							DEBUG_ZONE(LOG_ZONE_TEST,"HSZHandleZCLResponse:\n");
							DEBUG_ZONE(LOG_ZONE_MY,"ZCL Response: %s.\n",message);
							
							if(TYPE_TOPIC_ZCLRESPONSE == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							
							HSZHandleZCLResponse(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_DECEICE_STATE_CHANGE:
						{
							DEBUG_ZONE(LOG_ZONE_TEST,"HSZHandleDeveiceStateChange:\n");
							DEBUG_ZONE(LOG_ZONE_TEST,"Device Change Response:%s.\n",message);
							
							if(TYPE_TOPIC_DECEICE_STATE_CHANGE == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							
							HSZHandleDeveiceStateChange(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_RELAYS:
						{
							DEBUG_ZONE(LOG_ZONE_TEST,"TYPE_TOPIC_RELAYS\n");
							DEBUG_ZONE(LOG_ZONE_TEST,"Relays Message:%s.\n",message);
							
							if(TYPE_TOPIC_RELAYS == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							HSZHandleRelays(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_SETTINGS:
						{
							DEBUG_ZONE(LOG_ZONE_TEST,"HSZHandleSettings:\n");
							DEBUG_ZONE(LOG_ZONE_TEST,"Setting Message:%s.\n",message);
							
							if(TYPE_TOPIC_SETTINGS == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							HSZHandleSettings(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_EXECUTED:
						{
							DEBUG_ZONE(LOG_ZONE_TEST,"HSZHandleExecuted:\n");
							DEBUG_ZONE(LOG_ZONE_TEST,"Executed Message:%s.\n",message);
							
							if(TYPE_TOPIC_EXECUTED == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							HSZHandleExecuted(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_OTAEVENT:
						{
							DEBUG_ZONE(LOG_ZONE_TEST,"HSZHandleOtaevent:\n");
							DEBUG_ZONE(LOG_ZONE_TEST,"OTA Message:%s.\n",message);
							
							if(TYPE_TOPIC_OTAEVENT == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							HSZHandleOtaevent(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_APSRESPONSE:
						{
							DEBUG_ZONE(LOG_ZONE_TEST,"HSZHandleAPSResponse:\n");
							DEBUG_ZONE(LOG_ZONE_TEST,"Aps Response:%s.\n",message);
							
							if(TYPE_TOPIC_APSRESPONSE == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							HSZHandleAPSResponse(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_DEVLINESTCH:
						{
							DEBUG_ZONE(LOG_ZONE_TEST,"TYPE_TOPIC_DEVLINESTCH:\n");
							DEBUG_ZONE(LOG_ZONE_MY,"Device Line change State: %s.\n",message);
							
							if(TYPE_TOPIC_DEVLINESTCH == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							
							HSZHandleDeviceLineStateChange(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_ZDO_RESPONSE:
						{
							DEBUG_ZONE(LOG_ZONE_TEST,"TYPE_TOPIC_ZDO_RESPONSE:\n");
							DEBUG_ZONE(LOG_ZONE_TEST,"Device ZDO Response: %s.\n",message);
							
							if(TYPE_TOPIC_ZDO_RESPONSE == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							
							HSZHandleZdoResponse(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_NODEID_CHANGE:
						{
							DEBUG_ZONE(LOG_ZONE_MY,"****************************************************\n");
							DEBUG_ZONE(LOG_ZONE_MY,"TYPE_TOPIC_NODEID_CHANGE:%s.\n",message);
							DEBUG_ZONE(LOG_ZONE_MY,"****************************************************\n");
							
							if(TYPE_TOPIC_NODEID_CHANGE == sub_topic)
							{
								if (hsbk_top!=NULL)
								{
									(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
								}
							}
							
							HSZHandleNodeIdChanged(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_NCP_RESET:
						{
							DEBUG_ZONE(LOG_ZONE_MY,"**************NCP Rest,NCP Reset*********************************\n");
							DEBUG_ZONE(LOG_ZONE_MY,"TYPE_TOPIC_NODEID_CHANGE:%s.\n",message);
							DEBUG_ZONE(LOG_ZONE_MY,"**************NCP Rest,NCP Reset*********************************\n");
							
							HSZHandleNcpReset(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_PERMIT_JOIN_STATE:
						{
							DEBUG_ZONE(LOG_ZONE_MY,"^^^^^^^^^^^^^^^^^^Permit Join Status^^^^^^^^^^^^^^^^^^\n");
							DEBUG_ZONE(LOG_ZONE_MY,"TYPE_TOPIC_PERMIT_JOIN_STATE:%s.\n",message);
							DEBUG_ZONE(LOG_ZONE_MY,"^^^^^^^^^^^^^^^^^^Permit Join Status^^^^^^^^^^^^^^^^^^\n");
							
							HSZHandlePermitJoinState(jsonroot);
							
						}
						break;
						
						case TYPE_TOPIC_NCP_VERSION:
						{
							DEBUG_ZONE(LOG_ZONE_MY,"@@@@@@@@@@@@@@@@@NCP Version@@@@@@@@@@@@@@@@@\n");
							DEBUG_ZONE(LOG_ZONE_MY,"TYPE_TOPIC_NCP_VERSION:%s.\n",message);
							DEBUG_ZONE(LOG_ZONE_MY,"@@@@@@@@@@@@@@@@@NCP Version@@@@@@@@@@@@@@@@@\n");
							
							HSZHandleNcpVersion(jsonroot);
						}
						break;
						
						case TYPE_TOPIC_ZIGBEE_NETWORK_STATE:
						{
							DEBUG_ZONE(LOG_ZONE_MY,"^^^^^^^^^^^^^^^^^^^Network state^^^^^^^^^^^^^^^^^^^\n");
							DEBUG_ZONE(LOG_ZONE_MY,"TYPE_TOPIC_ZIGBEE_NETWORK_STATE:%s.\n",message);
							DEBUG_ZONE(LOG_ZONE_MY,"^^^^^^^^^^^^^^^^^^^Network state^^^^^^^^^^^^^^^^^^^\n");
							
							HSZHandleZigbeeNetworkState(jsonroot);
							
						}
						break;
						
						default:
						break;
					}

					cJSON_Delete(jsonroot);
					free(out);
				}
			}
			else
			{
				HSZDeviceError(HSZ_ERR_MSG_TOO_LONG,"Message too many to handle",NULL);
				DEBUG_ZONE(LOG_ZONE_TEST,"Message too many to handle.\n");
			}
		}
	}
	
}

/*****************************************************************************
 函 数 名  : HSZHaGatewayThread
 功能描述  : 检测HaGateway运行是否OK的线程
 输入参数  : void *arg  
 输出参数  : 无
 返 回 值  : void

 修改历史      :
  1.日    期   : 2017年6月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数
*****************************************************************************/
void *HSZHaGatewayThread(void *arg)
{
	time_t  heatbeat_interval = 0;

	while(1)
	{	
		{		
			if(0x00 != heatbeat_time)
			{
				time(&time_now);

				heatbeat_interval = ((time_now > heatbeat_time) ? (time_now - heatbeat_time):(heatbeat_time - time_now));

				// DEBUG_ZONE(LOG_ZONE_THREAD,"The heatbeat_interval:%d.\n",heatbeat_interval);
				
				if(heatbeat_interval<86400)
				{
					if(heatbeat_interval > HEATBEAT_TEST_TIME)
					{
						DEBUG_ZONE(LOG_ZONE_MY,"The %d seconds not receive HaGatewayReferenceHost heatbeat.\n",HEATBEAT_TEST_TIME);

						/*重启GatewayHost程序*/
						system("echo 1 > /tmp/restart_HsZigbeeSvr");//写文件
					}
				}
			}		
		}
	
		usleep(1000000);
	}
	
	DEBUG_ZONE(LOG_ZONE_THREAD,"HSZHaGatewayThread exit(0).\n");
	
	pthread_exit(0);
}
/*****************************************************************************
 函 数 名  : HSZCreateThread
 功能描述  : 创建线程
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年6月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZCreateThread(void)
{
	int ret = 0;
	
	pthread_t hagateway_id;
	
	ret = pthread_create(&hagateway_id,NULL,HSZHaGatewayThread,NULL);
	
	if(ret)
	{
		DEBUG_ZONE(LOG_ZONE_THREAD,"Create hagateway failed.\n");
		return 1;
	}
	
	return ret;
}

/*****************************************************************************
 函 数 名  : recv_msg
 功能描述  : 接收消息
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年1月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void recv_msg(char *ptopic, char *pmsg)
{
	/*Receive and Process Message*/
	DEBUG_ZONE(LOG_ZONE_TEST,"Receive,Topic:%s,Mesaage%s.\n",ptopic,pmsg);

	HSZHandleMessage(ptopic,pmsg);
	
	fflush(stdout);
}
/*****************************************************************************
 函 数 名  : HSZInit
 功能描述  : 系统初始化
 输入参数  : void  
 输出参数  : 无
 返 回 值  :  
 
 修改历史      :
  1.日    期   : 2017年6月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZInit(void)
{
	int rc = -1;
	
	pthread_mutex_init(&cs_log,NULL);
	HSZHeatbeatLockInit();
	
	/*打开Log配置信息文件 */
	HSZGetLogSetInfo();
	
	if (hsbk_onerror == NULL)
	{
		// DEBUG_ZONE(LOG_ZONE_ERROR,"Not set callback for onerror，please retry again.\n");
	}
	
	// HSZInitLineOnDeviceInfoBuffer();
	
	HSZInitJoinOnstatusInfoBuffer();
	
	create_ha_client_thread(recv_msg);
	
	HSZCreateThread();
	
	return rc;
}

/*****************************************************************************
 函 数 名  : HSZDeinit
 功能描述  :  取消初始化
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年7月26日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZDeinit(void)
{
	int rc = 0;
	
	/*关闭文件 */
	HSZCloseLogFile();
	
	pthread_mutex_destroy(&cs_log);
	
	return rc;
}

/*****************************************************************************
 函 数 名  : HSZGetDebugCode
 功能描述  : 获取log输出的级别
 输入参数  : char *path  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
DEBUG_LOGLEVEL HSZGetDebugCode(char *path)
{ 
	int code = LOG_ZONE_NONE; 
	
	if(strstr(path,"LOG_ZONE_SUBSCRIBE")!=NULL)
	{
		if(code == -1)
		{
			code = LOG_ZONE_SUBSCRIBE;
		}
		else
		{
			code |= LOG_ZONE_SUBSCRIBE;
		}
	}		
	
	if(strstr(path,"LOG_ZONE_PUBLIC")!=NULL)
	{
		if(code == -1)
		{
			code = LOG_ZONE_PUBLIC;
		}
		else
		{
			code |= LOG_ZONE_PUBLIC;
		}
	}		
	
	if(strstr(path,"LOG_ZONE_INPACK_MSG")!=NULL)
	{
		if(code == -1)
		{
			code = LOG_ZONE_INPACK_MSG;
		}
		else
		{
			code |= LOG_ZONE_INPACK_MSG;
		}
	}
	
	if(strstr(path,"LOG_ZONE_OUTPACK_MSG")!=NULL)
	{
		if(code == -1)
		{
			code = LOG_ZONE_OUTPACK_MSG;
		}
		else
		{
			code |= LOG_ZONE_OUTPACK_MSG;
		}
	}
	
	if(strstr(path,"LOG_ZONE_ERROR")!=NULL)
	{
		if(code == -1)
		{
			code = LOG_ZONE_ERROR;
		}
		else
		{
			code |= LOG_ZONE_ERROR;
		}
	}
	
	if(strstr(path,"LOG_ZONE_ZCL_DATA")!=NULL)
	{
		if(code == -1)
		{
			code = LOG_ZONE_ZCL_DATA;
		}
		else
		{
			code |= LOG_ZONE_ZCL_DATA;
		}
	}
	
	if(strstr(path,"LOG_ZONE_TEST")!=NULL)
	{
		if(code == -1)
		{
			code = LOG_ZONE_TEST;
		}
		else
		{
			code |= LOG_ZONE_TEST;
		}
	}
	
	if(strstr(path,"LOG_ZONE_MQTT")!=NULL)
	{
		if(code == -1)
		{
			code = LOG_ZONE_MQTT;
		}
		else
		{
			code |= LOG_ZONE_MQTT;
		}
	}
	
	if(strstr(path,"LOG_ZONE_THREAD")!=NULL)
	{
		if(code == -1)
		{
			code = LOG_ZONE_THREAD;
		}
		else
		{
			code |= LOG_ZONE_THREAD;
		}
	}
	
	if(strstr(path,"LOG_ZONE_MY")!=NULL)
	{
		if(code == -1)
		{
			code = LOG_ZONE_MY;
		}
		else
		{
			code |= LOG_ZONE_MY;
		}
	}
	
	if(strstr(path,"LOG_ZONE_DEVICE_DATA")!=NULL)
	{
		if(code == -1)
		{
			code = LOG_ZONE_DEVICE_DATA;
		}
		else
		{
			code |= LOG_ZONE_DEVICE_DATA;
		}
	}
	
	if(strstr(path,"LOG_ZONE_ANY")!=NULL)
	{

		code = LOG_ZONE_ANY;
	}
	
	return (DEBUG_LOGLEVEL)code; 
} 

/*****************************************************************************
 函 数 名  : HSZGetDebugOnOFF
 功能描述  : 获取log信息输出，返回值01为输出到屏幕，返回值02表示输出到文件
 输入参数  : char *path  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZGetDebugOnOFF(char *path)
{
	int debug_info = -1;
	
	if(strstr(path,"SDK_DEBUG_ON_FILE")!=NULL)     /*写Log到文件*/
	{
		debug_info  = 0x02;
	}
	else if((strstr(path,"SDK_DEBUG_ON_PRINT")!=NULL))/*写文件到终端显示*/
	{
		debug_info  = 0x01;
	}
	else
	{
		;
	}
	return debug_info;
}
/*****************************************************************************
 函 数 名  : 获取log信息设置
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZGetLogSetInfo(void)
{ 
	char path[512]={0x0}; 
	
	getcwd(path,sizeof(path)); 
	
	printf("Lib log path：%s.\n",path);
	
	strcat(path,"/log/log.conf"); 
	
	FILE *fpath = fopen(path,"r"); 
	
	if(fpath == NULL) 
	{
		printf("HSZGetLogSetInfo error.\n");
		
		return;
	}
    else
    {
		/*打开文件 */
		HSZOpenLogFile();
	}		
	
	fscanf(fpath,"path=%s\n",value); 
	
	printf("value:%s.\n",value);
	
	fclose(fpath); 
} 

/*****************************************************************************
 函 数 名  : HSZCheckDir
 功能描述  : 判断某个目录是否存在，不存在创建该目录，创建成功或者存在返回1，否则-1
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年11月01日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZCheckDir(char *dir)
{
	DIR *checkdir = NULL;
	
	if((checkdir=opendir(dir))==NULL)/*判断目录是否存在*/
	{
		int ret = mkdir(dir,MODE);/*创建目录*/
		
		if(ret==0)
		{
			//printf("The file created success.\n");
			
			return 1;/*创建成功*/	
		}
		else
		{
			printf("The file created failed.\n");
			
			return 0;/*创建失败*/
		}
	}
	else
	{
		//printf("The file exist.\n");
		
		return 1;/*文件存在*/
	}
}
/*****************************************************************************
 函 数 名  : HSZOpenLogFile
 功能描述  : 打开log文件获取要写入的文件
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月26日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZOpenLogFile(void)
{
	if(0x01 == HSZCheckDir("./log"))
	{
		printf("log file exit.\n");
	}
	else
	{
		printf("log file not exit.\n");
		
		return ;
	}
	
	MiddleWareFp = fopen(logmidwarename1,"a");
	
	if(MiddleWareFp==NULL)
	{
		printf("Openfile failed.\n");
	}
	
	log_file_index = 0x01;
	
	if(ftell(MiddleWareFp)<MAXMIDLOGSIZE)
	{
		fclose(MiddleWareFp);
		remove(logmidwarename1);
		MiddleWareFp = fopen(logmidwarename1,"a");
	
		return;
	}
	else
	{
		fclose(MiddleWareFp);
		
		MiddleWareFp = fopen(logmidwarename2,"a");
		
		log_file_index = 0x02;
	}
    
	
	if(ftell(MiddleWareFp)<MAXMIDLOGSIZE)
	{
		fclose(MiddleWareFp);
		
		remove(logmidwarename2);
		
		MiddleWareFp = fopen(logmidwarename2,"a");
		
		return;
	}
	else
	{
		fclose(MiddleWareFp);
		
		MiddleWareFp = fopen(logmidwarename3,"a");
		
		log_file_index = 0x03;
	}
	
	
	if(ftell(MiddleWareFp)<MAXMIDLOGSIZE)
	{
		fclose(MiddleWareFp);
		
		remove(logmidwarename3);
		
		MiddleWareFp = fopen(logmidwarename3,"a");
		
		return;
	}
	else
	{
		fclose(MiddleWareFp);
		
		MiddleWareFp = fopen(logmidwarename4,"a");
		
		log_file_index = 0x04;
	}
	
	
	if(ftell(MiddleWareFp)<MAXMIDLOGSIZE)
	{
		fclose(MiddleWareFp);
		
		remove(logmidwarename4);
		
		MiddleWareFp = fopen(logmidwarename4,"a");
		
		return;
	}
	else
	{
		fclose(MiddleWareFp);
		
		MiddleWareFp = fopen(logmidwarename5,"a");
		
		log_file_index = 0x05;
	}
	
	
	if(ftell(MiddleWareFp)<MAXMIDLOGSIZE)
	{
		fclose(MiddleWareFp);
		
		remove(logmidwarename5);
		
		MiddleWareFp = fopen(logmidwarename5,"a");
		
		return;
	}
	else
	{
		fclose(MiddleWareFp);
		
		remove(logmidwarename1);
		
		MiddleWareFp = fopen(logmidwarename1,"a");
		
		log_file_index = 0x01;
		
		return ;
	}
	
}
/*****************************************************************************
 函 数 名  : HSZCloseLogFile
 功能描述  : 关闭要写入的文件
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月26日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZCloseLogFile(void)
{
	/*关闭文件 */
	if(MiddleWareFp!=NULL)
	{
		fclose(MiddleWareFp);
	}
}

/*****************************************************************************
 函 数 名  : HSZWriteLogFile
 功能描述  : 写文件
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月26日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZWriteLogFile(void)
{
	if (ftell(MiddleWareFp)<MAXMIDLOGSIZE) 
	{
		return;
	}
	
	switch(log_file_index)
	{
		case 0x01:
		{
			remove(logmidwarename2);
			fclose(MiddleWareFp);
			MiddleWareFp = fopen(logmidwarename2,"a");
			log_file_index = 0x02;
		}
		break;
		
		case 0x02:
		{
			remove(logmidwarename3);
			fclose(MiddleWareFp);
			MiddleWareFp = fopen(logmidwarename3,"a");
			log_file_index = 0x03;
		}
		break;
		
		case 0x03:
		{
			remove(logmidwarename4);
			fclose(MiddleWareFp);
			MiddleWareFp = fopen(logmidwarename4,"a");
			log_file_index = 0x04;
		}
		break;
		
		case 0x04:
		{
			remove(logmidwarename5);
			fclose(MiddleWareFp);
			MiddleWareFp = fopen(logmidwarename5,"a");
			log_file_index = 0x05;
		}
		break;
		
		case 0x05:
		{
			remove(logmidwarename1);
			fclose(MiddleWareFp);
			MiddleWareFp = fopen(logmidwarename1,"a");
			log_file_index = 0x01;
		}
		break;

		default:
		{
			if(NULL!=MiddleWareFp)
			{
				fclose(MiddleWareFp);
				remove(logmidwarename1);
				MiddleWareFp = fopen(logmidwarename1,"a");
				log_file_index = 0x01;
			}
		}
		break;
	}
}
