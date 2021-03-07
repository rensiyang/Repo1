/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZHandleZclData.c
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
#include "HSZHandleZclData.h"
#include "HSZSmartLockMessage.h"



/*****************************************************************************
 函 数 名  : HSZProcessZoneDeviceType
 功能描述  : 分析Zone设备类型
 输入参数  : int zone_dev_type ：入网设备Zone类型
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessZoneDeviceType(int zone_dev_type)
{
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZProcessZoneDeviceType:0x%04x.\n",zone_dev_type);
	
	switch(zone_dev_type)
	{
		case STANDARD_CIE:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"STANDARD_CIE.\n");
		}
		break;
		
		case MOTION_SENSOR:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"MOTION_SENSOR.\n");
		}
		break;
		
		case CONTACT_SWITCH:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"CONTACT_SWITCH.\n");
		}
		break;
		
		case FIRE_SENSOR:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"FIRE_SENSOR.\n");
		}
		break;
		
		case WATER_SENSOR:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"WATER_SENSOR.\n");
		}
		break;
		
		case GAS_SENSOR:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"GAS_SENSOR.\n");
		}
		break;
		
		case PERSONAL_EMERGENCY_DEVICE:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"PERSONAL_EMERGENCY_DEVICE.\n");
		}
		break;
		
		case MOVEMENT_SENSOR:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"MOVEMENT_SENSOR.\n");
		}
		break;
		
		
		case REMOTE_CONTROL:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"REMOTE_CONTROL.\n");
		}
		break;
		
		case KEY_FOB:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"KEY_FOB.\n");
		}
		break;
		
		case KEY_PAD:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"KEY_PAD.\n");
		}
		break;
		
		case STANDARD_WARNING_DEVICE:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"STANDARD_WARNING_DEVICE.\n");
		}
		break;
		
		default:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"The device type is unkown:RESERCED.\n");
		}
		break;
	}
}

/*****************************************************************************
 函 数 名  : HSZProcessZoneDeviceZclData
 功能描述  : 分析ZCL Status data 数据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zone_dev_zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessZoneDeviceZclData(char* dev_mac,int dev_endpoint,int zone_dev_zcl_data)
{
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZProcessZoneDeviceZclDate:0x%04x.\n",zone_dev_zcl_data);
	
	int zone_data = 0x00;
	/*bit0 = 1:Alarm1->opened or alarmed
	       = 0:Alarm1->closed or not alarmed */	   
	if(BITMAP_16BIT_BIT0&zone_dev_zcl_data)
	{
		zone_data = zone_data|BITMAP_16BIT_BIT0;
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Alarm1->opened or alarmed，zone：%d.\n",zone_data);	
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Alarm1->closed or not alarmed，zone：%d.\n",zone_data);	
	}
	
	/*bit1 = 1:Alarm2->opened or alarmed
	       = 0:Alarm2->closed or not alarmed */		   
    if(BITMAP_16BIT_BIT1&zone_dev_zcl_data)
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Alarm2->opened or alarmed.\n");
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Alarm2->closed or not alarmed .\n");
	}
		   
	/*bit2 = 1:Tamper->Tampered
	       = 0:Tamper->Not Tampered */
	if(BITMAP_16BIT_BIT2&zone_dev_zcl_data)
	{
		zone_data = zone_data|BITMAP_16BIT_BIT1;
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Tamper->Tampere,zone%d.\n",zone_data);	
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Tamper->Not Tampered,zone%d.\n",zone_data);	
	}
    	
	/*bit3 = 1:Battery->Low battery
	       = 0:Battery->Battery OK */
	
	if(BITMAP_16BIT_BIT3&zone_dev_zcl_data)
	{
		zone_data = zone_data|BITMAP_16BIT_BIT2;
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Battery->Low battery,zone%d.\n",zone_data);	
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Battery->Battery OK,zone%d.\n",zone_data);	
	}
	
  
	
	/*bit4 = 1:Supervision reports (Note 1)->Low battery
		   = 0:Supervision reports (Note 1)->Battery OK */
	if(BITMAP_16BIT_BIT4&zone_dev_zcl_data)
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Supervision reports (Note 1)->Low battery.\n");
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Supervision reports (Note 1)->Battery OK.\n");
	}

	/*bit5 = 1:Supervision reports (Note 2)->Low battery
	       = 0:Supervision reports (Note 2)->Battery OK */   
	if(BITMAP_16BIT_BIT5&zone_dev_zcl_data)
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Supervision reports (Note 2)->Low battery.\n");
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Supervision reports (Note 2)->Battery OK.\n");
	}

	/*bit6 = 1:Trouble->Low battery
	       = 0:Trouble->Battery OK */
	if(BITMAP_16BIT_BIT6&zone_dev_zcl_data)
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Trouble->Low battery.\n");
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Trouble->Battery OK.\n");
	}

	/*bit7 = 1:AC (mains)->Low battery
	       = 0:AC (mains)->Battery OK */
	if(BITMAP_16BIT_BIT7&zone_dev_zcl_data)
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"AC (mains)->Low battery.\n");
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"AC (mains)->Battery OK.\n");
	}
		   
	/*bit8- bit15 Reserved */

	/*上报解析封装的传感器数据*/
	// DEBUG_ZONE(LOG_ZONE_MY,"Zone type：update line status and update zone_data：0x%02x.\n",zone_data);
	
	if((-1!= HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))&&(0x0403!= HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV)))
	{
		ResponsePutContactSensor(HSZGatewayMac(),dev_mac,dev_endpoint,zone_data);
	}
	else if((0x0403 == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV)))
	{
		ResponsePutWarningDeviceControlStatus(HSZGatewayMac(),dev_mac,dev_endpoint,zone_data);
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_MY,"----Zone type error ：zcl data.\n");
	}
}

/*****************************************************************************
 函 数 名  : HSZProcessZoneEnrollRequestCmdData
 功能描述  : 分析Zone设备enroll request command data
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessZoneEnrollRequestCmdData(char* dev_mac,int dev_endpoint,char* zcl_data)
{			
	/*		
	{"clusterId":"0x0500","commandId":"0x01","commandData":"0x010000311500","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000BEC608A","endpoint":1}}.							{"clusterId":"0x0500","commandId":"0x01","commandData":"0x0000003000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000BEC608A","endpoint":1}}.						{"clusterId":"0x0500","commandId":"0x01","commandData":"0x020000190100","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000BEC608A","endpoint":1}}.	
	
	*/
										
	if(strlen(zcl_data)>11)
	{
		char attri_id_buf[5] = {0};
		char attri_buf[5] = {0};
		char attri_data_type_buf[5] = {0};	

		int attri_id = -1;
		int attri_buf_data = -1;
		int attri_data_type = -1;

		int attribute_id = -1;
		int attribute_type = -1;
		int attribute_date = -1;

		/* Get the attribute id */
		strncpy(attri_id_buf,zcl_data + 2,4);
		attri_id = strtol(attri_id_buf,NULL,16);

		attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
					
		DEBUG_ZONE(LOG_ZONE_TEST,"Zone Enroll Request Cmd Data Attribute_id:0x%04x.\n",attribute_id);							

		/* Get the attribute data type */
		strncpy(attri_data_type_buf,zcl_data+6,4);	

		attri_data_type = strtol(attri_data_type_buf,NULL,16);

		attribute_type = (attri_data_type&0x00FF);

		DEBUG_ZONE(LOG_ZONE_TEST,"Zone Enroll Request Command Data Attribute_type:0x%02x.\n",attribute_type);


		switch(attribute_id)
		{	
			case ZONE_STATE:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Zone Enroll Request Command Data Attribute_type: ZONE_STATE.\n");

				if(ZCL_ENUM8_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"Zone Enroll Request ZONE_STATE Attribute Data Type:ZCL_ENUM8_ATTRIBUTE_TYPE.\n");
					
					/* Get the ZONE_STATE attribute data*/
					strncpy(attri_buf,zcl_data+10,2);	

					attri_buf_data = strtol(attri_buf,NULL,16);
												
					DEBUG_ZONE(LOG_ZONE_TEST,"Zone Enroll Request ZONE_STATE Attribute Data value:%02x.\n",attri_buf_data);
					
					if(attri_buf_data == 0x00)
					{
						DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Zone ZONE_STATE:enrolled.\n");
					}
					else if (attri_buf_data == 0x01)
					{
						DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Zone ZONE_STATE:Enrolled (the client will react to Zone State Change Notification commands from the server).\n");
					}
					else
					{
						DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Reserved.\n");
					}
				
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"ZONE_STATE Attribute Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"ZONE_STATE Attribute Data Type Error.",NULL);
				}
			}
			break;

			case ZONE_TYPE:
			{									
				DEBUG_ZONE(LOG_ZONE_TEST,"Zone Enroll Request Command Data：ZONE_TYPE.\n");
				char dev_type[16] = {0};
				
				if((ZCL_ENUM16_ATTRIBUTE_TYPE == attribute_type)||(ZCL_INT16U_ATTRIBUTE_TYPE== attribute_type))
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"ZONE_TYPE Attribute Value Type:ZCL_ENUM16_ATTRIBUTE_TYPE.\n");
					
					/* Get the ZONE_TYPE attribute data*/
					strncpy(attri_buf,zcl_data+10,4);	

					attri_buf_data = strtol(attri_buf,NULL,16);

					attribute_date =  ((attri_buf_data&0x00FF)<<8)|((attri_buf_data&0xFF00)>>8);
					
					DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"ZONE_TYPE Attribute Value:0x%04x.\n",attribute_date);
															
					sprintf(dev_type,"0x%04x",attribute_date);
					
					if(DEVICE_JOIN_ON_INIT == HSZFindJoinDevStatusInfo(dev_mac))
					{
						if ((hsbk_devicejoin!=NULL) && (-1!= HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))\
						&&(0x0403!= HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV)))
						{
							hsbk_devicejoin(HSZGatewayMac(),dev_type,dev_mac,dev_endpoint);
							
							printf("****************Device join and Report*************.\n");
						}
						else
						{
							printf("****************Device join and Not Report*************.\n");
						}
						
						HSZUpdateDeviceJoinStatus(dev_mac,DEVICE_JOIN_ON_FINISH);
					}
					else
					{
						printf("*********Device join and Not Report:DEVICE_JOIN_ON_FINISH*************.\n");
					}

					HSZProcessZoneDeviceType(attribute_date);
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"ZONE_TYPE Attribute Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"ZONE_TYPE Attribute Data Type Error.",NULL);
				}
			}
			break;

			case ZONE_STATUS:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Zone Enroll Request Command Data:ZONE_STATUS.\n");
				
				int get_status_data = -1;
				
				if(ZCL_BITMAP16_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"ZONE_STATUS Attribute Data Type:ZCL_BITMAP16_ATTRIBUTE_TYPE.\n");

					/* Get the ZONE_STATUS attribute data */
					strncpy(attri_buf,zcl_data+10,4);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_status_data = ((attri_buf_data&0x00FF)<<8)|((attri_buf_data&0xFF00)>>8);

					DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"ZONE_STATUS Attribute Data:0x%04x.\n",get_status_data);										
					
					HSZProcessZoneDeviceZclData(dev_mac,dev_endpoint,get_status_data);
					
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"ZONE_STATUS Attribute Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"ZONE_STATUS Attribute Data Type Error.",NULL);
				}				
			}
			break;
			
			default:
			break;
		}						
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"Zone Enroll Request Command ZCL data length Error.\n");
		HSZDeviceError(HSZ_ERR_ZCL_DATA_LEN,"Zone Enroll Request Command ZCL data length Error.",NULL);
	}
}

