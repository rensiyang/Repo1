/******************************************************************************

                  ��Ȩ���� (C), 2017-2025, �ൺ�����ǻۼҾ�ϵͳ�ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : HSZCommon.h
  �� �� ��   : ����
  ��    ��   : Zigbee Project Team
  ��������   : 2017��6��9��
  ����޸�   :
  ��������   : ͨ�ô���ӿں��������Լ�ȫ�ֱ������ṹ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��6��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ļ�

  2.��    ��   : 2017��6��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : ����Log��ģ����ƽӿ�

******************************************************************************/
#ifndef __HSZCOMMON_H__
#define __HSZCOMMON_H__

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "HSZInterface.h"	


#define  CRITICAL_SECTION   				pthread_mutex_t
#define  MAXMIDLOGSIZE  					1000000           /* Log��Ϣ�ļ���С���ֽڣ�*/


/* ��Ϣ�궨�� */
#define DEBUG_ZONE(zone,args...) 			DEBUG(zone,__FILE__,__FUNCTION__,__LINE__,args)
/* MQTT QOS Levle */			
#define QOS_LEVEL             				1
	
/* �����ϲ�ص����� */
#define CALLBACK_SUBSCRIBE_MSG  		    1     	
#define CALLBACK_ON_ERROR     	            2	 	/* ������ϢCallback     		*/
#define CALLBACK_GATEWAY_STATE    	        3	 	/* ����״̬Callback     		*/

#define CALLBACK_DEVICE_JOIN      			4   	/* �豸����Callback     		*/
#define CALLBACK_DEVICE_LEAVE				5   	/* �豸����Callback     		*/
#define CALLBACK_DEVICE_LIST                6   	/* �豸�б�Callback     		*/
#define CALLBACK_CMD_SEND                	7   	/* �����Callback     		*/
#define CALLBACK_DEVICE_LINE_STATE          8   	/* �豸��������״̬Callback     */

#define CALLBACK_GATEWAY_PERMIT_JOIN_STATE  9		/* �����Ƿ���������Callback     */

#define CALLBACK_COMMAND_EXECUTE            10		/* ����ִ��Callback     		*/
#define CALLBACK_DEV_SIGNAL_VALUE           11		/* ��ȡ�豸�ź�ǿ��Callback		*/
#define CALLBACK_DEV_ONLINE_LIST            12      /* �����豸�б�Callback			*/
#define CALLBACK_DEV_DATA                   13      /* �ϱ��豸����Callback 		*/
#define CALLBACK_COMMAND_FAILED             14      /* �ϲ��������ִ��ʧ��Callback */
#define CALLBACK_UPDATE_NCP                 15      /* NCP�����������              */

/* ��ӡ���Ʋ��� */
typedef enum{ 
 
    LOG_ZONE_NONE = -1,
    LOG_ZONE_SUBSCRIBE = 1<<0,						/* ��ӡ��������LOG */
    LOG_ZONE_PUBLIC = 1<<1,							/* ��ӡ��������LOG */
    LOG_ZONE_INPACK_MSG = 1<<2,						/* ��ӡ��Ϣ��װ����LOG */
    LOG_ZONE_OUTPACK_MSG = 1<<3,					/* ��ӡ��Ϣ��������LOG */
    LOG_ZONE_ERROR = 1<<4, 							/* ��ӡ������Ϣ����LOG */
	LOG_ZONE_ZCL_DATA = 1<<5, 						/* ��ӡZCL Data������LOG */
	LOG_ZONE_TEST = 1<<6, 							/* ��ӡ��������LOG */
	LOG_ZONE_MQTT = 1<<7, 							/* ��ӡMQTT�ص�LOG */
	LOG_ZONE_THREAD = 1<<8,							/* ��ӡ�߳���ϢLOG */
	LOG_ZONE_MY = 1<<9, 							/* �Լ����Դ�ӡ��LOG */
	LOG_ZONE_DEVICE_DATA = 1<<10, 					/* Device���ݷ���LOG*/
	LOG_ZONE_ANY = 0x11,							/* ��ӡ��������LOG*/
	
}DEBUG_LOGLEVEL;
	
/* Topic ���� */
enum HS_topic_type
{	
	TYPE_TOPIC_ERR = -1,                           

