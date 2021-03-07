/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZBindDevice.h
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2017年10月23日
  最近修改   :
  功能描述   : 处理ZCL Data
  函数列表   :
  修改历史   :
  1.日    期   : 2017年10月23日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

******************************************************************************/
#ifndef _HSZHANDLEZCLDATA_H_
#define _HSZHANDLEZCLDATA_H_

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define     LOW_BATTERY_PERCENTAGE   			20               /*设备低于20%电压，触发告警*/
#define     DEVICE_LOW_VOLTAGE                  0x08             /*设备处于低电量状态标志*/
#define     DEVICE_NORMAL_VOLTAGE               0x00             /*设备电量处于正常状态状态标志*/

#define     LOCK_CLUSTER_ID      				0xFD00
#define     LOCK_ATTRIBUTE_ID                   0x0000
#define     LOCK_ATTRIBUTE_SL920_ID             0x0001           /*SL920门锁设备数据*/
#define     LOCK_ATTRIBUTE_SL868_ID             0x0002           /*SL868门锁设备数据*/
#define     LOCK_ATTRIBUTE_SL500_ID             0x0003           /*SL500门锁设备数据*/
#define		LOCK_ATTRIBUTE_374_ID				0x0004			 /*374门锁设备数据*/

#define     LOCK_DATA_TYPE                      0x41

#define     THREE_IN_ONE_GAS1					0xFC02           /*三合一气体传感器 设备数据*/
#define     THREE_IN_ONE_GAS2					0xFC03			 /*三合一气体传感器 设备参数*/

#define     CLOTHES_HANGER_CLUSTER              0x0012           /*凉霸晾衣机 设备参数*/
#define     CLOTHES_ATTRIBUTE_ID                0x0000
#define     CLOTHES_DATA_TYPE                   0x41
#define     ZIGBEE_TO_485                       0xFC11           /*ZigBee转485 Cluster ID*/
typedef enum{ 
 
  ZONE_STATE = 0x0000,
  ZONE_TYPE = 0x0001,
  ZONE_STATUS = 0x0002,
  
}ATTRIBUTE_ID;


typedef enum{ 

  STANDARD_CIE = 0x0000,
  MOTION_SENSOR = 0x000D,
  CONTACT_SWITCH = 0x0015,
  FIRE_SENSOR = 0x0028,
  WATER_SENSOR = 0x002A,
  GAS_SENSOR= 0x002B,
  PERSONAL_EMERGENCY_DEVICE = 0x002C,
  MOVEMENT_SENSOR = 0x002D,
  REMOTE_CONTROL = 0x010F,
  KEY_FOB = 0x0115,
  KEY_PAD = 0x021D,
  STANDARD_WARNING_DEVICE = 0x0225,
  RESERCED = 0x7FFF,
  
}ZONE_DEVICE_TYPE;


typedef enum{
	
  BITMAP_16BIT_BIT0 = 0x0001,
  BITMAP_16BIT_BIT1 = 0x0002,
  BITMAP_16BIT_BIT2 = 0x0004,
  BITMAP_16BIT_BIT3 = 0x0008,
  BITMAP_16BIT_BIT4 = 0x0010,
  BITMAP_16BIT_BIT5 = 0x0020,
  BITMAP_16BIT_BIT6 = 0x0040,
  BITMAP_16BIT_BIT7 = 0x0080,

  BITMAP_16BIT_BIT8 = 0x0100,
  BITMAP_16BIT_BIT9 = 0x0200,
  BITMAP_16BIT_BIT10 = 0x0400,
  BITMAP_16BIT_BIT11 = 0x0800,
  BITMAP_16BIT_BIT12 = 0x1000,
  BITMAP_16BIT_BIT13 = 0x2000,
  BITMAP_16BIT_BIT14 = 0x4000,
  BITMAP_16BIT_BIT15 = 0x8000,
  
}BITMAP_16;

typedef enum{

	OCCUPANCY_PIR = 0x00,
	OCCUPANCY_ULTRASONIC = 0x01,
	OCCUPANCY_PIR_ULTRASONIC = 0x02,
	
}OCCUPANCY_SENSOR_TYPE;