/*****************************************************************************
 函 数 名  : HSZProcessZoneReportAttributeCmdData
 功能描述  : 分析Zone设备Report attribute command data
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessZoneReportAttributeCmdData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/* {"clusterId":"0x0500","commandId":"0x0A","commandData":"0x0200190000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000CAB1016","endpoint":1}}.
	{"clusterId":"0x0500","attributeId":"0x0002","attributeBuffer":"0x0000","attributeDataType":"0x19","deviceEndpoint":{"eui64":"0x000D6F000CAB1016","endpoint":1}}.
	
	*/
	
	if(strlen(zcl_data)>11)
	{
		char attri_id_buf[5] = {0};
		char attri_buf[5] = {0};
		char attri_data_type_buf[5] = {0};	

		int attri_id = -1;
		int attri_buf_data = -1;
		int attri_data_type = -1;

		int attribute_id = -1;
		int attribute_type = -1;

		/* Get the attribute id */
		strncpy(attri_id_buf,zcl_data + 2,4);
		attri_id = strtol(attri_id_buf,NULL,16);

		attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
					
		DEBUG_ZONE(LOG_ZONE_TEST,"Zone Report Attribute id:0x%04x.\n",attribute_id);							

		/* Get the attribute data type */
		strncpy(attri_data_type_buf,zcl_data+6,2);	

		attri_data_type = strtol(attri_data_type_buf,NULL,16);

		attribute_type = (attri_data_type&0xFF);

		DEBUG_ZONE(LOG_ZONE_TEST,"Zone Report Attribute Type:0x%02x.\n",attribute_type);


		switch(attribute_id)
		{				
			case ZONE_STATUS:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Zone Report Attribute Command Data:Zone Report Status.\n");
				
				int get_status_data = -1;
				
				if(ZCL_BITMAP16_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"Zone Attribute Data Type:ZCL_BITMAP16_ATTRIBUTE_TYPE.\n");

					/* Get the ZONE_STATUS attribute data */
					strncpy(attri_buf,zcl_data+8,4);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_status_data = ((attri_buf_data&0x00FF)<<8)|((attri_buf_data&0xFF00)>>8);

					DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Zone Report Attribute Status Data:0x%04x.\n",get_status_data);
										
					HSZProcessZoneDeviceZclData(dev_mac,dev_endpoint,get_status_data);
					
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"ZONE_STATUS Report Attribute Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"ZONE_STATUS Report Attribute Data Type Error.",NULL);
				}				
			}
			break;
			
			default:
			break;
		}						
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"Zone Enroll Report Command ZCL data length Error.\n");
		HSZDeviceError(HSZ_ERR_ZCL_DATA_LEN,"Zone Enroll Report Command ZCL data length Error.",NULL);
	}	
}
/*****************************************************************************
 函 数 名  : HSZProcessZoneStatusChangeNotifyCmdData
 功能描述  : 分析Zone设备状态改变通知数据
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessZoneStatusChangeNotifyCmdData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*		{"clusterId":"0x0500","commandId":"0x00","commandData":"0x000000000000","clusterSpecific":true,"deviceEndpoint":{"eui64":"0x000B57FFFE6E3D36","endpoint":1}}//合法闭合状态
	{"clusterId":"0x0500","commandId":"0x00","commandData":"0x010000000100","clusterSpecific":true,"deviceEndpoint":{"eui64":"0x00124B000B7B1156","endpoint":1}}.//合法打开状态
	{"clusterId":"0x0500","commandId":"0x00","commandData":"0x050000000100","clusterSpecific":true,"deviceEndpoint":{"eui64":"0x00124B000B7B1156","endpoint":1}}.//非法打开状态
	{"clusterId":"0x0500","commandId":"0x00","commandData":"0x040000000100","clusterSpecific":true,"deviceEndpoint":{"eui64":"0x00124B000B7B1156","endpoint":1}}.//非法闭合状态
	{"clusterId":"0x0500","commandId":"0x01","commandData":"0x100000F011AB07FEFF570B00","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B000B7B1156","endpoint":1}}
	{"clusterId":"0x0500","commandId":"0x04","commandData":"0x00","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B000B7B1156","endpoint":1}}
	{"clusterId":"0x0500","commandId":"0x07","commandData":"0x8D000200","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B000B7B1156","endpoint":1}}				
	*/
	
	if (strlen(zcl_data)>13)
	{
		char status_change_cmd_buf[5] = {0};

		int status_change_id = -1;
		int status_change_cmd_date = -1;

		/* Get the status_cmd_change data*/
		strncpy(status_change_cmd_buf,zcl_data + 2,4);
		status_change_id = strtol(status_change_cmd_buf,NULL,16);

		status_change_cmd_date = ((status_change_id&0x00FF)<<8)|((status_change_id&0xFF00)>>8);
		
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Zone Status Change Notify Command Data:0x%04x.\n",status_change_cmd_date);
		
		HSZProcessZoneDeviceZclData(dev_mac,dev_endpoint,status_change_cmd_date);
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"Zone Status Change Notify Command Data length Error.\n");
		HSZDeviceError(HSZ_ERR_ZCL_DATA_LEN,"Zone Status Change Notify Command Data length Error.",NULL);
	}
}
/*****************************************************************************
 函 数 名  : HSZProcessOccupancySensorReadData
 功能描述  : 分析occupany sensor read attribute data数据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessOccupancySensorReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	char occup_attri_id_buf[5] = {0};
	char occup_attri_buf[5] = {0};
	char occup_attri_data_type_buf[5] = {0};

	int attri_id = -1;
	int attri_buf_data = -1;
	int attri_data_type = -1;

	int occup_attribute_id = -1;
	int occup_attribute_type = -1;
	int occup_attribute_date = -1;
	
	int occup_dev_type = -1;
		
	/*						{"clusterId":"0x0406","commandId":"0x01","commandData":"0x0000001801","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE623488","endpoint":1}}.
	{"clusterId":"0x0406","commandId":"0x01","commandData":"0x0100003000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE623488","endpoint":1}}.
	*/

	if(strlen(zcl_data)>11)
	{
		/* Get the Occupancy attribute id */
		strncpy(occup_attri_id_buf,zcl_data + 2,4);
		attri_id = strtol(occup_attri_id_buf,NULL,16);

		occup_attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);

		DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Attribute Id:0x%04x.\n",occup_attribute_id);							

		/* Get the Occupancy attribute data type */
		strncpy(occup_attri_data_type_buf,zcl_data+6,4);	

		attri_data_type = strtol(occup_attri_data_type_buf,NULL,16);

		occup_attribute_type = (attri_data_type&0x00FF);

		DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Attribute Type:0x%02x.\n",occup_attribute_type);


		switch(occup_attribute_id)
		{
			case ZCL_OCCUPANCY_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Attribute ID:ZCL_OCCUPANCY_ATTRIBUTE_ID.\n");

				if(ZCL_BITMAP8_ATTRIBUTE_TYPE == occup_attribute_type)
				{
					/* Get the Occupancy attribute data*/

					DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Attribute Data Type:ZCL_BITMAP8_ATTRIBUTE_TYPE.\n");

					strncpy(occup_attri_buf,zcl_data+10,2);	

					attri_buf_data = strtol(occup_attri_buf,NULL,16);

					occup_attribute_date = (attri_buf_data&0x01);
										
					DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Occupancy Attribute Data:%02x.\n",occup_attribute_date);

					if(occup_attribute_date == 0x00)
					{
						DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Occupancy Sensor: Not Occupied.\n");
					}
					else if (occup_attribute_date == 0x01)
					{
						DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Occupancy Sensor:Occupied.\n");
					}
					else
					{
						DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Reserved.\n");
					}
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"ZCL_OCCUPANCY_ATTRIBUTE_ID Read Attribute Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"ZCL_OCCUPANCY_ATTRIBUTE_ID Read Attribute Data Type Error.",NULL);
				}	
				
			}
		    break;
		
			case ZCL_OCCUPANCY_SENSOR_TYPE_ATTRIBUTE_ID:
			{
				char occup_dev_type_buf[16] = {0};
				DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Attribute ID:ZCL_OCCUPANCY_SENSOR_TYPE_ATTRIBUTE_ID.\n");

				if(ZCL_ENUM8_ATTRIBUTE_TYPE == occup_attribute_type)
				{
					/* Get the Occupancy attribute data*/

					DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Attribute Data Type:ZCL_ENUM8_ATTRIBUTE_TYPE.\n");

					strncpy(occup_attri_buf,zcl_data+10,2);	

					attri_buf_data = strtol(occup_attri_buf,NULL,16);

					occup_attribute_date = (attri_buf_data&0xFF);
											
					DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Type Data:%02x.\n",occup_attribute_date);


					
					switch(occup_attribute_date)
					{
						case OCCUPANCY_PIR:
						{
							DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Occupancy Sensor Type:OCCUPANCY_PIR.\n");
							occup_dev_type = OCCUPANCY_PIR_TYPE;
						}
						break;

						case OCCUPANCY_ULTRASONIC:
						{
							DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Occupancy Sensor Type:OCCUPANCY_ULTRASONIC.\n");\
							occup_dev_type = OCCUPANCY_ULTRASONIC_TYPE;
						}
						break;

						case OCCUPANCY_PIR_ULTRASONIC:
						{
							DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Occupancy Sensor Type:OCCUPANCY_PIR_ULTRASONIC.\n");
							occup_dev_type = OCCUPANCY_PIR_ULTRASONIC_TYPE;
						}
						break;

						default:
						break;
					}
					
					/*上传Occupation 设备类型*/
					if(-1!= occup_dev_type)
					{
						sprintf(occup_dev_type_buf,"0x%04x",occup_dev_type);
						
						if(DEVICE_JOIN_ON_FINISH != HSZFindJoinDevStatusInfo(dev_mac))
						{
							if (hsbk_devicejoin!=NULL)
							{
								hsbk_devicejoin(HSZGatewayMac(),occup_dev_type_buf,dev_mac,dev_endpoint);
							}
							
							HSZUpdateDeviceJoinStatus(dev_mac,DEVICE_JOIN_ON_FINISH);
						}
					}
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"ZCL_OCCUPANCY_SENSOR_TYPE_ATTRIBUTE_ID Read Attribute Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"ZCL_OCCUPANCY_SENSOR_TYPE_ATTRIBUTE_ID Read Attribute Data Type Error.",NULL);
				}					
			}
			break;

			default:
			break;
		}

	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"Occupancy Sensor Read ZCL Data Length Error.\n");
		HSZDeviceError(HSZ_ERR_ZCL_DATA_LEN,"Occupancy Sensor Read ZCL Data Length Error.",NULL);
	}	
}

/*****************************************************************************
 函 数 名  : HSZProcessOccupancySensorReportData
 功能描述  : 分析occupany sensor report attribute data数据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessOccupancySensorReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	char occup_attri_id_buf[5] = {0};
	char occup_attri_buf[5] = {0};
	char occup_attri_data_type_buf[5] = {0};

	int attri_id = -1;
	int attri_buf_data = -1;
	int attri_data_type = -1;

	int occup_attribute_id = -1;
	int occup_attribute_type = -1;
	int occup_attribute_date = -1;
	
	
	/*						{"clusterId":"0x0406","commandId":"0x0A","commandData":"0x00001801","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE623488","endpoint":1}}.			
	{"clusterId":"0x0406","commandId":"0x0A","commandData":"0x01003000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE623488","endpoint":1}}.
	*/
	
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZProcessOccupancySensorReportData.\n");
	
	if(strlen(zcl_data)>9)
	{
		/* Get the Occupancy attribute id */
		strncpy(occup_attri_id_buf,zcl_data + 2,4);
		attri_id = strtol(occup_attri_id_buf,NULL,16);

		occup_attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);

		DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Sensor Report Data Attribute Id:0x%04x.\n",occup_attribute_id);							

		/* Get the Occupancy attribute data type */
		strncpy(occup_attri_data_type_buf,zcl_data+6,2);	

		attri_data_type = strtol(occup_attri_data_type_buf,NULL,16);

		occup_attribute_type = (attri_data_type&0xFF);

		DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Sensor Report Data Attribute Type:0x%02x.\n",occup_attribute_type);


		switch(occup_attribute_id)
		{
			case ZCL_OCCUPANCY_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Sensor Report Data Attribute:ZCL_OCCUPANCY_ATTRIBUTE_ID.\n");

				if(ZCL_BITMAP8_ATTRIBUTE_TYPE == occup_attribute_type)
				{
					/* Get the Occupancy attribute data*/

					DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_BITMAP8_ATTRIBUTE_TYPE.\n");

					strncpy(occup_attri_buf,zcl_data+8,2);	

					attri_buf_data = strtol(occup_attri_buf,NULL,16);

					occup_attribute_date = (attri_buf_data&0x01);
										
					DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Occupancy Sensor Report Date:%02x.\n",occup_attribute_date);

					if(occup_attribute_date == 0x00)
					{
						DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Occupancy sensor: Not occupied.\n");
					}
					else if (occup_attribute_date == 0x01)
					{
						DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Occupancy sensor:Occupied.\n");
					}
					else
					{
						DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Reserved.\n");
					}
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"ZCL_OCCUPANCY_ATTRIBUTE_ID Report Attribute Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"ZCL_OCCUPANCY_ATTRIBUTE_ID Report Attribute Data Type Error.",NULL);
				}	
			}
		    break;
		
			case ZCL_OCCUPANCY_SENSOR_TYPE_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Sensor Report Data Attribute:ZCL_OCCUPANCY_SENSOR_TYPE_ATTRIBUTE_ID.\n");

				if(ZCL_ENUM8_ATTRIBUTE_TYPE == occup_attribute_type)
				{
					/* Get the Occupancy attribute data*/

					DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Sensor Report Data Attribute Type:ZCL_ENUM8_ATTRIBUTE_TYPE.\n");

					strncpy(occup_attri_buf,zcl_data+8,2);	

					attri_buf_data = strtol(occup_attri_buf,NULL,16);

					occup_attribute_date = (attri_buf_data&0xFF);
											
					DEBUG_ZONE(LOG_ZONE_TEST,"Occupancy Sensor Report Data:%02x.\n",occup_attribute_date);

					switch(occup_attribute_date)
					{
					case OCCUPANCY_PIR:
					{
						DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Occupancy Sensor Type:OCCUPANCY_PIR.\n");
					}
					break;

					case OCCUPANCY_ULTRASONIC:
					{
						DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Occupancy Sensor Type:OCCUPANCY_ULTRASONIC.\n");
					}
					break;

					case OCCUPANCY_PIR_ULTRASONIC:
					{
						DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Occupancy Sensor Type:OCCUPANCY_PIR_ULTRASONIC.\n");
					}
					break;

					default:
					break;
					}
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"ZCL_OCCUPANCY_SENSOR_TYPE_ATTRIBUTE_ID Report Attribute Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"ZCL_OCCUPANCY_SENSOR_TYPE_ATTRIBUTE_ID Report Attribute Data Type Error.",NULL);
				}	
			}
			break;

			default:
			break;
		}

	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"Occupancy Sensor Report ZCL Data Length Error.\n");
		HSZDeviceError(HSZ_ERR_ZCL_DATA_LEN,"Occupancy Sensor Report ZCL Data Length Error.",NULL);
	}		
}

