/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZSelfProtocolSL500Lock.h
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2019年03月05日
  最近修改   :
  功能描述   : 处理SL500智能门锁协议的Data数据头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2018年03月05日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

******************************************************************************/
#ifndef _HSZSelfProtocolLock_H_
#define _HSZSelfProtocolLock_H_

#define SELF_PROTOCOL_DATA_LEN 1024

#define SELF_PROTOCOL_SMARTLOCK_COMMAND "FEEA"

/*******************smartlock->zigbee***************************/
#define SYNC_TIME_FAILED_ERRORCODE 0x01
#define PASSWD_ERROR_ERRORCODE 0x02
#define ADMIN_PASSWD_AUTH_FAILED_ERRORCODE 0x03
#define REMOTE_OPENDOOR_PASSWD_ERROR_ERRORCODE 0x04
#define OPENDOOR_FAILED_ERRORCODE 0x05
#define REMOTE_OPENDOOR_FORBID_ERRORCODE 0x06
//#define ADD_PASSWD_DUPLICATE_ERRORCODE "07"
#define ADD_USER_FAILED_ERRORCODE 0x08
#define DEL_USER_FAILED_ERRORCODE 0x09

/*************************RSY***************************/
#define ADD_TEMPPASSWD_DUPLICATE_ERRORCODE 0x07
#define PASSWD_AUTH_FAILED_ERRORCODE 0x10
#define MOD_OPENLOCK_MODE_FAILED_ERRORCODE 0x11
#define MOD_CLOSELOCK_MODE_FAILED_ERRORCODE 0x12
#define ADD_USER_CARD_DUPLICATE 0x13



#define SMARTLOCK_LOCK_CLOSED 0x00
#define SMARTLOCK_PASSWD_VALIDATION_ERROR_3TIMES 0x01
#define SMARTLOCK_FINGERPRINT_VALIDATION_ERROR_3TIMES 0x02
#define SMARTLOCK_CARD_VALIDATION_ERROR_3TIMES 0x03
#define SMARTLOCK_DEMOLITION_ALARM 0x04
#define SMARTLOCK_LOW_BATTERY_ALARM 0x05
#define SMARTLOCK_DOUBLE_LOCK1 0x06
#define SMARTLOCK_RELEASE_LOCK 0x07
#define SMARTLOCK_LOCAL_SETTING_REMOTE_UNLOK_PERMIT 0x08
#define SMARTLOCK_LOCAL_SETTING_REMOTE_UNLOK_FORBID 0x09
#define SMARTLOCK_FACTORY_RESET 0x0A
#define SMARTLOCK_RELEASE_SYSTEM_LOCK_IN 0x0B
#define SMARTLOCK_UNLATCHED_DOOR 0x0C
#define SMARTLOCK_DOORBELL 0x0D
#define SMARTLOCK_OUTMODE 0x0E


/*******************zigbee->gateway****************************/
/*闂ㄩ攣琚姩杩斿洖鍛戒护*/
#define OPENDOOR_FAILED "0A01"
#define OPENDOOR_PASSWD_ERROR "0A10"
#define REMOTE_OPENDOOR_FORBID "0301"
#define SYNC_TIME_FAILED "0A02"
#define USER_AUTH_FAILED "0A00"
#define ADMIN_PASSWD_AUTH_FAILED "0A11"
//#define ADD_PASSWD_DUPLICATE "0A12"
#define ADD_USER_FAILED "0A03"
#define DEL_USER_FAILED "0A04"

/*************************RSY***************************/
#define ADD_TEMPPASSWD_DUPLICATE "0A12"
#define ADD_TEMPCARD_DUPLICATE "0A08"
#define MOD_OPENLOCK_MODE_FAILED "0A06"
#define MOD_CLOSELOCK_MODE_FAILED "0A07"
#define ADD_ADMIN_DUPLICATE "0A09"
#define ADD_NORMAL_DUPLICATE "0A0A"
#define PASSWD_AUTHEN_FAILED "0A05"



