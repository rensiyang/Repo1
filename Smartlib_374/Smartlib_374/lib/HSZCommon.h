/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZCommon.h
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2017年6月9日
  最近修改   :
  功能描述   : 通用处理接口函数声明以及全局变量，结构体等
  函数列表   :
  修改历史   :
  1.日    期   : 2017年6月9日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

  2.日    期   : 2017年6月22日
    作    者   : Zigbee Project Team
    修改内容   : 加了Log分模块控制接口

******************************************************************************/
#ifndef __HSZCOMMON_H__
#define __HSZCOMMON_H__

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "HSZInterface.h"	


#define  CRITICAL_SECTION   				pthread_mutex_t
#define  MAXMIDLOGSIZE  					1000000           /* Log信息文件大小（字节）*/


/* 消息宏定义 */
#define DEBUG_ZONE(zone,args...) 			DEBUG(zone,__FILE__,__FUNCTION__,__LINE__,args)
/* MQTT QOS Levle */			
#define QOS_LEVEL             				1
	
/* 网关上层回调种类 */
#define CALLBACK_SUBSCRIBE_MSG  		    1     	
#define CALLBACK_ON_ERROR     	            2	 	/* 错误信息Callback     		*/
#define CALLBACK_GATEWAY_STATE    	        3	 	/* 网关状态Callback     		*/

#define CALLBACK_DEVICE_JOIN      			4   	/* 设备入网Callback     		*/
#define CALLBACK_DEVICE_LEAVE				5   	/* 设备退网Callback     		*/
#define CALLBACK_DEVICE_LIST                6   	/* 设备列表Callback     		*/
#define CALLBACK_CMD_SEND                	7   	/* 命令发送Callback     		*/
#define CALLBACK_DEVICE_LINE_STATE          8   	/* 设备上线离线状态Callback     */

#define CALLBACK_GATEWAY_PERMIT_JOIN_STATE  9		/* 网关是否允许入网Callback     */

#define CALLBACK_COMMAND_EXECUTE            10		/* 命令执行Callback     		*/
#define CALLBACK_DEV_SIGNAL_VALUE           11		/* 获取设备信号强度Callback		*/
#define CALLBACK_DEV_ONLINE_LIST            12      /* 在线设备列表Callback			*/
#define CALLBACK_DEV_DATA                   13      /* 上报设备数据Callback 		*/
#define CALLBACK_COMMAND_FAILED             14      /* 上层控制命令执行失败Callback */
#define CALLBACK_UPDATE_NCP                 15      /* NCP升级结果反馈              */

/* 打印控制参数 */
typedef enum{ 
 
    LOG_ZONE_NONE = -1,
    LOG_ZONE_SUBSCRIBE = 1<<0,						/* 打印订阅类型LOG */
    LOG_ZONE_PUBLIC = 1<<1,							/* 打印发布类型LOG */
    LOG_ZONE_INPACK_MSG = 1<<2,						/* 打印消息封装类型LOG */
    LOG_ZONE_OUTPACK_MSG = 1<<3,					/* 打印消息解析类型LOG */
    LOG_ZONE_ERROR = 1<<4, 							/* 打印错误信息类型LOG */
	LOG_ZONE_ZCL_DATA = 1<<5, 						/* 打印ZCL Data层类型LOG */
	LOG_ZONE_TEST = 1<<6, 							/* 打印测试类型LOG */
	LOG_ZONE_MQTT = 1<<7, 							/* 打印MQTT回调LOG */
	LOG_ZONE_THREAD = 1<<8,							/* 打印线程消息LOG */
	LOG_ZONE_MY = 1<<9, 							/* 自己调试打印的LOG */
	LOG_ZONE_DEVICE_DATA = 1<<10, 					/* Device数据分析LOG*/
	LOG_ZONE_ANY = 0x11,							/* 打印所有类型LOG*/
	
}DEBUG_LOGLEVEL;
	
/* Topic 类型 */
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
	
/* 网关离网类型 */
enum GATEWAY_OFFLINE_REASON
{
	HSZ_GATEWAY_OFFLINE_UNKNOWN = -1,				/* 未知原因离网 */
	HSZ_GATEWAY_OFFLINE_NORMAL = 0,					/* 正常离网 */
	HSZ_GATEWAY_OFFLINE_NCP,						/* NCP Error导致离网*/
	HSZ_GATEWAY_OFFLINE_BROKER,						/* MQTT Error导致离网 */
	HSZ_GATEWAY_OFFLINE_MQTT_CONNNECT,				/* MQTT DisConnnect 离网 */
	HSZ_GATEWAY_OFFLINE_HOST,						/* Host Error 导致离网 */
};

/*网关是否处在OTA模式*/
enum NCP_OTA_STATUS
{
	NCP_OFF_OTA_MODE = 0,							/* 未在OTA模式 */
	NCP_ON_OTA_MODE = 0x10,							/* 处在OTA模式 */
};

