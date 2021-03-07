/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZSmartLockMessage.h
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2017年12月13日
  最近修改   :
  功能描述   : 处理私有智能门锁协议的 Data数据
  函数列表   :
  修改历史   :
  1.日    期   : 2018年3月29日 2019年03月05日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

******************************************************************************/
#ifndef _HSZSmartLockMessage_H_
#define _HSZSmartLockMessage_H_

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define PACKAGE_HEADER 0xA1
#define PRIVATE_DATA_LEN 1024

#define RESERVED_BYTE 0xff
#define PROTOCOL_VERSION 0x14
#define SMARTLOCK_DEVICETYPE 0x20

#define SMARTLOCK_COMMAND "000A"

/*lock->zigbee*/
#define SMARTLOCK_GET_TIME "ffffffff"
#define SMARTLOCK_ADD_TEMCODE_FAIL "02"
#define SMARTLOCK_ADD_TEMCODE_REPEAT "01"
#define SMARTLOCK_DELETE_TEMCODE_SUCCESS "00"
#define SMARTLOCK_DELETE_TEMCODE_FAIL "01"
#define SMARTLOCK_ADMIN_AUTH_SUCCESS "00"
#define SMARTLOCK_ADMIN_PASSWORD_ERROR "01"
#define SMARTLOCK_ADMIN_AUTH_FAIL "02"
#define SMARTLOCK_SYNC_TIME_FAIL 01
#define SMARTLOCK_OPEN_DOOR_FAIL 02
#define SMARTLOCK_OPEN_DOOR_PASSWORD_ERROR 03
#define SMARTLOCK_FORBID_REMOTE_OPEN_DOOR 05
#define SMARTLOCK_LOCK   		"010a0100"
#define SMARTLOCK_RESERT 		"0109020001"
#define SMARTLOCK_BATTERY_LOW 	"0109020003"
#define SMARTLOCK_PRY           "0109020005"
#define SMARTLOCK_HIJACK        "0109020009"
#define SMARTLOCK_PASSWORD_ALARM    "010902000f"
#define SMARTLOCK_FINGPRINT_ALARM    "0109020010"
#define SMARTLOCK_CARD_ALARM    "0109020011"
#define SMARTLOCK_DOUBLE_LOCK   "010a0103"
#define SMARTLOCK_OPEN_DOOR     "0a0102"
#define SMARTLOCK_NO_TEMP_USER  "ff"


/*zigbee->smartgateway*/
#define REQUST_TIME "0502"
#define ADD_TEMCODE_FAIL "0A03"
#define ADD_TEMCODE_REPEAT "0A12"
#define DELETE_TEMCODE_FAIL "0A04"
#define DELETE_TEMCODE_SUCCESS "080F"
#define ADMIN_AUTH_SUCCESS  "0105"
#define ADMIN_AUTH_FAIL "0A00"
#define ADMIN_AUTH_PASSWORD_ERROR "0A11"
#define SYNC_TIME_FAIL "0A02"
#define OPEN_DOOR_FAIL "0A01"
#define OPEN_DOOR_PASSWORD_ERROR "0A10"
#define FORBID_REMOTE_OPEN_DOOR "0301"
#define DOOR_LOCK "0102"
#define LOCK_RESERT "020C"
#define LOCK_BATTERY_LOW "021C"
#define LOCK_PRY "021D"
#define LOCK_HIJACK "0809"
#define LOCK_PASSWORD_ALARM "021F"
#define LOCK_FINGPRINT_ALARM "0220"
#define LOCK_CARD_ALARM "0221"
#define LOCK_DOUBLE_LOCK "020A"
#define LOCK_OPEN_DOOR "0808"
#define LOCK_TIME "080A"
#define ADD_TEMCODE_SUCCESS "080E01"
#define LOCK_NO_TEMP_USER "08070101EE"
#define QUERY_TEM_USER_SUCCESS "0807"

/**********************************RSY********************************/
#define SL500_MOD_OPEN_MODE "0100"
#define SL500_MOD_CLOSE_MODE "0101"
#define SL500_USER_PASSWD_AUTH "0103"



#define TEM_USER 1
#define NORMAL_USER 2
#define ADMIN_USER 3

/**********************************RSY********************************/
#define ADMIN_LOCK_USER 00
#define NORMAL_LOCK_USER 01
#define TEM_PASSWD_USER 02
#define TEM_CARD_USER 06
/**********************************RSY********************************/


