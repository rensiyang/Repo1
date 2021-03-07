/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZBindDevice.c
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2017年10月23日
  最近修改   :
  功能描述   : 处理cluster绑定信息
  函数列表   :
  修改历史   :
  1.日    期   : 2017年10月23日
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
#include "HSZBindDevice.h"
#include "HSZDeviceData.h"
#include "HSZHandleZclData.h"



#define SET_REPORT_ATTRIBUTE_MAX_TIME     (3*60)   /*Report 时间间隔设置为3 minite*/
#define SET_REPORT_ATTRIBUTE_MIN_TIME     150	   /*Report 时间间隔最小设置为150s*/


#define SET_REPORT_ATTRIBUTE_MAX_TIME_WITH_POWER     70   /*有源设备Report时间间隔设置最大时间间隔为70秒数*/
#define SET_REPORT_ATTRIBUTE_MIN_TIME_WITH_POWER     50	   /*有源设备Report时间间隔最小设置为50s*/

/*****************************************************************************
 函 数 名  : HSZNewDeviceConfigReportWithPower
 功能描述  : 新设备入网消息上报配置
 输入参数  : int epcount： 入网设备Endpoint 
             char* dev_mac：入网设备的MAC地址  
             int clusterid：需要设定的Cluster ID 
			 int arrtibutestr：需要设定的Attribute ID
			 int arrtibute_type：Report数据的数据格式
			 int report_max_time：上报的最大时间间隔
             char *max_change_amount：上报的最大变化量  
 输出参数  : 无
 返 回 值  : 无
 
 修改历史      :
  1.日    期   : 2017年6月23日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZNewDeviceConfigReportWithPower(int epcount,char* dev_mac,int clusterid,int arrtibutestr,int arrtibute_type,int report_max_time,char *max_change_amount)
{
	/*
		zcl global send-me-a-report (args) 
		<uint16_t>  The cluster id of the requested report.                     
		<uint16_t>  The attribute id for requested report.						 
		<uint8_t>  The two byte ZigBee type value for the requested report.		 
		<uint16_t>  Minimum number of seconds between reports.					 
		<uint16_t>  Maximum number of seconds between reports.					 
		<string>  Amount of change to trigger a report.							 
	*/
	
	/*
	
	{"commands":[{"command":"zdo bind 0x6b2e 1 1 0x0006 {000b57fffe07ab16} {00000000000000}"}
	,{"command":"plugin device-table send {000b57fffe07ab16} 1","postDelayMs":100}]}
	"zcl global send-me-a-report 0x0402 0x0000 0x29 0x0001 0x0708 {32 00}"}
	
	*/
	
	char buf_topic[50] = {0};
	
	char msg[1024] = {0};
	
	char tmp[30] = {0};
	
	int rc = -1;
	
	{
		/* 头 */
		sprintf(msg,"{\"commands\":[{\"command\":\"zcl global send-me-a-report ");

		/* Cluster */
		sprintf(tmp," 0x%04x",clusterid);
		strcat(msg,tmp);
        
		/* Attr */
		sprintf(tmp," 0x%04x",arrtibutestr);
		strcat(msg,tmp);

		/* Type */
		sprintf(tmp," 0x%02x",arrtibute_type);
		strcat(msg,tmp);

		/* Min */
		sprintf(tmp," 0x%04x ",SET_REPORT_ATTRIBUTE_MIN_TIME_WITH_POWER);
		strcat(msg,tmp);

		/* Max */
		sprintf(tmp," 0x%04x ",report_max_time);
		strcat(msg,tmp);

		/* Amount of change */
		sprintf(tmp," {00}");
		strcat(msg,tmp);

		sprintf(tmp,"\"}");
		strcat(msg,tmp);


		/* Command */
		sprintf(tmp,",{\"command\":\"");
		strcat(msg,tmp);
		
		/* Command */
		sprintf(tmp,"plugin device-table send ");
		strcat(msg,tmp);

		/* Mac */
		sprintf(tmp," {%s}",dev_mac);
		strcat(msg,tmp);
		
		sprintf(tmp," %d\",",epcount);
		strcat(msg,tmp);

        /* DelayMs */
		sprintf(tmp,"\"postDelayMs\":100}]}"   );
		strcat(msg,tmp);

		sprintf(buf_topic,"gw/%s/commands",HSZGatewayMac());
		
		rc = mosquitto_publish_topic(buf_topic,msg);
		
	}
	
	return rc;
}



