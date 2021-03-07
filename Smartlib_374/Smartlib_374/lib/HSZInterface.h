/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZInterface.h
  版 本 号   : 初稿
  作作  者   : Zigbee Project Team
  生成日期   : 2017年6月12日
  最近修改   :
  功能描述   : 与上层SDK接口函数声明
  函数列表   :
  修改历史   :
  1.日    期   : 2017年6月12日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件
	
	命名规则
		1.回调函数 :HSZCallbak_****		
		2.接口函数	:HSZ_****		
		3.枚举	:HSZN_***	

	其它规则：		
		1.mac地址，全部使用大写，不带0X前缀，必须16个字符串长度	

  2.日    期   : 2017年6月23日
    作    者   : Zigbee Project Team
    修改内容   : 增加了回调函数和控制接口

  3.日    期   : 2017年7月11日
    作    者   : Zigbee Project Team
    修改内容   : 根据会议内容完善修改
		
******************************************************************************/
#ifndef __HSZINTERFACE_H__
#define __HSZINTERFACE_H__

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>




/*******************************************************************
*
*变量声明
*
********************************************************************/

	/* 网关上层回调种类 */
	#define APP_CALLBACK_SUBSCRIBE_MSG  		    1     	
	#define APP_CALLBACK_ON_ERROR     	            2	 	/* 错误信息Callback     		*/
	#define APP_CALLBACK_GATEWAY_STATE    	        3	 	/* 网关状态Callback     		*/

	#define APP_CALLBACK_DEVICE_JOIN      			4   	/* 设备入网Callback     		*/
	#define APP_CALLBACK_DEVICE_LEAVE				5   	/* 设备退网Callback     		*/
	#define APP_CALLBACK_DEVICE_LIST                6   	/* 设备列表Callback     		*/
	#define APP_CALLBACK_CMD_SEND                	7   	/* 命令发送Callback     		*/
	#define APP_CALLBACK_DEVICE_LINE_STATE          8   	/* 设备上线离线状态Callback     */

	#define APP_CALLBACK_GATEWAY_PERMIT_JOIN_STATE  9		/* 网关是否允许入网Callback     */

	#define APP_CALLBACK_COMMAND_EXECUTE            10		/* 命令执行Callback     		*/
	#define APP_CALLBACK_DEV_SIGNAL_VALUE           11		/* 获取设备信号强度Callback		*/
	#define APP_CALLBACK_DEV_ONLINE_LIST            12      /* 在线设备列表Callback			*/
	#define APP_CALLBACK_DEV_DATA                   13      /* 上报设备数据Callback 		*/
	#define APP_CALLBACK_COMMAND_FAILED             14      /* 上层控制命令执行失败Callback */
	#define APP_CALLBACK_UPDATE_NCP                 15      /*  NCP升级结果反馈             */


	/* 订阅消息种类 */
	#define	APP_TYPE_TOPIC_ERR                      0       /* 订阅如果不设定的话默认为此类型 */
	#define APP_TYPE_TOPIC_HEARTBEAT                1     	/* HEARTBEAT类型 */
	#define	APP_TYPE_TOPIC_DEVICES                  2		/* DEVICES错误的类型 */
	#define	APP_TYPE_TOPIC_ZCLRESPONSE              3		/* ZCLRESPONSE错误的类型 */
	#define APP_TYPE_TOPIC_DEVICESJOINED            4		/* DEVICESJOINED错误的类型 */
	#define	APP_TYPE_TOPIC_DEVICELEFT               5		/* DEVICELEFT错误的类型 */
	#define	APP_TYPE_TOPIC_DEVEICE_STATE_CHANGE     6		/* DEVEICE_STATE_CHANGE错误的类型 */
	#define	APP_TYPE_TOPIC_RELAYS                   7		/* RELAYS错误的类型 */
	#define	APP_TYPE_TOPIC_SETTINGS                 8		/* SETTINGS错误的类型 */
	#define	APP_TYPE_TOPIC_EXECUTED                 9		/* EXECUTED错误的类型 */
	#define	APP_TYPE_TOPIC_OTAEVENT                 10		/* OTAEVENT错误的类型 */
	#define	APP_TYPE_TOPIC_APSRESPONSE              11		/* APSRESPONSE错误的类型 */
	#define	APP_TYPE_TOPIC_DEVLINESTCH              12		/* DEVLINESTCH错误的类型 */
	
	/* HSZReformNetwork设定发射*/
    #define GATEWAY_MAX_POWER           			20
	#define GATEWAY_MID_POWER           			14
	#define GATEWAY_MIN_POWER           			8

/*******************************************************************
*
*初始化函数接口
*
********************************************************************/
	
	/* 
        功能描述  :系统初始化函数，在调用之前必须设置回调函数
        输入参数  : void  
        输出参数  : 无
        返 回 值  : int型 成功返回0，否则返回-1
	*/
	int HSZInit(void);
	
	/* 
        功能描述  :系统正常退出，断开与MQTT Broker连接
        输入参数  : void  
        输出参数  : 无
        返 回 值  : int型 成功返回0，否则返回-1
	*/
	int HSZDeinit(void);

	