#define QUERY_ADMIN_USER_TYPE "00"
#define QUERY_NORMAL_USER_TYPE "01"
#define QUERY_TEMP_USER_TYPE "02"
#define QUERY_TEMP_CARD_USER_TYPE "06"


enum DATA_CMD2
{	
		REGULAR_DATA_REPORT = 0x32,									/*固定数据上报 锁-ZigBee*/
		TIME_DATA_REPORT = 0x51,									/*时间同步 锁-ZigBee*/
		PRIVATE_DATA_REPORT = 0x55,									/*自定义数据上报 锁-ZigBee*/
		CONTROL_TIME = 0x52,										/*时间同步 ZigBee-锁*/
		CONTROL_OPENDOOR = 0x53,									/*远程开锁 ZigBee-锁*/
		CONTROL_REGULAR = 0x54,										/*自定义功能 ZigBee-锁*/
};

enum ATTRIBUTE_ID
{
		ADD_TMPPASSWORD = 0x00,										/*添加临时密码操作返回*/
		DELETE_TMPPASSWORD = 0x01,									/*删除临时密码操作返回*/
		QUERY_USER = 0x03,											/*查询用户信息返回*/
		ADMIN_AUTH = 0x04,											/*管理员密码认证操作返回*/
		RESPONSE_CONTROL = 0x10,									/*门锁控制信息反馈指示*/
					
};

/*smartgateway->zigbee*/
enum SMARTLOOK_COMMMAND_ID
{
		SMARTLOCK_TIME_SYNCHRONIZATION_COMMAND = 0x12,				/*时钟同步*/
		SMARTLOCK_PASSWD_AUTHEN_COMMAND = 0x15,						/*管理员密码认证*/	
		SMARTLOCK_QUERY_TEMUSER_INFO_COMMAND = 0x33,				/*查询锁具临时用户信息*/
		SMARTLOCK_ADD_NEW_TEMUSER_COMMAND = 0x21,					/*添加新用户*/	
		APP_DELETE_TEMUSER_COMMAND = 0x22,							/*APP删除用户*/	
		APP_REMOTR_OPEN_DOOR_COMMAND = 0x11,						/*APP开锁*/
		APP_QUERY_SMARTLOCK_TIME = 0x35,							/*APP查询锁具时间*/
	    SMARTLOCK_QUERY_ADMINUSER_INFO_COMMAND = 0x31,          	/*查询管理员用户信息*/
	    SMARTLOCK_QUERY_NORMALUSER_INFO_COMMAND = 0x32,        	 	/*鏌ヨ閿佸叿鏅?氱敤鎴蜂俊鎭?*/
	    SMARTLOCK_QUERY_LOCK_INFO_COMMAND = 0x34,               	/*查询锁具信息*/
	    SMARTLOCK_QUERY_OPENDOOR_HISTORY_COMMAND = 0x36,        	/*鏌ヨ寮?閿佸巻鍙蹭俊鎭?*/
	    SMARTLOCK_ADMIN_USER = 0x00,            					/*绠＄悊鍛樼敤鎴风被鍨?*/
	    SMARTLOCK_NORMAL_USER = 0x01,        						/*鏅?氱敤鎴风被鍨?*/
	    SMARTLOCK_TEP_USER = 0x02,        							/*涓存椂鐢ㄦ埛绫诲瀷*/
	    SMARTLOCK_TEP_CARD_USER = 0x06,								/*临时卡片用户*/
	    SMARTLOCK_RESET = 0x42,               						/*鎭㈠鍑哄巶璁剧疆*/
	    SMARTLOCK_OPRN_HISTORY = 0x41,
	/*************************RSY***************************/
		SELFPROTOCOLSL500LOCK_QUERY_TEMCARDUSER_INFO = 0x37,    	//查询临时卡片用户信息
		SELFPROTOCOLSL500LOCK_MOD_OPEN_MODE = 0x23,             	//修改开锁方式
		SELFPROTOCOLSL500LOCK_MOD_CLOSE_MODE = 0x24,            	//修改关锁方式
		SELFPROTOCOLSL500LOCK_ADD_ADMIN_OR_NORMAL_USER = 0x25,  	//无线添加管理员、普通用户
		SELFPROTOCOLSL500LOCK_USER_PASSWD_AUTHEN = 0x13,			//用户密码验证
	/******************************************************/
};