/*****************************************************************************
 函 数 名  : HSZNewDeviceConfigReport
 功能描述  : 新设备入网消息上报配置
 输入参数  : int epcount： 入网设备Endpoint 
             char* dev_mac：入网设备的MAC地址  
             int clusterid：需要设定的Cluster ID 
			 int arrtibutestr：需要设定的Attribute ID
			 int arrtibute_type：Report数据的数据格式
			 int report_max_time：上报的最大时间间隔
             char *max_change_amount：上报的最大变化量  
 输出参数  : 无
 返 回 值  : 无
 
 修改历史      :
  1.日    期   : 2017年6月23日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZNewDeviceConfigReport(int epcount,char* dev_mac,int clusterid,int arrtibutestr,int arrtibute_type,int report_max_time,char *max_change_amount)
{
	/*
		zcl global send-me-a-report (args) 
		<uint16_t>  The cluster id of the requested report.                     
		<uint16_t>  The attribute id for requested report.						 
		<uint8_t>  The two byte ZigBee type value for the requested report.		 
		<uint16_t>  Minimum number of seconds between reports.					 
		<uint16_t>  Maximum number of seconds between reports.					 
		<string>  Amount of change to trigger a report.							 
	*/
	
	/*
	
	{"commands":[{"command":"zdo bind 0x6b2e 1 1 0x0006 {000b57fffe07ab16} {00000000000000}"}
	,{"command":"plugin device-table send {000b57fffe07ab16} 1","postDelayMs":100}]}
	"zcl global send-me-a-report 0x0402 0x0000 0x29 0x0001 0x0708 {32 00}"}
	
	*/
	
	char buf_topic[50] = {0};
	
	char msg[1024] = {0};
	
	char tmp[30] = {0};
	
	int rc = -1;
	
	{
		/* 头 */
		sprintf(msg,"{\"commands\":[{\"command\":\"zcl global send-me-a-report ");

		/* Cluster */
		sprintf(tmp," 0x%04x",clusterid);
		strcat(msg,tmp);
        
		/* Attr */
		sprintf(tmp," 0x%04x",arrtibutestr);
		strcat(msg,tmp);

		/* Type */
		sprintf(tmp," 0x%02x",arrtibute_type);
		strcat(msg,tmp);

		/* Min */
		sprintf(tmp," 0x%04x ",SET_REPORT_ATTRIBUTE_MIN_TIME);
		strcat(msg,tmp);

		/* Max */
		sprintf(tmp," 0x%04x ",report_max_time);
		strcat(msg,tmp);

		/* Amount of change */
		sprintf(tmp," {00}");
		strcat(msg,tmp);

		sprintf(tmp,"\"}");
		strcat(msg,tmp);


		/* Command */
		sprintf(tmp,",{\"command\":\"");
		strcat(msg,tmp);
		
		/* Command */
		sprintf(tmp,"plugin device-table send ");
		strcat(msg,tmp);

		/* Mac */
		sprintf(tmp," {%s}",dev_mac);
		strcat(msg,tmp);
		
		sprintf(tmp," %d\",",epcount);
		strcat(msg,tmp);

        /* DelayMs */
		sprintf(tmp,"\"postDelayMs\":100}]}"   );
		strcat(msg,tmp);

		sprintf(buf_topic,"gw/%s/commands",HSZGatewayMac());
		
		rc = mosquitto_publish_topic(buf_topic,msg);
		
	}
	
	return rc;
}

/*****************************************************************************
 函 数 名  : HSZBindNewDevice
 功能描述  : 网关绑定设备(指ON-OFF类)
 输入参数  : 
             int nodeid：入网设备NodeID
             int epcount ：入网设备endpoint
             char* dev_mac：入网设备Mac地址
			 int clusterid：入网设备的Cluster ID
 输出参数  : 无
 返 回 值  : 无
 
 修改历史      :
  1.日    期   : 2017年6月23日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZBindNewDevice(int epcount,char* dev_mac,int clusterid)
{

	/*
	{"command":"zdo hsbind  1 1 0x0006 {00 0D 6F 00 0D 57 8B BB} {00 FD 9F FF FE 2E 90 E5}"}
	{"commands":[{"command":"zdo hsbind 2 1 0x0006 {00 0D 6F 00 0D 57 89 74} {90 FD 9F FF FE 2E 90 6B}"}]}
	
	{"commands":[{"command":"zdo hsbind 1 1 0x0006 {00 0D 6F 00 0D 57 89 74} {00 FD 9F FF FE 2E 90 E5}"}]}

	*/
	
	char buf_topic[50] = {0};

	char msg[1024] = {0};
	char tmp[30] = {0};
	char mac_str[30] = {0};
	int rc = -1;

	/* 头 */
	sprintf(msg,"{\"commands\":[{\"command\":\"zdo hsbind");
	
	/* endpoint */
	sprintf(tmp," %d 1",epcount);
	strcat(msg,tmp);
	
	/*cluster */
	sprintf(tmp," 0x%04x",clusterid);
	strcat(msg,tmp);

	/*device_mac */
	covert_eui64tostr(dev_mac,mac_str);
	sprintf(tmp," %s",mac_str);
	strcat(msg,tmp);
	
	/*gateway mac */
	memset(mac_str,0,30);
	covert_eui64tostr(HSZGatewayMac(),mac_str);
	sprintf(tmp," %s",mac_str);
	strcat(msg,tmp);
	
	sprintf(tmp,"\"}]}");
	strcat(msg,tmp);
	
	sprintf(buf_topic,"gw/%s/commands",HSZGatewayMac());
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZBindNewDevice,buf_topic:%s,msg:%s，endpoint:%d\n",buf_topic,msg,epcount);

	rc = mosquitto_publish_topic(buf_topic,msg);
	
	return rc;
}

