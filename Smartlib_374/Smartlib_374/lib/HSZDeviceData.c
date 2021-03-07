/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZDeviceData.c
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2017年8月12日
  最近修改   :
  功能描述   : 传感器累设备数据处理
  函数列表   :
             
  修改历史   :
  1.日    期   : 2017年8月12日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

******************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "HSZMessage.h"
#include "HSZCommon.h"
#include "HSZDeviceData.h"



struct join_dev_info DEV_INFO[PERMIT_MAX_JOIN_DEV];		/*记录入网设备的结构体*/

static int join_device_index;                       	/*记录入网设备个数*/



/*****************************************************************************
 函 数 名  : strupr_linux
 功能描述  : 小写字母转大写字母
 输入参数  : 
			char* src ：要转换字符串
 输出参数  : 无
 返 回 值  : 无
 
 修改历史      :
  1.日    期   : 2017年8月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
char* strupr_linux(char* src)
{
	while(*src !='\0')
	{
		if(*src >='a' && *src <= 'z')
		{
		 *src -= 32;
		}
		
		src++;
	}
	
	return src;
}
/*****************************************************************************
 函 数 名  : HSZSort
 功能描述  : 冒泡法排序数组(由小到大排序)
 输入参数  : 
			int array[]:要排序的数组
			int len：排序数组的长度   
 输出参数  : 无
 返 回 值  : 无
 
 修改历史      :
  1.日    期   : 2017年8月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZSort(int array[],int len)
{
	int i = 0;
	int j = 0;
	
	int temp = 0;
	
	for(i = 0;i<len-1;i++)
	{
		for(j = i+1;j<len;j++)
		{
			if(array[j]<array[i])
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
}
/*****************************************************************************
 函 数 名  : HSZInitDeviceInfoBuffer
 功能描述  : 入网设备管理列表初始化（清空）
 输入参数  : 无    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZInitDeviceInfoBuffer(void)
{
	DEBUG_ZONE(LOG_ZONE_MY,"HSZInitDeviceInfoBuffer.\n");
	
	memset(DEV_INFO,0,PERMIT_MAX_JOIN_DEV*sizeof(struct join_dev_info));/*清空缓存的buffer*/
	
	join_device_index = 0x00;/*统计个数归零*/
}

/*****************************************************************************
 函 数 名  : HSZPrintDeviceBufInfo
 功能描述  : 打印入网设备列表，用于调试信息
 输入参数  : 无    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
void HSZPrintDeviceBufInfo(void)
{
	int tmp;
	int tmp_index;
	
	for(tmp = 0;tmp <join_device_index;tmp++)
	{                     
		if(strcmp(DEV_INFO[tmp].dev_eui,"")!=0x00)
		{
			DEBUG_ZONE(LOG_ZONE_MY,"---------------------------------------------------------------.\n");
			
			DEBUG_ZONE(LOG_ZONE_MY,"The joined device Mac:%s.\n",DEV_INFO[tmp].dev_eui);
			DEBUG_ZONE(LOG_ZONE_MY,"The joined device Nodeid:0x%04x.\n",DEV_INFO[tmp].dev_nodeid);
			DEBUG_ZONE(LOG_ZONE_MY,"The joined device endpoint total:0x%02x.\n",DEV_INFO[tmp].endpoint_total);
			
			for(tmp_index = 0;tmp_index <DEV_INFO[tmp].endpoint_total;tmp_index++)
			{
				DEBUG_ZONE(LOG_ZONE_MY,"The <%d> joined device endpoint :0x%02x,status:0x%04x.\n",tmp_index+1,DEV_INFO[tmp].endpoint[tmp_index],
				DEV_INFO[tmp].dev_status[tmp_index]);
			}
			
			// DEBUG_ZONE(LOG_ZONE_MY,"The joined device voltage levle is:0x%02x.\n",DEV_INFO[tmp].low_voltage_statue);
			DEBUG_ZONE(LOG_ZONE_MY,"The joined device device type is:0x%04x.\n",DEV_INFO[tmp].dev_type);
			
			DEBUG_ZONE(LOG_ZONE_MY,"---------------------------------------------------------------.\n");
		}
	}
	
}

/*****************************************************************************
 函 数 名  : HSZFindEuiFromInfoBuffer
 功能描述  : 查找入网设备对应的EUI的Index
 输入参数  : 
			char* dev_ma:入网设备MAC地址 
			int max_index：查找的最大个数
 输出参数  : 
			返回对应的EUI的Index
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZFindEuiFromInfoBuffer(char* dev_mac,int max_index)
{
	int tmp;
	int find_index = -1;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZFindEuiFromInfoBuffer.\n");
	
	for(tmp = 0;tmp < max_index;tmp++)
	{
		if(0x00 == strcmp(dev_mac,DEV_INFO[tmp].dev_eui))
		{
			find_index = tmp;
			break;
		}
	}
	
	return find_index;
}

/*****************************************************************************
 函 数 名  : HSZFindDeviceTypeFromInfoBuffer
 功能描述  : 查找入网设备对应的EUI的Device Type
 输入参数  : 
			char* dev_ma:入网设备MAC地址 
			int max_index：查找的最大个数
 输出参数  : 
			返回对应的Device EUI的Device type
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年1月25日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZFindDeviceTypeFromInfoBuffer(char* dev_mac,int max_index)
{
	int tmp;
	int device_type = -1;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZFindDeviceTypeFromInfoBuffer.\n");
	
	for(tmp = 0;tmp < max_index;tmp++)
	{
		if(0x00 == strcmp(dev_mac,DEV_INFO[tmp].dev_eui))
		{
			device_type = DEV_INFO[tmp].dev_type;
			break;
		}
	}
	
	return device_type;
}
/*****************************************************************************
 函 数 名  : HSZUpdateNodeIdFromBuffer
 功能描述  : 更新入网设备对应的EUI的NodeID
 输入参数  : 
			char* dev_mac:入网设备MAC地址
			int new_nodeid：要更新的最新的NodeID
 输出参数  : 
			返回对应的EUI的Index
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZUpdateNodeIdFromBuffer(char* dev_mac,int new_nodeid)
{
	int index = HSZFindEuiFromInfoBuffer(dev_mac,PERMIT_MAX_JOIN_DEV);
	
	if(-1!=index)
	{
		DEV_INFO[index].dev_nodeid = new_nodeid;
		DEBUG_ZONE(LOG_ZONE_MY,"Update Node id success,EUI:%s,new_nodeid:%d.\n",dev_mac,new_nodeid);
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Update Node id error ,EUI:%s,new_nodeid:%d.\n",dev_mac,new_nodeid);
		HSZDeviceError(HSZ_ERR_UPDATE_NODEID,"Update Node id error.",NULL);
	}
	
	return index;
}


/*****************************************************************************
 函 数 名  : HSZIsNeedUpdateStatus
 功能描述  : 检查设备是否需要更新状态
 输入参数  : 
			char* dev_mac:设备mac地址
			int end_point：endpoint 
 输出参数  : 
			返回对应的Cluster ID
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年2月22日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZIsNeedUpdateStatus(char* dev_mac,int end_point,int status)
{
	int tmp;
	int tmp_index;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZIsNeedUpdateStatus.\n");
	
	for(tmp = 0;tmp < PERMIT_MAX_JOIN_DEV;tmp++)
	{	
        if(0x00 == strcmp(dev_mac,DEV_INFO[tmp].dev_eui))
		{
			for(tmp_index = 0;tmp_index < DEV_INFO[tmp].endpoint_total;tmp_index++)
			{
				if(end_point == DEV_INFO[tmp].endpoint[tmp_index])
				{
					if(status != DEV_INFO[tmp].dev_status[tmp_index])
					{
						DEV_INFO[tmp].dev_status[tmp_index] = status;
						
						DEBUG_ZONE(LOG_ZONE_MY,"The <%s> device endpoint:0x%02x,change status to:0x%02x.\n",DEV_INFO[tmp].dev_eui,DEV_INFO[tmp].endpoint[tmp_index],DEV_INFO[tmp].dev_status[tmp_index]);
						
						return 0x01;
					}
					else
					{
						DEBUG_ZONE(LOG_ZONE_MY,"The <%s> device endpoint:0x%02x,not need to update.\n",DEV_INFO[tmp].dev_eui,DEV_INFO[tmp].endpoint[tmp_index],DEV_INFO[tmp].dev_status[tmp_index]);
						
						return 0x00;
					}
				}
			}
		}		
	}
}
/*****************************************************************************
 函 数 名  : HSZSetInfoBuffer
 功能描述  : 将入网设备填入缓存buffer
 输入参数  : 
			char* dev_mac：入网设备地址；
			int nodeid：入网设备NodeID；
			int end_point：入网Endpoint；
			int dev_type:入网设备类型
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZSetInfoBuffer(char* dev_mac,int nodeid,int end_point,int dev_type)
{
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZSetInfoBuffer.\n");
    
	int find_index = -10;
	int temp_index = -1;
	
	if(join_device_index < PERMIT_MAX_JOIN_DEV)/*小于设定的存储设备数量*/
	{	
	    find_index = HSZFindEuiFromInfoBuffer(dev_mac,join_device_index+1);
		
		/*如果已经保存过了该Endpoint就跳过*/
		for(temp_index = 0;temp_index < PERMIT_MAX_ENDPOIN-1; temp_index++)
		{
			if(end_point == DEV_INFO[find_index].endpoint[temp_index])
			{
				DEV_INFO[find_index].dev_status[temp_index] = DEVICE_INIT_STATUS;

				DEBUG_ZONE(LOG_ZONE_TEST,"Device:Has saved the endpoint init status.\n");
				
				return 0;
			}
		}

		
	
	    if(-1 != find_index)/*不是新记录的数据*/
		{
			if(DEV_INFO[find_index].endpoint_total<(PERMIT_MAX_ENDPOIN-1))
			{
				DEV_INFO[find_index].endpoint_total = DEV_INFO[find_index].endpoint_total + 0x01;
				DEBUG_ZONE(LOG_ZONE_MY,"Not New device,EUI:%s,Total endpoint:<%d>,Current endpoint:0x%02x.\n",dev_mac,DEV_INFO[find_index].endpoint_total,end_point);
				
				DEV_INFO[find_index].dev_status[DEV_INFO[find_index].endpoint_total-1] =  DEVICE_INIT_STATUS;
				
				DEV_INFO[find_index].endpoint[DEV_INFO[find_index].endpoint_total-1] =  end_point;
				
				/*给之前数组endpoint 排序*/		
				HSZSort(DEV_INFO[find_index].endpoint,DEV_INFO[find_index].endpoint_total);
			}
			else
			{
				DEBUG_ZONE(LOG_ZONE_MY,"Device Total Endpoint out of range,Current EUI:%s,Total Endpoint:%d.\n",dev_mac,DEV_INFO[find_index].endpoint_total+1);
				
				HSZDeviceError(HSZ_ERR_RECORD_JOIN_DEV,"Device Total Endpoint out of range.",NULL);
			}
		}
		else /*首次记录的新数据*/
		{		
	        DEBUG_ZONE(LOG_ZONE_MY,"New Device EUI:%s.\n",dev_mac);
			strncpy(DEV_INFO[join_device_index].dev_eui,dev_mac,strlen(dev_mac));
			DEV_INFO[join_device_index].dev_nodeid = nodeid;
			DEV_INFO[join_device_index].endpoint_total += 0x01;
			DEV_INFO[join_device_index].endpoint[0] =  end_point;	
		    DEV_INFO[join_device_index].low_voltage_statue = DEVICE_INIT_VOLTAGE;/*电池电量为初始状态*/
			DEV_INFO[join_device_index].dev_type = dev_type;
			
			DEV_INFO[join_device_index].dev_status[0] = DEVICE_INIT_STATUS;
			
			join_device_index++;
		}
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Device number out of range,Current EUI:%s,Total dev number:%d.\n",dev_mac,join_device_index+1);
		HSZDeviceError(HSZ_ERR_RECORD_JOIN_DEV,"Device number out of range.",NULL);
	}
	
	return 0;
}

