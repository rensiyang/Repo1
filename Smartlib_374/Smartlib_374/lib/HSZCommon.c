/******************************************************************************

                  ��Ȩ���� (C), 2017-2025, �ൺ�����ǻۼҾ�ϵͳ�ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : HSZCommon.c
  �� �� ��   : ����
  ��    ��   : Zigbee Project Team
  ��������   : 2017��6��9��
  ����޸�   :
  ��������   : ͨ�ô���ӿں�������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��6��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ļ�

  2.��    ��   : 2017��6��23��
    ��    ��   : Zigbee Project Team
    �޸�����   : �޸��ļ��ܹ�

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
char *gatewayid  = NULL ;			/*������ȡ����MAC��ַ*/

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
 �� �� ��  : Lock
 ��������  : д�м��log����
 �������  : CRITICAL_SECTION *l  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void Lock(CRITICAL_SECTION *l) 
{
    pthread_mutex_lock(l);
}

/*****************************************************************************
 �� �� ��  : Unlock
 ��������  : дlog����
 �������  : CRITICAL_SECTION *l  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void Unlock(CRITICAL_SECTION *l) 
{
    pthread_mutex_unlock(l);
}


/*****************************************************************************
 �� �� ��  : HSZDebugZone
 ��������  : �жϴ�ӡ��������
 �������  : int zone  
 �������  : ��
 �� �� ֵ  :  
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��21��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZDebugZone(int zone)
{
	if(-1 == HSZGetDebugCode(value))
	{
		return 0x00;
	}
	
	if(LOG_ZONE_ANY == HSZGetDebugCode(value)) /*��ӡ���е�log*/
	{
		return 0x01;
	}
	
	if((zone & HSZGetDebugCode(value)) != 0x00) /*��ӡ��ӦȨ�޵�log*/
	{
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}

/*****************************************************************************
 �� �� ��  : DEBUG
 ��������  : дlog����
 �������  : int zone            
             const char *file    
             const char *func    
             int line            
             const char *pszFmt  
             ...                 
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
				fprintf(MiddleWareFp,"%s %s",datestr,timestr);/*д����+ʱ��*/
				snprintf(output,1023," <%s> [%s] %05d: ",file,func,line);/*д�ļ���+������+����*/
				fprintf(MiddleWareFp,"  %s",output);
				vfprintf(MiddleWareFp,pszFmt,argp);
				fflush(MiddleWareFp);
				
				HSZWriteLogFile();
 
			}
		}
	    else if(0x01 == HSZGetDebugOnOFF(value))
		{	
			printf("%s %s",datestr,timestr);/*��ӡ����+ʱ��*/
			snprintf(output,1023," <%s> [%s] %05d: ",file,func,line);/*��ӡ�ļ���+������+����*/
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
 �� �� ��  : HSZGatewayMac
 ��������  : ��ȡ����MIC��ַ
 �������  : ��
 �������  : ����MIC��ַ
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : mosquitto_publish_topic
 ��������  : ������Ϣ
 �������  : ��
 �������  : const char *topic��������Ϣ��Topic
			 const char *payload��������Ϣ��Message
 �� �� ֵ  : ����ʧ�ܷ���-1�����ͳɹ����ش���0��ֵ
 
 �޸���ʷ      :
  1.��    ��   : 2018��1��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int mosquitto_publish_topic(const char *topic,const char *payload)//payload="{\"commands\":[{\"command\":\"hswrattrdata 1 0xfd00 0x0003 {%s} {%s}\"}]}",dev_mac,cmd_data
{
	int rc = 0x00;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"Send Host Topic:%s,Message:%s\n",topic,payload);
	
	rc = add_ha_client_msg(topic,payload);
	
	return rc;
}
/*****************************************************************************
 �� �� ��  : substr_c
 ��������  : ��ȡ�ַ���
 �������  : const char*str  Ҫ��ȡ���ַ���
             unsigned start  ��ʼ��ȡ����ʼλ��
             unsigned end    ֹͣ��ȡ��λ��
 �������  : ��ȡ���ַ�������
 �� �� ֵ  :  
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : strstr_r
 ��������  : ƥ���ַ�������
 �������  : const char *s1  
             const char *s2  
 �������  : ��
 �� �� ֵ  : char
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZGetMessageType
 ��������  : ��ȡ������Ϣ����Ϣ���ͣ���Ҫ����heartbeat
 �������  : const struct mosquitto_message *message  ���ĵ�����Ϣ
 �������  : ��Ϣ����
 �� �� ֵ  : ��
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZGetMessageType(const char *topic)
{
	int ntopictype = TYPE_TOPIC_ERR;
	char* chtopictype;
	
	chtopictype = strstr_r(topic,"/heartbeat");

	if (chtopictype != NULL)
	{
		ntopictype = TYPE_TOPIC_HEARTBEAT;/*��Ϣ����:heartbeat */
	}

	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/devices");
		
		if (chtopictype != NULL)
		{
			ntopictype = TYPE_TOPIC_DEVICES;/*��Ϣ����:devices */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/zclresponse");
		
		if (chtopictype != NULL)
		{
			ntopictype = TYPE_TOPIC_ZCLRESPONSE;/*��Ϣ����:zclresponse */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/devicejoined");
		
		if (chtopictype != NULL)
		{
			ntopictype = TYPE_TOPIC_DEVICESJOINED;/*��Ϣ����:devicejoined */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/deviceleft");
		
		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_DEVICELEFT;/*��Ϣ����:deviceleft */
		}
	}

	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/devicestatechange");
		
		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_DECEICE_STATE_CHANGE;/*��Ϣ����:devicestatechange */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/relays");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_RELAYS;/*��Ϣ����:relays */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/settings");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_SETTINGS;/*��Ϣ����:settings */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/executed");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_EXECUTED;/*��Ϣ����:executed */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/otaevent");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_OTAEVENT;/*��Ϣ����:otaevent */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype=strstr_r(topic,"/apsresponse");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_APSRESPONSE;/*��Ϣ����:apsresponse */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/devlinestch");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_DEVLINESTCH;/*��Ϣ����:apsresponse */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/zdoresponse");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_ZDO_RESPONSE;/*��Ϣ����:zdoresponse */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/nodeidchanged");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_NODEID_CHANGE;/*��Ϣ����:NodeID Change */
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/ncpreset");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_NCP_RESET;/*��Ϣ����:NCP �������� */
		}
	}
	
		
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/pjoinres");
		
		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_PERMIT_JOIN_STATE;/*��Ϣ����:��������״̬���� */
		}
	}
	
		
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/ncpverres");

		if (chtopictype!=NULL)
		{
			ntopictype = TYPE_TOPIC_NCP_VERSION;/*��Ϣ����:����NCP�汾*/
		}
	}
	
	if (ntopictype==TYPE_TOPIC_ERR)
	{
		chtopictype = strstr_r(topic,"/znetstatus");

		if (chtopictype!=NULL)
		{
		ntopictype = TYPE_TOPIC_ZIGBEE_NETWORK_STATE;/*��Ϣ����:Zigbee ����״̬�����仯ʱtopic*/
	}
	}
	
	DEBUG_ZONE(LOG_ZONE_TEST,"ntopictype:%d\n", ntopictype);
	
	return ntopictype;
}