enum SMART374LOOK_COMMMAND_ID
{
		SMARTLOCK_374_TIME_SYNC_CMD = 0x12,				/*时钟同步*/
		SMARTLOCK_374_PASSWD_AUTHEN_CMD = 0x15,						/*管理员密码认证*/	
		SMARTLOCK_374_QUERY_TEMUSER_INFO_CMD = 0x33,				/*查询锁具临时用户信息*/
		SMARTLOCK_374_ADD_NEWTEMUSER_CMD = 0x21,					/*添加新用户*/	
		APP_374_DELETE_TEMUSER_CMD = 0x22,							/*APP删除用户*/	
		APP_374_REMOTR_OPENDOOR_CMD = 0x11,						/*APP开锁*/
		APP_374_QUERY_SMARTLOCK_TIME = 0x35,							/*APP查询锁具时间*/
	    SMARTLOCK_374_QUERY_ADMINUSER_INFO_CMD = 0x31,          	/*查询管理员用户信息*/
	    SMARTLOCK_374_QUERY_NORMALUSER_INFO_CMD = 0x32,        	 	/*鏌ヨ閿佸叿鏅?氱敤鎴蜂俊鎭?*/
	    SMARTLOCK_374_QUERY_LOCK_INFO_CMD = 0x34,               	/*查询锁具信息*/
	    SMARTLOCK_374_QUERY_OPENDOOR_HISTORY_CMD = 0x36,        	/*鏌ヨ寮?閿佸巻鍙蹭俊鎭?*/
	    SMARTLOCK_374_ADMIN_USER = 0x00,            					/*绠＄悊鍛樼敤鎴风被鍨?*/
	    SMARTLOCK_374_NORMAL_USER = 0x01,        						/*鏅?氱敤鎴风被鍨?*/
	    SMARTLOCK_374_TEP_USER = 0x02,        							/*涓存椂鐢ㄦ埛绫诲瀷*/
//	    SMARTLOCK_TEP_CARD_USER = 0x06,								/*临时卡片用户*/
	    SMARTLOCK_374_RESET = 0x42,               						/*鎭㈠鍑哄巶璁剧疆*/
	    SMARTLOCK_374_OPRN_HISTORY = 0x41,

		SMARTLOCK_374_QUERY_NORMALUSER_TIMELIMITED = 0x38,			//查询普通用户使用时限
		SMARTLOCK_374_MOD_NORMALUSER_TIMELIMITED = 0x39,			//修改普通用户使用时限
};


enum SL500_TEST_RETURN_COMMAND_ID
{
		SL500LOCK_RETURN_FOR_ACTIVE_SYSC_TIME = 0x51,
		SL500LOCK_RETURN_FOR_ERROR_CODE = 0x30,
		SL500LOCK_RETURN_FOR_OPENLOCK = 0x31,
		SL500LOCK_RETURN_QUERY_OPENLOCK_HISTORY = 0x32,
		SL500LOCK_RETURN_QUERY_LOCK_TIME = 0x33,
		SL500LOCK_RETURN_ACTIVE_GET_TIME = 0x34,
		SL500LOCK_RETURN_AUTO_LOCK = 0x36,
		SL500LOCK_RETURN_ADMIN_PASSWD_AUTHEN = 0x35,
		SL500LOCK_RETURN_WIRELESS_ADD_TEMP = 0x37,
		SL500LOCK_RETURN_WIRELESS_DEL_USER = 0x38,
		SL500LOCK_RETURN_USER_INFO = 0x39,
		SL500LOCK_QUERY_LOCK_INFO = 0x3A,
		SL500LOCK_LOCAL_ADD_USER = 0x3B,
		SL500LOCK_LOCAL_DEL_USER = 0x3C,
		SL500LOCK_LOCAL_MOD_USER = 0x3D, 
		SL500LOCK_MOD_OPEN_MODE = 0x3E,
		SL500LOCK_MOD_CLOSE_MODE = 0x3F,
		SL500LOCK_WIRELESS_ADD_ADMIN = 0x40,
		SL500LOCK_WIRELESS_ADD_NORMAL = 0x41,
		SL500LOCK_USER_PASSWD_AUTHEN = 0x42,
};


enum SMART374_TEST_RETURN_COMMAND_ID
{
		LOCK374_QUERY_NORUSER_TIMELIMITED = 0x43,
		LOCK374_MOD_NORUSER_TIMELIMITED = 0x44,
};



enum ATTRIBUTE_SET_ID
{
		ATTRIBUTE_SET_TEMP_CODE = 0x00,
		ATTRIBUTE_DELETE_TEMP_CODE = 0x01,
		ATTRIBUTE_QUERY_USER = 0x03,
		ATTRIBUTE_ADMIN_ARITIFY = 0x04,
};