/*****************************************************************************
 函 数 名  : HSZFindNodeidFromInfoBuffer
 功能描述  : 查找入网设备对应的Nodeid
 输入参数  : 
			char* dev_mac:入网设备MAC地址     
 输出参数  : 
			返回对应的Nodeid
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZFindNodeidFromInfoBuffer(char* dev_mac)
{
	int tmp;
	int find_nodeid = -1;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZFindNodeidFromInfoBuffer.\n");
	
	for(tmp = 0;tmp < PERMIT_MAX_JOIN_DEV;tmp++)
	{
		if(0x00 == strcmp(dev_mac,DEV_INFO[tmp].dev_eui))
		{
			find_nodeid = DEV_INFO[tmp].dev_nodeid;
			
			break;
		}
	}
	
	if(-1 == find_nodeid)
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Not find the nodeid,EUI:%s.\n",dev_mac);
		HSZDeviceError(HSZ_ERR_FIND_NODEID,"Not find the nodeid.",NULL);
		
	}
	
	return find_nodeid;
}

/*****************************************************************************
 函 数 名  : HSZFindEndpointFromInfoBuffer
 功能描述  : 查找入网设备对应Endpoint
 输入参数  : 
			char* dev_mac：入网设备的MAC地址
			int dev_index：控制设备Index    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZFindEndpointFromInfoBuffer(char* dev_mac,int dev_index)
{
	int tmp;
	int find_endpoint = -1;
	int endpoint = -1;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZFindEndpointFromInfoBuffer.\n");
	
	for(tmp = 0;tmp < PERMIT_MAX_JOIN_DEV;tmp++)
	{
		if(0x00 == strcmp(dev_mac,DEV_INFO[tmp].dev_eui))
		{
			find_endpoint = DEV_INFO[tmp].endpoint_total;
			
			if(dev_index>find_endpoint)
			{
				DEBUG_ZONE(LOG_ZONE_MY,"ERROR:Request Endpoint:%d,Total endpoint:%d.\n",dev_index,find_endpoint);
				HSZDeviceError(HSZ_ERR_FIND_ENDPOINT,"Request Endpoint Error",NULL);
			}
			else
			{
				endpoint = DEV_INFO[tmp].endpoint[dev_index-1];
			}
			
			break;
		}
	}
	
	return endpoint;
}

/*****************************************************************************
 函 数 名  : covert_eui64tostr
 功能描述  : 将对应的Mac地址转换为字符串，例如1234567890abcdef 
             转换为{12 34 56 78 90 ab cd ef}
 输入参数  : 
			char* mac：入网设备的MAC地址
			char* strbuf：转换后的字符串    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年1月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int covert_eui64tostr(char*mac,char* strbuf)
{
	int rc=-1;
	int n;
	if (strlen(mac)!=16)
	{
		return rc;
	}
	
	strbuf[0]='{';
	for (n=0;n<strlen(mac)/2;n++)
	{
		printf("%c",mac[n*2]);
		strbuf[n*3+1]=mac[n*2];
		strbuf[n*3+2]=mac[n*2+1];
		strbuf[n*3+3]=' ';
	}
	strbuf[8*3]='}';
	return 0;
}
/*****************************************************************************
 函 数 名  : CommandSetOnOffSwitch
 功能描述  : On-Off Switch 控制处理
 输入参数  : char* topic       
             char* dev_mac     
             int dev_endpoint  
             int cmd_type      
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetOnOffSwitch(char* topic,char* dev_mac,int dev_endpoint,int cmd_type)
{
	/*
	
	{"commands":[{"command":"zcl on-off on"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}
	{"commands":[{"command":"zcl on-off off"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}
	{"commands":[{"command":"zcl on-off toggle"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}
	
	*/
	int rc = -1;
	char msg[128] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetOnOffSwitch-Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,dev_endpoint:%d,cmd_type:%d.\n",dev_mac,dev_endpoint,cmd_type);
	
	switch(cmd_type)
	{
		case SET_ON_OFF_SWITCH_ON:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"ON_OFF_SWITCH_ON.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off on\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
			,dev_mac,dev_endpoint);
		}
		break;
		
		case SET_ON_OFF_SWITCH_OFF:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"ON_OFF_SWITCH_OFF.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off off\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
			,dev_mac,dev_endpoint);
		}
		break;
		
		case SET_ON_OFF_SWITCH_TOOGLE:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"ON_OFF_SWITCH_TOOGLE.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off toggle\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
			,dev_mac,dev_endpoint);
		}
		break;
		
		default:
		break;
	}
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);

	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSetLevelControlMove
 功能描述  : Command description for Move
 输入参数  : uint8_t move_mode       
             uint8_t rate     
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年12月07日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetLevelControlMove(char* topic,char* dev_mac,int dev_endpoint,int move_mode,int rate)
{
	int rc = -1;
	char msg[256] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetLevelControlMoveWithOnOff.\n");
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,dev_endpoint:%d,move_mode:%d,rate;%d.\n",dev_mac,dev_endpoint,move_mode,rate);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"zcl level-control move %d %d\"},{\"command\":\"plugin device-table send {%s} %d\"}]}",move_mode,rate,dev_mac,dev_endpoint);
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_MY,"Send Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}
/*****************************************************************************
 函 数 名  : CommandSetLevelControlMoveToLevel
 功能描述  : Command description for MoveToLevel
 输入参数  : uint8_t levle       
             uint16_t transition_time     
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年12月07日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetLevelControlMoveToLevel(char* topic,char* dev_mac,int dev_endpoint,int levle,int transition_time)
{
	int rc = -1;
	char msg[256] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetLevelControlMoveToLevel.\n");
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,dev_endpoint:%d,levle:%d,transition time.\n",dev_mac,dev_endpoint,levle,transition_time);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"zcl level-control mv-to-level %d %d\"},{\"command\":\"plugin device-table send {%s} %d\"}]}",levle,transition_time,dev_mac,dev_endpoint);
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_MY,"Send Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSetLevelControlMoveWithOnOff
 功能描述  : Command description for MoveWithOnOff
 输入参数  : uint8_t move_mode       
             uint8_t rate     
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年12月07日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetLevelControlMoveWithOnOff(char* topic,char* dev_mac,int dev_endpoint,int move_mode,int rate)
{
	int rc = -1;
	char msg[256] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetLevelControlMoveWithOnOff.\n");
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,dev_endpoint:%d,move_mode:%d,rate;%d.\n",dev_mac,dev_endpoint,move_mode,rate);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"zcl level-control o-move %d %d\"},{\"command\":\"plugin device-table send {%s} %d\"}]}",move_mode,rate,dev_mac,dev_endpoint);
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_MY,"Send Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSetLevelControlMoveToLevelWithOnOff
 功能描述  : Command description for MoveToLevelWithOnOff
 输入参数  : uint8_t levle       
             uint16_t transition_time     
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年12月07日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetLevelControlMoveToLevelWithOnOff(char* topic,char* dev_mac,int dev_endpoint,int levle,int transition_time)
{
	int rc = -1;
	char msg[256] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetLevelControlMoveToLevelWithOnOff.\n");
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,dev_endpoint:%d,levle:%d,transition time.\n",dev_mac,dev_endpoint,levle,transition_time);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"zcl level-control o-mv-to-level %d %d\"},{\"command\":\"plugin device-table send {%s} %d\"}]}",levle,transition_time,dev_mac,dev_endpoint);
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_MY,"Send Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}
 
/*****************************************************************************
 函 数 名  : CommandSetLevelControlStepWithOnOff
 功能描述  : Command description for StepWithOnOff
 输入参数  : uint8_t step_mode       
             uint8_t step_size   
			 uint16_t transition_time
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年12月07日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/ 
int CommandSetLevelControlStepWithOnOff(char* topic,char* dev_mac,int dev_endpoint,int step_mode,int step_size,int transition_time)
{
	int rc = -1;
	char msg[256] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetLevelControlMoveToLevel.\n");

	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,dev_endpoint:%d,step mode:%d,step size %d.transition time:%d.\n",dev_mac,dev_endpoint,step_mode,step_size,transition_time);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"zcl level-control o-step %d %d %d\"},{\"command\":\"plugin device-table send {%s} %d\"}]}",step_mode,step_size,transition_time,dev_mac,dev_endpoint);
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_MY,"Send Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSetLevelControlStopWithOnOff
 功能描述  : Command description for StopWithOnOff
 输入参数  : 
			char* topic：
			char* dev_mac：
			int dev_endpoint   
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年12月07日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetLevelControlStopWithOnOff(char* topic,char* dev_mac,int dev_endpoint)
{
	int rc = -1;
	char msg[256] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetLevelControlMoveToLevel.\n");
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,dev_endpoint:%d.\n",dev_mac,dev_endpoint);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"zcl level-control o-stop\"},{\"command\":\"plugin device-table send {%s} %d\"}]}",dev_mac,dev_endpoint);
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_MY,"Send Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSetLevelControlStep
 功能描述  : Command description for Step
 输入参数  : uint8_t step_mode       
             uint8_t step_size    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年12月07日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetLevelControlStep(char* topic,char* dev_mac,int dev_endpoint,int step_mode,int step_size,int transition_time)
{
	int rc = -1;
	char msg[256] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetLevelControlMoveToLevel.\n");
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,dev_endpoint:%d,step mode:%d,step size %d.transition time:%d.\n",dev_mac,dev_endpoint,step_mode,step_size,transition_time);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"zcl level-control step %d %d %d\"},{\"command\":\"plugin device-table send {%s} %d\"}]}",step_mode,step_size,transition_time,dev_mac,dev_endpoint);
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_MY,"Send Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSetLevelControlStop
 功能描述  : Command description for Stop
 输入参数  : 
				char* topic：
				char* dev_mac：
				int dev_endpoint
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年12月07日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetLevelControlStop(char* topic,char* dev_mac,int dev_endpoint)
{
	int rc = -1;
	char msg[256] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetLevelControlMoveToLevel.\n");
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,dev_endpoint:%d.\n",dev_mac,dev_endpoint);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"zcl level-control stop\"},{\"command\":\"plugin device-table send {%s} %d\"}]}",dev_mac,dev_endpoint);
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_MY,"Send Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSetLevelControlSwitch
 功能描述  : 设置调光开关处理函数
 输入参数  : char* topic       
             char* dev_mac     
             int dev_endpoint  
             int cmd_type      
             int level_value   
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月16日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetLevelControlSwitch(char* topic,char* dev_mac,char* cmd)
{
	int rc = -1;
	char msg[128] = {0};
	
	char cmd_buf[10] = {0};/*cmd*/
	int  cmd_type = 0;
	
	char len_c[10] = {0};/*buf*/
	int len = 0;
	
	char endpoint_c[10] = {0};/*endpoint*/
	int index = 0;
	int endpoint = 0;
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetLevelControlSwitch.\n");
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Topic:%s,dev_mac:%s,cmd:%d.\n",topic,dev_mac,cmd);
	
	
	/*获取commmad cmd type value*/	
	strncpy(cmd_buf,&cmd[4],2);
	
	cmd_type = strtol(cmd_buf,NULL,16);
	
	DEBUG_ZONE(LOG_ZONE_TEST,"cmd_buf:0x%02x.\n",cmd_type);
	
	strncpy(len_c,&cmd[6],2);
	len = strtol(len_c,NULL,16);
	
	if(len > 0)
	{
		strncpy(endpoint_c,&cmd[8],2);
		index = strtol(endpoint_c,NULL,16);
		
		if(index!=0)
		{
			endpoint = HSZFindEndpointFromInfoBuffer(dev_mac,index);
			
			if(endpoint<1)
			{
				DEBUG_ZONE(LOG_ZONE_ERROR,"Levle Control not find endpoint.\n");
				HSZDeviceError(HSZ_ERR_FIND_ENDPOINT,"Levle Control not find endpoint",NULL);
				
				return rc;
			}
		}
		
		rc = 0x00;
	}
	else
	{
		return rc;
	}
	
	switch(cmd_type)
	{
		case SET_LEVEL_CONTROL_ON:
		{
			if(0x01 == len)
			{
				sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off on\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
				,dev_mac,endpoint);

				DEBUG_ZONE(LOG_ZONE_MY,"Topic:%s,Send Message:%s.\n",topic,msg);
				rc = mosquitto_publish_topic(topic,msg);
			}
		}
		break;
		
		case SET_LEVEL_CONTROL_OFF:
		{
			if(0x01 == len)
			{
				sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off off\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
				,dev_mac,endpoint);
				
				DEBUG_ZONE(LOG_ZONE_MY,"Topic:%s,Send Message:%s.\n",topic,msg);
				rc = mosquitto_publish_topic(topic,msg);
			}
		}
		break;
		
		case SET_LEVEL_CONTROL_TOGGLE:
		{
			if(0x01 == len)
			{
				sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off toggle\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
				,dev_mac,endpoint);
				
				DEBUG_ZONE(LOG_ZONE_MY,"Topic:%s,Send Message:%s.\n",topic,msg);
				rc = mosquitto_publish_topic(topic,msg);
			}
		}
		break;
		case SET_LEVEL_CONTROL_MOVE_TO_LEVEL:
		{
			if(0x04 == len)
			{
				char levle_buf[10] = {0};
				int  level_value = 0;

				char transition_time_buf[10] = {0};
				int transition_time_value = 0;
				
				strncpy(levle_buf,&cmd[10],2);
				level_value = strtol(levle_buf,NULL,16);
				
				strncpy(transition_time_buf,&cmd[12],4);
				transition_time_value = strtol(transition_time_buf,NULL,16);
				
				if(level_value!=0)
				{
					CommandSetLevelControlMoveToLevelWithOnOff(topic,dev_mac,endpoint,level_value,transition_time_value);
				}
				else
				{
					sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off off\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
					,dev_mac,endpoint);

					DEBUG_ZONE(LOG_ZONE_MY,"Topic:%s,Send Message:%s.\n",topic,msg);
					
					rc = mosquitto_publish_topic(topic,msg);	
				}
				
			}
		}
		break;
		
