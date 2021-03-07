/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZMessage.h
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2017年6月12日
  最近修改   :
  功能描述   : 消息解析和消息封装函数定义接口

  修改历史   :
  1.日    期   : 2017年6月12日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

  2.日    期   : 2017年6月21日
    作    者   : Zigbee Project Team
    修改内容   : 增加了上层sdk控制接口函数定义
******************************************************************************/
#ifndef __HSZMESSAGE_H__
#define __HSZMESSAGE_H__

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "HSZInterface.h"



#define OWNERSHIP_ONE_ID   0xFE00
#define OWNERSHIP_TWO_ID   0xFE01

#define PERMIT_MAX_LINE_DEV         128			 /*能够存储的最多入网设备 */
#define LINE_ON_MAX_ENDPOINT        16			 /*能够允许的最大Endpoint*/
#define DEVICE_LINE_UPDATE          0x1E         /*设备上线并且状态更新成功*/
#define DEVICE_LINE_ON_INIT         0xFE         /*设备上线但是状态还未更新*/
#define DEVICE_LINE_OFF             0x36         /*设备离线状态*/


#define DEVICE_JOIN_ON_INIT         0x1A		 /*设备入网成功但是状态还未上报*/
#define DEVICE_JOIN_ON_FINISH       0x1B		 /*设备入网成功并且状态已经上报*/



/*保存上线设备MAC地址、主要Cluster ID以及是否完成了状态更新*/
struct line_on_dev_info
{
	char dev_eui[17];											/*入网设备eui*/
	
	int cluster_id;												/*表示要查询的主要cluster id */
	
	int endpoint_total;											/*表示要处理的端点总数 */
	
	int endpoint[LINE_ON_MAX_ENDPOINT];							/*最多处理16个端点*/
	
	int line_on_statue[LINE_ON_MAX_ENDPOINT];					/*是否完成了各路设备状态的更新*/
};


struct join_on_status_info
{
	char dev_eui[17];											/*入网设备eui*/
	
	int join_on_statue;											/*是否完成了入网设备状态的上报*/
};


extern struct line_on_dev_info LINE_ON_DEV_INFO[PERMIT_MAX_LINE_DEV];

extern struct join_on_status_info JOIN_ON_STAT_INFO[PERMIT_MAX_LINE_DEV]; 

/**处理Heatbeat消息*/
int HSZHostHeartBeatMsgHandle(cJSON *jsonroot);

/*处理设备列表消息*/
int HSZHandleDeviceList(cJSON * jsonroot);

/*处理设备入网消息*/
int	HSZHandleDeviceJoined(cJSON *jsonroot);

/*处理ZCL消息*/
int HSZHandleZCLResponse(cJSON *jsonroot);

/*处理设备离线消息*/
int HSZHandleDeviceLeft(cJSON *jsonroot);

/*处理设备状态改变消息*/
int HSZHandleDeveiceStateChange(cJSON *jsonroot);

/*处理Relays消息*/
int HSZHandleRelays(cJSON *jsonroot);

/*处理Setting消息*/
int HSZHandleSettings(cJSON *jsonroot);

/*处理Excuted消息*/
int HSZHandleExecuted(cJSON *jsonroot);

/*处理OTA消息*/
int HSZHandleOtaevent(cJSON *jsonroot);

/*处理APS消息*/
int HSZHandleAPSResponse(cJSON *jsonroot);

/*处理设备上线离线状态消息*/
int HSZHandleDeviceLineStateChange(cJSON *jsonroot);

/*处理ZDO消息*/
int HSZHandleZdoResponse(cJSON *jsonroot);

/*处理NOdeID更新消息*/
int HSZHandleNodeIdChanged(cJSON *jsonroot);

/*处理NCP重启的消息*/
int HSZHandleNcpReset(cJSON *jsonroot);

/*处理允许入网的消息*/
int HSZHandlePermitJoinState(cJSON *jsonroot);

/*解析NCP版本信息*/
int HSZHandleNcpVersion(cJSON *jsonroot);

/*处理Network状态发生改变的回调*/
int HSZHandleZigbeeNetworkState(cJSON *jsonroot);

/*除去网关地址前的0x*/
char* HSZRemoveMacAddr0X(char*mac);

/*读取入网设备的状态*/
int HSZReadLineDeviceAttribute(char* dev_mac,int epcount,int clusterid);

/*上线设备列表初始化*/
void HSZInitLineOnDeviceInfoBuffer(void);

/* 打印上线设备列表，用于调试信息*/
void HSZPrintLineOnDeviceStatusInfo(void);

/*查找上线设备对应的Cluster ID*/
int HSZFindClusteridFromInfoBuffer(char* dev_mac);

/*查找入网设备对应的EUI的Index*/
int HSZFindEuiFromLineOnInfoBuffer(char* dev_mac,int max_index);

/*将上线设备填入缓存buffer*/
int HSZSetLineOnInfoBuffer(char* dev_mac,int cluster_id,int end_point);

/*查找上线设备状态更新信息*/
int HSZUpdateLineOnDeviceInfoBuffer(int max_index);

/*更新入网设备状态是否已经上报信息*/
int HSZUpdateDeviceLineStatus(char* dev_mac,int end_point,int status);

/*入网设备状态列表初始化*/
void HSZInitJoinOnstatusInfoBuffer(void);

/* 打印入网设备设备列表，用于调试信息*/
void HSZPrintJoinOnstatusInfo(void);

/*将入网设备信息填入缓存buffer*/
int HSZSetJoinOnstatusInfo(char* dev_mac);

/*更新入网设备状态是否已经上报信息*/
int HSZUpdateDeviceJoinStatus(char* dev_mac,int status);

/*获取对应MAC地址设备的入网状态上报状态*/
int HSZFindJoinDevStatusInfo(char* dev_mac);

/*查找EUI对应的入网状态*/
int HSZFindEuiFromJoinOnInfoBuffer(char* dev_mac,int max_index);

/*冒泡法排序数组(由小到大排序)*/
void HSZSort(int array[],int len);


#endif