//#define LOCK_CLOSED "00"
#define PASSWD_VALIDATION_ERROR_3TIMES "021F"
#define FINGERPRINT_VALIDATION_ERROR_3TIMES "0220"
#define CARD_VALIDATION_ERROR_3TIMES "0221"
#define DEMOLITION_ALARM "021D"
#define LOW_BATTERY_ALARM "021C"
#define DOUBLE_LOCK "020A"
#define RELEASE_LOCK "020B"
#define LOCAL_SETTING_REMOTE_UNLOK_PERMIT "0302"
#define LOCAL_SETTING_REMOTE_UNLOK_FORBID "0301"
#define FACTORY_RESET "020C"
#define RELEASE_SYSTEM_LOCK_IN "021E"
#define UNLATCHED_DOOR "0206"
#define DOORBELL "0205"
#define LOCAL_REPORT_LOCK "0102"
#define OUTMODE_DOOR "020D"


#define SL920_ADMIN_USER "00"
#define SL920_NORMAL_USER "01"
#define SL920_TEMP_USER "02"
#define SL920_DATA_IS_NULL "00"

/*************************RSY***************************/
#define SL500_ADMIN_USER 00
#define SL500_NORMAL_USER 01
#define SL500_TEMP_PASSWD_USER 02
#define SL500_DATA_IS_NULL "00"						//进行字符串比较
#define SL500_TEMP_CARD_USER 06
/*************************RSY***************************/
#define TEMP_CARD_USER_IS_NULL "080400"
#define ADMIN_USER_IS_NULL "080500"
#define NORMAL_USER_IS_NULL "080600"
#define TEMP_USER_IS_NULL "080700"
#define SL500_ACTIVE_GET_TIME "0502"

#define QUERY_TIME "fa0b1451ffffffff20ffffffff82"


enum SELF_PROTOCOL_DATA_CMD1
{
        
        CMD1_NETWORK_CONFIG = 0x12,                /* 网络配置*/        
        CMD1_LOCK_COMMAND = 0x13,                  /* 无线控制门锁：ZigBee-锁*/
        CMD1_LOCK_RESPONSE = 0x14,                 /* 门锁数据上报：锁-ZigBee*/        
        CMD1_ACK_TO_LOCK = 0x15,                   /* ACK：ZigBee-锁*/
        CMD1_ACK_TO_ZIGBEE = 0x16,                 /* ACK：锁-ZigBee*/
};

enum SELF_PROTOCOL_DATA_CMD2
{
        
        CMD2_APP_OPEN_LOCK = 0x01,                 /*无线密码验证开锁*/ 
        
        CMD2_APP_QUERY_LOCK_TIME = 0x02,           /*无线查询锁具时间*/ 
        
        CMD2_LOCK_TIME_SYNCHRONIZATION = 0x03,     /*门锁时钟同步*/
        
        CMD2_PASSWD_AUTHEN_COMMAND = 0x04,         /*管理员密码认证*/
                
        CMD2_ERROR_INFO_RESPONSE = 0x30,                       
        
        CMD2_OPEN_DOOR_INFO_RESPONSE = 0x31,              
        
        CMD2_OPEN_DOOR_HISTORY_RESPONSE = 0x32,        
        
        CMD2_DOOR_TIME_RESPONSE = 0x33,                        

        CMD2_DOOR_REQUEST_TIME_SYNC = 0x34,            

        CMD2_ADMIN_PASSWD_AUTH_OK = 0x35,               

        CMD2_DOOR_EVENT_REPORT = 0x36,                       

        CMD2_REMOTE_ADD_USER_OK_REPORT = 0x37,     

        CMD2_RREMOTE_DEL_USER_OK_REPORT = 0x38,     

        CMD2_USER_QUERY_RESULT_REPORT = 0x39,          
         
        CMD2_DOOR_INFO_REPORT = 0x3A,                       

        CMD2_LOCAL_NEW_USER_REPORT = 0x3B,              