/*****************************************************************************
 函 数 名  : HSZRequestAttribute
 功能描述  : 请求读取设备属性
 输入参数  : char* dev_mac :设备MAC地址        
             int dev_ep ：设备endpoint           
             int cluid :设备clusterid        
             int arrtibutestr：设备属性  
 输出参数  : 无
 返 回 值  : 无
 
 修改历史      :
  1.日    期   : 2017年6月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数
*****************************************************************************/
int HSZRequestAttribute(char* dev_mac,int dev_ep,int cluid,int arrtibutestr)
{
    /*
	 {command: 'zcl global direction 0'},
	 {command: 'zcl global read ' + clusterString + ' ' + attributeString},
	 {command: 'plugin device-table send ' + eui64String + ' ' + deviceEndpoint.endpoint},
	*/
	
 	int rc = -1;
	char topic[100] = {0};
	char tmp[50] = {0};
	
	cJSON *cmds,*array,*cmd1,*cmd2,*cmd3;
	
	cmd1=cJSON_CreateObject();
	cmd2=cJSON_CreateObject();
	cmd3=cJSON_CreateObject();
	cmds=cJSON_CreateObject();
	array=cJSON_CreateArray();
	
	sprintf(tmp,"zcl global direction 0");
	cJSON_AddItemToObject(cmd1,"command",cJSON_CreateString(tmp));

	sprintf(tmp,"zcl global read 0x%04x 0x%04x",cluid,arrtibutestr);
	cJSON_AddItemToObject(cmd2,"command",cJSON_CreateString(tmp));

	sprintf(tmp,"plugin device-table send {%s} %d",dev_mac,dev_ep);
	cJSON_AddItemToObject(cmd3,"command",cJSON_CreateString(tmp));

	cJSON_AddItemToArray(array,cmd1);
	cJSON_AddItemToArray(array,cmd2);
	cJSON_AddItemToArray(array,cmd3);
	cJSON_AddItemToObject(cmds,"commands",array);

	char * p = cJSON_Print(cmds);
	
	sprintf(topic,"gw/%s/commands",HSZGatewayMac());

	DEBUG_ZONE(LOG_ZONE_INPACK_MSG,"%s",p);

	rc = mosquitto_publish_topic(topic,p);
	
	free(p);

	return rc;
}