#if 0
		case SET_LEVEL_CONTROL_MOVE:
		{
			if(0x03 == len)
			{
				char move_mode_buf[10] = {0};
				int  move_mode_value = 0;

				char rate_buf[10] = {0};
				int  rate_value = 0;
				
				strncpy(move_mode_buf,&cmd[10],2);
				move_mode_value = strtol(move_mode_buf,NULL,16);
				
				strncpy(rate_buf,&cmd[12],2);
				rate_value = strtol(rate_buf,NULL,16);
				
				CommandSetLevelControlMove(topic,dev_mac,endpoint,move_mode_value,rate_value);
			}
		}
		break;
		
		
		case SET_LEVEL_CONTROL_MOVE_WITH_ONOFF:
		{
			if(0x03 == len)
			{
				char move_mode_buf[10] = {0};
				int  move_mode_value = 0;

				char rate_buf[10] = {0};
				int  rate_value = 0;
				
				strncpy(move_mode_buf,&cmd[10],2);
				move_mode_value = strtol(move_mode_buf,NULL,16);
				
				strncpy(rate_buf,&cmd[12],2);
				rate_value = strtol(rate_buf,NULL,16);
				
				CommandSetLevelControlMoveWithOnOff(topic,dev_mac,endpoint,move_mode_value,rate_value);
			}
		}
		break;
		
		case SET_LEVEL_CONTROL_MOVE_TO_LEVEL_WITH_ONOFF:
		{
			if(0x04 == len)
			{
				char levle_buf[10] = {0};
				int  level_value = 0;

				char transition_time_buf[10] = {0};
				int transition_time_value = 0;
				
				strncpy(levle_buf,&cmd[10],2);
				level_value = strtol(levle_buf,NULL,16);
				
				strncpy(transition_time_buf,&cmd[12],4);
				transition_time_value = strtol(transition_time_buf,NULL,16);
				
				CommandSetLevelControlMoveToLevelWithOnOff(topic,dev_mac,endpoint,level_value,transition_time_value);
			}
		}
		break;
		
		case SET_LEVEL_CONTROL_STEP_WITH_ONOFF:
		{
			if(0x05 == len)
			{
				char step_mode_buf[10] = {0};
				int  step_mode_value = 0;
                
				char step_size_buf[10] = {0};
				int  step_size_value = 0;
				
				char transition_time_buf[10] = {0};
				int transition_time_value = 0;
				
				strncpy(step_mode_buf,&cmd[10],2);
				step_mode_value = strtol(step_mode_buf,NULL,16);
				
				strncpy(step_size_buf,&cmd[12],2);
				step_size_value = strtol(step_size_buf,NULL,16);
				
				strncpy(transition_time_buf,&cmd[14],4);
				transition_time_value = strtol(transition_time_buf,NULL,16);
				
				CommandSetLevelControlStepWithOnOff(topic,dev_mac,endpoint,step_mode_value,step_size_value,transition_time_value);
			}
		}
		break;
		
		case SET_LEVEL_CONTROL_STOP_WITH_ONOFF:
		{
			CommandSetLevelControlStopWithOnOff(topic,dev_mac,endpoint);
		}
		break;
		
		case SET_LEVEL_CONTROL_STEP:
		{
			if(0x05 == len)
			{
				char step_mode_buf[10] = {0};
				int  step_mode_value = 0;
                
				char step_size_buf[10] = {0};
				int  step_size_value = 0;
				
				char transition_time_buf[10] = {0};
				int transition_time_value = 0;
				
				strncpy(step_mode_buf,&cmd[10],2);
				step_mode_value = strtol(step_mode_buf,NULL,16);
				
				strncpy(step_size_buf,&cmd[12],2);
				step_size_value = strtol(step_size_buf,NULL,16);
				
				strncpy(transition_time_buf,&cmd[14],4);
				transition_time_value = strtol(transition_time_buf,NULL,16);
				
				CommandSetLevelControlStep(topic,dev_mac,endpoint,step_mode_value,step_size_value,transition_time_value);
			}
		}
		break;
		
		case SET_LEVEL_CONTROL_STOP:
		{
			CommandSetLevelControlStop(topic,dev_mac,endpoint);
		}
		break;