        CMD2_LOCAL_DEL_USER_REPORT = 0x3C,              

        CMD2_LOCAL_MOD_USER_REPORT = 0x3D,              

/*******************************RSY*******************************/ 
		CMD2_MOD_OPEN_LOCK_MODE = 0x3E,

		CMD2_MOD_CLOSE_LOCK_MODE = 0x3F,

		CMD2_REMOTE_ADD_ADMIN_OK_REPORT = 0x40,

		CMD2_REMOTE_ADD_NORMAL_OK_REPORT = 0x41,

		CMD2_USER_PASSWD_AUTHEN_OK_REPORT = 0x42,
};


/*协议格式*/
typedef struct smartlock_self_protocol_info
{
        int sof;                                            /*包头：一个字节0xFA*/
        
        int len;                                            /*长度：0x06~0x33*/
        
        int cmd1;                                           /*命令字1:协议版本号*/

        int cmd2;                                           /*命令字2*/

        int data_len;                                       /*数据段长度*/

        unsigned char data[SELF_PROTOCOL_DATA_LEN];         /*数据*/
        
        int fcs;                                            /*校验（包头到数据断尾的累加和）*/
        
}SMARTLOCK_SELF_PROTOCOL;


/* 计算传入数据的Check fcs数值 */
int HSZCalculateSelfProtocolSL500LockDataCheckSum(int sof,int len,unsigned char *data_buf);

/* 封装智能门锁控制指令数据 */
int HSZSendSelfProtocolLockData(char* dev_mac,char *data_buf);

/* 解析自有协议SL500智能门锁私有数据 */
int HSZAnalysisSelfProtocolSL500LockData(unsigned char *data,char* gw_mac,char* dev_mac);

/* 自有协议智能门锁无线开锁 */
int HSZSelfProtocolSL500LockCmdOpenDoor(char *data_buf);

/* 自有协议智能门锁查询时间 */
int HSZSelfProtocolSL500LockCmdQuerryTime(char *data_buf);

/* 自有协议智能门锁同步时间 */
int HSZSelfProtocolSL500LockCmdSynchronizeTime(char *data_buf);

/* 自有协议智能门锁管理员密码认证 */
int HSZSelfProtocolSL500LockCmdPasswdCheck(char *data_buf);

void Hal_Time_SecondToDate(unsigned long int second,char *TimeYear,char *TimeMonth,char *TimeDate,char *TimeHour,char *TimeMinute,char *TimeSecond);

unsigned long int Hal_Time_DateToSecond(const int g_ucTimeYear,const int g_ucTimeMonth,const int g_ucTimeDate,const int g_ucTimeHour,const int g_ucTimeMinute,const int g_ucTimeSecond);

/*无线添加临时用户*/
int HSZSelfProtocolSL500LockCmdAddUser(char *data_buf);

/*无线删除用户*/
int HSZSelfProtocolSL500DeleteUser(char *data_buf);

/*私有协议SL500门锁解析查询管理员、普通和临时用户信息*/
int HSZSelfProtocolSL500LockQueryUser(int user_type);

/*私有协议SL500门锁解析查询锁具信息*/
int HSZSelfProtocolSL500LockQueryLockInfo(char *data_buf);

/*私有协议SL500门锁解析恢复出厂设置状态确认*/
int HSZSelfProtocolSL500LockReset(void);

/*私有协议SL500门锁解析开锁历史查询*/
int HSZSelfProtocolSL500LockOpenDoorHistory(void);

/*私有协议SL500门锁解析查询开锁历史记录*/
int HSZSelfProtocolSL500LockHistory(char *data_buf);

/*私有协议SL500门锁事件上报(主动返回)*/
int HSZReturnDataForSL500LockEvent(char *data_buf,char* gw_mac,char* dev_mac);

/*私有协议SL500门锁解析错误密码返回数据*/
int HSZSelfProtocolSL500LockReturnDataForErrorCode(char *data_buf,char* gw_mac,char* dev_mac);