/*****************************************************************************
 函 数 名  : HSZBindNewDeviceReadAndConfigReport
 功能描述  : 根据入网设备Cluster 绑定设备类型，读取设备属性，配置上报Report
 输入参数  : char* dev_mac :入网设备MAC地址        
             int epcount ：入网设备endpoint           
             int clusterid :入网设备clusterid        
 输出参数  : 无
 返 回 值  : 无
 
 修改历史      :
  1.日    期   : 2017年6月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数
*****************************************************************************/
int HSZBindNewDeviceReadAndConfigReport(char* dev_mac,int epcount,int clusterid)
{	
    DEBUG_ZONE(LOG_ZONE_TEST,"The Joined device is edpoint:0x%04x,clusterid:0x%04x.\n",epcount,clusterid);
	
#if 1	

    if(0x01 == epcount)
	{
		
		HSZRequestAttribute(dev_mac,epcount,ZCL_BASIC_CLUSTER_ID,ZCL_SW_BUILD_ID_ATTRIBUTE_ID);
#if 0		
		HSZRequestAttribute(dev_mac,epcount,ZCL_BASIC_CLUSTER_ID,ZCL_MANUFACTURER_NAME_ATTRIBUTE_ID);

		HSZRequestAttribute(dev_mac,epcount,ZCL_BASIC_CLUSTER_ID,ZCL_VERSION_ATTRIBUTE_ID);
		HSZRequestAttribute(dev_mac,epcount,ZCL_BASIC_CLUSTER_ID,ZCL_APPLICATION_VERSION_ATTRIBUTE_ID);
		HSZRequestAttribute(dev_mac,epcount,ZCL_BASIC_CLUSTER_ID,ZCL_STACK_VERSION_ATTRIBUTE_ID);
		HSZRequestAttribute(dev_mac,epcount,ZCL_BASIC_CLUSTER_ID,ZCL_HW_VERSION_ATTRIBUTE_ID);

		HSZRequestAttribute(dev_mac,epcount,ZCL_BASIC_CLUSTER_ID,ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID);
		HSZRequestAttribute(dev_mac,epcount,ZCL_BASIC_CLUSTER_ID,ZCL_DATE_CODE_ATTRIBUTE_ID);
		HSZRequestAttribute(dev_mac,epcount,ZCL_BASIC_CLUSTER_ID,ZCL_POWER_SOURCE_ATTRIBUTE_ID);
#endif

	}
	
#endif	

	switch(clusterid)
	{
		case ZCL_ON_OFF_CLUSTER_ID:/*ZigBee On/off Cluster*/
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"The Joined device is ZCL_ON_OFF_CLUSTER_ID cluster,begin bind,read and config report.\n");
			
			/* Read ON_OFF Attribute */
			HSZRequestAttribute(dev_mac,epcount,ZCL_ON_OFF_CLUSTER_ID,ZCL_ON_OFF_ATTRIBUTE_ID);
            
			HSZBindNewDevice(epcount,dev_mac,ZCL_ON_OFF_CLUSTER_ID);
			
			// /* Config ON_OFF Attribute */
			// HSZNewDeviceConfigReportWithPower(epcount,dev_mac,ZCL_ON_OFF_CLUSTER_ID,
			// ZCL_ON_OFF_ATTRIBUTE_ID,ZCL_BOOLEAN_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME_WITH_POWER,NULL);
			
			/*Config RSSI Status*/			
			HSZNewDeviceConfigReportWithPower(epcount,dev_mac,ZCL_DIAGNOSTICS_CLUSTER_ID,\
			ZCL_LAST_MESSAGE_RSSI_ATTRIBUTE_ID,ZCL_BOOLEAN_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME_WITH_POWER,NULL);
			
			DEBUG_ZONE(LOG_ZONE_MY,"HSZNewDeviceConfigReportWithPower,Dev_mac:%s,endpoint:%d.\n",dev_mac,epcount);
		}
		break;
		
		case ZCL_COLOR_CONTROL_CLUSTER_ID:/* ZigBee Color-Control Cluster */
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is ZCL_COLOR_CONTROL_CLUSTER_ID cluster,begin bind ,read and config report.\n");
#if 0			
			/* Read CurrentHue */
			HSZRequestAttribute(dev_mac,epcount,ZCL_COLOR_CONTROL_CLUSTER_ID,ZCL_COLOR_CONTROL_CURRENT_HUE_ATTRIBUTE_ID);
			
			/* Read CurrentSaturation */
			HSZRequestAttribute(dev_mac,epcount,ZCL_COLOR_CONTROL_CLUSTER_ID,ZCL_COLOR_CONTROL_CURRENT_SATURATION_ATTRIBUTE_ID);
			
			/* Read RemainingTime */
			HSZRequestAttribute(dev_mac,epcount,ZCL_COLOR_CONTROL_CLUSTER_ID,ZCL_COLOR_CONTROL_REMAINING_TIME_ATTRIBUTE_ID);
			
			/* Read CurrentX */
			HSZRequestAttribute(dev_mac,epcount,ZCL_COLOR_CONTROL_CLUSTER_ID,ZCL_COLOR_CONTROL_CURRENT_X_ATTRIBUTE_ID);
			
			/* Read CurrentY */
			HSZRequestAttribute(dev_mac,epcount,ZCL_COLOR_CONTROL_CLUSTER_ID,ZCL_COLOR_CONTROL_CURRENT_Y_ATTRIBUTE_ID);
			
			HSZBindNewDevice(epcount,dev_mac,ZCL_COLOR_CONTROL_CLUSTER_ID);

			/* Config CurrentHue */
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_COLOR_CONTROL_CLUSTER_ID,\
			ZCL_COLOR_CONTROL_CURRENT_HUE_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			/* Config CurrentSaturation */
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_COLOR_CONTROL_CLUSTER_ID,\
			ZCL_COLOR_CONTROL_CURRENT_SATURATION_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			/* Config RemainingTime */
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_COLOR_CONTROL_CLUSTER_ID,\
			ZCL_COLOR_CONTROL_REMAINING_TIME_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			/* Config CurrentX */
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_COLOR_CONTROL_CLUSTER_ID,\
			ZCL_COLOR_CONTROL_CURRENT_X_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			/* Config CurrentY */
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_COLOR_CONTROL_CLUSTER_ID,\
			ZCL_COLOR_CONTROL_CURRENT_Y_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