#endif 
		
		default:
		break;
	}
	return rc;
}


/*****************************************************************************
 函 数 名  : CommandSetOnOffLight
 功能描述  : 设置On-Off Light
 输入参数  : char* topic       
             char* dev_mac     
             int dev_endpoint  
             int cmd_type      
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月16日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/

int CommandSetOnOffLight(char* topic,char* dev_mac,int dev_endpoint,int cmd_type)
{
	/*
	
	{"commands":[{"command":"zcl on-off on"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}
	{"commands":[{"command":"zcl on-off off"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}
	{"commands":[{"command":"zcl on-off toggle"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}
	
	*/
	int rc = -1;
	char msg[128] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetOnOffLight-Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,dev_endpoint:%d,cmd_type:%d.\n",dev_mac,dev_endpoint,cmd_type);
	
	switch(cmd_type)
	{
		case ON_OFF_LIGHT_ON:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"ON_OFF_LIGHT_ON.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off on\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
			,dev_mac,dev_endpoint);
		}
		break;
		
		case ON_OFF_LIGHT_OFF:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"ON_OFF_LIGHT_OFF.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off off\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
			,dev_mac,dev_endpoint);
		}
		break;
		
		case ON_OFF_LIGHT_TOOGLE:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"ON_OFF_LIGHT_TOOGLE.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off toggle\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
			,dev_mac,dev_endpoint);
		}
		break;
		
		default:
		break;
	}
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"THe Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);

	return rc;
}

/*****************************************************************************
 函 数 名  : ResponsePutContactSensor
 功能描述  : 门磁传感器Response接口函数
 输入参数  : char* gw_mac      
             char* dev_mac       
             int sensor_status  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月16日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int ResponsePutContactSensor(char* gw_mac,char* dev_mac,int dev_endpoint,int sensor_status)
{
	int rc = -1;
	char response[30] = {0};
	char tmp[10] = {0};
	
	/* 构建data字符串*/
	sprintf(response,"%04x",DEVICE_TYPE_CONTACT_SENSOR);/*Device Type*/
	
	sprintf(tmp,"%02x",RESPONSE_CONTACT_SENSOR_STATUS);/*Comamnd*/
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",0x02);/*Length*/
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_endpoint);/*Index*/
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",sensor_status);/*Data*/
	strcat(response,tmp);
	
	// HSZUpdateDeviceLineStatus(dev_mac,dev_endpoint,DEVICE_LINE_UPDATE);
	
	if(0x01 == HSZIsNeedUpdateStatus(dev_mac,dev_endpoint,sensor_status))
	{
		if (hsbk_devicedata!=NULL)
		{
			rc = 0x00;
			(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Device:%s endpoint:%d,not need  update status.\n",dev_mac,dev_endpoint);
	}
	
	return rc;
}

/*****************************************************************************
 函 数 名  : ResponsePutLevelControlSwitch
 功能描述  : 调光开关level Response函数回调
 输入参数  : char* dev_mac     
             char* dev_id      
             int dev_endpoint  
             int level_value   
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月16日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int ResponsePutLevelControlSwitch(char* gw_mac,char* dev_mac,int dev_endpoint,int level_value)
{
	int rc = -1;
	char response[30] = {0};
	char tmp[10] = {0};
	
	/*构建data字符串*/
	sprintf(response,"%04x",LEVEL_CONTROL_SWITCH);
	sprintf(tmp,"%02x",RESPONSE_LEVEL_CONTROL_SWITCH_LEVEL);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",0x02);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_endpoint);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",level_value);
	strcat(response,tmp);
	
	
	if(0x01 == HSZIsNeedUpdateStatus(dev_mac,dev_endpoint,level_value))
	{
		if (hsbk_devicedata!=NULL)
		{
			rc = 0x00;
			
			(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Device:%s endpoint:%d,not need update status.\n",dev_mac,dev_endpoint);
	}
	
	return rc;
}


/*****************************************************************************
 函 数 名  : ResponsePutLevelControlSwitchOnOff
 功能描述  : 调光开关On off函数回调
 输入参数  : char* dev_mac     
             char* dev_id      
             int dev_endpoint  
             int level_value   
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年8月16日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int ResponsePutLevelControlSwitchOnOff(char* gw_mac,char* dev_mac,int dev_endpoint,int level_value)
{
	int rc = -1;
	char response[30] = {0};
	char tmp[10] = {0};
	
	/*构建data字符串*/
	sprintf(response,"%04x",LEVEL_CONTROL_SWITCH);
	sprintf(tmp,"%02x",0x06);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",0x02);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_endpoint);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",level_value);
	strcat(response,tmp);
	
	if (hsbk_devicedata!=NULL)
	{
		(*hsbk_devicedata)(gw_mac,dev_mac,response);
	}
	
	return rc;
}


