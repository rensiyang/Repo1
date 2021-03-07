#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


/* 设备类型（Device Type）*/
#define 	ON_OFF_SWITCH     					0x0000          /* On/Off Switch*/
#define     LEVEL_CONTROL_SWITCH				0x0104          /* Dimmmer Switch */

#define     CONTACT_SENSOR 						0x0402			/* Contact sensor*/
#define     ON_OFF_LIGHT                        0x0100			/* On/Off Light*/

#define     SMART_PLUG                  		0x0051          /* Smart plug*/

#define     MAINS_POWER_OUTLET                  0x0009			/* Main Power OUTLET */

#define     OCCUPANCY_SENSOR					0x0107			/* Occupancy Sensor */

#define     DIMMABLE_LIGHT                      0x0101          /* Dimmable Light  */
#define     COLOR_DIMMABLE_LIGHT                0x0102          /* Color Dimmable Light */

/* On-Off Switch 开关设置*/
#define  	SET_ON_OFF_SWITCH_ON					0x01           /* Switch On*/  
#define		SET_ON_OFF_SWITCH_OFF					0x02           /* Switch Off*/
#define		SET_ON_OFF_SWITCH_TOOGLE				0x03           /* Switch Toggle*/

/* Level Control Switch 设置*/
#define		SET_LEVEL_CONTROL_SWITCH_LEVEL          0x01     	/* Set Level Control Switch level*/
#define		SET_LEVEL_CONTROL_SWITCH_NOCHANGE		0x02		/* Set Level Control Switch no change*/
#define		GET_LEVEL_CONTROL_SWITCH__LEVEL			0x03		/* Get Level Control Switch levle*/

/* Level Control Switch 应答*/
#define		RESPONSE_LEVEL_CONTROL_SWITCH_LEVEL    0x05		/* Response Level Control Switch Levle*/

/* Contact sensor 应答*/
#define		RESPONSE_CONTACT_SENSOR_STATUS			0x01		/* Response Contact sensor status*/

/* On-Off light 开关设置*/
#define  	ON_OFF_LIGHT_ON						0x01           /* Light On*/  
#define		ON_OFF_LIGHT_OFF					0x02           /* Light Off*/
#define		ON_OFF_LIGHT_TOOGLE					0x03           /* Light Toggle*/

/* Level Control Switch 应答*/
#define		RESPONSE_ON_OFF_LIGHT_STATUS    	0x04			/* Response ON-OFF light status*/

/* 三合一传感器 */

#define     RESPONSE_THREE_REPORT_DATA          0x03           /*上报三合一数据*/
#define     RESPONSE_THREE_PARA_DATA            0x04           /*上报三合一配置数据*/ 



/* Topic错误 类型 */
enum enum_HSZN_error
{
	APP_ERR_APPNOTDEALSTATECHANGED1	= -10000,/*App层没有处理状态的变化 */

	APP_ERR_UNKNOWN	= -1,

	APP_ERR_GATEWAY_MAC = 0x02,

	APP_ERR_DEVICE_ID = 0x03,

	APP_ERR_GATEWAY_FORM_NETWORK_FAIL = 0x04,

	APP_ERR_DEVICE_LIST = 0x05,

};


typedef enum{ 
 
    LOG_APP_TOPIC = 1<<0,/* APP打印TOPIC LOG */
    LOG_APP_TEST = 1<<1,/* APP打印测试型LOG */
    LOG_APP_DEV_LIST = 1<<2,/* APP打印设备列表LOG */ 
    LOG_APP_JOIN_ON = 1<<3,/* APP打印网关设备允许入网LOG */
    LOG_APP_JOIN_OFF = 1<<4,/* APP打印网关设备停止入网LOG */
	LOG_APP_ERROR = 1<<5,/* APP打印ERROR LOG */
	LOG_APP_CMD = 1<<6,/* APP打印控制设备指令LOG */
	LOG_APP_BINDS = 1<<7,/* APP打印绑定LOG */
	LOG_APP_DATA = 1<<8,/* APP打印设备数据LOG */
	LOG_APP_OTHER = 1<<9, /* APP其他打印LOG */
	LOG_APP_ZONE_ANY = 0x11,/*打印所有类型APP LOG*/
}LOGLEVEL;


#define MAXLOGSIZE 20000000
#define  CRITICAL_SECTION   pthread_mutex_t
	
int HSZSDKDebugZone(int zone);	
LOGLEVEL getcode(char *path);
int HSZGetAppDebugOnOFF(char *path);



void APP_DEBUG(int zone,const char *file,const char *func, int line,const char *pszFmt,...);


#define DEBUG_APP_ZONE(zone,args...) APP_DEBUG(zone,__FILE__,__FUNCTION__,__LINE__,args)

void getlogset(void);

char logfilename1[]="./log/AppLog1.log";
char logfilename2[]="./log/AppLog2.log";

FILE *App_fp;

char app_value[512] ={0x0}; 

// time_t timer = time(NULL); 

char logtime[20];	

CRITICAL_SECTION app_cs_log;