/*****************************************************************************
 函 数 名  : HSZProcessOnOffDeviceReportData
 功能描述  : 分析On off device report attribute data数据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessOnOffDeviceReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*		
	{"clusterid":"0x0006","commandid":"0x0a","commandData":"0x00001001","clusterspecific":false,"deviceendpoint":{"eui64":"0x000b57fffe07ab16","endpoint":1}}

	*/
	
	char status_buf[8] = {0};
	int  data_status = 0;
	
	char attribute_buf[8] = {0};
	int  attribute_id = -1;

	DEBUG_ZONE(LOG_ZONE_TEST,"HSZProcessOnOffDeviceReadData.\n");
	
	if(strlen(zcl_data)>9)
	{
		strncpy(attribute_buf,zcl_data+2,4);
		attribute_id = strtol(attribute_buf,NULL,16);

		if(ZCL_ON_OFF_ATTRIBUTE_ID == attribute_id )
		{
			strncpy(status_buf,zcl_data+8,2);
			data_status = strtol(status_buf,NULL,16);

			if((-1!= HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))&&((SCENE_LIGHT == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))||\
			(0xFA3F == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))||\		
			(0xFA4F == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))))
			{
				ResponseSceneSwitchStatus(HSZGatewayMac(),dev_mac,dev_endpoint,(data_status&0x0F));
			}
			else if((-1!= HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))&&((LEVEL_CONTROL_SWITCH == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))||\
			(0xFA5F == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))||\	
			(0xFA6F == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))))
			{
				ResponsePutLevelControlSwitchOnOff(HSZGatewayMac(),dev_mac,dev_endpoint,(data_status&0x0F));
			}
			else if((-1!= HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))&&(SMART_PLUG_IN == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV)))
			{
				ResponseSmartPlugInStatus(HSZGatewayMac(),dev_mac,dev_endpoint,(data_status&0x0F));
			}
			else
			{
				ResponsePutOnOffLightStatus(HSZGatewayMac(),dev_mac,dev_endpoint,(data_status&0x0F));
			}
		}
		else
		{
			DEBUG_ZONE(LOG_ZONE_ERROR,"On Off Device Report Data Type Error.\n");
			HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"On Off Device Report Data Type Error.",NULL);
		}
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"On Off Device Report Data Length Error.\n");
		HSZDeviceError(HSZ_ERR_ZCL_DATA_LEN,"On Off Device Report Data Length Error.",NULL);
	}
}
/*****************************************************************************
 函 数 名  : HSZProcessOnOffDeviceReadData
 功能描述  : 分析On off device read attribute data数据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessOnOffDeviceReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*
	{"clusterId":"0x0006","commandId":"0x01","commandData":"0x0000001000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000D578BBB","endpoint":1}}

	*/
	
	char status_buf[8] = {0};
	int  data_status = 0;
	
	char attribute_buf[8] = {0};
	int  attribute_id = -1;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZProcessOnOffDeviceReadData.\n");
	
	if(strlen(zcl_data)>11)
	{
		strncpy(attribute_buf,zcl_data+2,4);
		attribute_id = strtol(attribute_buf,NULL,16);
		
		if(ZCL_ON_OFF_ATTRIBUTE_ID == attribute_id )
		{
			strncpy(status_buf,zcl_data+10,2);
			data_status = strtol(status_buf,NULL,16);			
			
			if((-1!= HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))&&((SCENE_LIGHT == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))||\
			(0xFA3F == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))||\		
			(0xFA4F == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))))
			{
				ResponseSceneSwitchStatus(HSZGatewayMac(),dev_mac,dev_endpoint,(data_status&0x0F));
			}
			else if((-1!= HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))&&((LEVEL_CONTROL_SWITCH == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))||\
			(0xFA5F == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))||\	
			(0xFA6F == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))))
			{
				ResponsePutLevelControlSwitchOnOff(HSZGatewayMac(),dev_mac,dev_endpoint,(data_status&0x0F));
			}
			else if((-1!= HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV))&&(SMART_PLUG_IN == HSZFindDeviceTypeFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV)))
			{
				ResponseSmartPlugInStatus(HSZGatewayMac(),dev_mac,dev_endpoint,(data_status&0x0F));
			}
			else
			{
				ResponsePutOnOffLightStatus(HSZGatewayMac(),dev_mac,dev_endpoint,(data_status&0x0F));
			}
		}
		else
		{
			DEBUG_ZONE(LOG_ZONE_ERROR,"On Off Device Read Data Type Error.\n");
			HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"On Off Device Readt Data Type Error.",NULL);
		}
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"On Off Device Read Data Length Error.\n");
		HSZDeviceError(HSZ_ERR_ZCL_DATA_LEN,"On Off Device Read Data Length Error.",NULL);
	}
}


/*****************************************************************************
 函 数 名  : HSZProcessLevelControlDeviceReportData
 功能描述  : 分析Levle control device report attribute data数据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessLevelControlDeviceReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*
	{"clusterId":"0x0008","commandId":"0x0A","commandData":"0x00002000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1}}.	{"clusterId":"0x0008","attributeId":"0x0000","attributeBuffer":"0x00","attributeDataType":"0x20","deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1}}.

	{"clusterId":"0x0008","commandId":"0x0A","commandData":"0x1000210000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1}}.	{"clusterId":"0x0008","attributeId":"0x0010","attributeBuffer":"0x0000","attributeDataType":"0x21","deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1}}.

	{"clusterId":"0x0008","commandId":"0x0A","commandData":"0x11002064","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1}}.	{"clusterId":"0x0008","attributeId":"0x0011","attributeBuffer":"0x64","attributeDataType":"0x20","deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1}}.

	*/
	
	if(strlen(zcl_data)>9)
	{
		char attri_id_buf[5] = {0};
		char attri_buf[5] = {0};
		char attri_data_type_buf[5] = {0};	

		int attri_id = -1;
		int attri_buf_data = -1;
		int attri_data_type = -1;

		int attribute_id = -1;
		int attribute_type = -1;
		
		int get_data = -1;

		/* Get the attribute id */
		strncpy(attri_id_buf,zcl_data + 2,4);
		attri_id = strtol(attri_id_buf,NULL,16);

		attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
					
		DEBUG_ZONE(LOG_ZONE_TEST,"Level Report Control Attribute Id:0x%04x.\n",attribute_id);							

		/* Get the attribute data type */
		strncpy(attri_data_type_buf,zcl_data+6,2);	

		attri_data_type = strtol(attri_data_type_buf,NULL,16);

		attribute_type = (attri_data_type&0xFF);

		DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Attribute Type:0x%02x.\n",attribute_type);


		switch(attribute_id)
		{				
			case ZCL_CURRENT_LEVEL_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Report Data ID:ZCL_CURRENT_LEVEL_ATTRIBUTE_ID.\n");
								
				if(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Report Data Type:ZCL_INT8U_ATTRIBUTE_TYPE.\n");

					/* Get the current level data */
					strncpy(attri_buf,zcl_data+8,2);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = attri_buf_data&0xFF;

					DEBUG_ZONE(LOG_ZONE_MY,"Level Control Device Report Current Levle Data:0x%02x.\n",get_data);
					
					//上报上层接口	
					ResponsePutLevelControlSwitch(HSZGatewayMac(),dev_mac,dev_endpoint,get_data);
				} 
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"Level Control Device Report Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"Level Control Device Report Data Type Error.",NULL);
				}				
			}
			break;
			
			case ZCL_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Report Data ID:ZCL_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ID.\n");
				
				if(ZCL_INT16U_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Report Data Type:ZCL_INT16U_ATTRIBUTE_TYPE.\n");

					/* Get the current level data */
					strncpy(attri_buf,zcl_data+8,4);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = ((attri_buf_data&0x00FF)<<8)|((attri_buf_data&0xFF00)>>8);

					DEBUG_ZONE(LOG_ZONE_MY,"Level Control Device Report On Off Transition Time Data:0x%04x.\n",get_data);
							
				    //上报上层接口	
				} 
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"Level Control Device Report Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"Level Control Device Report Data Type Error.",NULL);
				}		
			}
			break;
			
			case ZCL_ON_LEVEL_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Report Data ID:ZCL_ON_LEVEL_ATTRIBUTE_ID.\n");
				
				if(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Report Data Type:ZCL_INT8U_ATTRIBUTE_TYPE.\n");

					/* Get the current level data */
					strncpy(attri_buf,zcl_data+8,2);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = attri_buf_data&0xFF;;

					DEBUG_ZONE(LOG_ZONE_MY,"Level Control Device Report On Level Data:0x%02x.\n",get_data);
							
				    //上报上层接口	
				} 
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"Level Control Device Report Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"Level Control Device Report Data Type Error.",NULL);
				}		
			}
			break;
			
			
			default:
			break;
		}						
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"On Off Device Read Data Length Error.\n");
		HSZDeviceError(HSZ_ERR_ZCL_DATA_LEN,"On Off Device Read Data Length Error.",NULL);
	}	
	
}

/*****************************************************************************
 函 数 名  : HSZProcessLevelControlDeviceReadData
 功能描述  : 分析Levle control device read attribute data数据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessLevelControlDeviceReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*
	{"clusterId":"0x0008","commandId":"0x01","commandData":"0x0000002000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1}}.	{"clusterId":"0x0008","attributeId":"0x0000","attributeBuffer":"0x00","attributeDataType":"0x20","deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1},"status":"0x00"}.
	{"clusterId":"0x0008","commandId":"0x01","commandData":"0x010086","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1}}.	{"clusterId":"0x0008","attributeId":"0x0001","attributeBuffer":"0x","attributeDataType":"0x02","deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1},"status":"0x86"}.

	{"clusterId":"0x0008","commandId":"0x01","commandData":"0x100000210000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1}}.	{"clusterId":"0x0008","attributeId":"0x0010","attributeBuffer":"0x0000","attributeDataType":"0x21","deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1},"status":"0x00"}.

	{"clusterId":"0x0008","commandId":"0x01","commandData":"0x1100002064","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1}}.	{"clusterId":"0x0008","attributeId":"0x0011","attributeBuffer":"0x64","attributeDataType":"0x20","deviceEndpoint":{"eui64":"0x000D6F000CAAE12C","endpoint":1},"status":"0x00"}.

	*/
	
	if(strlen(zcl_data)>7)
	{
		char attri_id_buf[5] = {0};
		char attri_buf[5] = {0};
		char attri_data_type_buf[5] = {0};	

		int attri_id = -1;
		int attri_buf_data = -1;
		int attri_data_type = -1;

		int attribute_id = -1;
		int attribute_type = -1;
		
		int get_data = -1;

		/* Get the attribute id */
		strncpy(attri_id_buf,zcl_data + 2,4);
		attri_id = strtol(attri_id_buf,NULL,16);

		attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
					
		DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Read Data Attribute Id:0x%04x.\n",attribute_id);							
		/* Get the attribute data type */
		strncpy(attri_data_type_buf,zcl_data+8,2);	

		attri_data_type = strtol(attri_data_type_buf,NULL,16);

		attribute_type = (attri_data_type&0xFF);

		DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Read Data Attribute Type:0x%02x.\n",attribute_type);


		switch(attribute_id)
		{				
			case ZCL_CURRENT_LEVEL_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Read Data:ZCL_CURRENT_LEVEL_ATTRIBUTE_ID.\n");
								
				if(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Read Data Attribute Type:ZCL_INT8U_ATTRIBUTE_TYPE.\n");

					/* Get the current level data */
					strncpy(attri_buf,zcl_data+10,2);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = attri_buf_data&0xFF;

					DEBUG_ZONE(LOG_ZONE_MY,"Level Control Device Read current levle data:0x%02x.\n",get_data);
							
				    //上报上层接口	
				} 
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"Level Control Device Read Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"Level Control Device Read Data Type Error.",NULL);
				}	
			}
			break;
			
			case ZCL_LEVEL_CONTROL_REMAINING_TIME_ATTRIBUTE_ID:/*数据没处理*/
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Read Data Attribute Type:ZCL_LEVEL_CONTROL_REMAINING_TIME_ATTRIBUTE_ID.\n");
			}
			break;
			
			
			case ZCL_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Read Data Attribute Type:ZCL_ON_OFF_TRANSITION_TIME_ATTRIBUTE_ID.\n");
				
				if(ZCL_INT16U_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Read Data Attribute Type:ZCL_INT16U_ATTRIBUTE_TYPE.\n");

					/* Get the current level data */
					strncpy(attri_buf,zcl_data+10,4);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = ((attri_buf_data&0x00FF)<<8)|((attri_buf_data&0xFF00)>>8);

					DEBUG_ZONE(LOG_ZONE_MY,"Level Control Device Read on off transition time data:0x%04x.\n",get_data);
							
				    //上报上层接口	
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"Level Control Device Read Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"Level Control Device Read Data Type Error.",NULL);
				}	
			}
			break;
			
			case ZCL_ON_LEVEL_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Read Data Attribute Type:ZCL_ON_LEVEL_ATTRIBUTE_ID.\n");
				
				if(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"Level Control Device Read Data Attribute Type:ZCL_INT8U_ATTRIBUTE_TYPE.\n");

					/* Get the current level data */
					strncpy(attri_buf,zcl_data+10,2);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = attri_buf_data&0xFF;;

					DEBUG_ZONE(LOG_ZONE_MY,"Level Control Device Read OnLevel Data:0x%02x.\n",get_data);
							
				    //上报上层接口	
				} 
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"Level Control Device Read Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"Level Control Device Read Data Type Error.",NULL);
				}	
			}
			break;
			
			
			default:
			break;
		}						
	}				
	
}