typedef enum{

	OCCUPANCY_PIR_TYPE = 0xF001,
	OCCUPANCY_ULTRASONIC_TYPE = 0xF002,
	OCCUPANCY_PIR_ULTRASONIC_TYPE = 0xF003,
	
}OCCUPANCY_SENSOR;

typedef enum{

	WINDOWS_COVER_UP = 0x65,
	WINDOWS_COVER_DOWN = 0x66,
	WINDOWS_COVER_STOP = 0x67,
	
}WINDOWS_COVERING_DATA;
/* Zone Device Data */
void HSZProcessZoneDeviceType(int zone_dev_type);

void HSZProcessZoneDeviceZclData(char* dev_mac,int dev_endpoint,int zone_dev_zcl_data);

void HSZProcessZoneEnrollRequestCmdData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessZoneReportAttributeCmdData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessZoneStatusChangeNotifyCmdData(char* dev_mac,int dev_endpoint,char* zcl_data);

/* Occupancy Sensor Device Data */
void HSZProcessOccupancySensorReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessOccupancySensorReportData(char* dev_mac,int dev_endpoint,char* zcl_data);

/* On-Off Device Data */
void HSZProcessOnOffDeviceReportData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessOnOffDeviceReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

/* Level Control Device Data */
void HSZProcessLevelControlDeviceReportData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessLevelControlDeviceReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

/* Color Control Device Data */
void HSZProcessColorControlDeviceReportData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessColorControlDeviceReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

/* OTA Bootloader Data */
void HSZProcessOtaAndBootloadReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessOtaAndBootloadReportData(char* dev_mac,int dev_endpoint,char* zcl_data);

/* Power Config Data */
void HSZProcessPowerConfigReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessPowerConfigReportData(char* dev_mac,int dev_endpoint,char* zcl_data);

/* Alarm Data */
void HSZProcessAlarmReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessAlarmReportData(char* dev_mac,int dev_endpoint,char* zcl_data);

/* Simple Metering*/
void HSZProcessSimpleMeterReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessSimpleMeterReportData(char* dev_mac,int dev_endpoint,char* zcl_data);


/*Electrical Measurement*/
void HSZProcessElectricalMeasurementReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessElectricalMeasurementReportData(char* dev_mac,int dev_endpoint,char* zcl_data);

/*Illuminance Measurement*/
void HSZProcessIlluminanceMeasurementReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessIlluminanceMeasurementReportData(char* dev_mac,int dev_endpoint,char* zcl_data);


/*Window Covering*/
void HSZProcessWindowCoveringReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessWindowCoveringReportData(char* dev_mac,int dev_endpoint,char* zcl_data);


/*Ownship Message*/
void HSZProcessOwnshipOneReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessOwnshipOneReportData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessOwnshipTwoReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessOwnshipTwoReportData(char* dev_mac,int dev_endpoint,char* zcl_data);


/*IAS WD*/
void HSZProcessIASWDReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessIASWDReportData(char* dev_mac,int dev_endpoint,char* zcl_data);


/*接收处理信号强度*/

void HSZProcessDiagnosticsReadData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessDiagnosticsReportData(char* dev_mac,int dev_endpoint,char* zcl_data);


/*智能门锁数据处理*/

void HSZProcessSmartLockReportData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessSmartLockEventData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessThreeInOneDeviceData(char* dev_mac,int dev_endpoint,char* zcl_data);

void HSZProcessThreeInOneParameterData(char* dev_mac,int dev_endpoint,char* zcl_data);
void HSZProcessClothesHangerReportData(char* dev_mac,int dev_endpoint,char* zcl_data);
/****************************************RSY***************************************/
int HSZAnalysisSelfProtocolSL500LockData(unsigned char *data,char* gw_mac,char* dev_mac);
int HSZAnalysisSelfProtocolLockData(unsigned char *data,char* gw_mac,char* dev_mac);
#endif