/*****************************************************************************
 �� �� ��  : HSZHeatbeatLockInit
 ��������  : ��ʼ��heartbeat��
 �������  : 
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��12��29��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZAcquireHeatbeatLock
 ��������  : heartbeat��
 �������  : 
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��12��29��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZReleaseHeatbeatLock
 ��������  : heartbeat����
 �������  : 
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��12��29��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZGetMacAddrAndUpdateNetwrokStatus
 ��������  : ��ȡ���ص�ַ,����֪ͨ�ϲ��������״̬
 �������  : 
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��1��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
static void HSZGetMacAddrAndUpdateNetwrokStatus(const char *topic)
{
	/*��һ�λ�ȡheatbeat�󣬽�����beatheart,��ȡ���ص�ַ����������*/
	gatewayid = substr_c(topic,3,19);
	DEBUG_ZONE(LOG_ZONE_MY,"Gateway MAC Address:%s.\n",gatewayid);
	
	/* ��һ�λ�ȡTopic����ȡGateway MAC��ַ������Host��ȡ�����豸�б�
	���洢��Ȼ�󽫳�ʼ�����״̬�ϱ��ϲ� */
	HSZGetDeviceList(gatewayid);
	
	if (hsbk_gatewaystate!=NULL)
	{
		hsbk_gatewaystate(HSZGatewayMac(),0x01);
	}		
}
/*****************************************************************************
 �� �� ��  : HSZHandleMessage
 ��������  : ������Ϣ���ʹ�����Ϣ
 �������  : const struct mosquitto_message *message  ���ĵ�����Ϣ
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
		/*���»�ȡhearbeat Time*/
		HSZAcquireHeatbeatLock();
		
		time(&heatbeat_time);
		
		// DEBUG_ZONE(LOG_ZONE_MY,"Heatbeat Message:%s.\n",message);
		
		// DEBUG_ZONE(LOG_ZONE_THREAD,"The heatbeat time is :%ld\n",heatbeat_time);
		
		HSZReleaseHeatbeatLock();
		
		/* ���������Heatbeat �ϱ���Ϣ����*/
		if (hsbk_top!=NULL)
		{
			if(TYPE_TOPIC_HEARTBEAT == sub_topic)
			{
				(*hsbk_top)(HSZGatewayMac(),(char*)topic,(char*)message,strlen(message));
			}
		}

#if 0	    
		/*���ڵ���ʱ��ʱ����ʱ��*/
		if(received_join_time!= 0x00)
		{
			sum_join_time = sum_join_time + 0x05;
			
		    if(sum_join_time>(received_join_time-1))/*����ʱʱ�䵽*/
			{
				if (hsbk_gatewaypermitjoinstate!=NULL)
				{
					hsbk_gatewaypermitjoinstate(HSZGatewayMac(),0x00);/*���������ص�*/
				}
				
				sum_join_time = 0x00;
				received_join_time = 0x00;			
			}
		}

