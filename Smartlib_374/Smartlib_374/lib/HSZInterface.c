/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZInterface.c
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2017年6月12日
  最近修改   :
  功能描述   : 与上层SDK接口函数定义
  函数列表   :
  修改历史   :
  1.日    期   : 2017年6月12日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

  2.日    期   : 2017年6月23日
    作    者   : Zigbee Project Team
    修改内容   : 修改文件架构，将Common中的消磁发布部分移到此部分

******************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include "HSZInterface.h"
#include "cJSON.h"
#include "HSZCommon.h"
#include "HSZMessage.h"
#include "HSZDeviceData.h"

#include "HSZSelfProtocolLock.h"
#include "HSZSelfProtocolSL500Lock.h"
#include "HSZSmartLockMessage.h"

#if !defined(WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif


extern int mosquitto_publish_topic(const char *topic,const char *payload);

int ncp_on_ota_mode = NCP_OFF_OTA_MODE; 

int gateway_network_mode = GATEWAY_NETWORK_INIT_MODE;

int sub_topic = TYPE_TOPIC_ERR;
/*****************************************************************************
 函 数 名  : HSZSetCallbackOnCommand
 功能描述  : 设置回调通用处理函数
 输入参数  : 
			char* gw_mac:网关MAC地址
			int callbacktype：异步回调类型
			void* p：异步回调函数指针         
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年7月5日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZSetCallbackOnCommand(int callbacktype,void* p)
{
    int rc = 0;
	
	switch(callbacktype)
	{
		case CALLBACK_SUBSCRIBE_MSG:/*Error Callback */
		{
			hsbk_top = (HSZCallbackSubscribeMsg*)p;
		}
		break;
		
		case CALLBACK_ON_ERROR:
		{
			hsbk_onerror = (HSZCallbackOnError*)p;
		}
		break;
		
		case CALLBACK_GATEWAY_STATE:
		{
			hsbk_gatewaystate = (HSZCallbackGatewayState*)p;
		}
		break;
		
		case CALLBACK_DEVICE_JOIN:
		{
			hsbk_devicejoin = (HSZCallbackOnDeviceJoin*)p; 
		}
		break;
		
		case CALLBACK_DEVICE_LEAVE:
		{
			hsbk_deviceleave = (HSZCallbackOnDeviceLeave*)p;
		}
		break;
		
		case CALLBACK_DEVICE_LIST:
		{
			hsbk_getdevicelist = (HSZCallbackGetDeviceList*)p;
		}
		break;
		
		case CALLBACK_CMD_SEND:
		{
			hsbk_commandsend = (HSZCallbackOnCommandSend*)p;
		}
		break;
		
		case CALLBACK_DEVICE_LINE_STATE:
		{
			hsbk_devicelineon = (HSZCallbackOnDeviceLineState*)p;
		}
		break;

		case CALLBACK_GATEWAY_PERMIT_JOIN_STATE:
		{
			hsbk_gatewaypermitjoinstate =  (HSZCallbackGatewayPermitJoinState*)p;
		}
		break;

		case CALLBACK_COMMAND_EXECUTE:
		{
			hsbk_cmdexcuted =  (HSZCallbackCommandExecuted*)p;
		}
		break;

		case CALLBACK_DEV_SIGNAL_VALUE:
		{
			hsbk_devsignalvalue =  (HSZCallbackDevSignalValue*)p;
		}
		break;
		
		case CALLBACK_DEV_ONLINE_LIST:
		{
			hsbk_devonlinelist = (HSZCallbackReportOnlineDeviceList*)p;
		}
		break;
		
		case CALLBACK_DEV_DATA:
		{
			hsbk_devicedata = (HSZCallbackDeviceData*)p;
		}
		break;
		
		case CALLBACK_COMMAND_FAILED:
		{
			hsbk_commandfailed = (HSZCallbackCommandFailed*)p;
		}
		break;
			
		case CALLBACK_UPDATE_NCP:
		{
			hsbk_ncpupdateresult = (HSZCallbackNcpUpdateResult*)p;
		}
		break;
		
		default:
		{
			rc = -1;
		}
		break;
	}
	
	return rc;
}