/*****************************************************************************
 函 数 名  : HSZProcessColorControlDeviceReportData
 功能描述  : 分析Color control device read attribute data数据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessColorControlDeviceReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*
	{"clusterId":"0x0300","commandId":"0x0A","commandData":"0x00002000010020000300216B610400217D60","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1}}.
	{"clusterId":"0x0300","attributeId":"0x0000","attributeBuffer":"0x00","attributeDataType":"0x20","deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1}}.
	{"clusterId":"0x0300","attributeId":"0x0001","attributeBuffer":"0x00","attributeDataType":"0x20","deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1}}.
	{"clusterId":"0x0300","attributeId":"0x0003","attributeBuffer":"0x6B61","attributeDataType":"0x21","deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1}}.
	{"clusterId":"0x0300","attributeId":"0x0004","attributeBuffer":"0x7D60","attributeDataType":"0x21","deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1}}.
	{"clusterId":"0x0300","commandId":"0x0A","commandData":"0x0200210000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1}}.
	{"clusterId":"0x0300","attributeId":"0x0002","attributeBuffer":"0x0000","attributeDataType":"0x21","deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1}}.
	
	*/
	
	if(strlen(zcl_data)>10)
	{
		char attri_id_buf[5] = {0};
		char attri_buf[5] = {0};
		char attri_data_type_buf[5] = {0};	

		int attri_id = -1;
		int attri_buf_data = -1;
		int attri_data_type = -1;

		int attribute_id = -1;
		int attribute_type = -1;
		
		int get_data = -1;

		/* Get the attribute id */
		strncpy(attri_id_buf,zcl_data + 2,4);
		attri_id = strtol(attri_id_buf,NULL,16);

		attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
					
		DEBUG_ZONE(LOG_ZONE_TEST,"Color control :Attribute_id:0x%04x.\n",attribute_id);							

		/* Get the attribute data type */
		strncpy(attri_data_type_buf,zcl_data+6,2);	

		attri_data_type = strtol(attri_data_type_buf,NULL,16);

		attribute_type = (attri_data_type&0xFF);

		DEBUG_ZONE(LOG_ZONE_TEST,"Color control ::Attribute_type:0x%02x.\n",attribute_type);

		if((ZCL_COLOR_CONTROL_CURRENT_HUE_ATTRIBUTE_ID == attribute_id)&&(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)&&(strlen(zcl_data)>0x25))
		{
/****************************************current Hue data*****************************************************************/			
			/* Get the color control current Hue data */
			strncpy(attri_buf,zcl_data+8,2);	

			attri_buf_data = strtol(attri_buf,NULL,16);	

			get_data = attri_buf_data&0xFF;

			DEBUG_ZONE(LOG_ZONE_TEST,"Color control current Hue data:0x%02x.\n",get_data);
				
			//上报上层接口	
			
/****************************************Current Saturation*****************************************************************/			
			/* Get the attribute id */
			strncpy(attri_id_buf,zcl_data + 10,4);
			attri_id = strtol(attri_id_buf,NULL,16);

			attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);

			DEBUG_ZONE(LOG_ZONE_TEST,"Color control Current Saturation :Attribute_id:0x%04x.\n",attribute_id);							

			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+ 14,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_TEST,"Color control Current Saturation:Attribute_type:0x%02x.\n",attribute_type);

			if((ZCL_COLOR_CONTROL_CURRENT_SATURATION_ATTRIBUTE_ID == attribute_id)&&(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type))
			{
				/* Get the color control Current Saturation data */
				strncpy(attri_buf,zcl_data+16,2);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data&0xFF;

				DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Color control Current Saturation data:0x%02x.\n",get_data);

				//上报上层接口				
			}
/****************************************CurrentX data****************************************/			
			/* Get the attribute id */
			strncpy(attri_id_buf,zcl_data + 18,4);
			attri_id = strtol(attri_id_buf,NULL,16);

			attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);

			DEBUG_ZONE(LOG_ZONE_TEST,"Color control CurrentX :Attribute_id:0x%04x.\n",attribute_id);							

			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+ 22,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_TEST,"Color control CurrentX:Attribute_type:0x%02x.\n",attribute_type);
			
			if((ZCL_COLOR_CONTROL_CURRENT_X_ATTRIBUTE_ID == attribute_id)&&(ZCL_INT16U_ATTRIBUTE_TYPE == attribute_type))
			{
				/* Get the color control CurrentX data */
				strncpy(attri_buf,zcl_data+24,4);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;

				DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Color control CurrentX data:0x%04x.\n",get_data);

				//上报上层接口				
			}
/****************************************CurrentY data*********************************************/		
			/* Get the attribute id */
			strncpy(attri_id_buf,zcl_data + 28,4);
			attri_id = strtol(attri_id_buf,NULL,16);

			attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);

			DEBUG_ZONE(LOG_ZONE_TEST,"Color control CurrentY :Attribute_id:0x%04x.\n",attribute_id);							

			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+ 32,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_TEST,"Color control CurrentY:Attribute_type:0x%02x.\n",attribute_type);
			
			if((ZCL_COLOR_CONTROL_CURRENT_Y_ATTRIBUTE_ID == attribute_id)&&(ZCL_INT16U_ATTRIBUTE_TYPE == attribute_type))
			{
				/* Get the color control CurrentY data */
				strncpy(attri_buf,zcl_data+34,4);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;

				DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Color control CurrentY data:0x%04x.\n",get_data);

				//上报上层接口				
			}
		}
		else if ((ZCL_COLOR_CONTROL_REMAINING_TIME_ATTRIBUTE_ID == attribute_id)&&(ZCL_INT16U_ATTRIBUTE_TYPE == attribute_type))
		{
/****************************************Remaining Tim***********************************************/			
			/* Get the color control Remaining Time data */
			strncpy(attri_buf,zcl_data+8,4);	

			attri_buf_data = strtol(attri_buf,NULL,16);	

			get_data = ((attri_buf_data&0x00FF)<<8)|((attri_buf_data&0xFF00)>>8);

			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Color control Remaining Time data:0x%04x.\n",get_data);
				
			//上报上层接口	
		}
							
	}					
	
}

/*****************************************************************************
 函 数 名  : HSZProcessColorControlDeviceReadData
 功能描述  : 分析Color control device read attribute data数据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessColorControlDeviceReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*	{"clusterId":"0x0300","commandId":"0x01","commandData":"0x0000002000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1}}. 	{"clusterId":"0x0300","attributeId":"0x0000","attributeBuffer":"0x00","attributeDataType":"0x20","deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1},"status":"0x00"}.
	{"clusterId":"0x0300","commandId":"0x01","commandData":"0x0100002000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1}}.
	{"clusterId":"0x0300","attributeId":"0x0001","attributeBuffer":"0x00","attributeDataType":"0x20","deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1},"status":"0x00"}.
	{"clusterId":"0x0300","commandId":"0x01","commandData":"0x020000210000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1}}.
	{"clusterId":"0x0300","attributeId":"0x0002","attributeBuffer":"0x0000","attributeDataType":"0x21","deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1},"status":"0x00"}.
	{"clusterId":"0x0300","commandId":"0x01","commandData":"0x030000216B61","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1}}.
	{"clusterId":"0x0300","attributeId":"0x0003","attributeBuffer":"0x6B61","attributeDataType":"0x21","deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1},"status":"0x00"}.
	{"clusterId":"0x0300","commandId":"0x01","commandData":"0x040000217D60","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1}}. {"clusterId":"0x0300","attributeId":"0x0004","attributeBuffer":"0x7D60","attributeDataType":"0x21","deviceEndpoint":{"eui64":"0x000B57FFFE47464A","endpoint":1},"status":"0x00"}.
	*/
	
	if(strlen(zcl_data)>7)
	{
		char attri_id_buf[5] = {0};
		char attri_buf[5] = {0};
		char attri_data_type_buf[5] = {0};	

		int attri_id = -1;
		int attri_buf_data = -1;
		int attri_data_type = -1;

		int attribute_id = -1;
		int attribute_type = -1;
		
		int get_data = -1;

		/* Get the attribute id */
		strncpy(attri_id_buf,zcl_data + 2,4);
		attri_id = strtol(attri_id_buf,NULL,16);

		attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
					
		DEBUG_ZONE(LOG_ZONE_TEST,"Color Control:Attribute_id:0x%04x.\n",attribute_id);							

		/* Get the attribute data type */
		strncpy(attri_data_type_buf,zcl_data+8,2);	

		attri_data_type = strtol(attri_data_type_buf,NULL,16);

		attribute_type = (attri_data_type&0xFF);

		DEBUG_ZONE(LOG_ZONE_TEST,"Color Control:Attribute_type:0x%02x.\n",attribute_type);


		switch(attribute_id)
		{				
			case ZCL_COLOR_CONTROL_CURRENT_HUE_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"HSZProcessColorControlDeviceReadData:ZCL_COLOR_CONTROL_CURRENT_HUE_ATTRIBUTE_ID.\n");
								
				if(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_INT8U_ATTRIBUTE_TYPE.\n");

					/* Get the current level data */
					strncpy(attri_buf,zcl_data+10,2);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = attri_buf_data&0xFF;

					DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Read current Current Hue:0x%02x.\n",get_data);
							
				    //上报上层接口	
				} 
			}
			break;
			
			case ZCL_COLOR_CONTROL_CURRENT_SATURATION_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"HSZProcessColorControlDeviceReadData:ZCL_COLOR_CONTROL_CURRENT_SATURATION_ATTRIBUTE_ID.\n");
				
				if(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_INT8U_ATTRIBUTE_TYPE.\n");

					/* Get the current level data */
					strncpy(attri_buf,zcl_data+10,2);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = attri_buf_data&0xFF;

					DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Read Current Saturation:0x%02x.\n",get_data);
							
				    //上报上层接口	
				} 
			}
			break;
			
			
			case ZCL_COLOR_CONTROL_REMAINING_TIME_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"HSZProcessColorControlDeviceReadData:ZCL_COLOR_CONTROL_REMAINING_TIME_ATTRIBUTE_ID.\n");
				
				if(ZCL_INT16U_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_INT16U_ATTRIBUTE_TYPE.\n");

					/* Get the current level data */
					strncpy(attri_buf,zcl_data+10,4);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = ((attri_buf_data&0x00FF)<<8)|((attri_buf_data&0xFF00)>>8);

					DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Read Remaining Time data:0x%04x.\n",get_data);
							
				    //上报上层接口	
				} 
			}
			break;
			
			case ZCL_COLOR_CONTROL_CURRENT_X_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"HSZProcessColorControlDeviceReadData:ZCL_COLOR_CONTROL_CURRENT_X_ATTRIBUTE_ID.\n");
				
				if(ZCL_INT16U_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_INT16U_ATTRIBUTE_TYPE.\n");

					/* Get the current level data */
					strncpy(attri_buf,zcl_data+10,4);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = attri_buf_data;

					DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Read Current X data:0x%04x.\n",get_data);
							
				    //上报上层接口	
				} 
			}
			break;
			
			case ZCL_COLOR_CONTROL_CURRENT_Y_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"HSZProcessColorControlDeviceReadData:ZCL_COLOR_CONTROL_CURRENT_Y_ATTRIBUTE_ID.\n");
				
				if(ZCL_INT16U_ATTRIBUTE_TYPE == attribute_type)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_INT16U_ATTRIBUTE_TYPE.\n");

					/* Get the current level data */
					strncpy(attri_buf,zcl_data+10,4);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = attri_buf_data;

					DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Read Current Y data:0x%04x.\n",get_data);
							
				    //上报上层接口	
				} 
			}
			break;
			
			default:
			break;
		}						
	}				
	
}