#endif
		}
		break;
		
		case ZCL_LEVEL_CONTROL_CLUSTER_ID:/* ZigBee Level-Control CLuster*/
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is ZCL_LEVEL_CONTROL_CLUSTER_ID cluster,begin bind ,read and config report.\n");
#if 0			
			/* Read CurrentLevel */
			HSZRequestAttribute(dev_mac,epcount,ZCL_LEVEL_CONTROL_CLUSTER_ID,ZCL_CURRENT_LEVEL_ATTRIBUTE_ID);
			/* Read RemainingTime */
			HSZRequestAttribute(dev_mac,epcount,ZCL_LEVEL_CONTROL_CLUSTER_ID,ZCL_LEVEL_CONTROL_REMAINING_TIME_ATTRIBUTE_ID);
			/* Read OnOffTransitionTime */
			HSZRequestAttribute(dev_mac,epcount,ZCL_LEVEL_CONTROL_CLUSTER_ID,ZCL_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ID);
			/* Read OnLevel*/
			HSZRequestAttribute(dev_mac,epcount,ZCL_LEVEL_CONTROL_CLUSTER_ID,ZCL_ON_LEVEL_ATTRIBUTE_ID);
			
			HSZBindNewDevice(epcount,dev_mac,ZCL_LEVEL_CONTROL_CLUSTER_ID);
			
			/* Config CurrentLevel */
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_LEVEL_CONTROL_CLUSTER_ID,\
			ZCL_CURRENT_LEVEL_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			/* Config RemainingTime */
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_LEVEL_CONTROL_CLUSTER_ID,\
			ZCL_LEVEL_CONTROL_REMAINING_TIME_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			/* Config OnOffTransitionTime */
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_LEVEL_CONTROL_CLUSTER_ID,\
			ZCL_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			/* Config OnLevel*/
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_LEVEL_CONTROL_CLUSTER_ID,\
			ZCL_ON_LEVEL_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);	
#endif
		}
		break;
				
		case ZCL_IAS_ZONE_CLUSTER_ID:/*ZigBee IAS Zone Cluster*/
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is ZCL_IAS_ZONE_CLUSTER_ID cluster,begin bind,read and config report.\n");
			
			/*Read Zone Type*/
			HSZRequestAttribute(dev_mac,epcount,ZCL_IAS_ZONE_CLUSTER_ID,ZCL_ZONE_TYPE_ATTRIBUTE_ID);			
			/*Read Zone State*/
			HSZRequestAttribute(dev_mac,epcount,ZCL_IAS_ZONE_CLUSTER_ID,ZCL_ZONE_STATE_ATTRIBUTE_ID);			
			/*Read Zone Status*/
			HSZRequestAttribute(dev_mac,epcount,ZCL_IAS_ZONE_CLUSTER_ID,ZCL_ZONE_STATUS_ATTRIBUTE_ID);
		
			//HSZBindNewDevice(epcount,dev_mac,ZCL_IAS_ZONE_CLUSTER_ID);
		
			/*Config Zone Status*/
			// HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_POWER_CONFIG_CLUSTER_ID,
			// ZCL_BATTERY_PERCENTAGE_REMAINING_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);	
			
			/*Config RSSI Status*/
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_DIAGNOSTICS_CLUSTER_ID,\
			ZCL_LAST_MESSAGE_RSSI_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);	
		}
		break;
		
		case ZCL_OCCUPANCY_SENSING_CLUSTER_ID: /*Occupancy Sensing Cluster*/
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is ZCL_OCCUPANCY_SENSING_CLUSTER_ID cluster,begin bind,read and config report.\n");
#if 0			
			/*Read Occupancy Attribute*/
			HSZRequestAttribute(dev_mac,epcount,ZCL_OCCUPANCY_SENSING_CLUSTER_ID,ZCL_OCCUPANCY_ATTRIBUTE_ID);			
			/*Occupancy Sensor Type*/
			HSZRequestAttribute(dev_mac,epcount,ZCL_OCCUPANCY_SENSING_CLUSTER_ID,ZCL_OCCUPANCY_SENSOR_TYPE_ATTRIBUTE_ID);

			HSZBindNewDevice(epcount,dev_mac,ZCL_OCCUPANCY_SENSING_CLUSTER_ID);

			/*Occupancy Attribute:8-bit bitmap*/
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_OCCUPANCY_SENSING_CLUSTER_ID,\
			ZCL_OCCUPANCY_ATTRIBUTE_ID,ZCL_BITMAP8_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			/*Occupancy Sensor Type:8-bit enumeration*/
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_OCCUPANCY_SENSING_CLUSTER_ID,\
			ZCL_OCCUPANCY_SENSOR_TYPE_ATTRIBUTE_ID,ZCL_ENUM8_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
