/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZDeviceData.h
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2017年8月11日
  最近修改   :
  功能描述   : 描述上层与中间层设备控制指令跟应答通信数据
  函数列表   :
  修改历史   :
  1.日    期   : 2017年8月11日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

******************************************************************************/
#ifndef _HSZDEVICEDATA_H_
#define _HSZDEVICEDATA_H_

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


/*设备大类型（目前暂知有四类）*/
#define 	DEVICE_TYPE_SENSOR					0x0302
#define		DEVICE_TYPE_CONTACT_SENSOR			0x0402			/* Contact sensor type*/
#define		DEVICE_TYPE_SMART_PLUG				0x0051			/* Smart plug type*/
#define		DEVICE_TYPE_OCCUPANCY_SENSOR		0x0107			/* Occupancy Sensor type*/

/* 设备类型（Device Type）*/
enum DEVICE_TYPE_ENMU
{
	ON_OFF_SWITCH         		= 0x0000,		/* On/Off Switch*/
	LEVEL_CONTROL_SWITCH		= 0x0104,		/* Level Control Switch */
	CONTACT_SWITCH_DEVICE		= 0x0015,		/* Contact sensor*/
	ON_OFF_LIGHT				= 0x0100,		/* On/Off Light*/
	REMORT_SMART_CONTROLER      = 0x0115,       /* Remort Smart control */
	
	MAINS_POWER_OUTLET			= 0x0009,		/* Main Power OUTLET */
	DIMMABLE_LIGHT				= 0x0101,		/* Dimmable Light  */
	COLOR_DIMMABLE_LIGHT		= 0x0102,		/* Color Dimmable Light */
	WARNING_DEVICE				= 0x0403,		/* Waring Device*/
	
	WINDOW_COVER_CONTROL		= 0x0203,		/* Window Covering Control*/
	
	SMART_LOCK_SL838            = 0x000A,		/* Smart Lock SL838 */
	THREE_IN_ONE                = 0x000C,		/* Three In one */
	
    SMART_LOCK_SL920            = 0x000B,           /* Smart Lock SL920 */
	SCENE_LIGHT                 = 0xF100,        /* Scene Light */
	SMART_PLUG_IN               = 0x0051,        /* Smart Plug In*/
	
	SMART_LOCK_SL868            = 0x100B,        /* Smart Lock SL868 */
	SMART_LOCK_SL500            = 0x100C,        /* Smart Lock SL500 */
	SMART_LOCK_374				= 0x100D,		 /* Smart Lock 374 */
	
	
	CLOTHES_HANGER              = 0xFE1E,		 /* Clothes Hanger*/
	ZIGBEE_TO_485               = 0xFE55,        /* ZigBee To 485*/
};

/* On-Off Switch 开关设置*/
enum ON_OFF_SWITCH_CONTROL_MODE
{
	SET_ON_OFF_SWITCH_ON		= 0x01,			/* Switch On*/
	SET_ON_OFF_SWITCH_OFF		= 0x02,			/* Switch Off*/
	SET_ON_OFF_SWITCH_TOOGLE	= 0x03,			/* Switch Toggle*/
};


/* Smart Plug  智能插座设置*/
enum SMART_PLUG_IN_CONTROL_MODE
{
	SET_SMART_PLUG_IN_ON		= 0x01,			/* Smart Plug In On */
	SET_SMART_PLUG_IN_OFF		= 0x02,			/* Smart Plug In Off */
	SET_SMART_PLUG_IN_TOOGLE	= 0x03,			/* Smart Plug In Toggle */
	
	RESPONSE_SMART_PLUG_IN_STATUS = 0x04,       /* Smart Plug In Command */
};

/* Level Control Switch 设置*/
enum LEVEL_CONTROL_DEVICE_MODE
{
	SET_LEVEL_CONTROL_ON				= 0x01,
	SET_LEVEL_CONTROL_OFF				= 0x02,
	SET_LEVEL_CONTROL_TOGGLE			= 0x03,
	SET_LEVEL_CONTROL_MOVE_TO_LEVEL     = 0x04,
	
