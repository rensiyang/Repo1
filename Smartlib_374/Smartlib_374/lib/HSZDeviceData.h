/******************************************************************************

                  ��Ȩ���� (C), 2017-2025, �ൺ�����ǻۼҾ�ϵͳ�ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : HSZDeviceData.h
  �� �� ��   : ����
  ��    ��   : Zigbee Project Team
  ��������   : 2017��8��11��
  ����޸�   :
  ��������   : �����ϲ����м���豸����ָ���Ӧ��ͨ������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _HSZDEVICEDATA_H_
#define _HSZDEVICEDATA_H_

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


/*�豸�����ͣ�Ŀǰ��֪�����ࣩ*/
#define 	DEVICE_TYPE_SENSOR					0x0302
#define		DEVICE_TYPE_CONTACT_SENSOR			0x0402			/* Contact sensor type*/
#define		DEVICE_TYPE_SMART_PLUG				0x0051			/* Smart plug type*/
#define		DEVICE_TYPE_OCCUPANCY_SENSOR		0x0107			/* Occupancy Sensor type*/

/* �豸���ͣ�Device Type��*/
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

/* On-Off Switch ��������*/
enum ON_OFF_SWITCH_CONTROL_MODE
{
	SET_ON_OFF_SWITCH_ON		= 0x01,			/* Switch On*/
	SET_ON_OFF_SWITCH_OFF		= 0x02,			/* Switch Off*/
	SET_ON_OFF_SWITCH_TOOGLE	= 0x03,			/* Switch Toggle*/
};


/* Smart Plug  ���ܲ�������*/
enum SMART_PLUG_IN_CONTROL_MODE
{
	SET_SMART_PLUG_IN_ON		= 0x01,			/* Smart Plug In On */
	SET_SMART_PLUG_IN_OFF		= 0x02,			/* Smart Plug In Off */
	SET_SMART_PLUG_IN_TOOGLE	= 0x03,			/* Smart Plug In Toggle */
	
	RESPONSE_SMART_PLUG_IN_STATUS = 0x04,       /* Smart Plug In Command */
};

/* Level Control Switch ����*/
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

/* Level Control Switch Ӧ�� */
#define		RESPONSE_LEVEL_CONTROL_SWITCH_LEVEL    0x05		      /* Response Level Control Switch Levle*/

/* Contact sensor Ӧ�� */
#define		RESPONSE_CONTACT_SENSOR_STATUS		   0x01		      /* Response Contact sensor status*/

/* Scene light on ���� */
#define  SET_SCENE_LIGHT_ON                0x01
#define  RESPONSE_SCENE_LIGHT              0x04

/* On-Off light ��������*/
enum ON_OFF_LIGHT_CONTROL_MODE
{
	ON_OFF_LIGHT_ON			= 0x01,				/* Light On*/ 
	ON_OFF_LIGHT_OFF		= 0x02, 			/* Light Off*/
	ON_OFF_LIGHT_TOOGLE		= 0x03, 			/* Light Toggle*/
};

/* Level Control Switch Ӧ��*/
#define		RESPONSE_ON_OFF_LIGHT_STATUS    		0x04		 /* Response ON-OFF light status*/

/* ����һ������ */

#define     RESPONSE_THREE_REPORT_DATA          0x03           	 /*�ϱ�����һ����*/
#define     RESPONSE_THREE_PARA_DATA            0x04             /*�ϱ�����һ��������*/ 

/* Warning Device  Control Ӧ��*/
#define    WARNING_DEVICE_CMD                       0x01

enum WARNING_DEVICE_CONTROL_MODE  	/*���ⱨ��������ָ�� */
{
	// SET_WARNING_DEVICE_ALARM_TYPE  		= 0x01,  	/*�������ⱨ��������������*/
	// SET_WARNING_DEVICE_LIGTH_ONOFF 		= 0x02,		/*�������ⱨ��������ƿ���*/
	// SET_WARNING_DEVICE_ALARM_LEVEL 		= 0x03,		/*�������ⱨ����������ǿ��*/
	// SET_WARNING_DEVICE_SUSUTAIN_TIME 	= 0x04,		/*�������ⱨ�����澯����ʱ��*/
	// SET_WARNING_DEVICE_LIGHT_BLINK_TIME = 0x05,		/*�������ⱨ�����������˸Ƶ��*/
	// SET_WARNING_DEVICE_LIGHT_LEVLE 		= 0x06,		/*�������ⱨ��������ǿ��*/
	
	PUT_WARNING_DEVICE_STATUS			= 0x02,		/*�ϱ����ⱨ����״̬*/
};

enum WARNING_DEVICE_SOUND_MODE		/*�澯������*/
{
	WARNING_DEVICE_NO_ALARM				= 0x00,		/*�޸澯��������*/
	WARNING_DEVICE_FIRE_ALARM			= 0x01,		/*��*/
	WARNING_DEVICE_EMERGENCY_ALARM		= 0x02,		/*�����澯*/
	WARNING_DEVICE_NORMAL_ALARM			= 0x03, 	/*����*/
};

enum LEVEL_CONTROL_MODE   			/*����ģʽ�����ⶼ������ */
{
	LOW_LEVEL_CONTROL 	= 0x00,						/* low level control*/
	MID_LEVEL_CONTROL 	= 0x01,						/* Middle levle control */
	HIGH_LEVEL_CONTROL 	= 0x02,						/* high levle control */
	SUPER_LEVEL_CONTROL = 0x03,						/* super high level control */
};

