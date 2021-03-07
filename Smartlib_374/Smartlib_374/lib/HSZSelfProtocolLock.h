/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZSelfProtocolLock.h
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2018年8月124日
  最近修改   :
  功能描述   : 处理私有智能门锁协议的 Data数据
  函数列表   :
  修改历史   :
  1.日    期   : 2018年8月24日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

******************************************************************************/
#ifndef _HSZSelfProtocolLock_H_
#define _HSZSelfProtocolLock_H_


#define SELF_PROTOCOL_DATA_LEN                                          1024



#define SELF_PROTOCOL_SMARTLOCK_COMMAND             "FEEA"

/*******************smartlock->zigbee***************************/

/*闂ㄩ攣閿欒鐮?*/
#define SYNC_TIME_FAILED_ERRORCODE "01"
#define PASSWD_ERROR_ERRORCODE "02"
#define ADMIN_PASSWD_AUTH_FAILED_ERRORCODE "03"
#define REMOTE_OPENDOOR_PASSWD_ERROR_ERRORCODE "04"
#define OPENDOOR_FAILED_ERRORCODE "05"
#define REMOTE_OPENDOOR_FORBID_ERRORCODE "06"
#define ADD_PASSWD_DUPLICATE_ERRORCODE "07"
#define ADD_USER_FAILED_ERRORCODE "08"
#define DEL_USER_FAILED_ERRORCODE "09"

/*閿佸叿浜嬩欢鍙傛暟*/
#define SMARTLOCK_LOCK_CLOSED "00"
#define SMARTLOCK_PASSWD_VALIDATION_ERROR_3TIMES "01"
#define SMARTLOCK_FINGERPRINT_VALIDATION_ERROR_3TIMES "02"
#define SMARTLOCK_CARD_VALIDATION_ERROR_3TIMES "03"
#define SMARTLOCK_DEMOLITION_ALARM "04"
#define SMARTLOCK_LOW_BATTERY_ALARM "05"
#define SMARTLOCK_DOUBLE_LOCK1 "06"
#define SMARTLOCK_RELEASE_LOCK "07"
#define SMARTLOCK_LOCAL_SETTING_REMOTE_UNLOK_PERMIT "08"
#define SMARTLOCK_LOCAL_SETTING_REMOTE_UNLOK_FORBID "09"
#define SMARTLOCK_FACTORY_RESET "0A"
#define SMARTLOCK_RELEASE_SYSTEM_LOCK_IN "0B"
#define SMARTLOCK_UNLATCHED_DOOR "0C"
#define SMARTLOCK_DOORBELL "0D"
#define SMARTLOCK_OUTMODE "0E"
/****************************************************************/


/*******************zigbee->gateway****************************/

/*闂ㄩ攣琚姩杩斿洖鍛戒护*/
#define OPENDOOR_FAILED "0A01"
#define OPENDOOR_PASSWD_ERROR "0A10"
#define REMOTE_OPENDOOR_FORBID "0301"
#define SYNC_TIME_FAILED "0A02"
#define USER_AUTH_FAILED "0A00"
#define ADMIN_PASSWD_AUTH_FAILED "0A11"
#define ADD_PASSWD_DUPLICATE "0A12"
#define ADD_USER_FAILED "0A03"
#define DEL_USER_FAILED "0A04"

/*闂ㄩ攣涓诲姩杩斿洖鍛戒护*/
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

#define ADMIN_USER_IS_NULL "080500"
#define NORMAL_USER_IS_NULL "080600"
#define TEMP_USER_IS_NULL "080700"
#define SL920_QUERY_TIME "0502"

#define QUERY_TIME "fa0b1451ffffffff20ffffffff82"

/**************************************************************/

enum SELF_PROTOCOL_DATA_CMD1{
        
        CMD1_NETWORK_CONFIG = 0x12,                /* 网络配置*/
        
        CMD1_LOCK_COMMAND = 0x13,                  /* 无线控制门锁：ZigBee-锁*/
        CMD1_LOCK_RESPONSE = 0x14,                 /* 门锁数据上报：锁-ZigBee*/
        
        CMD1_ACK_TO_LOCK = 0x15,                   /* ACK：ZigBee-锁*/
        CMD1_ACK_TO_ZIGBEE = 0x16,                 /* ACK：锁-ZigBee*/
};

enum SELF_PROTOCOL_DATA_CMD2{
        
        CMD2_APP_OPEN_LOCK = 0x01,                 /*无线密码验证开锁*/ 
        
        CMD2_APP_QUERY_LOCK_TIME = 0x02,           /*无线查询锁具时间*/ 
        
        CMD2_LOCK_TIME_SYNCHRONIZATION = 0x03,     /*门锁时钟同步*/
        
        CMD2_PASSWD_AUTHEN_COMMAND = 0x04,         /*管理员密码认证*/
        
        
        CMD2_ERROR_INFO_RESPONSE = 0x30,                        /*閿欒淇℃伅涓婃姤*/
        
        CMD2_OPEN_DOOR_INFO_RESPONSE = 0x31,              /*寮?閿佹暟鎹笂鎶?*/
        
        CMD2_OPEN_DOOR_HISTORY_RESPONSE = 0x32,         /*寮?閿佸巻鍙茶褰曚笂鎶?*/
        