/*****************************************************************************
 函 数 名  : ResponsePutOnOffLightStatus
 功能描述  : 灯状态发生改变应答接口函数
 输入参数  : char* gw_mac      
             char* dev_mac     
             int dev_endpoint  
             int dev_status    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年9月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int ResponsePutOnOffLightStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int dev_status)
{
	int rc = -1;
	char response[30] = {0};
	char tmp[10] = {0};
	
	/*构建data字符串*/
	sprintf(response,"%04x",ON_OFF_LIGHT);
	sprintf(tmp,"%02x",RESPONSE_ON_OFF_LIGHT_STATUS);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",0x02);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_endpoint);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_status);
	strcat(response,tmp);
	
	// HSZUpdateDeviceLineStatus(dev_mac,dev_endpoint,DEVICE_LINE_UPDATE);
	
	if(0x01 == HSZIsNeedUpdateStatus(dev_mac,dev_endpoint,dev_status))
	{
		if (hsbk_devicedata!=NULL)
		{
			rc = 0x00;
			(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Device:%s endpoint:%d,not need update status.\n",dev_mac,dev_endpoint);
	}
	return rc;
}
/*****************************************************************************
 函 数 名  : ResponseUpdateBatteryLevleInfo
 功能描述  : 更新电池类设备电量信息
 输入参数  : char* gw_mac  ：网关MAC地址    
             char* dev_mac ：设备MAC地址    
             int battery_levle_status：设备电量状态  
  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年9月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int ResponseUpdateBatteryLevleInfo(char* gw_mac,char* dev_mac,int battery_levle_status)
{
	int rc = -1;
	char response[30] = {0};
	char tmp[10] = {0};
	
	
	sprintf(response,"%04x",DEVICE_TYPE_CONTACT_SENSOR);/*Device type*/
	
	sprintf(tmp,"%02x",0x01);/*Command*/
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",0x02);/*Length*/
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",0x01);/*Index*/
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",battery_levle_status);/*Data*/
	strcat(response,tmp);
	
	if (hsbk_devicedata!=NULL)
	{
		rc = 0x00;
		(*hsbk_devicedata)(gw_mac,dev_mac,response);
	}
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSetWarningDeviceControl
 功能描述  : 声光报警器控制
 输入参数  : char* gw_mac  ：网关MAC地址    
             char* dev_mac ：设备MAC地址    
             int battery_levle_status：设备电量状态  
  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年1月5日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetWarningDeviceControl(char* topic,char* dev_mac,char* cmd)
{
	int rc = -1;
	char msg[256] = {0};
	
	char cmd_buf[10] = {0};/*cmd*/
	int  cmd_type = 0;
	
	char len_c[10] = {0};/*buf*/
	int len = 0;
	
	char endpoint_c[10] = {0};/*endpoint*/
	int index = 0;
	int endpoint = 0;
	
	char data_buf[10] = {0};/*获取data1数据*/
	
	int data1_alarm = 0x00;/*报警声大小及开关*/
	int data2_light = 0x00;/*闪光开关*/
	int data3_alarm_level = 0x00;/*告警声强度*/
	
	int data4_alarm_time = 0x00;/*告警声持续时间*/
	int data5_light_time = 0x00;/*闪光灯占空比*/
	int data6_light_levle = 0x00;/*闪光灯光强度*/
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetWarningDeviceControl.\n");
	DEBUG_ZONE(LOG_ZONE_MY,"Topic:%s,dev_mac:%s,cmd:%d.\n",topic,dev_mac,cmd);
	
	/*获取commmad cmd type value*/	
	strncpy(cmd_buf,&cmd[4],2);
	
	cmd_type = strtol(cmd_buf,NULL,16);
	
	DEBUG_ZONE(LOG_ZONE_MY,"cmd_buf:0x%02x.\n",cmd_type);
	
	strncpy(len_c,&cmd[6],2);
	len = strtol(len_c,NULL,16);
	
	if(len > 0)
	{
		strncpy(endpoint_c,&cmd[8],2);
		index = strtol(endpoint_c,NULL,16);
		
		if(index!=0)
		{
			endpoint = HSZFindEndpointFromInfoBuffer(dev_mac,index);
			
			if(endpoint<1)
			{
				DEBUG_ZONE(LOG_ZONE_ERROR,"Warning Device not find endpoint.\n");
				HSZDeviceError(HSZ_ERR_FIND_ENDPOINT,"Warning Device not find endpoint",NULL);
				
				return rc;
			}
		}
		
		rc = 0x00;
	}
	else
	{
		return rc;
	}

	if((0x01 == cmd_type)&&(0x08 == len))
	{
		strncpy(data_buf,&cmd[10],2);
		data1_alarm = strtol(data_buf,NULL,16);
		memset(data_buf,0x00,10);
		
		strncpy(data_buf,&cmd[12],2);
		data2_light = strtol(data_buf,NULL,16);
		memset(data_buf,0x00,10);
		
		strncpy(data_buf,&cmd[14],2);
		data3_alarm_level = strtol(data_buf,NULL,16);
		memset(data_buf,0x00,10);
		
		DEBUG_ZONE(LOG_ZONE_MY,"data1_alarm:0x%02x,data2_light:0x%02x,data3_alarm_level:0x%02x.\n",\
		data1_alarm,data2_light,data3_alarm_level);
		
		strncpy(data_buf,&cmd[16],4);
		data4_alarm_time = strtol(data_buf,NULL,16);
		memset(data_buf,0x00,10);
		
		strncpy(data_buf,&cmd[20],2);
		data5_light_time = strtol(data_buf,NULL,16);
		memset(data_buf,0x00,10);
		
		strncpy(data_buf,&cmd[22],2);
		data6_light_levle = strtol(data_buf,NULL,16);
		
		DEBUG_ZONE(LOG_ZONE_MY,"data4_alarm_time:0x%02x,data5_light_time:0x%02x,data6_light_levle:0x%02x.\n",\
		data4_alarm_time,data5_light_time,data6_light_levle);
		
		/*
			zcl wd warn {00158D00018BF16E}1 1 0 3 1 0 0

			7个参数：
			"0:device mac",
			"1:2b endpoint",
			"2:4b,Warning Mode,0-7",
			"3:2b,Strobe,0,1",
			"4:2b,Strobe Level,0-3",
			"5:16b,Warning Duration(in seconds)",
			"6:8b,Strobe Duty Cycle,0-100",
			"7:8b,Strobe Level,0x00-0x03",
		*/
		// sprintf(msg,"{\"commands\":[{\"command\":\"zcl wd warn {%s} %d %d %d %d %d %d %d\"},{\"command\":\"plugin device-table send {%s} %d\"}]}",dev_mac,dev_endpoint);
		
		sprintf(msg,"{\"commands\":[{\"command\":\"zcl wd warn {%s} %d %d %d %d %d %d %d\"}]}",dev_mac,endpoint,data1_alarm,\
		data2_light,data3_alarm_level,data4_alarm_time,data5_light_time,data6_light_levle);
		
		rc = mosquitto_publish_topic(topic,msg);
		
		if(data1_alarm&&data2_light)
		{
			ResponsePutWarningDeviceControlStatus(HSZGatewayMac(),dev_mac,endpoint,0x01);//01是触发告警
		}
	}
	
	return rc;
}

/*****************************************************************************
 函 数 名  : ResponsePutWarningDeviceControlStatus
 功能描述  : 声光报警器控制
 输入参数  : char* gw_mac  :网关地址
             char* dev_mac ：设备MAC地址 
			 int dev_endpoint：设备Endpoint index
             int sensor_status：设备状态
  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年1月5日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/	
int ResponsePutWarningDeviceControlStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int sensor_status)
{

	int rc = -1;
	char response[30] = {0};
	char tmp[10] = {0};
	
	/* 构建data字符串*/
	sprintf(response,"%04x",WARNING_DEVICE);
	sprintf(tmp,"%02x",PUT_WARNING_DEVICE_STATUS);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",0x02);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_endpoint);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",sensor_status);
	strcat(response,tmp);
	
	if(0x01 == HSZIsNeedUpdateStatus(dev_mac,dev_endpoint,sensor_status))
	{
		if (hsbk_devicedata!=NULL)
		{
			rc = 0x00;
			(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}
	}
	
	return rc;
 
}
/*****************************************************************************
 函 数 名  : CommandSetWindowCoveringController
 功能描述  : 窗帘控制器控制处理
 输入参数  : char* topic       
             char* dev_mac     
             int dev_endpoint  
             int cmd_type      
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年1月6日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetWindowCoveringController(char* topic,char* dev_mac,int dev_endpoint,int cmd_type)
{
	/*
	
	{"commands":[{"command":"zcl window-covering up"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}
	{"commands":[{"command":"zcl window-covering down"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}
	{"commands":[{"command":"zcl window-covering stop"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}
	
	*/
	int rc = -1;
	char msg[128] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetWindowCoveringController-Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Dev_mac:%s,Dev_endpoint:%d,cmd_type:%d.\n",dev_mac,dev_endpoint,cmd_type);
	
	switch(cmd_type)
	{
		case WINDOW_COVER_CONTROL_UP:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"WINDOW_COVER_CONTROL_UP.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl window-covering up\"},{\"command\":\"plugin device-table send {%s} %d\"}]}",dev_mac,dev_endpoint);
		}
		break;
		
		case WINDOW_COVER_CONTROL_DOWN:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"WINDOW_COVER_CONTROL_DOWN.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl window-covering down\"},{\"command\":\"plugin device-table send {%s} %d\"}]}",dev_mac,dev_endpoint);
		}
		break;
		
		case WINDOW_COVER_CONTROL_STOP:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"WINDOW_COVER_CONTROL_STOP.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl window-covering stop\"},{\"command\":\"plugin device-table send {%s} %d\"}]}",dev_mac,dev_endpoint);
		}
		break;
		
		default:
		break;
	}
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	// ResponsePutWindowCoveringControllerStatus(HSZGatewayMac(),dev_mac,dev_endpoint,cmd_type);

	return rc;
}