/*******************************************************************
*
*上层函数接口定义
*
********************************************************************/
   
    /* 
        功能描述: 发生错误回调定义
        输入参数  : 
					char* gw_mac:网关MAC地址
					int errtype :错误种类                         
        输出参数  : 无
        返 回 值  : 无
    */
	typedef void HSZCallbackOnError(char* gw_mac,int errtype);

	/*
        功能描述: 网关设备工作状态
        输入参数  : 
					char* gateway_mac:网关MAC地址
					int gateway_state:0表示网络NG，1表示网络OK，-1表示系统NG
					                  99 表示NCP重启成功，98表示NCP重启失败
        输出参数  : 无
        返 回 值  : 无
    */
    typedef void HSZCallbackGatewayState(char* gw_mac,int gw_state);
	
	/*
	    功能描述: 有新设备入网的回调
        输入参数  : 
					char* gw_mac:网关MAC地址				               
                    char* dev_type：设备类型
                    char* dev_mac：设备MAC地址
                    int dev_endpoint：设备端点号总数 
        输出参数  : 无
        返 回 值  : 无
	*/
	typedef void HSZCallbackOnDeviceJoin(char* gw_mac,char* dev_type,char* dev_mac,int endpoint_count);

    /*
        功能描述 : 设备退网回调，可能设备主动退网
        输入参数  : 
					char* gw_mac:网关MAC地址 
                    char* dev_mac ：设备MAC地址
        输出参数  : 无
        返 回 值  : 无
    */
	typedef void HSZCallbackOnDeviceLeave(char* gw_mac,char* dev_mac);


	 /*
        功能描述 : 设备上线离线状态回调
        输入参数  : 
		            char* gw_mac:网关MAC地址
					char* dev_mac: 设备nodeid   
					int dev_state：设备数据,0x01代表设备上线，0x00代表设备离线
        输出参数  : 无
        返 回 值  : 无
    */
	typedef void HSZCallbackOnDeviceLineState(char* gw_mac,char* device_mac,int dev_state);
	
	/*
		功能描述 : 网关停止设备入网回调
		输入参数  : 
					char* gw_mac:网关MAC地址
					permit_state: 0x00,停止允许入网，0x01允许入网
		输出参数  : 无

		返 回 值  : 无
	*/
	typedef void HSZCallbackGatewayPermitJoinState(char* gw_mac,int permit_state);

	/*
		功能描述 : 网关中的设备有数据上报时回调该函数
		输入参数  : 
					char* gw_mac:网关MAC地址
					char* device_mac:设备mac地址
					char* data:设备数据
		输出参数  : 无

		返 回 值  : 无
	*/
	typedef void HSZCallbackDeviceData(char* gw_mac,char* device_mac,char* data);
	
	/*
		功能描述 : 网关命令执行出错上报回调
		输入参数  : 
		char* gw_mac:网关MAC地址
		char* device_mac:设备mac地址
		char* data:设备数据
		输出参数  : 无

		返 回 值  : 无
	*/
	typedef void HSZCallbackCommandFailed(char* gw_mac,char* device_mac,int data);
	
	/*
		功能描述 : NCP升级结果反馈
		输入参数  : 
		char* gw_mac:网关MAC地址
		char* data:设备数据
		输出参数  : 无

		返 回 值  : 反馈0x66代表升级成功，反馈0x11表示升级失败；
	*/
	typedef void HSZCallbackNcpUpdateResult(char* gw_mac,int data);
/*******************************************************************
*
*上层回调函数设置
*
********************************************************************/
	
	/*
	    功能描述  : 设置获取命令执行的回调函数
        输入参数  : 
					int callbacktype：异步回调类型，参考网关上层回调种类
					void* p：异步回调函数指针
        输出参数  : 无
        返 回 值  : int型，正常0
	*/
	
	int HSZSetCallbackOnCommand(int callbacktype,void* p);
	