/*****************************************************************************
 函 数 名  : HSZDeviceError
 功能描述  : 返回上层错误信息
 输入参数  : int errtype        
             const char* reason  
             const char*others  
 输出参数  : 无
 返 回 值  :  
 
 修改历史      :
  1.日    期   : 2017年6月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZDeviceError(int errtype,const char* reason,const char*others)
{
	DEBUG_ZONE(LOG_ZONE_ERROR,"--------------------ERROR--------------------\r\n");
	
	DEBUG_ZONE(LOG_ZONE_ERROR,"Errot Type %d \r\n",errtype);
	
	if (hsbk_onerror!=NULL)
	{
		(*hsbk_onerror)(HSZGatewayMac(),errtype);
	}
	
	if (reason)
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"Error reason %s.\r\n",reason);
	}
	
	if (others)
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"Error others %s.\r\n",others);
	}

	DEBUG_ZONE(LOG_ZONE_ERROR,"--------------------ERROR--------------------\r\n");
	
	return 0;
}


/*****************************************************************************
 函 数 名  : HSZRemoveOneDevice
 功能描述  : 删除设备
 输入参数  : 
			char* gw_mac： 网关MAC地址
			int nnodeid:设备nodeid
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年6月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZRemoveDevice(char* gw_mac,char* dev_mac)
{
	
	/*{"command":"zdo hsleave {00 0D 6F 00 0D 57 8B BB}"}
	  {"commands":[{"command":"zdo hsleave {00 0D 6F 00 0D 57 89 74}"}]}
	*/
	
	int rc = -1;
	char topic[100] = {0};
    char msg[128]= {0};
	char mac_str[30] = {0};
	// int nnodeid;
		
	DEBUG_ZONE(LOG_ZONE_TEST,"Remove One Device.");
	
	if (hsbk_deviceleave!=NULL)
	{
		hsbk_deviceleave(HSZGatewayMac(),dev_mac);
	}

    covert_eui64tostr(dev_mac,mac_str);

    sprintf(msg,"{\"commands\":[{\"command\":\"zdo hsleave %s\"}]}",mac_str);

	sprintf(topic,"gw/%s/commands",HSZGatewayMac());

	DEBUG_ZONE(LOG_ZONE_MY,"The Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_MY,"THe Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;	
}

/*****************************************************************************
 函 数 名  : HSZPermitPJoinGatewayOn
 功能描述  : 允许设备入网
 输入参数  : int seconds：允许入网时间
			 char* gw_mac： 网关MAC地址
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年6月13日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZPermitJoinOn(char* gw_mac,int seconds)
{
	int rc = -1;
	char topic[100] = {0};
	char msg[512]={0};
	char tmp[50] = {0};
	time_t join_on_time;
	
	time(&join_on_time);
	
	/*
	{"commands":[{"command":"network broad-pjoin 10","postDelayMs":100}]}
	*/
	
	if((seconds > 0xff)||(seconds < 0x00))
	{
		DEBUG_ZONE(LOG_ZONE_PUBLIC,"Error seconds");
		return rc;
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_PUBLIC,"Permit Pjoin network:%d s.\n.",seconds);
	}
	
#if 0
	
	if(GATEWAY_NETWORK_INIT_MODE == gateway_network_mode)
	{
		DEBUG_ZONE(LOG_ZONE_PUBLIC,"Has not get the network mode.\n");
		
		HSZGetDeviceList(HSZGatewayMac());
		
		usleep(500000);
	}
	
	if(GATEWAY_NETWORK_UP_MODE != gateway_network_mode)
	{
		DEBUG_ZONE(LOG_ZONE_PUBLIC,"The Network down,should Reform Network Auto.\n");
		
		HSZReformNetworkAuto(HSZGatewayMac());
		
		usleep(200000);
	}
#endif 
	
	DEBUG_ZONE(LOG_ZONE_TEST,"The gateway Mac is:%s.\n",gw_mac);
	
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":");
	
	sprintf(tmp,"\"network broad-pjoin %d",seconds);
	
	
	strcat(msg,tmp);
	
#if 0	
	sprintf(tmp,"\",\"postDelayMs\":100}]}");
	strcat(msg,tmp);
#else
	sprintf(tmp,"\"}]}");
	strcat(msg,tmp);
#endif
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"The Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"THe Message:%s.\n",msg);
	
	rc = mosquitto_publish_topic(topic,msg);

#if 0	

	if(0xFF!=seconds)
	{
		if (hsbk_gatewaypermitjoinstate!=NULL)
		{
			hsbk_gatewaypermitjoinstate(HSZGatewayMac(),0x01);
		}
		
		received_join_time = seconds;
		
		sum_join_time = 0;
	}
	else
	{
		if (hsbk_gatewaypermitjoinstate!=NULL)
		{
			hsbk_gatewaypermitjoinstate(HSZGatewayMac(),0x01);
		}
		
		received_join_time = 0;
		
		sum_join_time = 0;
	}

#endif
	
	return rc;
}