/*****************************************************************************
 函 数 名  : ResponsePutWindowCoveringControllerStatus
 功能描述  : 声光报警器控制
 输入参数  : char* gw_mac  :网关地址
             char* dev_mac ：设备MAC地址 
			 int dev_endpoint：设备Endpoint index
             int sensor_status：设备状态
  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年1月5日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int ResponsePutWindowCoveringControllerStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int sensor_status)
{
	int rc = -1;
	char response[32] = {0};
	char tmp[16] = {0};
	
	/* 构建data字符串*/
	sprintf(response,"%04x",WINDOW_COVER_CONTROL);
	sprintf(tmp,"%02x",PUT_WINDOW_COVER_STATUS);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",0x02);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_endpoint);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",sensor_status);
	strcat(response,tmp);
	
	// HSZUpdateDeviceLineStatus(dev_mac,dev_endpoint,DEVICE_LINE_UPDATE);
	DEBUG_ZONE(LOG_ZONE_MY,"-----status:0x%02x.\n",sensor_status);
	
	if(0x01 == HSZIsNeedUpdateStatus(dev_mac,dev_endpoint,sensor_status))
	{
		if (hsbk_devicedata!=NULL)
		{
			rc = 0x00;
			
			(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Device:%s endpoint:%d,not need not update status.\n",dev_mac,dev_endpoint);
	}
	
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSendDataToSmartLock
 功能描述  : 发送智能门锁控制数据
 输入参数  : char* gw_mac       
             char* dev_mac     
             char* cmd_data      
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年5月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSendDataToSmartLock(char* gw_mac,char* dev_mac,char* cmd_data)
{
	int rc = -1;
	
	char topic[128] = {0};
	char msg[512] = {0};

	DEBUG_ZONE(LOG_ZONE_MY,"Dev_mac:%s,cmd_data:%s.\n",dev_mac,cmd_data);
		
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"hswrattrdata 1 0xfd00 0x0000 {%s} {%s}\"}]}",dev_mac,cmd_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Topic:%s.\n",topic);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Message:%s.\n",msg);
	
	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSendDataToSmartLockSL920
 功能描述  : 发送智能门锁控制数据
 输入参数  : char* gw_mac       
             char* dev_mac     
             char* cmd_data      
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年5月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSendDataToSmartLockSL920(char* gw_mac,char* dev_mac,char* cmd_data)
{
	int rc = -1;
	
	char topic[128] = {0};
	char msg[512] = {0};

	DEBUG_ZONE(LOG_ZONE_MY,"Dev_mac:%s,cmd_data:%s.\n",dev_mac,cmd_data);
		
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"hswrattrdata 1 0xfd00 0x0001 {%s} {%s}\"}]}",dev_mac,cmd_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Topic:%s.\n",topic);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Message:%s.\n",msg);
	
	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSendDataToSmartLockSL868
 功能描述  : 发送智能门锁控制数据
 输入参数  : char* gw_mac       
             char* dev_mac     
             char* cmd_data      
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年12月5日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSendDataToSmartLockSL868(char* gw_mac,char* dev_mac,char* cmd_data)
{
	int rc = -1;
	
	char topic[128] = {0};
	char msg[512] = {0};

	DEBUG_ZONE(LOG_ZONE_MY,"Dev_mac:%s,cmd_data:%s.\n",dev_mac,cmd_data);
		
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"hswrattrdata 1 0xfd00 0x0002 {%s} {%s}\"}]}",dev_mac,cmd_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Topic:%s.\n",topic);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Message:%s.\n",msg);
	
	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSendDataToSmartLock374
 功能描述  : 发送智能门锁控制数据
 输入参数  : char* gw_mac       
             char* dev_mac     
             char* cmd_data      
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年5月8日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSendDataToSmartLock374(char* gw_mac,char* dev_mac,char* cmd_data)
{
	int rc = -1;
	
	char topic[128] = {0};
	char msg[512] = {0};

	DEBUG_ZONE(LOG_ZONE_MY,"Dev_mac:%s,cmd_data:%s.\n",dev_mac,cmd_data);
		
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"hswrattrdata 1 0xfd00 0x0002 {%s} {%s}\"}]}",dev_mac,cmd_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Topic:%s.\n",topic);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Message:%s.\n",msg);
	
	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSendDataToSmartLockSL500
 功能描述  : 发送智能门锁控制数据
 输入参数  : char* gw_mac       
             char* dev_mac     
             char* cmd_data      
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年2月21日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSendDataToSmartLockSL500(char* gw_mac,char* dev_mac,char* cmd_data)
{
	int rc = -1;
	
	char topic[128] = {0};
	char msg[512] = {0};

	DEBUG_ZONE(LOG_ZONE_MY,"Dev_mac:%s,cmd_data:%s.\n",dev_mac,cmd_data);
		
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"hswrattrdata 1 0xfd00 0x0003 {%s} {%s}\"}]}",dev_mac,cmd_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Topic:%s.\n",topic);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Message:%s.\n",msg);
	
	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}
/*****************************************************************************
 函 数 名  : CommandSendDataToThreeInOne
 功能描述  : 发送智能门锁控制数据
 输入参数  : char* gw_mac       
             char* dev_mac     
             int cmd_data:0x01表示主动查询设备数据；0x02表示主动查询设备参数   
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年5月15日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSendDataToThreeInOne(char* gw_mac,char* dev_mac,int cmd_data)
{
	int rc = -1;
	
	char topic[128] = {0};
	char msg[256] = {0};
	
	DEBUG_ZONE(LOG_ZONE_MY,"Dev_mac:%s,cmd_data:%d.\n",dev_mac,cmd_data);
		
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	if(0x01 == cmd_data)
	{
		sprintf(msg,"{\"commands\":[{\"command\":\"hswrattrdata 1 0xfc04 0x0000 {%s} {01}\"}]}",dev_mac);
	}
	else if(0x02 == cmd_data)
	{
		sprintf(msg,"{\"commands\":[{\"command\":\"hswrattrdata 1 0xfc05 0x0000 {%s} {02}\"}]}",dev_mac);
	}
	else
	{
		return -1;
	}
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Three In One Topic:%s.\n",topic);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Three In One Message:%s.\n",msg);
	
	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}



/*****************************************************************************
 函 数 名  : CommandSendDataToRemortControl
 功能描述  : 发送清除紧急按钮状态的数据
 输入参数  : char* gw_mac       
             char* dev_mac     
             int cmd_data:0x00表示清除按钮的数据
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年5月15日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSendDataToRemortControl(char* gw_mac,char* dev_mac,int cmd_data)
{
	int tmp;
	
	for(tmp = 0;tmp < PERMIT_MAX_JOIN_DEV;tmp++)
	{
		if(0x00 == strcmp(dev_mac,DEV_INFO[tmp].dev_eui))
		{
			DEV_INFO[tmp].dev_status[0] = DEV_INFO[tmp].dev_status[0]&0x06;

			DEBUG_ZONE(LOG_ZONE_MY,"The <%s> device change status to normal.\n",DEV_INFO[tmp].dev_eui);

			return 0x00;
		}
	}
	
	return -1;
}


/*****************************************************************************
 函 数 名  : CommandSetSceneSwitch
 功能描述  : 场景面板 控制处理
 输入参数  : char* topic       
             char* dev_mac     
             int dev_endpoint  
             int cmd_type      
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年7月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetSceneSwitch(char* topic,char* dev_mac,int dev_endpoint,int cmd_type)
{
	/*
	
	{"commands":[{"command":"zcl on-off on"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}

	*/
	int rc = -1;
	char msg[128] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetSceneSwitch-Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,dev_endpoint:%d,cmd_type:%d.\n",dev_mac,dev_endpoint,cmd_type);
	
	switch(cmd_type)
	{
		case SET_SCENE_LIGHT_ON:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetSceneSwitch.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off on\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
			,dev_mac,dev_endpoint);
		}
		break;
		
		default:
		break;
	}
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);

	return rc;
}