	SET_LEVEL_CONTROL_MOVE				= 0x05,
	SET_LEVEL_CONTROL_STEP				= 0x06,
	SET_LEVEL_CONTROL_STOP				= 0x07,
	
	SET_LEVEL_CONTROL_MOVE_WITH_ONOFF	= 0x08,
	SET_LEVEL_CONTROL_MOVE_TO_LEVEL_WITH_ONOFF	= 0x09,
	SET_LEVEL_CONTROL_STEP_WITH_ONOFF	= 0x0A,
	SET_LEVEL_CONTROL_STOP_WITH_ONOFF	= 0x0B,
	
	GET_LEVEL_CONTROL_SWITCH_LEVEL		= 0x0C,
};

/* Level Control Switch 应答 */
#define		RESPONSE_LEVEL_CONTROL_SWITCH_LEVEL    0x05		      /* Response Level Control Switch Levle*/

/* Contact sensor 应答 */
#define		RESPONSE_CONTACT_SENSOR_STATUS		   0x01		      /* Response Contact sensor status*/

/* Scene light on 控制 */
#define  SET_SCENE_LIGHT_ON                0x01
#define  RESPONSE_SCENE_LIGHT              0x04

/* On-Off light 开关设置*/
enum ON_OFF_LIGHT_CONTROL_MODE
{
	ON_OFF_LIGHT_ON			= 0x01,				/* Light On*/ 
	ON_OFF_LIGHT_OFF		= 0x02, 			/* Light Off*/
	ON_OFF_LIGHT_TOOGLE		= 0x03, 			/* Light Toggle*/
};

/* Level Control Switch 应答*/
#define		RESPONSE_ON_OFF_LIGHT_STATUS    		0x04		 /* Response ON-OFF light status*/

/* 三合一传感器 */

#define     RESPONSE_THREE_REPORT_DATA          0x03           	 /*上报三合一数据*/
#define     RESPONSE_THREE_PARA_DATA            0x04             /*上报三合一配置数据*/ 

/* Warning Device  Control 应答*/
#define    WARNING_DEVICE_CMD                       0x01

enum WARNING_DEVICE_CONTROL_MODE  	/*声光报警器控制指令 */
{
	// SET_WARNING_DEVICE_ALARM_TYPE  		= 0x01,  	/*设置声光报警器报警声种类*/
	// SET_WARNING_DEVICE_LIGTH_ONOFF 		= 0x02,		/*设置声光报警器闪光灯开关*/
	// SET_WARNING_DEVICE_ALARM_LEVEL 		= 0x03,		/*设置声光报警器报警声强度*/
	// SET_WARNING_DEVICE_SUSUTAIN_TIME 	= 0x04,		/*设置声光报警器告警持续时间*/
	// SET_WARNING_DEVICE_LIGHT_BLINK_TIME = 0x05,		/*设置声光报警器闪光灯闪烁频率*/
	// SET_WARNING_DEVICE_LIGHT_LEVLE 		= 0x06,		/*设置声光报警器光照强度*/
	
	PUT_WARNING_DEVICE_STATUS			= 0x02,		/*上报声光报警器状态*/
};

enum WARNING_DEVICE_SOUND_MODE		/*告警声种类*/
{
	WARNING_DEVICE_NO_ALARM				= 0x00,		/*无告警（消警）*/
	WARNING_DEVICE_FIRE_ALARM			= 0x01,		/*火警*/
	WARNING_DEVICE_EMERGENCY_ALARM		= 0x02,		/*紧急告警*/
	WARNING_DEVICE_NORMAL_ALARM			= 0x03, 	/*报警*/
};