#endif
		
		/*Heartbeat Message ����*/
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
 �� �� ��  : HSZHaGatewayThread
 ��������  : ���HaGateway�����Ƿ�OK���߳�
 �������  : void *arg  
 �������  : ��
 �� �� ֵ  : void

 �޸���ʷ      :
  1.��    ��   : 2017��6��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���
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

						/*����GatewayHost����*/
						system("echo 1 > /tmp/restart_HsZigbeeSvr");//д�ļ�
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
 �� �� ��  : HSZCreateThread
 ��������  : �����߳�
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : recv_msg
 ��������  : ������Ϣ
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��1��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void recv_msg(char *ptopic, char *pmsg)
{
	/*Receive and Process Message*/
	DEBUG_ZONE(LOG_ZONE_TEST,"Receive,Topic:%s,Mesaage%s.\n",ptopic,pmsg);

	HSZHandleMessage(ptopic,pmsg);
	
	fflush(stdout);
}
/*****************************************************************************
 �� �� ��  : HSZInit
 ��������  : ϵͳ��ʼ��
 �������  : void  
 �������  : ��
 �� �� ֵ  :  
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZInit(void)
{
	int rc = -1;
	
	pthread_mutex_init(&cs_log,NULL);
	HSZHeatbeatLockInit();
	
	/*��Log������Ϣ�ļ� */
	HSZGetLogSetInfo();
	
	if (hsbk_onerror == NULL)
	{
		// DEBUG_ZONE(LOG_ZONE_ERROR,"Not set callback for onerror��please retry again.\n");
	}
	
	// HSZInitLineOnDeviceInfoBuffer();
	
	HSZInitJoinOnstatusInfoBuffer();
	
	create_ha_client_thread(recv_msg);
	
	HSZCreateThread();
	
	return rc;
}

/*****************************************************************************
 �� �� ��  : HSZDeinit
 ��������  :  ȡ����ʼ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��26��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZDeinit(void)
{
	int rc = 0;
	
	/*�ر��ļ� */
	HSZCloseLogFile();
	
	pthread_mutex_destroy(&cs_log);
	
	return rc;
}

/*****************************************************************************
 �� �� ��  : HSZGetDebugCode
 ��������  : ��ȡlog����ļ���
 �������  : char *path  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZGetDebugOnOFF
 ��������  : ��ȡlog��Ϣ���������ֵ01Ϊ�������Ļ������ֵ02��ʾ������ļ�
 �������  : char *path  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZGetDebugOnOFF(char *path)
{
	int debug_info = -1;
	
	if(strstr(path,"SDK_DEBUG_ON_FILE")!=NULL)     /*дLog���ļ�*/
	{
		debug_info  = 0x02;
	}
	else if((strstr(path,"SDK_DEBUG_ON_PRINT")!=NULL))/*д�ļ����ն���ʾ*/
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
 �� �� ��  : ��ȡlog��Ϣ����
 ��������  : 
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void HSZGetLogSetInfo(void)
{ 
	char path[512]={0x0}; 
	
	getcwd(path,sizeof(path)); 
	
	printf("Lib log path��%s.\n",path);
	
	strcat(path,"/log/log.conf"); 
	
	FILE *fpath = fopen(path,"r"); 
	
	if(fpath == NULL) 
	{
		printf("HSZGetLogSetInfo error.\n");
		
		return;
	}
    else
    {
		/*���ļ� */
		HSZOpenLogFile();
	}		
	
	fscanf(fpath,"path=%s\n",value); 
	
	printf("value:%s.\n",value);
	
	fclose(fpath); 
} 

/*****************************************************************************
 �� �� ��  : HSZCheckDir
 ��������  : �ж�ĳ��Ŀ¼�Ƿ���ڣ������ڴ�����Ŀ¼�������ɹ����ߴ��ڷ���1������-1
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��01��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZCheckDir(char *dir)
{
	DIR *checkdir = NULL;
	
	if((checkdir=opendir(dir))==NULL)/*�ж�Ŀ¼�Ƿ����*/
	{
		int ret = mkdir(dir,MODE);/*����Ŀ¼*/
		
		if(ret==0)
		{
			//printf("The file created success.\n");
			
			return 1;/*�����ɹ�*/	
		}
		else
		{
			printf("The file created failed.\n");
			
			return 0;/*����ʧ��*/
		}
	}
	else
	{
		//printf("The file exist.\n");
		
		return 1;/*�ļ�����*/
	}
}
/*****************************************************************************
 �� �� ��  : HSZOpenLogFile
 ��������  : ��log�ļ���ȡҪд����ļ�
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��26��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZCloseLogFile
 ��������  : �ر�Ҫд����ļ�
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��26��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void HSZCloseLogFile(void)
{
	/*�ر��ļ� */
	if(MiddleWareFp!=NULL)
	{
		fclose(MiddleWareFp);
	}
}

/*****************************************************************************
 �� �� ��  : HSZWriteLogFile
 ��������  : д�ļ�
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��26��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