/*****************************************************************************
 函 数 名  : HSZPermitPJoinGatewayOff
 功能描述  : 关闭设备入网
 输入参数  : 
			char* gw_mac 网关MAC地址
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年6月13日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZPermitJoinOff(char* gw_mac)
{
	int rc = -1;
	char topic[100] = {0};
	char msg[512]={0};
	char tmp[50] = {0};
	
	/*
		{"commands":[{"command":"network broad-pjoin 0","postDelayMs":100}]}
	*/
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"Off permit to Pjoin.\n");
	
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":");
	
	sprintf(tmp,"\"network broad-pjoin 0");
	strcat(msg,tmp);
	
	sprintf(tmp,"\",\"postDelayMs\":100}]}");
	strcat(msg,tmp);
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"The Topic:%s",topic);
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"THe Message:%s",msg);
	
	rc = mosquitto_publish_topic(topic,msg);
	
	sum_join_time = 0x00;
	received_join_time = 0x00;
	
	if (hsbk_gatewaypermitjoinstate!=NULL)
	{
		hsbk_gatewaypermitjoinstate(HSZGatewayMac(),0x00);
	}

	return rc;
}

/*****************************************************************************
 函 数 名  : HSZReformNetwork
 功能描述  : Reform network
 输入参数  : 
			char* gw_mac: 网关MAC地址
			int channel  :广播信道
			int tx_power :发射功率
			int panid    :pan id
 输出参数  : 无
 返 回 值  : 
 函数说明  : 如果panid 为0，则采用network find unused 创建网络
             否则采用传入的参数，使用network form 创建网络
 
 修改历史      :
  1.日    期   : 2017年6月13日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

  2.日    期   : 2017年7月5日
    作    者   : Zigbee Project Team
    修改内容   : 去除自动创建网络接口

*****************************************************************************/
int HSZReformNetwork(char* gw_mac,int channel,int tx_power,int panid)
{
	/*
	{"commands":[
			{"command":"plugin device-table clear"},
			{"command":"plugin command-relay clear","postDelayMs":100},
			{"command":"plugin ias-zone-client clear-all","postDelayMs":100},
			{"command":"network leave","postDelayMs":1000},
			{"command":"network form 14 -22 0x6C16","postDelayMs":1000},
			{"command":"network broad-pjoin 0"}
			]}
	*/
	
	int rc = -1;
	char topic[100] = {0};
	char msg[1024] = {0};

	DEBUG_ZONE(LOG_ZONE_PUBLIC,"network form network.");
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"The Reform channel:%d,tx_power:%d,panid:%d",channel,tx_power,panid);
	
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"plugin device-table clear\"},\
			{\"command\":\"plugin command-relay clear\",\"postDelayMs\":100},\
			{\"command\":\"plugin ias-zone-client clear-all\",\"postDelayMs\":100},\
			{\"command\":\"network leave\",\"postDelayMs\":1000},\
			{\"command\":\"network form %d -%d 0x%x\",\"postDelayMs\":1000},\
			{\"command\":\"network broad-pjoin 0\"}]}",channel,tx_power,panid);

	DEBUG_ZONE(LOG_ZONE_PUBLIC,"The Topic:%s",topic);
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"THe Message:%s",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	HSZInitDeviceInfoBuffer();
	HSZInitJoinOnstatusInfoBuffer();
	// HSZInitLineOnDeviceInfoBuffer();
	return rc;
}

/*****************************************************************************
 函 数 名  : HSZReformNetworkAuto
 功能描述  : 自动建立网络，不需要设定参数
 输入参数  : 
			char* gw_mac： 网关MAC地址  
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年7月11日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZReformNetworkAuto(char* gw_mac)
{
	/*
		{"commands":[
		{"command":"plugin device-table clear"},
		{"command":"plugin command-relay clear","postDelayMs":100},
		{"command":"plugin ias-zone-client clear-all","postDelayMs":100},
		{"command":"network leave","postDelayMs":1000},
		{"command":"network find unused","postDelayMs":1000},
		{"command":"network broad-pjoin 0"}
		]}
	*/
	
	int rc = -1;
	char topic[100] = {0};
	char msg[1024] = {0};

	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"Form network auto.\r\n");
	
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"plugin device-table clear\"},\
			{\"command\":\"plugin command-relay clear\",\"postDelayMs\":100},\
			{\"command\":\"plugin ias-zone-client clear-all\",\"postDelayMs\":100},\
			{\"command\":\"network leave\",\"postDelayMs\":1000},\
			{\"command\":\"network find unused\",\"postDelayMs\":1000},\
			{\"command\":\"network broad-pjoin 0\"}]}"\
			);
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"The Topic:%s",topic);
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"THe Message:%s",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	HSZInitDeviceInfoBuffer();
	HSZInitJoinOnstatusInfoBuffer();
	// HSZInitLineOnDeviceInfoBuffer();
	
	return rc;
}
/*****************************************************************************
 函 数 名  : HSZGetDeviceList
 功能描述  : 获取设备列表
 输入参数  : 
			char* gw_mac： 网关MAC地址
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年6月20日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZGetDeviceList(char* gw_mac)
{
	int rc = -1;
	char topic[100] = {0};
	char msg[256] = {0};

	/*gw/0022A300000963E8/publishstate*/
	
	sprintf(topic,"gw/%s/publishstate",gw_mac);
	sprintf(msg,"{}");
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"Init the device Info buffer.\n");
	
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"Get Device List.\n");
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"The Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"THe Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);

	return rc;
}