/*****************************************************************************
 函 数 名  : HSZProcessOtaAndBootloadReadData
 功能描述  : 分析OTA 以及BootLoader据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessOtaAndBootloadReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	
}
/*****************************************************************************
 函 数 名  : HSZProcessOtaAndBootloadReportData
 功能描述  :  分析OTA 以及BootLoader据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessOtaAndBootloadReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	
}
/*****************************************************************************
 函 数 名  : HSZProcessPowerConfigReadData
 功能描述  : 分析读取的Power config数据 
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessPowerConfigReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	
}
/*****************************************************************************
 函 数 名  : HSZProcessPowerConfigReportData
 功能描述  : 分析Power config 上报据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessPowerConfigReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*
	{"clusterId":"0x0001","commandId":"0x0A","commandData":"0x210020C8","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B000FA4AC8F","endpoint":1}}.
	*/
	DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Power Config Report Data:%s.\n",zcl_data);
	
	int data_len = -1;

	char attri_id_buf[5] = {0};
	char attri_data_type_buf[5] = {0};	
	char attri_buf[5] = {0};

	int attri_id = -1;
	int attri_data_type = -1;
	int attri_buf_data = -1;
	
	int attribute_id = -1;
	int attribute_type = -1;
	int get_data = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Power Config Report Data Length:%d.\n",data_len);
	
	
	if(data_len>9)
	{
		/* Get the attribute id */
		strncpy(attri_id_buf,zcl_data + 2,4);
		attri_id = strtol(attri_id_buf,NULL,16);

		attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
					
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Power Config Report Attribute Id:0x%04x.\n",attribute_id);							

		switch(attribute_id)
		{
			case ZCL_BATTERY_VOLTAGE_ATTRIBUTE_ID:
			{
				/* Get the attribute data type */
				strncpy(attri_data_type_buf,zcl_data+6,2);	

				attri_data_type = strtol(attri_data_type_buf,NULL,16);

				attribute_type = (attri_data_type&0xFF);

				DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Power Config Report Attribute Type:0x%02x.\n",attribute_type);
				
				if(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)
				{
					/* Get the battery percent data */				
					strncpy(attri_buf,zcl_data+8,2);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = attri_buf_data;
					
					/* The BatteryVoltage attribute in units of 100mV */
					/* The value 0xff indicates an invalid or unknown reading. */
					DEBUG_ZONE(LOG_ZONE_MY,"Power Config Report Battery Voltage Data:0x%02x.\n",get_data);
				}
			}
			break;
			
			case ZCL_BATTERY_PERCENTAGE_REMAINING_ATTRIBUTE_ID:
			{
				// int find_device_index;
				/* Get the attribute data type */
				strncpy(attri_data_type_buf,zcl_data+6,2);	

				attri_data_type = strtol(attri_data_type_buf,NULL,16);

				attribute_type = (attri_data_type&0xFF);

				DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"ower Config Report Attribute Type:0x%02x.\n",attribute_type);

				if(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)
				{
					/* Get the battery percent data */				
					strncpy(attri_buf,zcl_data+8,2);	

					attri_buf_data = strtol(attri_buf,NULL,16);	

					get_data = attri_buf_data;
					
					/*With 0x00 = 0%, 0x64 = 50%, and 0xC8 = 100%, other with a range between zero and 100%*/
					get_data = get_data/2;/* The half of the value */
					DEBUG_ZONE(LOG_ZONE_MY,"Power Config battery percentage data:0x%02x.\n",get_data);

#if 0				
					find_device_index = HSZFindEuiFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV);
					
					if(-1!=find_device_index)
					{
						if(get_data < LOW_BATTERY_PERCENTAGE)/*低于限定电压值*/
						{
							DEBUG_ZONE(LOG_ZONE_MY,"Device:%s,Power Low.\n",dev_mac);
							ResponseUpdateBatteryLevleInfo(HSZGatewayMac(),dev_mac,DEVICE_LOW_VOLTAGE);
							DEV_INFO[find_device_index].low_voltage_statue = DEVICE_LOW_VOLTAGE;
						}
						
						if((DEVICE_LOW_VOLTAGE == DEV_INFO[find_device_index].low_voltage_statue)||\
							(DEVICE_INIT_VOLTAGE == DEV_INFO[find_device_index].low_voltage_statue))
						{
							if(get_data > LOW_BATTERY_PERCENTAGE)/*大于限定的电压值*/
							{
								DEBUG_ZONE(LOG_ZONE_MY,"Device:%s,Power high.\n",dev_mac);
								ResponseUpdateBatteryLevleInfo(HSZGatewayMac(),dev_mac,DEVICE_NORMAL_VOLTAGE);
								DEV_INFO[find_device_index].low_voltage_statue = DEVICE_NORMAL_VOLTAGE;
							}
						}
					}
					
#endif

				}
			}
			break;
			
			default:
			break;
		}
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"Power Config Report Data length Error.\n");
		HSZDeviceError(HSZ_ERR_ZCL_DATA_LEN,"Power Config Report Data length Error.",NULL);
	}
}

/*****************************************************************************
 函 数 名  : HSZProcessAlarmReadData
 功能描述  : 分析读取的Alarm据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessAlarmReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	
}

/*****************************************************************************
 函 数 名  : HSZProcessAlarmReportData
 功能描述  : 分析上报的Alarm据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessAlarmReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	
}


/*****************************************************************************
 函 数 名  : HSZProcessSimpleMeterReadData
 功能描述  : 分析读取的Simple Metering据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessSimpleMeterReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*
	
	{"clusterId":"0x0702","commandId":"0x01","commandData":"0x000086","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0702","commandId":"0x01","commandData":"0x010386","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0702","commandId":"0x01","commandData":"0x000386","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0702","commandId":"0x01","commandData":"0x03030018B2","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0702","commandId":"0x01","commandData":"0x020386","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0702","commandId":"0x01","commandData":"0x03030018B2","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0702","commandId":"0x01","commandData":"0x03030018B2","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	
	*/
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessSimpleMeterReportData,zcl_data:%s.\n",zcl_data);
	
	int data_len = -1;

	char attri_id_buf[5] = {0};
	char attri_data_type_buf[5] = {0};	
	char attri_buf[5] = {0};

	int attri_id = -1;
	int attri_data_type = -1;
	int attri_buf_data = -1;
	
	int attribute_id = -1;
	int attribute_type = -1;
	int get_data = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessSimpleMeterReportData,zcl_data len:%d.\n",data_len);
	
	/* Get the attribute id */
	strncpy(attri_id_buf,zcl_data + 2,4);
	attri_id = strtol(attri_id_buf,NULL,16);

	attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
				
	DEBUG_ZONE(LOG_ZONE_MY,"Simple Meter:Attribute_id:0x%02x.\n",attribute_id);	

	switch(attribute_id)
	{
		case ZCL_SUMMATION_FORMATTING_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+8,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Simple Meter:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_BITMAP8_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+10,2);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"Summation Formatting Attribute data:0x%02x.\n",get_data);
			}
		}
		break;
			
		default:
		break;
	}
	
}
/*****************************************************************************
 函 数 名  : HSZProcessSimpleMeterReportData
 功能描述  : 分析上报的Simple Metering据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessSimpleMeterReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	
}


/*****************************************************************************
 函 数 名  : HSZProcessElectricalMeasurementReadData
 功能描述  : 分析读取的Electrical Measurement据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessElectricalMeasurementReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*
	{"clusterId":"0x0B04","commandId":"0x01","commandData":"0x080500210000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0B04","commandId":"0x01","commandData":"0x0B0500290000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0B04","commandId":"0x01","commandData":"0x000886","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0B04","commandId":"0x01","commandData":"0x010886","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0B04","commandId":"0x01","commandData":"0x050500210000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0B04","commandId":"0x01","commandData":"0x020886","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0B04","commandId":"0x01","commandData":"0x030886","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	{"clusterId":"0x0B04","commandId":"0x01","commandData":"0x030886","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B0016BBB5B9","endpoint":1}}.
	*/
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessElectricalMeasurementReadData,zcl_data:%s.\n",zcl_data);
	
	int data_len = -1;

	char attri_id_buf[5] = {0};
	char attri_data_type_buf[5] = {0};	
	char attri_buf[5] = {0};

	int attri_id = -1;
	int attri_data_type = -1;
	int attri_buf_data = -1;
	
	int attribute_id = -1;
	int attribute_type = -1;
	int get_data = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessSimpleMeterReportData,zcl_data len:%d.\n",data_len);
	
	/* Get the attribute id */
	strncpy(attri_id_buf,zcl_data + 2,4);
	attri_id = strtol(attri_id_buf,NULL,16);

	attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
				
	DEBUG_ZONE(LOG_ZONE_MY,"Electrical Measurement:Attribute_id:0x%04x.\n",attribute_id);	
	
	switch(attribute_id)
	{
		case ZCL_RMS_CURRENT_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+8,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Electrical Measurement:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_INT16U_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+10,4);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"RMS Current Attribute data:0x%04x.\n",get_data);
			}
		}
		break;
		
		case ZCL_ACTIVE_POWER_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+8,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Electrical Measurement:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_INT16S_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+10,4);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = ((attri_buf_data&0x00FF)<<8)|((attri_buf_data&0xFF00)>>8);
				
				DEBUG_ZONE(LOG_ZONE_MY,"Active power data:0x%04x.\n",get_data);
			}
		}
		break;
		
		default:
		break;
	}
}
/*****************************************************************************
 函 数 名  : HSZProcessElectricalMeasurementReportData
 功能描述  : 分析上报的Electrical Measurement据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessElectricalMeasurementReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*
	{"clusterId":"0x0B04","commandId":"0x0A","commandData":"0x050521D556","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B000FE2F9BD","endpoint":1}}.
	
	*/
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessElectricalMeasurementReportData,zcl_data:%s.\n",zcl_data);
	
	int data_len = -1;

	char attri_id_buf[5] = {0};
	char attri_data_type_buf[5] = {0};	
	char attri_buf[5] = {0};

	int attri_id = -1;
	int attri_data_type = -1;
	int attri_buf_data = -1;
	
	int attribute_id = -1;
	int attribute_type = -1;
	int get_data = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessElectricalMeasurementReportData,zcl_data len:%d.\n",data_len);
	
	/* Get the attribute id */
	strncpy(attri_id_buf,zcl_data + 2,4);
	attri_id = strtol(attri_id_buf,NULL,16);

	attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
				
	DEBUG_ZONE(LOG_ZONE_MY,"Electrical Measurement:Attribute_id:0x%04x.\n",attribute_id);	
	
	switch(attribute_id)
	{
		case ZCL_RMS_CURRENT_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+6,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Electrical Measurement:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_INT16U_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+8,4);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"RMS Current Attribute data:0x%04x.\n",get_data);
			}
		}
		break;
		
		case ZCL_ACTIVE_POWER_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+8,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Electrical Measurement:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_INT16S_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+8,4);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = ((attri_buf_data&0x00FF)<<8)|((attri_buf_data&0xFF00)>>8);
				
				DEBUG_ZONE(LOG_ZONE_MY,"Active power data:0x%04x.\n",get_data);
			}
		}
		break;
		
		default:
		break;
	}
}
/*****************************************************************************
 函 数 名  : HSZProcessIlluminanceMeasurementReportData
 功能描述  : 分析上报的Illuminance Measurement据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessIlluminanceMeasurementReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	
}
/*****************************************************************************
 函 数 名  : HSZProcessIlluminanceMeasurementReportData
 功能描述  : 分析上报的Illuminance Measurement据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessIlluminanceMeasurementReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*
	{"clusterId":"0x0400","commandId":"0x0A","commandData":"0x00002003","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B000FA4AB63","endpoint":1}}.
	{"clusterId":"0x0400","commandId":"0x0A","commandData":"0x00002001","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B000FA4AB63","endpoint":1}}.
	{"clusterId":"0x0400","commandId":"0x0A","commandData":"0x00002004","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B000FA4AB63","endpoint":1}}.

	*/
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessIlluminanceMeasurementReportData,zcl_data:%s.\n",zcl_data);
	
	int data_len = -1;

	char attri_id_buf[5] = {0};
	char attri_data_type_buf[5] = {0};	
	char attri_buf[5] = {0};

	int attri_id = -1;
	int attri_data_type = -1;
	int attri_buf_data = -1;
	
	int attribute_id = -1;
	int attribute_type = -1;
	int get_data = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessIlluminanceMeasurementReportData,zcl_data len:%d.\n",data_len);
	
	/* Get the attribute id */
	strncpy(attri_id_buf,zcl_data + 2,4);
	attri_id = strtol(attri_id_buf,NULL,16);

	attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
				
	DEBUG_ZONE(LOG_ZONE_MY,"Illuminance Measurement:Attribute_id:0x%04x.\n",attribute_id);	
	
	switch(attribute_id)
	{
		case ZCL_ILLUM_MEASURED_VALUE_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+6,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Illuminance Measurement:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_INT16U_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+8,2);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"Measured Value data:0x%02x.\n",get_data);
			}
		}
		break;
		
		case ZCL_MEASUREMENT_LIGHT_SENSOR_TYPE_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+6,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Illuminance Measurement:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_ENUM8_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+8,2);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"Light Sensor Type data:0x%02x.\n",get_data);
				
				if(0x00 == get_data)
				{
					DEBUG_ZONE(LOG_ZONE_MY,"Light Sensor Type:Photodiode.\n");
				}
				else if(0x01 == get_data)
				{
					DEBUG_ZONE(LOG_ZONE_MY,"Light Sensor Type:CMOS.\n");
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_MY,"Light Sensor Type: Unknown\n");
				}
			}
		}
		break;
		
		default:
		break;
	}
}