/*网关的工作状态*/	
enum GATEWAY_NETWORK_MODE
{
	GATEWAY_NETWORK_INIT_MODE = 0x11,				/* Network Init */
	GATEWAY_NETWORK_DOWN_MODE = 0x22,				/* Network Down */
	GATEWAY_NETWORK_UP_MODE = 0x33,					/* Network Up */
};


/* Topic错误 类型 */
enum enum_HSZN_error
{
	HSZN_ERR_APPNOTDEALSTATECHANGED1	= -10000,   /*App层没有处理状态的变化 */
	HSZN_ERR_UNKNOWN					= -1,      	/*未知类型错误 */	
	HSZ_ERR_GATEWAY_MAC                 = 0x02,		/*网关MAC地址格式错误 */
	HSZ_ERR_DEVICE_ID                   = 0x03,		/*获取Device ID error */
	HSZ_ERR_GATEWAY_FORM_NETWORK_FAIL   = 0x04,		/*未能获取网关的MAC地址 */
	HSZ_ERR_DEVICE_LIST                 = 0x05,		/*存储的设备列表错误 */
	HSZ_ERR_FIND_NODEID                 = 0x06,		/*获取设备列表NodeID Error */
	HSZ_ERR_FIND_ENDPOINT               = 0x07,		/*获取的Endpoint Error*/
	HSZ_ERR_RECORD_JOIN_DEV             = 0x08,		/*存储设备列表Error */
	HSZ_ERR_MSG_TOO_LONG                = 0x09,		/*Topic Message 太长，无法处理 */
	HSZ_ERR_MSG_TYPE                    = 0x0A,		/*Topic Message 内容错误 */
	
	HSZ_ERR_PRIVATE_MSG_TYPE			= 0x0B,		/*私有消息数据类型错误*/
	HSZ_ERR_PRIVATE_MSG_CHECKSUM        = 0x0C,		/*接收私有消息数据校验值错误*/
	HSZ_ERR_UPDATE_NODEID               = 0x0D,     /*更新NodeID 出错*/
	
	HSZ_ERR_ZCL_DATA_LEN                = 0x0E,     /*接收ZCL Response Data长度出错*/
	HSZ_ERR_ZCL_ATTR_DATA_TYPE          = 0x0F,		/*接收ZCL Response Data数据类型出错*/
	
	HSZ_ERR_SEND_TOPIC                  = 0x10,     /*发送Topic失败*/
	
	HSZ_ERR_SMARTLOCK_MSG_CHECKSUM      = 0x11,     /*门锁信息校验失败*/
	HSZ_ERR_SMARTLOCK_MSG_TYPE          = 0x12,     /*门锁消息类型错误*/
	HSZ_ERR_SMARTLOCK_MSG_ERR           = 0x13,     /*门锁信息的错误*/
	
};

extern char Zigbee_stack[30];					 	/*Zigbee协议栈版本*/
extern int received_join_time;					 	/*接收的入网配置时间*/
extern int sum_join_time ;						 	/*已经统计的时间*/
extern struct mosquitto *mosq;


/***********************内部调试回调相关函数声明************************************/	
typedef void HSZCallbackSubscribeMsg(char* gw_mac,char*topic,char*msg,int len);
typedef void HSZCallbackGetDeviceList(char* gw_mac,char* nodeid,int dev_state,char* dev_type, char* dev_mac,int endpoint);
typedef void HSZCallbackOnCommandSend(char* gw_mac,char* device_mac,char* command_id,char* status);
typedef void HSZCallbackCommandExecuted(char* gw_mac,time_t curtime,char* command_data);
typedef void HSZCallbackDevSignalValue(char* gw_mac,char* dev_mac,int signal_value);
typedef void HSZCallbackReportOnlineDeviceList(char* gw_mac,char* device_mac,char* data);
/***********************************************************************************/

/***********************打印log相关函数声明****************************************/	
int HSZDebugZone(int zone);	
int HSZGetDebugOnOFF(char *path);
void DEBUG(int zone,const char *file,const char *func, int line,const char *pszFmt,...);
DEBUG_LOGLEVEL HSZGetDebugCode(char *path);
void HSZGetLogSetInfo(void);
void HSZOpenLogFile(void);
void HSZCloseLogFile(void);
void HSZWriteLogFile(void);
/***********************************************************************************/

/*查找匹配字符串*/
char *strstr_r(const char *s1,const char *s2);  
 	
/*获取设备列表*/
int HSZGetDeviceList(char* gw_mac);

/*获取在线设备列表*/
int HSZGetOnlineDeviceList(char* gw_mac);

/*获取网关MAC地址*/
char* HSZGatewayMac(void);

/*重启Zigbee服务*/
int HSZRestartZigBeeService (char* gw_mac);

/*设置订阅Topic类型*/
int HSZSetSubTopic(char* gw_mac,int topic_index);

/*打印错误信息*/
int HSZDeviceError(int errtype,const char* reason,const char*others);

/*MQTT Client初始化*/
int HSZMosquittoInit(void);

/*发布消息给MQTT Broker*/
int mosquitto_publish_topic(const char *topic,const char *payload);

/*销毁创建的子线程*/
int HSZDestoryThread(void);

/***************************回调函数引用声明****************************************/
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