/*****************************************************************************
 函 数 名  : HSZGetGatewayState
 功能描述  : 主动获取设备当前状态
 输入参数  : 
			char* gw_mac： 网关MAC地址
 输出参数  : 无
 返 回 值  :  
 
 修改历史      :
  1.日    期   : 2017年6月20日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZGetGatewayState(char* gw_mac)
{
	int rc = -1;
	char topic[100] = {0};
	char msg[256] = {0};

	/*gw/0022A300000963E8/publishstate*/
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"Get Device State.\n");
	
	sprintf(topic,"gw/%s/publishstate",gw_mac);
	sprintf(msg,"{}");
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"The Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"THe Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);

	return rc;
}
/*****************************************************************************
 函 数 名  : HSZGetGatewayNetworkInfo
 功能描述  : 主动获取当前网络的networkid、channel、信号强度等信息
 输入参数  :
			char* gw_mac： 网关MAC地址
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年6月20日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZGetGatewayNetworkInfo(char* gw_mac)
{
	int rc = -1;
	char topic[100] = {0};
	char msg[128] = {0};

	/*
		gw/0022A300000963E8/publishstate
	*/
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"Get Network Info.\n");
	
	sprintf(topic,"gw/%s/publishstate",gw_mac);
	sprintf(msg,"{}");
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"The Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"THe Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);

	return rc;
}

/*****************************************************************************
 函 数 名  : HSZSetGatewayFactory
 功能描述  : 设置网关工厂复位
 输入参数  : 
			char* gw_mac： 网关MAC地址
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年6月28日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

  2.日    期   : 2017年6月29日
    作    者   : Zigbee Project Team
    修改内容   : 修改恢复工厂复位模式为，自动创建网络，但是不允许入网

  3.日    期   : 2017年7月6日
    作    者   : Zigbee Project Team
    修改内容   : 根据讨论，修改Zigbee出厂状态为：默认无网

*****************************************************************************/
int HSZSetGatewayFactory(char* gw_mac)
{
	/*
	{"commands":[
			{"command":"plugin device-table clear"},
			{"command":"plugin command-relay clear","postDelayMs":100},
			{"command":"plugin ias-zone-client clear-all","postDelayMs":100},
			{"command":"network leave","postDelayMs":1000},
			{"command":"network find unused","postDelayMs":1000},
			{"command":"network broad-pjoin 0"}
			]}
	*/
	
	/*
	{"commands":[
			{"command":"plugin device-table clear"},
			{"command":"plugin command-relay clear","postDelayMs":100},
			{"command":"plugin ias-zone-client clear-all","postDelayMs":100},
			{"command":"network leave","postDelayMs":1000}
			]}
	*/
	
	int rc = -1;
	char topic[100] = {0};
	char msg[1024] = {0};
	
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"plugin device-table clear\"},\
			{\"command\":\"plugin command-relay clear\",\"postDelayMs\":100},\
			{\"command\":\"plugin ias-zone-client clear-all\",\"postDelayMs\":100},\
			{\"command\":\"network leave\",\"postDelayMs\":1000},\
			{\"command\":\"network find unused\",\"postDelayMs\":1000},\
			{\"command\":\"network broad-pjoin 0\"}]}"\
			);
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"Gateway Factory Reset.");
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"The Topic:%s.\n",topic);
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"THe Message:%s.\n",msg);

	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;
	
}