#endif
		}
		break;
		
		case ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID: /* Humidity Cluster*/
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is HUMIDITY_CLUSTER cluster,begin bind,read and config report.\n");
#if 0			
			/* Read Measured Value*/
			HSZRequestAttribute(dev_mac,epcount,ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID,ZCL_RELATIVE_HUMIDITY_MEASURED_VALUE_ATTRIBUTE_ID);
			/* Read Min Measured Value */
			HSZRequestAttribute(dev_mac,epcount,ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID,ZCL_RELATIVE_HUMIDITY_MIN_MEASURED_VALUE_ATTRIBUTE_ID);
			/* Read MAX Measured Value */
			HSZRequestAttribute(dev_mac,epcount,ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID,ZCL_RELATIVE_HUMIDITY_MAX_MEASURED_VALUE_ATTRIBUTE_ID);

			HSZBindNewDevice(epcount,dev_mac,ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID);

			/* Read Measured Value*/
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID,\
			ZCL_RELATIVE_HUMIDITY_MEASURED_VALUE_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);

			/* Read Min Measured Value*/
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID,\
			ZCL_RELATIVE_HUMIDITY_MIN_MEASURED_VALUE_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);

			/* Read MAX Measured Value*/
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID,\
			ZCL_RELATIVE_HUMIDITY_MAX_MEASURED_VALUE_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
#endif
		}
		break;
			
		case ZCL_TEMP_MEASUREMENT_CLUSTER_ID:/* ZigBee Temperature Cluster */
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is TEMPERATURE_CLUSTER cluster,begin bind,read and config report.\n");
#if 0			
			/*Read Current Temperature */
			HSZRequestAttribute(dev_mac,epcount,ZCL_TEMP_MEASUREMENT_CLUSTER_ID,ZCL_TEMP_MEASURED_VALUE_ATTRIBUTE_ID);
			
			HSZBindNewDevice(epcount,dev_mac,ZCL_TEMP_MEASUREMENT_CLUSTER_ID);
			
			/*Set Report Current Temperature */
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_TEMP_MEASUREMENT_CLUSTER_ID,\
			ZCL_TEMP_MEASURED_VALUE_ATTRIBUTE_ID,ZCL_INT16S_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
#endif 
		}
		break;
		
		case ZCL_ILLUM_MEASUREMENT_CLUSTER_ID: /* Illuminance Cluster */
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is TEMPERATURE_CLUSTER cluster,begin bind,read and config report.\n");
#if 0			
			/* Read Measured Value*/
			HSZRequestAttribute(dev_mac,epcount,ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,ZCL_ILLUM_MEASURED_VALUE_ATTRIBUTE_ID);
			/* Read Min Measured Value */
			HSZRequestAttribute(dev_mac,epcount,ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,ZCL_ILLUM_MIN_MEASURED_VALUE_ATTRIBUTE_ID);
			/* Read MAX Measured Value */
			HSZRequestAttribute(dev_mac,epcount,ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,ZCL_ILLUM_MAX_MEASURED_VALUE_ATTRIBUTE_ID);
			
			HSZBindNewDevice(epcount,dev_mac,ZCL_ILLUM_MEASUREMENT_CLUSTER_ID);
			
			/* Set Report Measured Value*/
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,\
			ZCL_ILLUM_MEASURED_VALUE_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			
			/* Set Report Min Measured Value*/
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,\
			ZCL_ILLUM_MIN_MEASURED_VALUE_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			
			/* Set Report MAX Measured Value*/
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_ILLUM_MEASUREMENT_CLUSTER_ID,\
			ZCL_ILLUM_MAX_MEASURED_VALUE_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
#endif				
		}
		break;
		
		case ZCL_SIMPLE_METERING_CLUSTER_ID: /* Simple Metering Cluster */
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is SIMPLE_METERING cluster,begin bind,read and config report.\n");
#if 0			
			HSZRequestAttribute(dev_mac,epcount,ZCL_SIMPLE_METERING_CLUSTER_ID,ZCL_CURRENT_SUMMATION_DELIVERED_ATTRIBUTE_ID);
			HSZRequestAttribute(dev_mac,epcount,ZCL_SIMPLE_METERING_CLUSTER_ID,ZCL_UNIT_OF_MEASURE_ATTRIBUTE_ID);
			HSZRequestAttribute(dev_mac,epcount,ZCL_SIMPLE_METERING_CLUSTER_ID,ZCL_MULTIPLIER_ATTRIBUTE_ID);
			HSZRequestAttribute(dev_mac,epcount,ZCL_SIMPLE_METERING_CLUSTER_ID,ZCL_DIVISOR_ATTRIBUTE_ID);
			HSZRequestAttribute(dev_mac,epcount,ZCL_SIMPLE_METERING_CLUSTER_ID,ZCL_SUMMATION_FORMATTING_ATTRIBUTE_ID);
			
			HSZBindNewDevice(epcount,dev_mac,ZCL_SIMPLE_METERING_CLUSTER_ID);
						
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_SIMPLE_METERING_CLUSTER_ID,\
			ZCL_CURRENT_SUMMATION_DELIVERED_ATTRIBUTE_ID,ZCL_INT48U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_SIMPLE_METERING_CLUSTER_ID,\
			ZCL_UNIT_OF_MEASURE_ATTRIBUTE_ID,ZCL_ENUM8_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_SIMPLE_METERING_CLUSTER_ID,\
			ZCL_MULTIPLIER_ATTRIBUTE_ID,ZCL_INT24U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_SIMPLE_METERING_CLUSTER_ID,\
			ZCL_DIVISOR_ATTRIBUTE_ID,ZCL_INT24U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_SIMPLE_METERING_CLUSTER_ID,\
			ZCL_SUMMATION_FORMATTING_ATTRIBUTE_ID,ZCL_BITMAP8_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