/*******************************************************************
*
*控制函数接口
*
********************************************************************/

    /*
        功能描述  : 允许设备入网
        输入参数  : 
					char* gw_mac: 网关MAC地址
					int seconds:允许入网的秒数,最长为254s，如果大于此值，选择设置该值为0xFF，为永久允许入网
					同时由上层发送HSZPermitJoinOff控制停止入网
        输出参数  : 无
        返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZPermitJoinOn(char* gw_mac,int seconds);

    
    /*
        功能描述  : 禁止设备入网
        输入参数  : 
					char* gw_mac :网关MAC地址
        输出参数  : 无
        返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZPermitJoinOff(char* gw_mac);

    /*
         功能描述  : 重新建立网络
         输入参数  : 
					char* gw_mac: 网关MAC地址
					int channel:广播频道，需推荐 
					int tx_power: TX发射功率(需要设定推荐值)
					int panid: PID（0x0001-0x7FFF）
         输出参数  : 无
         返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZReformNetwork(char* gw_mac,int channel,int tx_power,int panid);

	 /*
         功能描述  : 自动重新建立网络（无需输入参数）
         输入参数  : 
					char* gw_mac 网关MAC地址
         输出参数  : 无
         返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZReformNetworkAuto(char* gw_mac);
	
	/*
         功能描述  : 主动获取网关状态
         输入参数  : 
					char* gw_mac： 网关MAC地址
         输出参数  : 无
         返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZGetGatewayState(char* gw_mac);
	 
    /*
         功能描述  :主动获取当前网络的channel信号强度等信息
         输入参数  : 
					 char* gw_mac： 网关MAC地址
         输出参数  : 无
         返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZGetGatewayNetworkInfo(char* gw_mac);
    
    /*
         功能描述  : 删除设备
         输入参数  : 
					char* gw_mac： 网关MAC地址
					char* dev_mac: 删除设备的MAC地址
         输出参数  : 无
         返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZRemoveDevice(char* gw_mac,char* dev_mac);

    
    /*
         功能描述  : 恢复出厂设置
         输入参数  : 
					char* gw_mac： 网关MAC地址
         输出参数  : 无
         返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZSetGatewayFactory(char* gw_mac);

	
    /*
         功能描述  : OTA模式设置
         输入参数  : 
					char* gw_mac： 网关MAC地址
					char* ncp_firmware_name 网关NCP软件名称
         输出参数  : 无
         返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZSetNCPOTAMode(char* gw_mac,char * ncp_firmware_name);
	
	
	/* 
		功能描述  :获取网关Zigbee协议栈版本（包括NCP端与Host端，并且两边Zigbee协议栈要匹配）
		输入参数  : 
				   char* gw_mac： 网关MAC地址
        输出参数  : 无
        返 回 值  : char字符串型，返回网关Zigbee协议栈版本
	*/
	char* HSZGetZigbeeStackVersion(char* gw_mac);
	
	/* 
		功能描述  :发送通用控制指令
		输入参数  :
					char* gw_mac：网关MAC地址
					char* dev_mac：设备ID
					char* cmd：控制指令
        输出参数  : 无
        返 回 值  : int型，正常非-1，错误值返回-1
	*/
	int HSZCommandsSend(char* gw_mac,char* dev_mac,char* cmd);
	
	/* 
		功能描述  :验证通用上报指令
		输入参数  :
					char* gw_mac：网关MAC地址
					char* dev_mac：设备ID
					char* cmd：控制指令
        输出参数  : 无
        返 回 值  : int型，正常非-1，错误值返回-1
	*/

	int HSZCommandsActiveReturn(char* device_mac,char* cmd);
	
	/* 
		功能描述  :获取设备的发射功率值,异步回调上报
		输入参数  : 
					char* gw_mac：网关MAC地址
					char* dev_mac：设备ID
        输出参数  : 无
        返 回 值  :
	*/
	int HSZGetDevSignalValue(char* gw_mac,char* dev_mac);
	
    /* 
        功能描述  :获取网关NCP OTA状态
        输入参数  :
            		char* gw_mac：网关MAC地址,通过回调返回OTA状态
        输出参数  : 无
        返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZGetNcpOtaStatus(char* gw_mac);
	
	/* 
        功能描述  :获取网关NCP版本
        输入参数  :
            		char* gw_mac：网关MAC地址,通过回调返回NCP版本
        输出参数  : 无
        返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZGetNcpVersion(char* gw_mac);
	
	/* 
        功能描述  :设置华羿开关图标
        输入参数  :
					char* gw_mac:  网关MAC ID
            		char* dev_mac：设备ID
					int index:设备 endpoint
					int icon_id    图标ID
					
        输出参数  : 无
        返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZSetHuayiDeviceIcon(char* gw_mac,char* dev_mac,int index,int icon_id);
	
	
	/* 
        功能描述  :设置华羿开关名称
        输入参数  :
					char* gw_mac： 网关MAC ID
            		char* dev_mac：设备ID
					int index:设备 endpoint
					char* switch_name 设置图标名称；
        输出参数  : 无
        返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZSetHuayiDeviceName(char* gw_mac,char* dev_mac,int index,char* switch_name);
	
	
	/* 
        功能描述  :设置华羿开关名称
        输入参数  :
					char* gw_mac： 网关MAC ID
            		char* dev_mac：设备ID
					int seconds：允许加网时间
					
        返 回 值  : int型，正常非-1，错误值返回-1
    */
	int HSZJoinDeviceByScanCode(char* gw_mac,char* dev_mac,int seconds);
	
	
	
	
	
#endif	