/*协议格式*/
typedef struct smartlock_protocol_info
{
		int sof;											/*包头：一个字节*/
		
		int len;											/*长度：0x06~0x33*/
		
		int cmd1;											/*命令字1:协议版本号*/

		int cmd2;                                           /*命令字2*/

		int src_low;                                        /*预留字节:0xFF*/

		int src_high;                                       /*预留字节:0xFF*/

		int dest_low;                                       /*预留字节:0xFF*/
		
		int dest_high;                                      /*预留字节:0xFF*/

		unsigned char msg[PRIVATE_DATA_LEN];
		
		int deviceType;

		int dataNumber;                                     /*控制单元个数*/

		int type;                                           /*数据类型: 一个字节*/

		int len1;											/*通用数据长度*/

		int val;
		
		int clusterID_low;

		int clusterID_high;

		int attributeID_low;

		int attributeID_high;
		
		unsigned char data[PRIVATE_DATA_LEN];				/*数据段（为锁具或无线模块需要传递的数据内容）*/

		int data_unit_len;
		
		unsigned char data_unit_val[PRIVATE_DATA_LEN];		/*自定义数据段*/
		
		int fcs;											/*校验（包头到数据断尾的累加和）*/	
}SMARTLOCK_PROTOCOL;


#if 0
struct tm
{
		char year;											/*年*/	
		char month;											/*月*/	
		char days;											/*日*/	
		char hour;											/*时*/		
		char minute;										/*分*/	
		char second;										/*秒*/
};

#endif
/*封装智能门锁控制指令数据*/
int HSZSendSmartLockData(char* dev_mac,char *data_buf);

/* 计算传入数据的Check fcs数值*/
int HSZCalculateSmartLockDataCheckSum(int sof,int len,unsigned char *data_buf);					

/*解析智能门锁私有数据*/
int HSZAnalysisSmartLockData(unsigned char *data,char* gw_mac,char* dev_mac);							

/*解析智能门锁时间同步请求*/
int HSZSmartLockTimeSync(char *data_buf,char* gw_mac,char* dev_mac);		

/*解析门锁固定上报数据*/
int HSZAnalysisSmartLockRegularData(char *data_buf,char* gw_mac,char* dev_mac);

/*解析添加临时密码操作返回数据*/
int HSZReturnDataForAddTemCode(char *data_buf,char* gw_mac,char* dev_mac);

/*解析查询用户操作返回数据*/
int HSZReturnDataForQueryUser(char *data_buf,char* gw_mac,char* dev_mac);

/*解析删除临时密码操作返回数据*/
int HSZReturnDataForDeleteTemCode(char *data_buf,char* gw_mac,char* dev_mac);

/*解析管理员密码认证返回数据*/
int HSZReturnDataForAdminAuth(char *data_buf,char* gw_mac,char* dev_mac);

/*解析门锁控制信息反馈数据*/
int HSZReturnDataForControl(char *data_buf,char* gw_mac,char* dev_mac);

/*判断是否是闰年*/
int CheckLeapYear(unsigned int year);

/*时间戳换算为时间*/
void Hal_Time_SecondToDate(unsigned long int second,char *TimeYear,char *TimeMonth,char *TimeDate,char *TimeHour,char *TimeMinute,char *TimeSecond);

/*日期换算为时间戳*/
unsigned long int Hal_Time_DateToSecond(const int g_ucTimeYear,const int g_ucTimeMonth,const int g_ucTimeDate,const int g_ucTimeHour,const int g_ucTimeMinute,const int g_ucTimeSecond);

/*查询临时用户信息*/
int HSZQueryTemUser();

/*查询锁具内部时间*/
int HSZQueryLockTime();

/*删除临时用户*/
int HSZDeleteTemUser(char *data_buf);

/*封装智能门锁控制指令数据*/
int HSZSendSmartLockData(char* dev_mac,char *send_buf);

/*时钟同步*/
int HSZSmartLockTimeSynchronize(char *data_buf);

/*远程开锁*/
int HSZSmartLockRemoteOpenDoor(char *data_buf);

/*管理员密码认证*/
int HSZSmartLockPasswdCheck(char *data_buf);

/*添加临时用户*/
int HSZAddTemUser(char *data_buf);

/*数组转字符串*/
void HSZConvertUnCharToStr(char* str,unsigned char* UnChar, int ucLen);

#endif