/*****************************************************************************
 函 数 名  : ResponseSceneSwitchStatus
 功能描述  : 场景状态发生改变应答接口函数
 输入参数  : char* gw_mac      
             char* dev_mac     
             int dev_endpoint  
             int dev_status    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年7月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int ResponseSceneSwitchStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int dev_status)
{
	int rc = -1;
	char response[32] = {0};
	char tmp[16] = {0};
	
	/*构建data字符串*/
	sprintf(response,"%04x",SCENE_LIGHT);
	sprintf(tmp,"%02x",RESPONSE_SCENE_LIGHT);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",0x02);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_endpoint);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_status);
	strcat(response,tmp);
	
	strupr_linux(response);

	if (hsbk_devicedata!=NULL)
	{
		rc = 0x00;
		(*hsbk_devicedata)(gw_mac,dev_mac,response);
	}
	
	return rc;
}

/*****************************************************************************
 函 数 名  : CommandSetSmartPlugIn
 功能描述  : Smart Plug In 控制处理
 输入参数  : char* topic       
             char* dev_mac     
             int dev_endpoint  
             int cmd_type      
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年7月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetSmartPlugIn(char* topic,char* dev_mac,int dev_endpoint,int cmd_type)
{
	/*
	
	{"commands":[{"command":"zcl on-off on"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}
	{"commands":[{"command":"zcl on-off off"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}
	{"commands":[{"command":"zcl on-off toggle"},{"command":"plugin device-table send {000B57FFFE389B6E} 1"}]}
	
	*/
	int rc = -1;
	char msg[128] = {0};
	
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetSmartPlugIn-Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,dev_endpoint:%d,cmd_type:%d.\n",dev_mac,dev_endpoint,cmd_type);
	
	switch(cmd_type)
	{
		case SET_SMART_PLUG_IN_ON:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"SET_SMART_PLUG_IN_ON.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off on\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
			,dev_mac,dev_endpoint);
		}
		break;
		
		case SET_SMART_PLUG_IN_OFF:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"SET_SMART_PLUG_IN_OFF.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off off\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
			,dev_mac,dev_endpoint);
		}
		break;
		
		case SET_SMART_PLUG_IN_TOOGLE:
		{
			DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"SET_SMART_PLUG_IN_TOOGLE.\n");
			sprintf(msg,"{\"commands\":[{\"command\":\"zcl on-off toggle\"},{\"command\":\"plugin device-table send {%s} %d\"}]}"\
			,dev_mac,dev_endpoint);
		}
		break;
		
		default:
		break;
	}
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);

	return rc;
}

/*****************************************************************************
 函 数 名  : ResponseSmartPlugInStatus
 功能描述  : Smart Plug In状态发生改变应答接口函数
 输入参数  : char* gw_mac      
             char* dev_mac     
             int dev_endpoint  
             int dev_status    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年7月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int ResponseSmartPlugInStatus(char* gw_mac,char* dev_mac,int dev_endpoint,int dev_status)
{
	int rc = -1;
	char response[32] = {0};
	char tmp[16] = {0};
	
	/*构建data字符串*/
	sprintf(response,"%04x",SMART_PLUG_IN);
	sprintf(tmp,"%02x",RESPONSE_SMART_PLUG_IN_STATUS);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",0x02);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_endpoint);
	strcat(response,tmp);
	
	sprintf(tmp,"%02x",dev_status);
	strcat(response,tmp);
	
	if(0x01 == HSZIsNeedUpdateStatus(dev_mac,dev_endpoint,dev_status))
	{
		if (hsbk_devicedata!=NULL)
		{
			rc = 0x00;
			(*hsbk_devicedata)(gw_mac,dev_mac,response);
		}
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Device:%s endpoint:%d,not need update status.\n",dev_mac,dev_endpoint);
	}
	return rc;
}


/*****************************************************************************
 函 数 名  : CommandSetClothesHanger
 功能描述  : Smart Plug In 控制处理
 输入参数  : char* topic       
             char* dev_mac     
             int dev_endpoint  
             int cmd_type      
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年7月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetClothesHanger(char* gw_mac,char* dev_mac,char* cmd)
{
	int rc = -1;
	
	char msg[256] = {0};
	char topic[128] = {0};
	char cmd_data[128] = {0};
	
	char buf[16] = {0};   		/*command buf*/
	char tmp[16] = {0};
	int  check_sum = 0x00;
	char cmd_buf[16] = {0};		/*cmd*/
	int  cmd_type = 0;
	char len_c[32] = {0};		/*buf*/
	int len = 0;
	char endpoint_c[16] = {0};	/*endpoint*/
	int endpoint = 0;
	sprintf(topic,"gw/%s/commands",gw_mac);
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"CommandSetClothesHanger.\n");
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"dev_mac:%s,topic:%s,cmd:%s.\n",dev_mac,topic,cmd);
	
	
	strncpy(cmd_buf,&cmd[4],2);
	cmd_type = strtol(cmd_buf,NULL,16);
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Clothes Hanger command:0x%02x.\n",cmd_type);
	
	strncpy(len_c,&cmd[6],2);
	len = strtol(len_c,NULL,16);
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Clothes Hanger len:0x%02x.\n",len);
	
	strncpy(endpoint_c,&cmd[8],2);
	endpoint = strtol(endpoint_c,NULL,16);
	DEBUG_ZONE(LOG_ZONE_DEVICE_DATA,"Clothes Hanger endpoint:0x%02x.\n",endpoint);
	
	switch(cmd_type)
	{
		case 0x01:
		{
			if(0x05 == len)
			{
				strncpy(buf,&cmd[10],8);
				sprintf(cmd_data,"%02x",0xBB);  /* 帧头 */
				
				sprintf(tmp,"%02x",0xA3);		/* D0 */
				strcat(cmd_data,tmp);

				sprintf(tmp,"%02x",0x01);		/* D1 */
				strcat(cmd_data,tmp);
				
				strcat(cmd_data,buf);           /* D2~D5 */
				
				sprintf(tmp,"%04x",0x0000);     /* D6~D7 */
				strcat(cmd_data,tmp);
				
				DEBUG_ZONE(LOG_ZONE_MY,"cmd_data:%s.\n",cmd_data);
				
				check_sum = HSZCalculateClothesHangerSendDataCheckSum(cmd_data);
				
				sprintf(tmp,"%04x",check_sum);   /* D8~D9 */
				strcat(cmd_data,tmp);
				
				sprintf(tmp,"%02x",0x44);        /* D10 */
				strcat(cmd_data,tmp);
			}
			else
			{
				return rc;
			}
		}
		break;
		
		case 0x02:
		{
			if(0x01 == len)
			{
				sprintf(cmd_data,"%02x",0xBB);  /* 帧头 */
				sprintf(tmp,"%02x",0xA3);		/* D0 */
				strcat(cmd_data,tmp);
				
				sprintf(tmp,"%02x",0x02);		/* D1 */
				strcat(cmd_data,tmp);
				
				sprintf(tmp,"%04x",0x0000);     /* D2~D3 */
				strcat(cmd_data,tmp);
				
				sprintf(tmp,"%04x",0x0000);     /* D4~D5 */
				strcat(cmd_data,tmp);
				
				sprintf(tmp,"%04x",0x0000);     /* D6~D7 */
				strcat(cmd_data,tmp);
				
				DEBUG_ZONE(LOG_ZONE_MY,"cmd_data:%s.\n",cmd_data);
				check_sum = HSZCalculateClothesHangerSendDataCheckSum(cmd_data);
				sprintf(tmp,"%04x",check_sum);   /* D8~D9 */
				strcat(cmd_data,tmp);
				
				sprintf(tmp,"%02x",0x44);        /* D10 */
				strcat(cmd_data,tmp);
			}
			else
			{
				return rc;
			}
		}
		break;
		
		default:
		break;
	}
	strupr_linux(cmd_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Message:%s.\n",cmd_data);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"hswrattrdata 1 0x0012 0x0000 {%s} {%s}\"}]}",dev_mac,cmd_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Message:%s.\n",msg);
	
	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}