/*****************************************************************************
 函 数 名  : HSZGetNcpOtaStatus
 功能描述  : 获取NCP OTA状态
 输入参数  : char* gw_mac  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年8月9日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZGetNcpOtaStatus(char* gw_mac)
{
	return ncp_on_ota_mode;
}


/*****************************************************************************
 函 数 名  : HSZSetNCPOTAMode
 功能描述  : 设置NCP OTA模式
 输入参数  : 
			char* gw_mac： 网关MAC地址
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年6月28日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZSetNCPOTAMode(char* gw_mac,char * ncp_firmware_name)
{
	int rc = -1;
	
	char cmd[300] = {0};
	
	ncp_on_ota_mode = NCP_ON_OTA_MODE;
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"NCP OTA Mode On.\n");
	
	system("./lib/hagateway/Restart_HaGatewayReferenceHost");/*停止Host服务*/
	
    DEBUG_ZONE(LOG_ZONE_PUBLIC,"The ncp_firmware_name:%s.\n",ncp_firmware_name);
	sprintf(cmd, "./lib/hagateway/Update_ncp %.*s",strlen(ncp_firmware_name),ncp_firmware_name);
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"The cmd is:%s.\n",cmd);
	system(cmd);
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"NCP OTA Mode On.\n");
	
	ncp_on_ota_mode = NCP_OFF_OTA_MODE;
	
	system("./lib/hagateway/Kill_nodejs nodejs");
	system("./lib/hagateway/Restart_HaGatewayReferenceHost");
	system("./lib/hagateway/HaGatewayReferenceHost  -n 1 -p /dev/ttyACM0>>./hagateway/hagatewaylog.log ");
	
	return rc;
}