#endif
		}
		break;
		
		case ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID: /* Electrical Measurement Cluster */ 
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is ELECTRICAL_MEASUREMENT cluster,begin bind,read and config report.\n");
#if 0			
			HSZRequestAttribute(dev_mac,epcount,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,ZCL_RMS_VOLTAGE_ATTRIBUTE_ID);
			HSZRequestAttribute(dev_mac,epcount,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,ZCL_RMS_CURRENT_ATTRIBUTE_ID);
			HSZRequestAttribute(dev_mac,epcount,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,ZCL_ACTIVE_POWER_ATTRIBUTE_ID);
			
			HSZRequestAttribute(dev_mac,epcount,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,ZCL_AC_OVERLOAD_ALARMS_MASK_ATTRIBUTE_ID);
			HSZRequestAttribute(dev_mac,epcount,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,ZCL_AC_VOLTAGE_OVERLOAD_ATTRIBUTE_ID);
			HSZRequestAttribute(dev_mac,epcount,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,ZCL_AC_CURRENT_OVERLOAD_ATTRIBUTE_ID);
			HSZRequestAttribute(dev_mac,epcount,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,ZCL_AC_POWER_OVERLOAD_ATTRIBUTE_ID);
						
			HSZBindNewDevice(epcount,dev_mac,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID);
			
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,\
			ZCL_RMS_VOLTAGE_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,\
			ZCL_RMS_CURRENT_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,\
			ZCL_ACTIVE_POWER_ATTRIBUTE_ID,ZCL_INT16S_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,\
			ZCL_AC_OVERLOAD_ALARMS_MASK_ATTRIBUTE_ID,ZCL_BITMAP16_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,\
			ZCL_AC_VOLTAGE_OVERLOAD_ATTRIBUTE_ID,ZCL_INT16S_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,\
			ZCL_AC_CURRENT_OVERLOAD_ATTRIBUTE_ID,ZCL_INT16S_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID,\
			ZCL_AC_POWER_OVERLOAD_ATTRIBUTE_ID,ZCL_INT16S_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
#endif
		}
		break;
		
		case ZCL_OTA_BOOTLOAD_CLUSTER_ID:  /* OTA Bootload Cluster */
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is ZCL_OTA_BOOTLOAD_CLUSTER_ID cluster,begin bind,read and config report.\n");
			
			HSZRequestAttribute(dev_mac,epcount,ZCL_OTA_BOOTLOAD_CLUSTER_ID,ZCL_CURRENT_FILE_VERSION_ATTRIBUTE_ID);
			HSZRequestAttribute(dev_mac,epcount,ZCL_OTA_BOOTLOAD_CLUSTER_ID,ZCL_IMAGE_TYPE_ID_ATTRIBUTE_ID);
			HSZRequestAttribute(dev_mac,epcount,ZCL_OTA_BOOTLOAD_CLUSTER_ID,ZCL_MANUFACTURER_ID_ATTRIBUTE_ID);
			
			HSZBindNewDevice(epcount,dev_mac,ZCL_OTA_BOOTLOAD_CLUSTER_ID);
#if 0			
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_OTA_BOOTLOAD_CLUSTER_ID,\
			ZCL_CURRENT_FILE_VERSION_ATTRIBUTE_ID,ZCL_INT32U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_OTA_BOOTLOAD_CLUSTER_ID,\
			ZCL_IMAGE_TYPE_ID_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_OTA_BOOTLOAD_CLUSTER_ID,\
			ZCL_MANUFACTURER_ID_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
