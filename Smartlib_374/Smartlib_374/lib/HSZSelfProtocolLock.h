/******************************************************************************

                  ��Ȩ���� (C), 2017-2025, �ൺ�����ǻۼҾ�ϵͳ�ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : HSZSelfProtocolLock.h
  �� �� ��   : ����
  ��    ��   : Zigbee Project Team
  ��������   : 2018��8��124��
  ����޸�   :
  ��������   : ����˽����������Э��� Data����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��8��24��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _HSZSelfProtocolLock_H_
#define _HSZSelfProtocolLock_H_


#define SELF_PROTOCOL_DATA_LEN                                          1024



#define SELF_PROTOCOL_SMARTLOCK_COMMAND             "FEEA"

/*******************smartlock->zigbee***************************/

/*门锁错误�?*/
#define SYNC_TIME_FAILED_ERRORCODE "01"
#define PASSWD_ERROR_ERRORCODE "02"
#define ADMIN_PASSWD_AUTH_FAILED_ERRORCODE "03"
#define REMOTE_OPENDOOR_PASSWD_ERROR_ERRORCODE "04"
#define OPENDOOR_FAILED_ERRORCODE "05"
#define REMOTE_OPENDOOR_FORBID_ERRORCODE "06"
#define ADD_PASSWD_DUPLICATE_ERRORCODE "07"
#define ADD_USER_FAILED_ERRORCODE "08"
#define DEL_USER_FAILED_ERRORCODE "09"

/*锁具事件参数*/
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

/*门锁被动返回命令*/
#define OPENDOOR_FAILED "0A01"
#define OPENDOOR_PASSWD_ERROR "0A10"
#define REMOTE_OPENDOOR_FORBID "0301"
#define SYNC_TIME_FAILED "0A02"
#define USER_AUTH_FAILED "0A00"
#define ADMIN_PASSWD_AUTH_FAILED "0A11"
#define ADD_PASSWD_DUPLICATE "0A12"
#define ADD_USER_FAILED "0A03"
#define DEL_USER_FAILED "0A04"

/*门锁主动返回命令*/
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
        
        CMD1_NETWORK_CONFIG = 0x12,                /* ��������*/
        
        CMD1_LOCK_COMMAND = 0x13,                  /* ���߿���������ZigBee-��*/
        CMD1_LOCK_RESPONSE = 0x14,                 /* ���������ϱ�����-ZigBee*/
        
        CMD1_ACK_TO_LOCK = 0x15,                   /* ACK��ZigBee-��*/
        CMD1_ACK_TO_ZIGBEE = 0x16,                 /* ACK����-ZigBee*/
};

enum SELF_PROTOCOL_DATA_CMD2{
        
        CMD2_APP_OPEN_LOCK = 0x01,                 /*����������֤����*/ 
        
        CMD2_APP_QUERY_LOCK_TIME = 0x02,           /*���߲�ѯ����ʱ��*/ 
        
        CMD2_LOCK_TIME_SYNCHRONIZATION = 0x03,     /*����ʱ��ͬ��*/
        
        CMD2_PASSWD_AUTHEN_COMMAND = 0x04,         /*����Ա������֤*/
        
        
        CMD2_ERROR_INFO_RESPONSE = 0x30,                        /*错误信息上报*/
        
        CMD2_OPEN_DOOR_INFO_RESPONSE = 0x31,              /*�?锁数据上�?*/
        
        CMD2_OPEN_DOOR_HISTORY_RESPONSE = 0x32,         /*�?锁历史记录上�?*/
        
        CMD2_DOOR_TIME_RESPONSE = 0x33,                         /*门锁本地时间上报*/

        CMD2_DOOR_REQUEST_TIME_SYNC = 0x34,             /*门锁主动请求校时*/

        CMD2_ADMIN_PASSWD_AUTH_OK = 0x35,                /*管理员密码认证成�?*/

        CMD2_DOOR_EVENT_REPORT = 0x36,                        /*锁端事件上报*/

        CMD2_REMOTE_ADD_USER_OK_REPORT = 0x37,      /*无线添加用户成功结果上报*/

        CMD2_RREMOTE_DEL_USER_OK_REPORT = 0x38,     /*无线删除用户成功结果上报*/

        CMD2_USER_QUERY_RESULT_REPORT = 0x39,          /*用户查询结果上报*/
         
        CMD2_DOOR_INFO_REPORT = 0x3A,                          /*锁端信息上报*/

        CMD2_LOCAL_NEW_USER_REPORT = 0x3B,                /*本地新增用户上报*/

        CMD2_LOCAL_DEL_USER_REPORT = 0x3C,              /*本地删除用户上报*/

        CMD2_LOCAL_MOD_USER_REPORT = 0x3D,              /*本地编辑用户上报*/

        
};


/*Э���ʽ*/
typedef struct smartlock_self_protocol_info
{
        int sof;                                                                                        /*��ͷ��һ���ֽ�0xFA*/
        
        int len;                                                                                        /*���ȣ�0x06~0x33*/
        
        int cmd1;                                                                                       /*������1:Э��汾��*/

        int cmd2;                                           /*������2*/

        int data_len;                                                                       /*���ݶγ���*/

        unsigned char data[SELF_PROTOCOL_DATA_LEN];                     /*����*/
        
        int fcs;                                                                                        /*У�飨��ͷ�����ݶ�β���ۼӺͣ�*/
        
}SMARTLOCK_SELF_PROTOCOL;

int HSZSendSelfProtocolLockData(char* dev_mac,char *send_buf);


/* ���㴫�����ݵ�Check fcs��ֵ */
int HSZCalculateSelfProtocolLockDataCheckSum(int sof,int len,unsigned char *data_buf);

/* ��װ������������ָ������ */
int HSZSendSelfProtocolLockData(char* dev_mac,char *data_buf);

/* ��������Э����������˽������ */
int HSZAnalysisSelfProtocolLockData(unsigned char *data,char* gw_mac,char* dev_mac);


/* ����Э�������������߿��� */
int HSZSelfProtocolLockCmdOpenDoor(char *data_buf);

/* ����Э������������ѯʱ�� */
int HSZSelfProtocolLockCmdQuerryTime(char *data_buf);

/* ����Э����������ͬ��ʱ�� */
int HSZSelfProtocolLockCmdSynchronizeTime(char *data_buf);

/* ����Э��������������Ա������֤ */
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