	TYPE_TOPIC_HEARTBEAT = 1,						
	TYPE_TOPIC_DEVICES,								
	TYPE_TOPIC_ZCLRESPONSE,							
	TYPE_TOPIC_DEVICESJOINED,						
	TYPE_TOPIC_DEVICELEFT,							
	TYPE_TOPIC_DECEICE_STATE_CHANGE,				
	TYPE_TOPIC_RELAYS,								
	TYPE_TOPIC_SETTINGS,							
	TYPE_TOPIC_EXECUTED,							
	TYPE_TOPIC_OTAEVENT,							
	TYPE_TOPIC_APSRESPONSE,							
	TYPE_TOPIC_DEVLINESTCH,	
    TYPE_TOPIC_ZDO_RESPONSE,
	TYPE_TOPIC_NODEID_CHANGE,
	TYPE_TOPIC_NCP_RESET,
	TYPE_TOPIC_PERMIT_JOIN_STATE,
	TYPE_TOPIC_NCP_VERSION,
	TYPE_TOPIC_ZIGBEE_NETWORK_STATE,
};
	
/* ������������ */
enum GATEWAY_OFFLINE_REASON
{
	HSZ_GATEWAY_OFFLINE_UNKNOWN = -1,				/* δ֪ԭ������ */
	HSZ_GATEWAY_OFFLINE_NORMAL = 0,					/* �������� */
	HSZ_GATEWAY_OFFLINE_NCP,						/* NCP Error��������*/
	HSZ_GATEWAY_OFFLINE_BROKER,						/* MQTT Error�������� */
	HSZ_GATEWAY_OFFLINE_MQTT_CONNNECT,				/* MQTT DisConnnect ���� */
	HSZ_GATEWAY_OFFLINE_HOST,						/* Host Error �������� */
};

/*�����Ƿ���OTAģʽ*/
enum NCP_OTA_STATUS
{
	NCP_OFF_OTA_MODE = 0,							/* δ��OTAģʽ */
	NCP_ON_OTA_MODE = 0x10,							/* ����OTAģʽ */
};

/*���صĹ���״̬*/	
enum GATEWAY_NETWORK_MODE
{
	GATEWAY_NETWORK_INIT_MODE = 0x11,				/* Network Init */
	GATEWAY_NETWORK_DOWN_MODE = 0x22,				/* Network Down */
	GATEWAY_NETWORK_UP_MODE = 0x33,					/* Network Up */
};


/* Topic���� ���� */
enum enum_HSZN_error
{
	HSZN_ERR_APPNOTDEALSTATECHANGED1	= -10000,   /*App��û�д���״̬�ı仯 */
	HSZN_ERR_UNKNOWN					= -1,      	/*δ֪���ʹ��� */	
	HSZ_ERR_GATEWAY_MAC                 = 0x02,		/*����MAC��ַ��ʽ���� */
	HSZ_ERR_DEVICE_ID                   = 0x03,		/*��ȡDevice ID error */
	HSZ_ERR_GATEWAY_FORM_NETWORK_FAIL   = 0x04,		/*δ�ܻ�ȡ���ص�MAC��ַ */
	HSZ_ERR_DEVICE_LIST                 = 0x05,		/*�洢���豸�б���� */
	HSZ_ERR_FIND_NODEID                 = 0x06,		/*��ȡ�豸�б�NodeID Error */
	HSZ_ERR_FIND_ENDPOINT               = 0x07,		/*��ȡ��Endpoint Error*/
	HSZ_ERR_RECORD_JOIN_DEV             = 0x08,		/*�洢�豸�б�Error */
	HSZ_ERR_MSG_TOO_LONG                = 0x09,		/*Topic Message ̫�����޷����� */
	HSZ_ERR_MSG_TYPE                    = 0x0A,		/*Topic Message ���ݴ��� */
	
	HSZ_ERR_PRIVATE_MSG_TYPE			= 0x0B,		/*˽����Ϣ�������ʹ���*/
	HSZ_ERR_PRIVATE_MSG_CHECKSUM        = 0x0C,		/*����˽����Ϣ����У��ֵ����*/
	HSZ_ERR_UPDATE_NODEID               = 0x0D,     /*����NodeID ����*/
	
	HSZ_ERR_ZCL_DATA_LEN                = 0x0E,     /*����ZCL Response Data���ȳ���*/
	HSZ_ERR_ZCL_ATTR_DATA_TYPE          = 0x0F,		/*����ZCL Response Data�������ͳ���*/
	