#endif
		}
		break;
		
		case ZCL_POWER_CONFIG_CLUSTER_ID: /* Power Config Cluster */
		{
			/*------20180119 peiyongxia: Do not need to bing power config
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is ZCL_POWER_CONFIG_CLUSTER_ID cluster,begin bind,read and config report.\n");
			
			HSZRequestAttribute(dev_mac,epcount,ZCL_POWER_CONFIG_CLUSTER_ID,ZCL_MAINS_VOLTAGE_ATTRIBUTE_ID);
			HSZRequestAttribute(dev_mac,epcount,ZCL_POWER_CONFIG_CLUSTER_ID,ZCL_BATTERY_VOLTAGE_ATTRIBUTE_ID);
			
			HSZBindNewDevice(epcount,dev_mac,ZCL_POWER_CONFIG_CLUSTER_ID);
			
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_POWER_CONFIG_CLUSTER_ID,\
			ZCL_MAINS_VOLTAGE_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);			
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_POWER_CONFIG_CLUSTER_ID,\
			ZCL_BATTERY_VOLTAGE_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
			*/
		}
		break;
		
		case ZCL_ALARM_CLUSTER_ID:/* Alarm Cluster */
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is ZCL_ALARM_CLUSTER_ID cluster,begin bind,read and config report.\n");
#if 0				
			/* Alarm Count*/
			HSZRequestAttribute(dev_mac,epcount,ZCL_ALARM_CLUSTER_ID,ZCL_ALARM_COUNT_ATTRIBUTE_ID);
			
			HSZBindNewDevice(epcount,dev_mac,ZCL_ALARM_CLUSTER_ID);
		
			/* Set Alarm Count*/
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_ALARM_CLUSTER_ID,\
			ZCL_ALARM_COUNT_ATTRIBUTE_ID,ZCL_INT16U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);		
#endif
		}
		break;
		
		
		case ZCL_WINDOW_COVERING_CLUSTER_ID:/*Window covering*/
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is ZCL_WINDOW_COVERING_CLUSTER_ID cluster,begin bind,read and config report.\n");
			
			// HSZRequestAttribute(dev_mac,epcount,ZCL_WINDOW_COVERING_CLUSTER_ID,ZCL_COVERING_TYPE_ATTRIBUTE_ID);
			
			// HSZBindNewDevice(epcount,dev_mac,ZCL_WINDOW_COVERING_CLUSTER_ID);
			
			// HSZNewDeviceConfigReportWithPower(epcount,dev_mac,ZCL_WINDOW_COVERING_CLUSTER_ID,
			// ZCL_CONFIG_STATUS_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME_WITH_POWER,NULL);
			
			/*Config RSSI Status*/	
			
			HSZNewDeviceConfigReportWithPower(epcount,dev_mac,ZCL_DIAGNOSTICS_CLUSTER_ID,\
			ZCL_LAST_MESSAGE_RSSI_ATTRIBUTE_ID,ZCL_BOOLEAN_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME_WITH_POWER,NULL);
			
			DEBUG_ZONE(LOG_ZONE_MY,"HSZNewDeviceConfigReportWithPower,ZCL_WINDOW_COVERING_CLUSTER_ID：\
			Dev_mac:%s,endpoint:%d.\n",dev_mac,epcount);

		}
		break;
		
		case ZCL_IAS_WD_CLUSTER_ID:/*IAS Warning Device*/
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is ZCL_IAS_WD_CLUSTER_ID cluster,begin bind,read and config report.\n");
			
			HSZRequestAttribute(dev_mac,epcount,ZCL_IAS_WD_CLUSTER_ID,ZCL_MAX_DURATION_ATTRIBUTE_ID);
						
			/*Config RSSI Status*/
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_DIAGNOSTICS_CLUSTER_ID,\
			ZCL_LAST_MESSAGE_RSSI_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);	
			
#if 0			
			HSZBindNewDevice(epcount,dev_mac,ZCL_IAS_WD_CLUSTER_ID);
			
			
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_IAS_WD_CLUSTER_ID,\
			ZCL_CONFIG_STATUS_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);
#endif 
		}
		break;
		case ZCL_DOOR_LOCK_CLUSTER_ID:
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Joined device is ZCL_DOOR_LOCK_CLUSTER_ID cluster,begin bind,read and config report.\n");
			HSZNewDeviceConfigReport(epcount,dev_mac,ZCL_DIAGNOSTICS_CLUSTER_ID,\
			ZCL_LAST_MESSAGE_RSSI_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME,NULL);	
		}
		break;
		case CLOTHES_HANGER_CLUSTER:
		{
			HSZNewDeviceConfigReportWithPower(epcount,dev_mac,ZCL_DIAGNOSTICS_CLUSTER_ID,\
			ZCL_LAST_MESSAGE_RSSI_ATTRIBUTE_ID,ZCL_INT8U_ATTRIBUTE_TYPE,SET_REPORT_ATTRIBUTE_MAX_TIME_WITH_POWER,NULL);
		}
		break;
	
		default:
		{
			DEBUG_ZONE(LOG_ZONE_TEST,"Warning:This cluster no need to bind :0x%04x,.\n",clusterid);
		}
		break;
	}
	
	return 0;
}