/*****************************************************************************
 函 数 名  : HSZWindowCoveringType
 功能描述  : 分析Window Covering设备类型
 输入参数  : int dev_type ：入网设备Window Covering类型
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZWindowCoveringType(int dev_type)
{
	switch(dev_type)
	{
		case 0x00:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering Type is Rollershade.\n");	
		}
		break;
		
		case 0x01:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering Type is Rollershade -2 Motor.\n");
		}
		break;
		
		case 0x02:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering Type is Rollershade -Exterior.\n");
		}
		break;
		
		case 0x03:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering Type is Rollershade -Exterior -2 Motor.\n");
		}
		break;
		
		case 0x04:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering Type is Drapcry .\n");
		}
		break;
		
		case 0x05:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering Type is Awning.\n");
		}
		break;
		
		case 0x06:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering Type is Shutter.\n");
		}
		break;
		
		case 0x07:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering Type is Tilt Blind - Tilt Only.\n");
		}
		break;
		
		case 0x08:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering Type is Tilt Blind - Lift Tilt.\n");
		}
		break;
		
		case 0x09:
		{
			DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering Type is Projector Screen.\n");
		}
		break;
		
		default:
		break;
		
	}
}
/*****************************************************************************
 函 数 名  : HSZWindowCoveringStatusBitMeanings
 功能描述  : 分析Window Covering Status Bit Meanings类型
 输入参数  : int Status_Bit ：入网设备Status Bit Meanings类型
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZWindowCoveringStatusBitMeanings(int Status_Bit)
{
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZWindowCoveringStatusBitMeanings:0x%02x.\n",Status_Bit);
	
	/*bit0 = 1: Operational
	       = 0: Not Operational*/	   
	if(BITMAP_16BIT_BIT0&Status_Bit)
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering is operational.\n");	
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering is Not operational.\n");
	}
	
	/*bit1 = 1: Online
	       = 0: Not Online*/		
    if(BITMAP_16BIT_BIT1&Status_Bit)
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering is Online.\n");
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Window Covering is Not Online.\n");
	}

	/*bit2 = 1: Open/Up Commands have been reversed
	       = 0: Commands are normal*/	
	if(BITMAP_16BIT_BIT2&Status_Bit)
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Open/Up Commands have been reversed.\n");
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Commands are normal.\n");
	}
	
	/*bit3 = 1: Lift control is Closed Loop
	       = 0: Lift control is Open Loop*/	
	if(BITMAP_16BIT_BIT3&Status_Bit)
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Lift control is Closed Loop.\n");
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Lift control is Open Loop.\n");
	}
	
	/*bit4 = 1:Tilt control is Closed Loop
	       = 0:Tilt control is Open Loop*/		
	if(BITMAP_16BIT_BIT4&Status_Bit)
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Tilt control is Closed Loop.\n");
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Tilt control is Open Loop.\n");
	}
  
	/*bit5 = 1:Encoder Controlled
	       = 0:Timer Controlled*/	
	if(BITMAP_16BIT_BIT5&Status_Bit)
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Encoder Controlled.\n");
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Timer Controlled.\n");
	}

	/*bit6 = 1:Encoder Controlled
	       = 0:Timer Controlled*/	
	if(BITMAP_16BIT_BIT6&Status_Bit)
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Encoder Controlled.\n");
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ZCL_DATA,"Timer Controlled.\n");
	}

	/*bit7 Reserved*/	
}
/*****************************************************************************
 函 数 名  : HSZProcessWindowCoveringReadData
 功能描述  : 分析上报的Window Covering据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessWindowCoveringReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*	{"clusterId":"0x0102","commandId":"0x01","commandData":"0x0000003004","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B000FE2F8AB","endpoint":2}}.
	*/
		DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessWindowCoveringReadData,zcl_data:%s.\n",zcl_data);
	
	int data_len = -1;

	char attri_id_buf[5] = {0};
	char attri_data_type_buf[5] = {0};	
	char attri_buf[5] = {0};

	int attri_id = -1;
	int attri_data_type = -1;
	int attri_buf_data = -1;
	
	int attribute_id = -1;
	int attribute_type = -1;
	int get_data = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessWindowCoveringReadData,zcl_data len:%d.\n",data_len);
	
	/* Get the attribute id */
	strncpy(attri_id_buf,zcl_data + 2,4);
	attri_id = strtol(attri_id_buf,NULL,16);

	attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
				
	DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:Attribute_id:0x%04x.\n",attribute_id);	
	
	switch(attribute_id)
	{
		case ZCL_COVERING_TYPE_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+8,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_ENUM8_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+10,2);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"Window Covering Type data:0x%02x.\n",get_data);
				
				HSZWindowCoveringType(get_data);
			}
		}
		break;
		
		case ZCL_CONFIG_STATUS_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+8,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_BITMAP8_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+10,2);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"Window Covering Bit Meanings for the Config/Status Attribute data:0x%02x.\n",get_data);
				
				HSZWindowCoveringStatusBitMeanings(get_data);
			}
		}
		break;
		
		case ZCL_CURRENT_LIFT_PERCENTAGE_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+8,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+10,2);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"Window Covering Current Position Lift Percentage:%02x%.\n",get_data);
				
				HSZWindowCoveringStatusBitMeanings(get_data);
			}
		}
		break;
		
		case ZCL_CURRENT_TILT_PERCENTAGE_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+8,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+10,2);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"Window Covering Current Position Tilt Percentage:%02x%.\n",get_data);
				
				HSZWindowCoveringStatusBitMeanings(get_data);
			}
		}
		break;
		
		default:
		break;
	}
}

/*****************************************************************************
 函 数 名  : HSZProcessWindowCoveringReportData
 功能描述  : 分析上报的Window Covering据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessWindowCoveringReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*
	{"clusterId":"0x0102","commandId":"0x0A","commandData":"0x08002065","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B00171113AB","endpoint":1}}.-----up
	
	{"clusterId":"0x0102","commandId":"0x0A","commandData":"0x08002067","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B00171113AB","endpoint":1}}.-----stop
	
	
	{"clusterId":"0x0102","commandId":"0x0A","commandData":"0x08002066","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B00171113AB","endpoint":1}}.------down
	*/
	
	int data_len = -1;
	char attri_id_buf[5] = {0};
	char attri_data_type_buf[5] = {0};	
	char attri_buf[5] = {0};

	int attri_id = -1;
	int attri_data_type = -1;
	int attri_buf_data = -1;
	
	int attribute_id = -1;
	int attribute_type = -1;
	int get_data = -1;
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessWindowCoveringReportData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
	/* Get the attribute id */
	strncpy(attri_id_buf,zcl_data + 2,4);
	attri_id = strtol(attri_id_buf,NULL,16);

	attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
				
	DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:Attribute_id:0x%04x.\n",attribute_id);	
	
	switch(attribute_id)
	{
		case ZCL_COVERING_TYPE_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+6,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_ENUM8_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+8,2);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"Window Covering Type data:0x%02x.\n",get_data);
				
				HSZWindowCoveringType(get_data);
			}
		}
		break;
		
		case ZCL_CONFIG_STATUS_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+6,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_BITMAP8_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data + 8,2);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"Window Covering Bit Meanings for the Config/Status Attribute data:0x%02x.\n",get_data);
				
				HSZWindowCoveringStatusBitMeanings(get_data);
			}
		}
		break;
		
		case ZCL_CURRENT_LIFT_PERCENTAGE_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+6,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+8,2);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"Window Covering Current Position Lift Percentage:%02x%.\n",get_data);
				
				// HSZWindowCoveringStatusBitMeanings(get_data);
				
				if(WINDOWS_COVER_UP == get_data)
				{
					DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:WINDOWS_COVER_UP.\n");
					
					ResponsePutWindowCoveringControllerStatus(HSZGatewayMac(),dev_mac,dev_endpoint,WINDOW_COVER_CONTROL_UP);
				}
				else if(WINDOWS_COVER_DOWN == get_data)
				{
					DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:WINDOWS_COVER_DOWN.\n");
					
					ResponsePutWindowCoveringControllerStatus(HSZGatewayMac(),dev_mac,dev_endpoint,WINDOW_COVER_CONTROL_DOWN);
				}
				else if(WINDOWS_COVER_STOP == get_data)
				{
					DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:WINDOWS_COVER_STOP.\n");
					
					ResponsePutWindowCoveringControllerStatus(HSZGatewayMac(),dev_mac,dev_endpoint,WINDOW_COVER_CONTROL_STOP);
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"Window Covering Position Data error.\n");
				}
			}
		}
		break;
		
		case ZCL_CURRENT_TILT_PERCENTAGE_ATTRIBUTE_ID:
		{
			/* Get the attribute data type */
			strncpy(attri_data_type_buf,zcl_data+6,2);	

			attri_data_type = strtol(attri_data_type_buf,NULL,16);

			attribute_type = (attri_data_type&0xFF);

			DEBUG_ZONE(LOG_ZONE_MY,"Window Covering:Attribute_type:0x%02x.\n",attribute_type);
			
			if(ZCL_INT8U_ATTRIBUTE_TYPE == attribute_type)
			{
				strncpy(attri_buf,zcl_data+8,2);	

				attri_buf_data = strtol(attri_buf,NULL,16);	

				get_data = attri_buf_data;
				
				DEBUG_ZONE(LOG_ZONE_MY,"Window Covering Current Position Tilt Percentage:%02x%.\n",get_data);
				
				HSZWindowCoveringStatusBitMeanings(get_data);
			}
		}
		break;
		
		default:
		break;
	}
}