enum LEVEL_CONTROL_MODE   			/*控制模式（声光都包括） */
{
	LOW_LEVEL_CONTROL 	= 0x00,						/* low level control*/
	MID_LEVEL_CONTROL 	= 0x01,						/* Middle levle control */
	HIGH_LEVEL_CONTROL 	= 0x02,						/* high levle control */
	SUPER_LEVEL_CONTROL = 0x03,						/* super high level control */
};

/* 窗帘控制器*/
enum WINDOW_COVER_CONTROL_MODE
{
	WINDOW_COVER_CONTROL_UP		= 0x01,				/*Window Covering Control up*/
	WINDOW_COVER_CONTROL_DOWN	= 0x02,				/*Window Covering Control down*/
	WINDOW_COVER_CONTROL_STOP	= 0x03,				/*Window Covering Control stop*/
	
	PUT_WINDOW_COVER_STATUS     = 0x04,
};
											
/*************************************************************************************************/
#define 	PERMIT_MAX_JOIN_DEV                 128				 /*能够存储的最多入网设备 */
#define 	PERMIT_MAX_ENDPOIN                  16				 /*表示能够存储的最多Endpoint */

#define     DEVICE_BATTERY_MARK                 0xFFFE           /*上报设备低电压标志*/
#define     DEVICE_INIT_VOLTAGE                 0x22             /*设备电量处于初始化状态 */

#define     DEVICE_INIT_STATUS                  0xAE             /*设备一入网的状态*/

/*保存入网设备MAC地址、Nodeid、Endpoint*/
struct join_dev_info
{
	char dev_eui[17];											/*入网设备eui*/
	
	int dev_nodeid;												/*入网设备nodeid*/
	
	int endpoint_total;											/*表示要处理的端点总数 */
	
	int endpoint[PERMIT_MAX_ENDPOIN];							/*最多处理16个端点*/
	
	int low_voltage_statue;										/*低电量标志，低电量时设置该值为0x1F*/
	
	int dev_type;                                               /*设备类型*/
	
	int	dev_status[PERMIT_MAX_ENDPOIN];							/*入网设备的状态*/
};


extern struct join_dev_info DEV_INFO[PERMIT_MAX_JOIN_DEV];

/***********************************存储入网设备列表接口*******************************************/

/*设备列表缓存区初始化*/
void HSZInitDeviceInfoBuffer(void);

/*打印设备列表缓存区数据*/
void HSZPrintDeviceBufInfo(void);

/*查找缓存区对应的NodeID*/
int HSZFindNodeidFromInfoBuffer(char* dev_mac);

/*查找缓存区对应的Endpoint*/
int HSZFindEndpointFromInfoBuffer(char* dev_mac,int dev_index);

/*填充入网设备列表信息到缓存区*/
int HSZSetInfoBuffer(char* dev_mac,int nodeid,int endpoint,int dev_type);

/*更新缓存区入网设备的NodeID*/
int HSZUpdateNodeIdFromBuffer(char* dev_mac,int new_nodeid);

/*查找缓存区入网设备对应EUI的设备类型*/
int HSZFindDeviceTypeFromInfoBuffer(char* dev_mac,int max_index);

/* 检查设备是否需要更新状态 */
int HSZIsNeedUpdateStatus(char* dev_mac,int end_point,int status);

/* 将对应的Mac地址转换为字符串*/
int covert_eui64tostr(char*mac,char* strbuf);

/**************************************************************************************************/

/*数据处理接口*/

/*Command*/

/*ON_OFF Switch 控制*/
int CommandSetOnOffSwitch(char* topic,char* dev_mac,int dev_endpoint,int cmd_type);

/*ON_OFF Light控制*/
int CommandSetOnOffLight(char* topic,char* dev_mac,int dev_endpoint,int cmd_type);

/*Level Control设备控制*/
int CommandSetLevelControlSwitch(char* topic,char* dev_mac,char* cmd);
int ResponsePutLevelControlSwitchOnOff(char* gw_mac,char* dev_mac,int dev_endpoint,int level_value);