	HSZ_ERR_SEND_TOPIC                  = 0x10,     /*����Topicʧ��*/
	
	HSZ_ERR_SMARTLOCK_MSG_CHECKSUM      = 0x11,     /*������ϢУ��ʧ��*/
	HSZ_ERR_SMARTLOCK_MSG_TYPE          = 0x12,     /*������Ϣ���ʹ���*/
	HSZ_ERR_SMARTLOCK_MSG_ERR           = 0x13,     /*������Ϣ�Ĵ���*/
	
};

extern char Zigbee_stack[30];					 	/*ZigbeeЭ��ջ�汾*/
extern int received_join_time;					 	/*���յ���������ʱ��*/
extern int sum_join_time ;						 	/*�Ѿ�ͳ�Ƶ�ʱ��*/
extern struct mosquitto *mosq;


/***********************�ڲ����Իص���غ�������************************************/	
typedef void HSZCallbackSubscribeMsg(char* gw_mac,char*topic,char*msg,int len);
typedef void HSZCallbackGetDeviceList(char* gw_mac,char* nodeid,int dev_state,char* dev_type, char* dev_mac,int endpoint);
typedef void HSZCallbackOnCommandSend(char* gw_mac,char* device_mac,char* command_id,char* status);
typedef void HSZCallbackCommandExecuted(char* gw_mac,time_t curtime,char* command_data);
typedef void HSZCallbackDevSignalValue(char* gw_mac,char* dev_mac,int signal_value);
typedef void HSZCallbackReportOnlineDeviceList(char* gw_mac,char* device_mac,char* data);
/***********************************************************************************/

/***********************��ӡlog��غ�������****************************************/	
int HSZDebugZone(int zone);	
int HSZGetDebugOnOFF(char *path);
void DEBUG(int zone,const char *file,const char *func, int line,const char *pszFmt,...);
DEBUG_LOGLEVEL HSZGetDebugCode(char *path);
void HSZGetLogSetInfo(void);
void HSZOpenLogFile(void);
void HSZCloseLogFile(void);
void HSZWriteLogFile(void);
/***********************************************************************************/

/*����ƥ���ַ���*/
char *strstr_r(const char *s1,const char *s2);  
 	
/*��ȡ�豸�б�*/
int HSZGetDeviceList(char* gw_mac);

/*��ȡ�����豸�б�*/
int HSZGetOnlineDeviceList(char* gw_mac);

/*��ȡ����MAC��ַ*/
char* HSZGatewayMac(void);

/*����Zigbee����*/
int HSZRestartZigBeeService (char* gw_mac);

/*���ö���Topic����*/
int HSZSetSubTopic(char* gw_mac,int topic_index);

/*��ӡ������Ϣ*/
int HSZDeviceError(int errtype,const char* reason,const char*others);

/*MQTT Client��ʼ��*/
int HSZMosquittoInit(void);

/*������Ϣ��MQTT Broker*/
int mosquitto_publish_topic(const char *topic,const char *payload);

/*���ٴ��������߳�*/
int HSZDestoryThread(void);

/***************************�ص�������������****************************************/
extern HSZCallbackSubscribeMsg* hsbk_top;
extern HSZCallbackOnError* hsbk_onerror;
extern HSZCallbackGatewayState* hsbk_gatewaystate;
extern HSZCallbackOnDeviceJoin* hsbk_devicejoin;
extern HSZCallbackOnDeviceLeave* hsbk_deviceleave;
extern HSZCallbackGetDeviceList* hsbk_getdevicelist;
extern HSZCallbackOnCommandSend* hsbk_commandsend;
extern HSZCallbackOnDeviceLineState* hsbk_devicelineon;
extern HSZCallbackGatewayPermitJoinState* hsbk_gatewaypermitjoinstate;
extern HSZCallbackCommandExecuted* hsbk_cmdexcuted;
extern HSZCallbackDevSignalValue* hsbk_devsignalvalue;
extern HSZCallbackReportOnlineDeviceList* hsbk_devonlinelist;
extern HSZCallbackDeviceData* hsbk_devicedata;
extern HSZCallbackCommandFailed* hsbk_commandfailed;
extern HSZCallbackNcpUpdateResult* hsbk_ncpupdateresult;
/***********************************************************************************/

#endif