/*****************************************************************************
 函 数 名  : ResponseClothesHangerStatus
 功能描述  : 晾衣架状态发生改变应答接口函数
 输入参数  : char* gw_mac      
             char* dev_mac     
             int dev_endpoint  
             int dev_status    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年7月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int ResponseClothesHangerStatus(char* gw_mac,char* dev_mac,unsigned char* dev_status)
{
	int rc = -1;
	char response[128] = {0};
	char tmp[16] = {0};	
	int temp = 0;
	
#if 0	

	for(temp = 0x01;temp <dev_status[0];temp++)
	{
		DEBUG_ZONE(LOG_ZONE_MY,"0x%02x \n",dev_status[temp]);
	}
	
#endif 	

	if(0xBB == dev_status[1])
	{
		int  check_sum_cal = 0x00;
		int  get_cal_value = 0x00;
		
		check_sum_cal = HSZCalculateClothesHangerDataCheckSum(dev_status);/*计算收到的校验值*/
		
		// get_cal_value = dev_status[dev_status[0]-0x02]<<8|dev_status[dev_status[0]-0x01];
		get_cal_value = dev_status[10]<<8|dev_status[11];
		
		
		DEBUG_ZONE(LOG_ZONE_MY,"Clothes Hanger get calculate Checksum:0x%04x.\n",get_cal_value);
		
		if(check_sum_cal == get_cal_value)
		{
			if(0xA303 == dev_status[2]<<8|dev_status[3])
			{
				sprintf(response,"%04x",CLOTHES_HANGER);
				sprintf(tmp,"%02x",0x03);
				strcat(response,tmp);
				
				sprintf(tmp,"%02x",0x06);
				strcat(response,tmp);
				
				sprintf(tmp,"%02x",0x01);
				strcat(response,tmp);
				
				for(temp = 0x04;temp < 0x09;temp++)
				{
					sprintf(tmp,"%02x",dev_status[temp]);
					
					strcat(response,tmp);
				}
				
				strupr_linux(response);
				
				if (hsbk_devicedata!=NULL)
				{
					rc = 0x00;
					
					(*hsbk_devicedata)(gw_mac,dev_mac,response);
				}
				else
				{
					return rc;
				}
			}
			
		}
		else
		{
			DEBUG_ZONE(LOG_ZONE_MY,"Clothes Hanger get calculate Checksum error.\n");
			
			return rc;
		}
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Clothes Hanger get header error.\n");
		
		return rc;
	}

	return rc;
}

/*****************************************************************************
 函 数 名  : HSZCalculateClothesHangerDataCheckSum
 功能描述  : 计算传入数据的Check fcs数值
 输入参数  : 
			int len:字节长度
			char *data_buf:数组字节
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年3月29日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZCalculateClothesHangerDataCheckSum(unsigned char *data_buf)
{
	int index = 0;
	int sum = 0x00;
	

#if 0	

	for(index = 0x01;index <data_buf[0];index++)
	{
		DEBUG_ZONE(LOG_ZONE_MY,"0x%02x \n",data_buf[index]);
	}
	
#endif 	
	
	if(data_buf[1] != 0xbb)/*如果数据帧的头格式不符合*/
	{
		HSZDeviceError(HSZ_ERR_SMARTLOCK_MSG_TYPE,"Clothes Hanger data header wrong",NULL);
		
		return -1;
	}
	
	// for(index = 2;index <(data_buf[0]-0x02);index++)		/*data*/
	
	for(index = 2;index < 10;index++)		/*data*/
	{
		sum = sum + data_buf[index];
	}
	
	DEBUG_ZONE(LOG_ZONE_MY,"sum:0x%04x \n",sum);
	
	return sum;
}	


/*****************************************************************************
 函 数 名  : HSZCalculateClothesHangerSendDataCheckSum
 功能描述  : 计算发送数据的Check fcs数值
 输入参数  : 
			int len:字节长度
			char *data_buf:数组字节
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年11月12日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZCalculateClothesHangerSendDataCheckSum(char *data_buf)
{
	int index = 0;
	int sum = 0x00;
	
	char buf[16] = {0};		
	int  data = 0;
	
	for(index = 0;index < 9;index++)											
	{
		strncpy(buf,&data_buf[0+2*index],2);
		
	    data = strtol(buf,NULL,16);
		
	}
	
	
	strncpy(buf,&data_buf[0],2);	
	data = strtol(buf,NULL,16);
	
	if(data != 0xbb)/*如果数据帧的头格式不符合*/
	{
		HSZDeviceError(HSZ_ERR_SMARTLOCK_MSG_TYPE,"Clothes Hanger data header wrong",NULL);
		
		return -1;
	}
	
	for(index = 1;index < 9;index++)		/*data*/
	{
		strncpy(buf,&data_buf[0+2*index],2);
		
	    data = strtol(buf,NULL,16);
		
		sum = sum + data;
	}
	
	DEBUG_ZONE(LOG_ZONE_MY,"sum:0x%04x \n",sum);
	
	return sum;
}	

/*****************************************************************************
 函 数 名  : CommandSetZigBeeTo485
 功能描述  : zigbee to 485状态发生改变应答接口函数
 输入参数  : char* gw_mac      
             char* dev_mac     
             int dev_endpoint  
             int dev_status    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年7月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int CommandSetZigBeeTo485(char* gw_mac,char* dev_mac,char* cmd)
{
	int rc = -1;
	
	char msg[256] = {0};
	char topic[128] = {0};
	char cmd_data[128] = {0};
	
	char cmd_buf[16] = {0};		/*cmd*/
	int  cmd_type = 0;
	char len_c[32] = {0};		/*buf*/
	int len = 0;
	char endpoint_c[16] = {0};	/*endpoint*/
	int endpoint = 0;
	
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	DEBUG_ZONE(LOG_ZONE_MY,"CommandSetZigBeeTo485.\n");
	
	DEBUG_ZONE(LOG_ZONE_MY,"dev_mac:%s,topic:%s,cmd:%s.\n",dev_mac,topic,cmd);
	
	
	strncpy(cmd_buf,&cmd[4],2);
	cmd_type = strtol(cmd_buf,NULL,16);
	DEBUG_ZONE(LOG_ZONE_MY,"Zigbee to 485 command:0x%02x.\n",cmd_type);
	
	strncpy(len_c,&cmd[6],2);
	len = strtol(len_c,NULL,16);
	DEBUG_ZONE(LOG_ZONE_MY,"Zigbee to 485 len:0x%02x.\n",len);
	
	strncpy(endpoint_c,&cmd[8],2);
	endpoint = strtol(endpoint_c,NULL,16);
	DEBUG_ZONE(LOG_ZONE_MY,"Zigbee to 485 endpoint:0x%02x.\n",endpoint);

	
	strncpy(cmd_data,&cmd[10],(2*len-2));
	
	strupr_linux(cmd_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Zigbee to 485 Message:%s.\n",cmd_data);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"hswrattrdata 1 0xfc11 0x0000 {%s} {%s}\"}]}",dev_mac,cmd_data);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Zigbee to 485 Message:%s.\n",msg);
	
	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
}