/*声光报警器控制*/
int CommandSetWarningDeviceControl(char* topic,char* dev_mac,char* cmd);

/*声光报警器状态上报*/
int ResponsePutWarningDeviceControlStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int sensor_status);

/*Zone类设备状态上报*/
int ResponsePutContactSensor(char* dev_mac,char* dev_id,int dev_endpoint,int sensor_status);

/*Levle Control状态上报*/
int ResponsePutLevelControlSwitch(char* dev_mac,char* dev_id,int dev_endpoint,int level_value);

/*ON_OFF Light 设备状态上报*/
int ResponsePutOnOffLightStatus(char* dev_mac,char* dev_id,int dev_endpoint,int dev_status);


/**********************************************Level Control 接口***************************************************/
int CommandSetLevelControlMove(char* topic,char* dev_mac,int dev_endpoint,int move_mode,int rate);
int CommandSetLevelControlMoveToLevel(char* topic,char* dev_mac,int dev_endpoint,int levle,int transition_time);
int CommandSetLevelControlStep(char* topic,char* dev_mac,int dev_endpoint,int step_mode,int step_size,int transition_time);
int CommandSetLevelControlStop(char* topic,char* dev_mac,int dev_endpoint);

int CommandSetLevelControlMoveWithOnOff(char* topic,char* dev_mac,int dev_endpoint,int move_mode,int rate);
int CommandSetLevelControlMoveToLevelWithOnOff(char* topic,char* dev_mac,int dev_endpoint,int levle,int transition_time);
int CommandSetLevelControlStepWithOnOff(char* topic,char* dev_mac,int dev_endpoint,int step_mode,int step_size,int transition_time);
int CommandSetLevelControlStopWithOnOff(char* topic,char* dev_mac,int dev_endpoint);
int ResponseUpdateBatteryLevleInfo(char* gw_mac,char* dev_mac,int battery_levle_status);
int CommandSendDataToSmartLock(char* gw_mac,char* dev_mac,char* cmd_data);
int CommandSendDataToSmartLockSL920(char* gw_mac,char* dev_mac,char* cmd_data);
int CommandSendDataToSmartLockSL868(char* gw_mac,char* dev_mac,char* cmd_data);
int CommandSendDataToThreeInOne(char* gw_mac,char* dev_mac,int cmd_data);

int CommandSendDataToRemortControl(char* gw_mac,char* dev_mac,int cmd_data);

int CommandSendDataToSmartLockSL500(char* gw_mac,char* dev_mac,char* cmd_data);
/*********************************************************************************************************************/

/*窗帘控制器控制*/
int CommandSetWindowCoveringController(char* topic,char* dev_mac,int dev_endpoint,int cmd_type);
/*窗帘控制器状态反馈*/
int ResponsePutWindowCoveringControllerStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int sensor_status);

/*控制场景面板*/
int CommandSetSceneSwitch(char* topic,char* dev_mac,int dev_endpoint,int cmd_type);
/*场景面板数据上报*/
int ResponseSceneSwitchStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int dev_status);

/*控制智能插座*/
int CommandSetSmartPlugIn(char* topic,char* dev_mac,int dev_endpoint,int cmd_type);
/*智能插座数据上报*/
int ResponseSmartPlugInStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int dev_status);


/*控制智能晾衣机*/
int CommandSetClothesHanger(char* gw_mac,char* dev_mac,char* cmd_type);
/*智能晾衣架数据上报*/
int ResponseClothesHangerStatus(char* gw_mac,char* dev_mac,unsigned char* dev_status);
/*智能晾衣架check数据监测*/
int HSZCalculateClothesHangerDataCheckSum(unsigned char *data_buf);
int HSZCalculateClothesHangerSendDataCheckSum(char *data_buf);

int CommandSetZigBeeTo485(char* gw_mac,char* dev_mac,char* cmd);
#endif