/*私有协议SL500门锁主动时间同步*/
int HSZSelfProtocolSL500LockReturnActiveSyncTime(char *data_buf,char* gw_mac,char* dev_mac);


/*私有协议SL500门锁无线密码验证开锁(主动返回)*/
int HSZSelfProtocolSL500LockReturnForUnlockInfo(char *data_buf,char* gw_mac,char* dev_mac);


int SL500_my_tolower(int c);
int SL500_my_htoi(char s[]);

/*解析管理员密码认证返回数据*/
int HSZSelfProtocolSL500LockReturnAdminPasswdAuthSuccess(char *data_buf,char* gw_mac,char* dev_mac);

/*解析添加临时用户成功结果上报*/
int HSZSelfProtocolSL500LockReturnDataForAddTemUser(char *data_buf,char* gw_mac,char* dev_mac);

/*私有协议SL500门锁查询开锁历史记录(主动返回)*/
int HSZSelfProtocolSL500LockReturnForHistory(char *data_buf,char* gw_mac,char* dev_mac);

/*私有协议SL500门锁查询锁端时间(主动返回)*/
int HSZSelfProtocolSL500LockReturnForSyncTime(char *data_buf,char* gw_mac,char* dev_mac);

/*解析查询管理员、普通和临时密码用户信息成功结果上报*/
int HSZSelfProtocolSL500LockReturnDataForUserInfo(char *data_buf,char* gw_mac,char* dev_mac);

/*解析查询锁具信息成功结果上报*/
int HSZSelfProtocolSL500LockReturnForLockVersionInfo(char *data_buf,char* gw_mac,char* dev_mac);

/*解析本地新增用户结果上报*/
int HSZSelfProtocolSL500LockReturnDataForLocalAddUser(char *data_buf,char* gw_mac,char* dev_mac);

/*解析本地修改用户结果上报*/
int HSZSelfProtocolSL500LockReturnDataForLocalModUser(char *data_buf,char* gw_mac,char* dev_mac);

/*解析本地删除用户结果上报*/
int HSZSelfProtocolSL500LockReturnDataForLocalDelUser(char *data_buf,char* gw_mac,char* dev_mac);

/*解析无线删除用户结果上报*/
int HSZSelfProtocolSL500LockReturnDataForDelUser(char *data_buf,char* gw_mac,char* dev_mac);

/*解析锁具主动获取时间返回数据*/
int HSZSelfProtocolSL500LockReturnForQueryTime(char *data_buf,char* gw_mac,char* dev_mac);

/*封装智能门锁控制指令数据*/
int HSZSendSelfProtocolSL500LockData(char* dev_mac,char *data_buf);

/*私有协议SL500门锁时间同步失败*/
int HSZSelfProtocolSL500LockReturnSyncTimeFail(char *data_buf,char* gw_mac,char* dev_mac);

/*****************************************RSY********************************************/
/*解析修改开锁方式成功返回数据*/
int HSZSelfProtocolLockReturnDataForModOpenMode(char *data_buf,char* gw_mac,char* dev_mac);

/*解析修改关锁方式成功返回数据*/
int HSZSelfProtocolLockReturnDataForModCloseMode(char *data_buf,char* gw_mac,char* dev_mac);

/*解析无线添加管理员用户成功返回数据*/
int HSZSelfProtocolLockReturnDataForRemoteAddAdminUser(char *data_buf,char* gw_mac,char* dev_mac);

/*解析无线添加普通用户成功返回数据*/
int HSZSelfProtocolLockReturnDataForRemoteAddNormalUser(char *data_buf,char* gw_mac,char* dev_mac);

/*解析无线添加普通用户成功返回数据*/
int HSZSelfProtocolLockReturnDataForRemoteAddNormalUser(char *data_buf,char* gw_mac,char* dev_mac);

/*解析用户密码认证返回数据*/
int HSZSelfProtocolLockReturnDataForUserPasswdAuthen(char *data_buf,char* gw_mac,char* dev_mac);


#endif