/* ����������*/
enum WINDOW_COVER_CONTROL_MODE
{
	WINDOW_COVER_CONTROL_UP		= 0x01,				/*Window Covering Control up*/
	WINDOW_COVER_CONTROL_DOWN	= 0x02,				/*Window Covering Control down*/
	WINDOW_COVER_CONTROL_STOP	= 0x03,				/*Window Covering Control stop*/
	
	PUT_WINDOW_COVER_STATUS     = 0x04,
};
											
/*************************************************************************************************/
#define 	PERMIT_MAX_JOIN_DEV                 128				 /*�ܹ��洢����������豸 */
#define 	PERMIT_MAX_ENDPOIN                  16				 /*��ʾ�ܹ��洢�����Endpoint */

#define     DEVICE_BATTERY_MARK                 0xFFFE           /*�ϱ��豸�͵�ѹ��־*/
#define     DEVICE_INIT_VOLTAGE                 0x22             /*�豸�������ڳ�ʼ��״̬ */

#define     DEVICE_INIT_STATUS                  0xAE             /*�豸һ������״̬*/

/*���������豸MAC��ַ��Nodeid��Endpoint*/
struct join_dev_info
{
	char dev_eui[17];											/*�����豸eui*/
	
	int dev_nodeid;												/*�����豸nodeid*/
	
	int endpoint_total;											/*��ʾҪ����Ķ˵����� */
	
	int endpoint[PERMIT_MAX_ENDPOIN];							/*��ദ��16���˵�*/
	
	int low_voltage_statue;										/*�͵�����־���͵���ʱ���ø�ֵΪ0x1F*/
	
	int dev_type;                                               /*�豸����*/
	
	int	dev_status[PERMIT_MAX_ENDPOIN];							/*�����豸��״̬*/
};


extern struct join_dev_info DEV_INFO[PERMIT_MAX_JOIN_DEV];

/***********************************�洢�����豸�б�ӿ�*******************************************/

/*�豸�б�������ʼ��*/
void HSZInitDeviceInfoBuffer(void);

/*��ӡ�豸�б���������*/
void HSZPrintDeviceBufInfo(void);

/*���һ�������Ӧ��NodeID*/
int HSZFindNodeidFromInfoBuffer(char* dev_mac);

/*���һ�������Ӧ��Endpoint*/
int HSZFindEndpointFromInfoBuffer(char* dev_mac,int dev_index);

/*��������豸�б���Ϣ��������*/
int HSZSetInfoBuffer(char* dev_mac,int nodeid,int endpoint,int dev_type);

/*���»����������豸��NodeID*/
int HSZUpdateNodeIdFromBuffer(char* dev_mac,int new_nodeid);

/*���һ����������豸��ӦEUI���豸����*/
int HSZFindDeviceTypeFromInfoBuffer(char* dev_mac,int max_index);

/* ����豸�Ƿ���Ҫ����״̬ */
int HSZIsNeedUpdateStatus(char* dev_mac,int end_point,int status);

/* ����Ӧ��Mac��ַת��Ϊ�ַ���*/
int covert_eui64tostr(char*mac,char* strbuf);

/**************************************************************************************************/

/*���ݴ���ӿ�*/

/*Command*/

/*ON_OFF Switch ����*/
int CommandSetOnOffSwitch(char* topic,char* dev_mac,int dev_endpoint,int cmd_type);

/*ON_OFF Light����*/
int CommandSetOnOffLight(char* topic,char* dev_mac,int dev_endpoint,int cmd_type);

/*Level Control�豸����*/
int CommandSetLevelControlSwitch(char* topic,char* dev_mac,char* cmd);
int ResponsePutLevelControlSwitchOnOff(char* gw_mac,char* dev_mac,int dev_endpoint,int level_value);

/*���ⱨ��������*/
int CommandSetWarningDeviceControl(char* topic,char* dev_mac,char* cmd);

/*���ⱨ����״̬�ϱ�*/
int ResponsePutWarningDeviceControlStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int sensor_status);

/*Zone���豸״̬�ϱ�*/
int ResponsePutContactSensor(char* dev_mac,char* dev_id,int dev_endpoint,int sensor_status);

/*Levle Control״̬�ϱ�*/
int ResponsePutLevelControlSwitch(char* dev_mac,char* dev_id,int dev_endpoint,int level_value);

/*ON_OFF Light �豸״̬�ϱ�*/
int ResponsePutOnOffLightStatus(char* dev_mac,char* dev_id,int dev_endpoint,int dev_status);


/**********************************************Level Control �ӿ�***************************************************/
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

/*��������������*/
int CommandSetWindowCoveringController(char* topic,char* dev_mac,int dev_endpoint,int cmd_type);
/*����������״̬����*/
int ResponsePutWindowCoveringControllerStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int sensor_status);

/*���Ƴ������*/
int CommandSetSceneSwitch(char* topic,char* dev_mac,int dev_endpoint,int cmd_type);
/*������������ϱ�*/
int ResponseSceneSwitchStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int dev_status);

/*�������ܲ���*/
int CommandSetSmartPlugIn(char* topic,char* dev_mac,int dev_endpoint,int cmd_type);
/*���ܲ��������ϱ�*/
int ResponseSmartPlugInStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int dev_status);


/*�����������»�*/
int CommandSetClothesHanger(char* gw_mac,char* dev_mac,char* cmd_type);
/*�������¼������ϱ�*/
int ResponseClothesHangerStatus(char* gw_mac,char* dev_mac,unsigned char* dev_status);
/*�������¼�check���ݼ��*/
int HSZCalculateClothesHangerDataCheckSum(unsigned char *data_buf);
int HSZCalculateClothesHangerSendDataCheckSum(char *data_buf);

int CommandSetZigBeeTo485(char* gw_mac,char* dev_mac,char* cmd);
#endif