/*****************************************************************************
 函 数 名  : HSZRestartZigBeeService
 功能描述  : 重启Zigbee服务
 输入参数  : 
			char* gw_mac： 网关MAC地址
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年7月4日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZRestartZigBeeService (char* gw_mac)
{
	int rc = -1;
	FILE *fp;
	int count = 0;

	char buf[500];
	char command[500];
	
	sprintf(command,"ps|grep -v grep| grep -c HaGatewayReferenceHost");

	if((fp = popen(command,"r")) == NULL)
	{
		DEBUG_ZONE(LOG_ZONE_THREAD,"Exit with Error.\n");
	}
	
	if((fgets(buf,500,fp)) != NULL)
	{
		count = atoi(buf);
		printf("The count is %d.\n",count);
		if(count >0)
		{
			system("./hagateway/Restart_HaGatewayReferenceHost");
		}
	}
	
	return rc;
}

/*****************************************************************************
 函 数 名  : HSZGetZigbeeStackVersion
 功能描述  : 获取Zigbee协议栈版本信息
 输入参数  : 
			char* gw_mac： 网关MAC地址
 输出参数  : 无
 返 回 值  :  
 
 修改历史      :
  1.日    期   : 2017年7月4日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
char* HSZGetZigbeeStackVersion(char* gw_mac)
{

	if (0x00 != Zigbee_stack[0])
	{
		DEBUG_ZONE(LOG_ZONE_PUBLIC,"The zigbee stack version is:%s.\n",Zigbee_stack );
	}
	
	return  Zigbee_stack ;
}

/*****************************************************************************
 函 数 名  : HSZCommandsSend
 功能描述  : 通用控制命令:需要根据不同的传感器，解析传送指令，发送对应设备
 输入参数  :
			char* gw_mac：网关MAC地址
			char* device_id：设备ID
			char* cmd：控制指令    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年7月6日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZCommandsSend(char* gw_mac, char* device_mac,char* cmd)
{
	int rc = -1;
	char topic[100] = {0};
	
	char device_type_buf[10] = {0};
	char cmd_buf[10] = {0};
	int  device_type;
	int  cmd_type;
	char len_c[10] = {0};
	char endpoint_c[10] = {0};
	int len;
	int endpont;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZCommandsSend.\n");
	
	DEBUG_ZONE(LOG_ZONE_MY,"gw_mac:%s,device_mac:%s,cmd:%s\n",gw_mac,device_mac,cmd);
	
	if ((gw_mac == NULL) || (strlen(gw_mac)!=16))
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"MAC ADDR is NULL or the len of  mac is not right.\n");
		
		return rc; 
	}
	
	/*获取commmad device type value*/
	strncpy(device_type_buf,cmd,4);
	
	device_type = strtol(device_type_buf,NULL,16);

	DEBUG_ZONE(LOG_ZONE_TEST,"device_type:0x%04x.\n",device_type);
	

	/*获取commmad cmd type value*/	
	strncpy(cmd_buf,&cmd[4],2);
	
	cmd_type = strtol(cmd_buf,NULL,16);
	
	DEBUG_ZONE(LOG_ZONE_TEST,"cmd_buf:0x%02x.\n",cmd_type);
	
	/*获取MQTT Topic*/	
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	
	switch(device_type)
	{
			case ON_OFF_SWITCH:
			{
					DEBUG_ZONE(LOG_ZONE_TEST,"ON_OFF_SWITCH.\n");
					
					strncpy(len_c,&cmd[6],2);
					len = strtol(len_c,NULL,16);
					
					if(0x01 == len)
					{
							strncpy(endpoint_c,&cmd[8],2);
							endpont = strtol(endpoint_c,NULL,16);
							
							endpont = HSZFindEndpointFromInfoBuffer(device_mac,endpont);
							
							if(endpont>0)
							{
								CommandSetOnOffSwitch(topic,device_mac,endpont,cmd_type);
							}
							else
							{
								DEBUG_ZONE(LOG_ZONE_ERROR,"On Off switch:%s could not find endpoint.\n",device_mac);
								HSZDeviceError(HSZ_ERR_FIND_ENDPOINT,"On Off switch could not find endpoint",NULL);
							}
					}
			}  
			break;
			
			case LEVEL_CONTROL_SWITCH:
			{
					DEBUG_ZONE(LOG_ZONE_TEST,"LEVEL_CONTROL_SWITCH.\n");
					CommandSetLevelControlSwitch(topic,device_mac,cmd);
			}
			break;
			
			case DEVICE_TYPE_CONTACT_SENSOR:/*No cmd*/
			{
				
					DEBUG_ZONE(LOG_ZONE_PUBLIC,"LEVEL_CONTROL_SWITCH.\n");
			}
			break;
			
			case SMART_PLUG_IN:
			case ON_OFF_LIGHT:
			{
					DEBUG_ZONE(LOG_ZONE_TEST,"ON_OFF_LIGHT.\n");
					
					strncpy(len_c,&cmd[6],2);
					len = strtol(len_c,NULL,16);
					
					if(0x01 == len)
					{
							strncpy(endpoint_c,&cmd[8],2);
							endpont = strtol(endpoint_c,NULL,16);
							
							endpont = HSZFindEndpointFromInfoBuffer(device_mac,endpont);
							
							if(endpont > 0)
							{
								CommandSetOnOffLight(topic,device_mac,endpont,cmd_type);
							}
							else
							{
								DEBUG_ZONE(LOG_ZONE_ERROR,"On Off Light:%s could not find endpoint.\n",device_mac);
								HSZDeviceError(HSZ_ERR_FIND_ENDPOINT,"On Off Light could not find endpoint",NULL);
							}
					}
			}
			break;
			
			case WARNING_DEVICE:
			{
					DEBUG_ZONE(LOG_ZONE_TEST,"WARNING_DEVICE.\n");
					CommandSetWarningDeviceControl(topic,device_mac,cmd);
			}
			break;
			
			case WINDOW_COVER_CONTROL:
			{
					DEBUG_ZONE(LOG_ZONE_TEST,"WINDOW_COVER_CONTROL.\n");
					
					strncpy(len_c,&cmd[6],2);
					len = strtol(len_c,NULL,16);
					
					if(0x01 == len)
					{
						strncpy(endpoint_c,&cmd[8],2);
						endpont = strtol(endpoint_c,NULL,16);
						
						endpont = HSZFindEndpointFromInfoBuffer(device_mac,endpont);
						
						if(endpont>0)
						{
								DEBUG_ZONE(LOG_ZONE_MY,"WINDOW_COVER_CONTROL.\n");
								CommandSetWindowCoveringController(topic,device_mac,endpont,cmd_type);
						}
						else
						{
								DEBUG_ZONE(LOG_ZONE_ERROR,"Window Cover control:%s could not find endpoint.\n",device_mac);
								HSZDeviceError(HSZ_ERR_FIND_ENDPOINT,"Window Cover control could not find endpoint",NULL);
						}
					}
			}
			break;
			
			case SMART_LOCK_SL838:
			{
					DEBUG_ZONE(LOG_ZONE_TEST,"SMART_LOCK_SL838.\n");
					HSZSendSmartLockData(device_mac,cmd);
			}
			break;
			
			case THREE_IN_ONE:
			{
					DEBUG_ZONE(LOG_ZONE_TEST,"THREE_IN_ONE.\n");					
					CommandSendDataToThreeInOne(gw_mac,device_mac,cmd_type);
			}
			break;
			
			case REMORT_SMART_CONTROLER:
			{
	                DEBUG_ZONE(LOG_ZONE_TEST,"REMORT_SMART_CONTROLER.\n");                                  
					CommandSendDataToRemortControl(gw_mac,device_mac,0x00);
			}
			break;
			
	        case SMART_LOCK_SL920:
	        {
					DEBUG_ZONE(LOG_ZONE_TEST,"SMART_LOCK_SL920.\n");
					HSZSendSelfProtocolLockData(device_mac,cmd);
	        }
	        break;
			case SMART_LOCK_SL868:
			{
					DEBUG_ZONE(LOG_ZONE_TEST,"SL868 command data.\n");
					HSZSendSelfProtocolSL868LockData(device_mac,cmd);
			}
			break;
			
			case SMART_LOCK_SL500:
			{
					DEBUG_ZONE(LOG_ZONE_MY,"SL500 command data.\n");
					
					HSZSendSelfProtocolSL500LockData(device_mac,cmd);
			}
			break;

			case SMART_LOCK_374:
			{
					DEBUG_ZONE(LOG_ZONE_MY,"374 command data.\n");
					
					HSZSendSelfProtocol374LockData(device_mac,cmd);
			}
			break;
			
			case CLOTHES_HANGER:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"CLOTHES_HANGER.\n");							
				CommandSetClothesHanger(gw_mac,device_mac,cmd);
			}
			break;
			case ZIGBEE_TO_485:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"ZIGBEE_TO_485.\n");							
				CommandSetZigBeeTo485(gw_mac,device_mac,cmd);
			}
			break;
			default:
			break;
		
	}
	
	return rc;
}