/*****************************************************************************
 函 数 名  : HSZProcessOwnshipOneReadData
 功能描述  : 分析上报的私有指令一据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessOwnshipOneReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	int data_len = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessOwnshipOneReadData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
}

/*****************************************************************************
 函 数 名  : HSZProcessOwnshipOneReportData
 功能描述  : 分析上报的私有指令一据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessOwnshipOneReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	int data_len = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessOwnshipOneReportData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
}

/*****************************************************************************
 函 数 名  : HSZProcessOwnshipTwoReadData
 功能描述  : 分析上报的私有指令二据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessOwnshipTwoReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	int data_len = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessOwnshipTwoReadData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
}
/*****************************************************************************
 函 数 名  : HSZProcessOwnshipTwoReportData
 功能描述  : 分析上报的私有指令二据值
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessOwnshipTwoReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	int data_len = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessOwnshipTwoReportData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
}

/*****************************************************************************
 函 数 名  : HSZProcessIASWDReadData
 功能描述  : 分析主动读的声光报警器数据
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年01月08日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessIASWDReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	int data_len = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessIASWDReadData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
	
}
/*****************************************************************************
 函 数 名  : HSZProcessIASWDReportData
 功能描述  : 分析主动上报的声光报警器数据
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年01月08日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessIASWDReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	int data_len = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessIASWDReportData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
}

/*****************************************************************************
 函 数 名  : HSZProcessDiagnosticsReadData
 功能描述  : 分许主动读的信号强度数据
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年01月08日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessDiagnosticsReadData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	int data_len = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessDiagnosticsReadData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
	
}

/*****************************************************************************
 函 数 名  : HSZProcessDiagnosticsReportData
 功能描述  : 分析上报的信号强度数据
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年01月08日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessDiagnosticsReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*	{"clusterId":"0x0B05","commandId":"0x0A","commandData":"0x1D0128D3","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x3CBD4A00F8C24A01","endpoint":1}}.
	*/
	int data_len = -1;
	
	data_len = strlen(zcl_data);
	
	// DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessDiagnosticsReportData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);	
	
	if(data_len>9)
	{
		char rssi_attri_id_buf[5] = {0};
		char rssi_attri_buf[5] = {0};
		char rssi_attri_data_type_buf[5] = {0};

		int attri_id = -1;
		int attri_buf_data = -1;
		int attri_data_type = -1;

		int rssi_attribute_id = -1;
		int rssi_attribute_type = -1;
		int rssi_attribute_date = -1;
		
		/* Get the Diagnostics attribute id */
		strncpy(rssi_attri_id_buf,zcl_data + 2,4);
		attri_id = strtol(rssi_attri_id_buf,NULL,16);

		rssi_attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);

		DEBUG_ZONE(LOG_ZONE_TEST,"Diagnostics Report Data Attribute Id:0x%04x.\n",rssi_attribute_id);							

		/* Get the Diagnostics attribute data type */
		strncpy(rssi_attri_data_type_buf,zcl_data+6,2);	

		attri_data_type = strtol(rssi_attri_data_type_buf,NULL,16);

		rssi_attribute_type = (attri_data_type&0xFF);

		DEBUG_ZONE(LOG_ZONE_TEST,"Diagnostics Report Data Attribute Type:0x%02x.\n",rssi_attribute_type);
		
		switch(rssi_attribute_id)
		{
			case ZCL_LAST_MESSAGE_RSSI_ATTRIBUTE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"Diagnostics Report Data Attribute:ZCL_LAST_MESSAGE_RSSI_ATTRIBUTE_ID.\n");
				
				if(ZCL_INT8S_ATTRIBUTE_TYPE == rssi_attribute_type)
				{
					/* Get the Last ISSI attribute data*/
					strncpy(rssi_attri_buf,zcl_data+8,2);	

					attri_buf_data = strtol(rssi_attri_buf,NULL,16);

					rssi_attribute_date = (attri_buf_data&0xFF);
					
					if (hsbk_devsignalvalue!=NULL)
					{
						hsbk_devsignalvalue(HSZGatewayMac(),dev_mac,rssi_attribute_date);
					}
										
					DEBUG_ZONE(LOG_ZONE_MY,"Diagnostics Report Last RSSI Date:%02x.\n",rssi_attribute_date);
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_ERROR,"ZCL_LAST_MESSAGE_RSSI_ATTRIBUTE_ID Report Attribute Data Type Error.\n");
					HSZDeviceError(HSZ_ERR_ZCL_ATTR_DATA_TYPE,"ZCL_LAST_MESSAGE_RSSI_ATTRIBUTE_ID Report Attribute Data Type Error.",NULL);
				}
			}
			break;
			
			default:
			break;
			
		}
		
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"Diagnostics Report RSSI Data Length Error.\n");
		HSZDeviceError(HSZ_ERR_ZCL_DATA_LEN,"Diagnostics Report RSSI Data Length Error.",NULL);
	}
}



int changestrtochar(char s[],char bits[])
{
	int i,n = 0;
	
	for(i = 0; s[i]; i += 2) 
	{
		if(s[i] >= 'A' && s[i] <= 'F')
		{
			bits[n] = s[i] - 'A' + 10;
		}
		else
		{			
			bits[n] = s[i] - '0';
		}
		
		if(s[i + 1] >= 'A' && s[i + 1] <= 'F')
		{
			bits[n] = (bits[n] << 4) | (s[i + 1] - 'A' + 10);
		}
		else
		{			
			bits[n] = (bits[n] << 4) | (s[i + 1] - '0');
		}
		
		++n;
	}
	
	return n;
}

/*****************************************************************************
 函 数 名  : HSZProcessSmartLockReportData
 功能描述  : 分析上报的SmartLock数据
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年04月23日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessSmartLockReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*
	{"clusterId":"0xFD00","commandId":"0x0A","commandData":"0x00004115FA121432FFFFFFFF20020A0102040500000001038A","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B001477BA09","endpoint":1}}.
    {"clusterId":"0xFD00","commandId":"0x0A","commandData":"0x0000410EFA0B1451FFFFFFFF20FFFFFFFF82","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B001477BA09","endpoint":1}}.
	 
	*/
	
	/* 
	  数据为：4位Attribute ID，一个字节数据类型（0x41），一个字节数据长度（0x15），剩余的为具体数据0xFA... ...
	*/
	
	int data_len = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessSmartLockReportData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
	
	if(data_len>9)
	{
		char lock_attri_id_buf[8] = {0};
		char lock_attri_data_type_buf[8] = {0};
		char lock_attri_len_buf[8] = {0};

		int attri_id = -1;
		int attri_buf_len = -1;
		int attri_data_type = -1;

		int lock_attribute_id = -1;
		int lock_attribute_type = -1;
		int lock_attribute_len = -1;
		
		
		/* Get the lock attribute id */
		strncpy(lock_attri_id_buf,zcl_data + 2,4);
		
		attri_id = strtol(lock_attri_id_buf,NULL,16);
		
		lock_attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
		
		DEBUG_ZONE(LOG_ZONE_TEST,"Lock Report Data Attribute Id:0x%04x.\n",lock_attribute_id);							

		/* Get the lock attribute data type */
		strncpy(lock_attri_data_type_buf,zcl_data + 6,2);	

		attri_data_type = strtol(lock_attri_data_type_buf,NULL,16);

		lock_attribute_type = (attri_data_type&0xFF);

		DEBUG_ZONE(LOG_ZONE_TEST,"lock Report Data Attribute Type:0x%02x.\n",lock_attribute_type);
		
		/* Get the lock attribute data len */
		strncpy(lock_attri_len_buf,zcl_data + 8,2);	

		attri_buf_len = strtol(lock_attri_len_buf,NULL,16);

		lock_attribute_len = (attri_buf_len&0xFF);

		DEBUG_ZONE(LOG_ZONE_TEST,"lock Report Data Attribute len:0x%02x.\n",lock_attribute_len);
		
		if((LOCK_ATTRIBUTE_ID == lock_attribute_id)&&(LOCK_DATA_TYPE == lock_attribute_type))
		{
			unsigned char data_buf[512] = {0};
			char data_temp[4] = {0};
			int temp = 0x00;
			
			/* Get the lock data */		
			DEBUG_ZONE(LOG_ZONE_MY,"lock Report Data:%s.\n",zcl_data + 10);
		
			for(temp = 0x00;temp <lock_attribute_len;temp++)
			{
				strncpy(data_temp,zcl_data + 10 + 2*temp,2);	

				data_buf[temp] = strtoul(data_temp,NULL,16); 
				
			}
#if 0	
			for(temp = 0x00;temp <lock_attribute_len;temp++)
			{
				DEBUG_ZONE(LOG_ZONE_MY,"0x%02x \n",data_buf[temp]);
			}
#endif 			
            HSZAnalysisSmartLockData(data_buf,HSZGatewayMac(),dev_mac);
			
        }
		else if((LOCK_ATTRIBUTE_SL920_ID == lock_attribute_id)&&(LOCK_DATA_TYPE == lock_attribute_type))
		{
			unsigned char data_buf[512] = {0};
			
			char data_temp[4] = {0};
			
			int temp = 0x00;
			
			DEBUG_ZONE(LOG_ZONE_MY,"SL920 lock Report Data:%s.\n",zcl_data + 10);
			
			for(temp = 0x00;temp <lock_attribute_len;temp++)
			{
				strncpy(data_temp,zcl_data + 10 + 2*temp,2);	
				data_buf[temp] = strtoul(data_temp,NULL,16); 
			}
			
#if 0	
			for(temp = 0x00;temp <lock_attribute_len;temp++)
			{
			 DEBUG_ZONE(LOG_ZONE_MY,"0x%02x \n",data_buf[temp]);
			}
#endif 	
			
			/* 数据处理 */
			DEBUG_ZONE(LOG_ZONE_MY,"LIZHOUYE.\n");
			
			HSZAnalysisSelfProtocolLockData(data_buf,HSZGatewayMac(),dev_mac);
			
		}
		else if ((LOCK_ATTRIBUTE_SL868_ID == lock_attribute_id)&&(LOCK_DATA_TYPE == lock_attribute_type))
		{
			unsigned char data_buf[512] = {0};
			
			char data_temp[4] = {0};
			
			int temp = 0x00;
			
			DEBUG_ZONE(LOG_ZONE_MY,"SL868 lock Report Data:%s.\n",zcl_data + 10);
			
			for(temp = 0x00;temp <lock_attribute_len;temp++)
			{
				strncpy(data_temp,zcl_data + 10 + 2*temp,2);	
				data_buf[temp] = strtoul(data_temp,NULL,16); 
			}
			
#if 1	
			for(temp = 0x00;temp <lock_attribute_len;temp++)
			{
			 DEBUG_ZONE(LOG_ZONE_MY,"0x%02x \n",data_buf[temp]);
			}
#endif 	
			HSZAnalysisSelfProtocolLockData(data_buf,HSZGatewayMac(),dev_mac);
		}
		else if((LOCK_ATTRIBUTE_SL500_ID == lock_attribute_id)&&(LOCK_DATA_TYPE == lock_attribute_type))
		{
			unsigned char data_buf_sl500[512] = {0};
			
			char data_temp_sl500[4] = {0};
			
			int temp_sl500 = 0x00;
			
			DEBUG_ZONE(LOG_ZONE_MY,"SL500 lock Report Data:%s.\n",zcl_data + 10);
			
			for(temp_sl500 = 0x00;temp_sl500 <lock_attribute_len;temp_sl500++)
			{
				strncpy(data_temp_sl500,zcl_data + 10 + 2*temp_sl500,2);	
				data_buf_sl500[temp_sl500] = strtoul(data_temp_sl500,NULL,16); 
			}
			
#if 1	
			for(temp_sl500 = 0x00;temp_sl500 <lock_attribute_len;temp_sl500++)
			{
			 DEBUG_ZONE(LOG_ZONE_MY,"0x%02x \n",data_buf_sl500[temp_sl500]);
			}
#endif 	
			HSZAnalysisSelfProtocolSL500LockData(data_buf_sl500,HSZGatewayMac(),dev_mac);
		}
		else if((LOCK_ATTRIBUTE_374_ID == lock_attribute_id)&&(LOCK_DATA_TYPE == lock_attribute_type))
		{
			unsigned char data_buf_374[512] = {0};
			
			char data_temp_374[4] = {0};
			
			int temp_374 = 0x00;
			
			DEBUG_ZONE(LOG_ZONE_MY,"374 lock Report Data:%s.\n",zcl_data + 10);
			
			for(temp_374 = 0x00;temp_374 <lock_attribute_len;temp_374++)
			{
				strncpy(data_temp_374,zcl_data + 10 + 2*temp_374,2);	
				data_buf_374[temp_374] = strtoul(data_temp_374,NULL,16); 
			}
			
#if 1	
			for(temp_374 = 0x00;temp_374 <lock_attribute_len;temp_374++)
			{
			 DEBUG_ZONE(LOG_ZONE_MY,"0x%02x \n",data_buf_374[temp_374]);
			}
#endif 	
			HSZAnalysisSelfProtocol374LockData(data_buf_374,HSZGatewayMac(),dev_mac);
		}
		else
		{
			DEBUG_ZONE(LOG_ZONE_ERROR,"Report Smart Lock Data Error.\n");
			HSZDeviceError(HSZ_ERR_SMARTLOCK_MSG_ERR,"Report Smart Lock Data Error.",NULL);
		}
		
	}	
}