        CMD2_DOOR_TIME_RESPONSE = 0x33,                         /*闂ㄩ攣鏈湴鏃堕棿涓婃姤*/

        CMD2_DOOR_REQUEST_TIME_SYNC = 0x34,             /*闂ㄩ攣涓诲姩璇锋眰鏍℃椂*/

        CMD2_ADMIN_PASSWD_AUTH_OK = 0x35,                /*绠＄悊鍛樺瘑鐮佽璇佹垚鍔?*/

        CMD2_DOOR_EVENT_REPORT = 0x36,                        /*閿佺浜嬩欢涓婃姤*/

        CMD2_REMOTE_ADD_USER_OK_REPORT = 0x37,      /*鏃犵嚎娣诲姞鐢ㄦ埛鎴愬姛缁撴灉涓婃姤*/

        CMD2_RREMOTE_DEL_USER_OK_REPORT = 0x38,     /*鏃犵嚎鍒犻櫎鐢ㄦ埛鎴愬姛缁撴灉涓婃姤*/

        CMD2_USER_QUERY_RESULT_REPORT = 0x39,          /*鐢ㄦ埛鏌ヨ缁撴灉涓婃姤*/
         
        CMD2_DOOR_INFO_REPORT = 0x3A,                          /*閿佺淇℃伅涓婃姤*/

        CMD2_LOCAL_NEW_USER_REPORT = 0x3B,                /*鏈湴鏂板鐢ㄦ埛涓婃姤*/

        CMD2_LOCAL_DEL_USER_REPORT = 0x3C,              /*鏈湴鍒犻櫎鐢ㄦ埛涓婃姤*/

        CMD2_LOCAL_MOD_USER_REPORT = 0x3D,              /*鏈湴缂栬緫鐢ㄦ埛涓婃姤*/

        
};


/*协议格式*/
typedef struct smartlock_self_protocol_info
{
        int sof;                                                                                        /*包头：一个字节0xFA*/
        
        int len;                                                                                        /*长度：0x06~0x33*/
        
        int cmd1;                                                                                       /*命令字1:协议版本号*/

        int cmd2;                                           /*命令字2*/

        int data_len;                                                                       /*数据段长度*/

        unsigned char data[SELF_PROTOCOL_DATA_LEN];                     /*数据*/
        
        int fcs;                                                                                        /*校验（包头到数据断尾的累加和）*/
        
}SMARTLOCK_SELF_PROTOCOL;

int HSZSendSelfProtocolLockData(char* dev_mac,char *send_buf);


/* 计算传入数据的Check fcs数值 */
int HSZCalculateSelfProtocolLockDataCheckSum(int sof,int len,unsigned char *data_buf);

/* 封装智能门锁控制指令数据 */
int HSZSendSelfProtocolLockData(char* dev_mac,char *data_buf);

/* 解析自有协议智能门锁私有数据 */
int HSZAnalysisSelfProtocolLockData(unsigned char *data,char* gw_mac,char* dev_mac);


/* 自有协议智能门锁无线开锁 */
int HSZSelfProtocolLockCmdOpenDoor(char *data_buf);

/* 自有协议智能门锁查询时间 */
int HSZSelfProtocolLockCmdQuerryTime(char *data_buf);

/* 自有协议智能门锁同步时间 */
int HSZSelfProtocolLockCmdSynchronizeTime(char *data_buf);

/* 自有协议智能门锁管理员密码认证 */
int HSZSelfProtocolLockCmdPasswdCheck(char *data_buf);

void Hal_Time_SecondToDate(unsigned long int second,char *TimeYear,char *TimeMonth,char *TimeDate,char *TimeHour,char *TimeMinute,char *TimeSecond);

unsigned long int Hal_Time_DateToSecond(const int g_ucTimeYear,const int g_ucTimeMonth,const int g_ucTimeDate,const int g_ucTimeHour,const int g_ucTimeMinute,const int g_ucTimeSecond);

int HSZSelfProtocolLockCmdAddUser(char *data_buf);

int HSZSelfProtocolDeleteUser(char *data_buf);

int HSZSelfProtocolLockQueryUser(int user_type);

int HSZSelfProtocolLockQueryLockInfo(char *data_buf);
int HSZSelfProtocolLockReset(void);
int HSZSelfProtocolLockOpenDoorHistory(void);
int HSZSelfProtocolLockHistory(char *data_buf);
int HSZReturnDataForLockEvent(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSelfProtocolLockReturnDataForErrorCode(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSelfProtocolLockReturnForUnlockInfo(char *data_buf,char* gw_mac,char* dev_mac);


int my_tolower(int c);
int my_htoi(char s[]);
int HSZSelfProtocolLockReturnAdminPasswdAuthSuccess(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSelfProtocolLockReturnDataForAddTemUser(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSelfProtocolLockReturnForHistory(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSelfProtocolLockReturnForSyncTime(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSelfProtocolLockReturnDataForUserInfo(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSelfProtocolLockReturnForLockVersionInfo(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSelfProtocolLockReturnDataForLocalAddUser(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSelfProtocolLockReturnDataForLocalModUser(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSelfProtocolLockReturnDataForLocalDelUser(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSelfProtocolLockReturnDataForDelUser(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSelfProtocolLockReturnForQueryTime(char *data_buf,char* gw_mac,char* dev_mac);
int HSZSendSelfProtocolSL868LockData(char* dev_mac,char *data_buf);
#endif