/*****************************************************************************
 函 数 名  : HSZCommandsActiveReturn
 功能描述  : 通用数据段返回:需要根据不同的传感器，解析主动返回命令，打印测试
 输入参数  :
 			char* device_mac: 设备MAC
			char* cmd：模拟设备发送命令（"设备类型"+"CMD2"+"数据段内容"）    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年3月1日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
/****************************test return**********************************
int HSZCommandsActiveReturn(char* device_mac,char* cmd)									//cmd=100C310100040201
{
	int device_type = 0;
	char device_buf[8] = {0};
	char data_buf[64] = {0};
	
	
	strncpy(device_buf,cmd,4);
	printf("device_buf:%s \n",device_buf);
	device_type = strtol(device_buf,NULL,16);

	strncpy(data_buf,cmd+4,strlen(cmd)-4);												//data_buf=310100040201
	printf("data_buf=%s \n",data_buf);

	switch(device_type)
	{
			case SMART_LOCK_SL500:
			{
					//printf("Test SMART_LOCK_SL500 Active Return...");
					//HSZReturnSelfProtocolSL500LockData(device_mac,data_buf);
			}
			break;

			case SMART_LOCK_374:
			{
					printf("Test SMART_LOCK_374 Active Return...\n");
					HSZReturnSelfProtocol374LockData(device_mac,data_buf);
			}
			break;

			default:
			break;
	}

	return 0;
}
/*****************************test return**********************************/

/*****************************************************************************
 函 数 名  : HSZCommandsPassiveReturn
 功能描述  : 通用数据段返回:需要根据不同的传感器，解析被动返回命令，打印测试
 输入参数  :
 			char* device_mac: 设备MAC
			char* cmd：模拟设备发送命令（"设备类型"+"CMD2"+"数据段内容"）    
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年3月1日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
/*
int HSZCommandsPassiveReturn(char* device_mac,char* cmd)
{

	strncpy(device_buf,cmd,4);
	device_type = strtol(device_buf,NULL,16);

	switch{device_type}
	{
			case SMART_LOCK_SL500:
			{
					//被动返回函数
			}
			break;

			default:
			break;
	}
	
	return 0;
}
*/


/*****************************************************************************
 函 数 名  : HSZGetDevSignalValue
 功能描述  : 获取设备的发射信号强度
 输入参数  : 
			char* gw_mac：网关MAC地址
			char* device_mac：设备ID
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年7月6日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZGetDevSignalValue(char* gw_mac,char* device_mac)
{
	int rc = -1;
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"Get device signal value.\n");

	return rc;
}
/*****************************************************************************
 函 数 名  : HSZGetOnlineDeviceList
 功能描述  : 获取网关在线设备列表
 输入参数  : 
			char* gw_mac：网关MAC地址
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年7月6日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZGetOnlineDeviceList(char* gw_mac)
{
	int rc = -1;
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"HSZGetOnlineDeviceList.\n");
	
	return rc;
}

/*****************************************************************************
 函 数 名  : 
 功能描述  : 设置订阅Topic类型
 输入参数  : 
			char* gw_mac：网关MAC地址
			int topic_index：设置订阅Topic类型
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年10月25日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZSetSubTopic(char* gw_mac,int topic_index)
{
	sub_topic = topic_index;
	
	return sub_topic;
}
/*****************************************************************************
 函 数 名  : HSZFindDevice
 功能描述  : 发现设备
 输入参数  : 
			char* gw_mac：网关MAC地址
			char* device_mac：设备ID
			int time：调用后设备闪烁的秒数
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年7月6日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZFindDevice(char* gw_mac, char* device_mac, int time)
{
	int rc = -1;
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"HSZFindDevice.\n");
	
	return rc;
}

/*****************************************************************************
 函 数 名  : HSZRefreshDevice
 功能描述  : 刷新设备
 输入参数  : 
			char* gw_mac：网关MAC地址
			char* device_mac：设备ID
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2017年7月6日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZRefreshDevice(char* gw_mac, char* device_mac)
{
	int rc = -1;
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"HSZRefreshDevice.\n");
	
	return rc;
}

/*****************************************************************************
 函 数 名  : HSZGetNcpVersion
 功能描述  : 获取NCP的版本
 输入参数  : 
			char* gw_mac：网关MAC地址
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2018年3月13日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZGetNcpVersion(char* gw_mac)
{
	int rc = -1;
	char topic[100] = {0};
	char msg[512]={0};
	char tmp[50] = {0};
	
	/*
		{"commands":[{"command":"hsncpver"}]}
	*/
	
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":");
	
	sprintf(tmp,"\"hsncpver\"}]}");
	strcat(msg,tmp);
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"The Topic:%s",topic);
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"THe Message:%s",msg);
	
	rc = mosquitto_publish_topic(topic,msg);

	return rc;
}