/*****************************************************************************
 函 数 名  : HSZProcessSmartLockEventData
 功能描述  : 分析上报的SmartLock Event 数据
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年04月23日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessSmartLockEventData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	/*	
	{"clusterId":"0x0101","commandId":"0x20","commandData":"0x0002000002000000000103","clusterSpecific":true,"deviceEndpoint":{"eui64":"0x00124B001477BA19","endpoint":1}}
	
	*/
	
	int data_len = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessSmartLockEventData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
	
}
/*****************************************************************************
 函 数 名  : HSZProcessThreeInOneDeviceData
 功能描述  : 分析上报的三合一传感器设备 数据
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年04月23日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessThreeInOneDeviceData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	int rc = -1;
	
	int data_len = -1;
	char response[128] = {0};
	char tmp[64] = {0};
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessThreeInOneDeviceData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
	

	/*构建data字符串*/
	sprintf(response,"%04x",THREE_IN_ONE);
	sprintf(tmp,"%02x",RESPONSE_THREE_REPORT_DATA);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",0x0D);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_endpoint);
	strcat(response,tmp);
	
	if(data_len>30)
	{
		char data_buf[8] = {0};
        int attr_id = -1;
		int attr_type = -1;
		
		int pm10_data = -1;
		int pm25_data = -1;
		int pm1_data = -1;
		
		int temp_data = -1;
		int humidity_data = -1;
		int co2_data = -1;
		
		/* Get attribute id data  */
		strncpy(data_buf,zcl_data + 2,4);
		
		attr_id = strtol(data_buf,NULL,16);
		
		DEBUG_ZONE(LOG_ZONE_MY,"Report attribute id data:0x%04x.\n",attr_id);	
		
		if(0x0000 == attr_id)
		{
			
			/* Get attribute type and len  data  */
			strncpy(data_buf,zcl_data + 6,4);
		
		    attr_type = strtol(data_buf,NULL,16);
		
		    DEBUG_ZONE(LOG_ZONE_MY,"Report attribute type data:0x%02x.\n",(attr_type>>8)&0xff);	
			
		    DEBUG_ZONE(LOG_ZONE_MY,"Report attribute data len:0x%02x.\n",attr_type&0x00ff);	
			
		}
		
		if(0x410C == attr_type)
		{
			/* Get the pm10 data  */
			strncpy(data_buf,zcl_data + 10,4);
			
			pm10_data = strtol(data_buf,NULL,16);
			
			DEBUG_ZONE(LOG_ZONE_MY,"Report Data pm10 data:0x%04x.\n",pm10_data);	

			/* Get the pm25 data  */
			strncpy(data_buf,zcl_data + 14,4);
			
			pm25_data = strtol(data_buf,NULL,16);
			
			DEBUG_ZONE(LOG_ZONE_MY,"Report Data pm25 data:0x%04x.\n",pm25_data);	
			
			
			/* Get the pm1.0 data  */
			strncpy(data_buf,zcl_data + 18,4);
			
			pm1_data = strtol(data_buf,NULL,16);
			
			DEBUG_ZONE(LOG_ZONE_MY,"Report Data pm1 data:0x%04x.\n",pm1_data);	
			
			
			/* Get the temp data  */
			strncpy(data_buf,zcl_data + 22,4);
			
			temp_data = strtol(data_buf,NULL,16);
			
			DEBUG_ZONE(LOG_ZONE_MY,"Report temp_data:0x%04x.\n",temp_data);	
			
			
			/* Get the humidity data */
			strncpy(data_buf,zcl_data + 26,4);
			
			humidity_data = strtol(data_buf,NULL,16);
			
			DEBUG_ZONE(LOG_ZONE_MY,"Report humidity data:0x%04x.\n",humidity_data);	
			
			
			/* Get the pm10 data  */
			strncpy(data_buf,zcl_data + 30,4);
			
			co2_data = strtol(data_buf,NULL,16);
			
			DEBUG_ZONE(LOG_ZONE_MY,"Report co2 data:0x%04x.\n",co2_data);	
			
			strcat(response,zcl_data + 10);
			
			if (hsbk_devicedata!=NULL)
			{
				rc = 0x00;
				(*hsbk_devicedata)(HSZGatewayMac(),dev_mac,response);
			}
		}
	}
}
/*****************************************************************************
 函 数 名  : HSZProcessThreeInOneParameterData
 功能描述  : 分析上报的三合一传感器配置参数
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年04月23日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessThreeInOneParameterData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	int rc = -1;
	int data_len = -1;
	char response[128] = {0};
	char tmp[16] = {0};
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessThreeInOneParameterData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
	
	/*构建data字符串*/
	sprintf(response,"%04x",THREE_IN_ONE);
	sprintf(tmp,"%02x",RESPONSE_THREE_PARA_DATA);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",0x02);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_endpoint);
	strcat(response,tmp);
	
	
	if(data_len > 10)
	{
		char data_buf[8] = {0};

		int time_intervel_data = -1;
		int reversed_data = -1;
		int firmware_version_data = -1;
		
		int time_intervel = -1;
		
		/* Get the time intervel data  */
		strncpy(data_buf,zcl_data + 10,2);
		
		time_intervel_data = strtol(data_buf,NULL,16);
		
		DEBUG_ZONE(LOG_ZONE_MY,"Report time interve data:0x%02x.\n",time_intervel_data);	
		
		if(0x06 == time_intervel_data)
		{
			time_intervel = 10;
		}
		else if(0x07 == time_intervel_data)
		{
			time_intervel = 30;
		}
		else
		{
			time_intervel = time_intervel_data;
		}
		
		sprintf(tmp,"%02x",time_intervel);
	    strcat(response,tmp);
		
		/* Get the time intervel data  */
		strncpy(data_buf,zcl_data + 12,2);
		
		reversed_data = strtol(data_buf,NULL,16);
		
		DEBUG_ZONE(LOG_ZONE_MY,"Report reversed data:0x%02x.\n",reversed_data);	
		
		
		/* Get the firmware version data  */
		strncpy(data_buf,zcl_data + 14,2);
		
		firmware_version_data = strtol(data_buf,NULL,16);
		
		DEBUG_ZONE(LOG_ZONE_MY,"Report firmware version data:%d.\n",firmware_version_data);	
		
		DEBUG_ZONE(LOG_ZONE_MY,"The firmware version:%d.%d.%d\n",firmware_version_data/100,(firmware_version_data/10)%10,firmware_version_data%100);	
		
		
		if (hsbk_devicedata!=NULL)
		{
			rc = 0x00;
			(*hsbk_devicedata)(HSZGatewayMac(),dev_mac,response);
		}
		
	}
}


/*****************************************************************************
 函 数 名  : HSZProcessClothesHangerReportData
 功能描述  : 分析上报的晾衣机数据
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年11月8日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessClothesHangerReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	
	/*
	{"clusterId":"0x0012","commandId":"0x0A",
	"commandData":"0x0000410CBBA3030F3030000200011744","clusterSpecific":false,
	"deviceEndpoint":{"eui64":"0x00124B00198D65EE","endpoint":1}}.
	*/
	
	int data_len = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessClothesHangerReportData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
	
	if(data_len>8)
	{
		char clothes_attri_id_buf[8] = {0};
		char clothes_attri_data_type_buf[8] = {0};
		char clothes_attri_len_buf[8] = {0};

		int attri_id = -1;
		int attri_buf_len = -1;
		int attri_data_type = -1;

		int clothes_attribute_id = -1;
		int clothes_attribute_type = -1;
		int clothes_attribute_len = -1;
		
		
		/* Get the clothes attribute id */
		strncpy(clothes_attri_id_buf,zcl_data + 2,4);
		
		attri_id = strtol(clothes_attri_id_buf,NULL,16);
		
		clothes_attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
		
		DEBUG_ZONE(LOG_ZONE_TEST,"Clothes Hanger Data Attribute Id:0x%04x.\n",clothes_attribute_id);							

		/* Get the clothes attribute data type */
		strncpy(clothes_attri_data_type_buf,zcl_data + 6,2);	

		attri_data_type = strtol(clothes_attri_data_type_buf,NULL,16);

		clothes_attribute_type = (attri_data_type&0xFF);

		DEBUG_ZONE(LOG_ZONE_TEST,"Clothes Hanger Data Attribute Type:0x%02x.\n",clothes_attribute_type);
		
		/* Get the clothes attribute data len */
		strncpy(clothes_attri_len_buf,zcl_data + 8,2);	

		attri_buf_len = strtol(clothes_attri_len_buf,NULL,16);

		clothes_attribute_len = (attri_buf_len&0xFF);

		DEBUG_ZONE(LOG_ZONE_TEST,"Clothes Hanger Data Attribute len:0x%02x.\n",clothes_attribute_len);
	
	
	    if((CLOTHES_ATTRIBUTE_ID == clothes_attribute_id)&&(CLOTHES_DATA_TYPE == clothes_attribute_type))
		{
			unsigned char data_buf[512] = {0};
			char data_temp[4] = {0};
			int temp = 0x00;
			
			/* Get the Clothes Hanger data */		
			DEBUG_ZONE(LOG_ZONE_MY,"Clothes Hanger Report Data:%s.\n",zcl_data + 10);
		
			for(temp = 0x00;temp <clothes_attribute_len;temp++)
			{
				strncpy(data_temp,zcl_data + 8 + 2*temp,2);	

				data_buf[temp] = strtoul(data_temp,NULL,16); 
				
			}
#if 1	
			for(temp = 0x00;temp <clothes_attribute_len;temp++)
			{
				DEBUG_ZONE(LOG_ZONE_MY,"0x%02x \n",data_buf[temp]);
			}
#endif 			

			/* 数据处理 */
			ResponseClothesHangerStatus(HSZGatewayMac(),dev_mac,data_buf);
		}
		else
		{
			DEBUG_ZONE(LOG_ZONE_ERROR,"Report Clothes Hanger Data Error.\n");
			HSZDeviceError(HSZ_ERR_SMARTLOCK_MSG_ERR,"Report Clothes Hanger Data Error.",NULL);
		}
	}
}
/*****************************************************************************
 函 数 名  : HSZProcessZigBeeTo485ReportData
 功能描述  : 分析上报ZigBee转485数据
 输入参数  : 
			char* dev_mac,设备MAC地址
			int dev_endpoint：设备端点
			int zcl_data：设备数据 
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年02月21日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZProcessZigBeeTo485ReportData(char* dev_mac,int dev_endpoint,char* zcl_data)
{
	
	/*
	{"clusterId":"0xFC11","commandId":"0x0A","commandData":"0x000041112233445566778899000000000000000000",
	                       "clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B001AAACD2D","endpoint":1}}.
	*/
	
	/*
	ZCL Response: {"clusterId":"0xFC11","commandId":"0x0A","commandData":"0x00004112131415161718192021222324252627282930",
	                       "clusterSpecific":false,"deviceEndpoint":{"eui64":"0x00124B001AAACD2D","endpoint":1}}.
	*/
	
	int data_len = -1;
	
	data_len = strlen(zcl_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZProcessZigBeeTo485ReportData,zcl data:%s,zcl data len:%d.\n",zcl_data,data_len);
	
	if(data_len > 0)
	{
		char zigbee_attri_id_buf[8] = {0};
		char zigbee_attri_data_type_buf[8] = {0};
		char zigbee_attri_len_buf[8] = {0};

		int attri_id = -1;
		int attri_buf_len = -1;
		int attri_data_type = -1;

		int zigbee_attribute_id = -1;
		int zigbee_attribute_type = -1;
		int zigbee_attribute_len = -1;
		
		
		/* Get the zigbee to 485 attribute id */
		strncpy(zigbee_attri_id_buf,zcl_data + 2,4);
		
		attri_id = strtol(zigbee_attri_id_buf,NULL,16);
		
		zigbee_attribute_id = ((attri_id&0x00FF)<<8)|((attri_id&0xFF00)>>8);
		
		DEBUG_ZONE(LOG_ZONE_MY,"ZigBee to 485 Data Attribute Id:0x%04x.\n",zigbee_attribute_id);							

		/* Get the zigbee to 485 attribute data type */
		strncpy(zigbee_attri_data_type_buf,zcl_data + 6,2);	

		attri_data_type = strtol(zigbee_attri_data_type_buf,NULL,16);

		zigbee_attribute_type = (attri_data_type&0xFF);

		DEBUG_ZONE(LOG_ZONE_MY,"ZigBee to 485 Data Attribute Type:0x%02x.\n",zigbee_attribute_type);
		
		/* Get the zigbee to 485 attribute data len */
		strncpy(zigbee_attri_len_buf,zcl_data + 8,2);	

		attri_buf_len = strtol(zigbee_attri_len_buf,NULL,16);

		zigbee_attribute_len = (attri_buf_len&0xFF);

		DEBUG_ZONE(LOG_ZONE_MY,"ZigBee to 485 Data Attribute len:0x%02x.\n",zigbee_attribute_len);
		
		
		if((0x0000 == zigbee_attribute_id)&&(0x41 == zigbee_attribute_type))
		{
			char response[128] = {0};
			char tmp[64] = {0};
			
			/*构建data字符串*/
			sprintf(response,"%04x",0xFE55);
			sprintf(tmp,"%02x",0x02);
			strcat(response,tmp);

			sprintf(tmp,"%02x",zigbee_attribute_len + 0x02); /*增加ZigBee endpoint长度*/
			strcat(response,tmp);
			
			sprintf(tmp,"%02x",dev_endpoint);
			strcat(response,tmp);
			
            strcat(response,zcl_data + 10);
			
			DEBUG_ZONE(LOG_ZONE_MY,"ZigBee to 485 Send to Gateway:%s.\n",response);
			
			if (hsbk_devicedata!=NULL)
			{	
				(*hsbk_devicedata)(HSZGatewayMac(),dev_mac,response);
			}
		}
		else
		{
			DEBUG_ZONE(LOG_ZONE_ERROR,"Report zigbee to 485 Data Error.\n");
			HSZDeviceError(HSZ_ERR_SMARTLOCK_MSG_ERR,"Report zigbee to 485 Data Error.",NULL);
		}
	}
	
}