/*****************************************************************************
 函 数 名  : HSZSetHuayiDeviceIcon
 功能描述  : 设置华羿开关图标
 输入参数  : 
            char* gw_mac:  网关MAC ID
			char* dev_mac：设备ID
			int index:设备 endpoint
			int icon_id   图标ID
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年1月13日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZSetHuayiDeviceIcon(char* gw_mac,char* dev_mac,int index,int icon_id)
{
	int rc = -1;
		
	char topic[128] = {0};
	
	char msg[512] = {0};
	
	char tmp[16] = {0};
		
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(tmp,"0x%04x",icon_id);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"hswrattrda_u16 %d 0x0000 0x4000 0x1037 {%s} %s\"}]}",index,dev_mac,tmp);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Topic:%s.\n",topic);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Message:%s.\n",msg);
	
	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;	
}

/*****************************************************************************
 函 数 名  : HSZGetNcpVersion
 功能描述  : 设置华羿开关名称
 输入参数  : 
            char* gw_mac:  网关MAC ID
			char* dev_mac：设备ID
			int index:设备 endpoint
		    char* switch_name 设置图标名称(unicode编码)；
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年1月13日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZSetHuayiDeviceName(char* gw_mac,char* dev_mac,int index,char* switch_name)
{
	int rc = -1;
	
	char topic[128] = {0};
	
	char msg[512] = {0};
		
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":\"hswrattrda_str %d 0x0000 0x4001 0x1037 {%s} {%s}\"}]}",index,dev_mac,switch_name);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Topic:%s.\n",topic);
	
	DEBUG_ZONE(LOG_ZONE_MY,"Send Lock Message:%s.\n",msg);
	
	rc = mosquitto_publish_topic(topic,msg);
	
	return rc;	

}

/*****************************************************************************
 函 数 名  : HSZGetNcpVersion
 功能描述  : 设置华羿开关名称
 输入参数  : 
            char* gw_mac:  网关MAC ID
			char* dev_mac：设备ID
			int seconds：允许加网时间（0xFF：永久加网，0x00：禁止加网）
 输出参数  : 无
 返 回 值  : 
 
 修改历史      :
  1.日    期   : 2019年1月13日
    作    者   : Zigbee Project Team
    修改内容   : 新生成函数

*****************************************************************************/
int HSZJoinDeviceByScanCode(char* gw_mac,char* dev_mac,int seconds)
{
	int rc = -1;
	
	char topic[128] = {0};
	
	char msg[512]= {0};
	
	char tmp[64] = {0};
	
	char mac_str[64] = {0};
	
	/*
	{"commands":[{"command":"network broadjoinmac 10 {00 15 8D 00 02 23 19 DC}"}]}
	*/
	
	if(seconds > 0xff)
	{
		DEBUG_ZONE(LOG_ZONE_PUBLIC,"Error seconds");
		
		return rc;
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_PUBLIC,"Permit Pjoin network:%d s.\n.",seconds);
	}
	
	
	DEBUG_ZONE(LOG_ZONE_TEST,"The gateway Mac is:%s.\n",gw_mac);
	
	sprintf(topic,"gw/%s/commands",gw_mac);
	
	sprintf(msg,"{\"commands\":[{\"command\":");
	
	memset(mac_str,0,64);
	
	covert_eui64tostr(dev_mac,mac_str);
	
	sprintf(tmp,"\"network broadjoinmac %d %s",seconds,mac_str);
	
	strcat(msg,tmp);

	sprintf(tmp,"\"}]}");
	
	strcat(msg,tmp);

	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"The Topic:%s.\n",topic);
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"THe Message:%s.\n",msg);
	
	rc = mosquitto_publish_topic(topic,msg);

	return rc;
}
