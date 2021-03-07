/******************************************************************************

                  ��Ȩ���� (C), 2017-2025, �ൺ�����ǻۼҾ�ϵͳ�ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : HSZSelfProtocolSL500Lock.h
  �� �� ��   : ����
  ��    ��   : Zigbee Project Team
  ��������   : 2019��03��05��
  ����޸�   :
  ��������   : ����SL500��������Э���Data����ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��03��05��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ļ�

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
/*门锁被动返回命令*/
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
#define SL500_DATA_IS_NULL "00"						//�����ַ����Ƚ�
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
        
        CMD1_NETWORK_CONFIG = 0x12,                /* ��������*/        
        CMD1_LOCK_COMMAND = 0x13,                  /* ���߿���������ZigBee-��*/
        CMD1_LOCK_RESPONSE = 0x14,                 /* ���������ϱ�����-ZigBee*/        
        CMD1_ACK_TO_LOCK = 0x15,                   /* ACK��ZigBee-��*/
        CMD1_ACK_TO_ZIGBEE = 0x16,                 /* ACK����-ZigBee*/
};

enum SELF_PROTOCOL_DATA_CMD2
{
        
        CMD2_APP_OPEN_LOCK = 0x01,                 /*����������֤����*/ 
        
        CMD2_APP_QUERY_LOCK_TIME = 0x02,           /*���߲�ѯ����ʱ��*/ 
        
        CMD2_LOCK_TIME_SYNCHRONIZATION = 0x03,     /*����ʱ��ͬ��*/
        
        CMD2_PASSWD_AUTHEN_COMMAND = 0x04,         /*����Ա������֤*/
                
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


/*Э���ʽ*/
typedef struct smartlock_self_protocol_info
{
        int sof;                                            /*��ͷ��һ���ֽ�0xFA*/
        
        int len;                                            /*���ȣ�0x06~0x33*/
        
        int cmd1;                                           /*������1:Э��汾��*/

        int cmd2;                                           /*������2*/

        int data_len;                                       /*���ݶγ���*/

        unsigned char data[SELF_PROTOCOL_DATA_LEN];         /*����*/
        
        int fcs;                                            /*У�飨��ͷ�����ݶ�β���ۼӺͣ�*/
        
}SMARTLOCK_SELF_PROTOCOL;


/* ���㴫�����ݵ�Check fcs��ֵ */
int HSZCalculateSelfProtocolSL500LockDataCheckSum(int sof,int len,unsigned char *data_buf);

/* ��װ������������ָ������ */
int HSZSendSelfProtocolLockData(char* dev_mac,char *data_buf);

/* ��������Э��SL500��������˽������ */
int HSZAnalysisSelfProtocolSL500LockData(unsigned char *data,char* gw_mac,char* dev_mac);

/* ����Э�������������߿��� */
int HSZSelfProtocolSL500LockCmdOpenDoor(char *data_buf);

/* ����Э������������ѯʱ�� */
int HSZSelfProtocolSL500LockCmdQuerryTime(char *data_buf);

/* ����Э����������ͬ��ʱ�� */
int HSZSelfProtocolSL500LockCmdSynchronizeTime(char *data_buf);

/* ����Э��������������Ա������֤ */
int HSZSelfProtocolSL500LockCmdPasswdCheck(char *data_buf);

void Hal_Time_SecondToDate(unsigned long int second,char *TimeYear,char *TimeMonth,char *TimeDate,char *TimeHour,char *TimeMinute,char *TimeSecond);

unsigned long int Hal_Time_DateToSecond(const int g_ucTimeYear,const int g_ucTimeMonth,const int g_ucTimeDate,const int g_ucTimeHour,const int g_ucTimeMinute,const int g_ucTimeSecond);

/*���������ʱ�û�*/
int HSZSelfProtocolSL500LockCmdAddUser(char *data_buf);

/*����ɾ���û�*/
int HSZSelfProtocolSL500DeleteUser(char *data_buf);

/*˽��Э��SL500����������ѯ����Ա����ͨ����ʱ�û���Ϣ*/
int HSZSelfProtocolSL500LockQueryUser(int user_type);

/*˽��Э��SL500����������ѯ������Ϣ*/
int HSZSelfProtocolSL500LockQueryLockInfo(char *data_buf);

/*˽��Э��SL500���������ָ���������״̬ȷ��*/
int HSZSelfProtocolSL500LockReset(void);

/*˽��Э��SL500��������������ʷ��ѯ*/
int HSZSelfProtocolSL500LockOpenDoorHistory(void);

/*˽��Э��SL500����������ѯ������ʷ��¼*/
int HSZSelfProtocolSL500LockHistory(char *data_buf);

/*˽��Э��SL500�����¼��ϱ�(��������)*/
int HSZReturnDataForSL500LockEvent(char *data_buf,char* gw_mac,char* dev_mac);

/*˽��Э��SL500���������������뷵������*/
int HSZSelfProtocolSL500LockReturnDataForErrorCode(char *data_buf,char* gw_mac,char* dev_mac);

/*˽��Э��SL500��������ʱ��ͬ��*/
int HSZSelfProtocolSL500LockReturnActiveSyncTime(char *data_buf,char* gw_mac,char* dev_mac);


/*˽��Э��SL500��������������֤����(��������)*/
int HSZSelfProtocolSL500LockReturnForUnlockInfo(char *data_buf,char* gw_mac,char* dev_mac);


int SL500_my_tolower(int c);
int SL500_my_htoi(char s[]);

/*��������Ա������֤��������*/
int HSZSelfProtocolSL500LockReturnAdminPasswdAuthSuccess(char *data_buf,char* gw_mac,char* dev_mac);

/*���������ʱ�û��ɹ�����ϱ�*/
int HSZSelfProtocolSL500LockReturnDataForAddTemUser(char *data_buf,char* gw_mac,char* dev_mac);

/*˽��Э��SL500������ѯ������ʷ��¼(��������)*/
int HSZSelfProtocolSL500LockReturnForHistory(char *data_buf,char* gw_mac,char* dev_mac);

/*˽��Э��SL500������ѯ����ʱ��(��������)*/
int HSZSelfProtocolSL500LockReturnForSyncTime(char *data_buf,char* gw_mac,char* dev_mac);

/*������ѯ����Ա����ͨ����ʱ�����û���Ϣ�ɹ�����ϱ�*/
int HSZSelfProtocolSL500LockReturnDataForUserInfo(char *data_buf,char* gw_mac,char* dev_mac);

/*������ѯ������Ϣ�ɹ�����ϱ�*/
int HSZSelfProtocolSL500LockReturnForLockVersionInfo(char *data_buf,char* gw_mac,char* dev_mac);

/*�������������û�����ϱ�*/
int HSZSelfProtocolSL500LockReturnDataForLocalAddUser(char *data_buf,char* gw_mac,char* dev_mac);

/*���������޸��û�����ϱ�*/
int HSZSelfProtocolSL500LockReturnDataForLocalModUser(char *data_buf,char* gw_mac,char* dev_mac);

/*��������ɾ���û�����ϱ�*/
int HSZSelfProtocolSL500LockReturnDataForLocalDelUser(char *data_buf,char* gw_mac,char* dev_mac);

/*��������ɾ���û�����ϱ�*/
int HSZSelfProtocolSL500LockReturnDataForDelUser(char *data_buf,char* gw_mac,char* dev_mac);

/*��������������ȡʱ�䷵������*/
int HSZSelfProtocolSL500LockReturnForQueryTime(char *data_buf,char* gw_mac,char* dev_mac);

/*��װ������������ָ������*/
int HSZSendSelfProtocolSL500LockData(char* dev_mac,char *data_buf);

/*˽��Э��SL500����ʱ��ͬ��ʧ��*/
int HSZSelfProtocolSL500LockReturnSyncTimeFail(char *data_buf,char* gw_mac,char* dev_mac);

/*****************************************RSY********************************************/
/*�����޸Ŀ�����ʽ�ɹ���������*/
int HSZSelfProtocolLockReturnDataForModOpenMode(char *data_buf,char* gw_mac,char* dev_mac);

/*�����޸Ĺ�����ʽ�ɹ���������*/
int HSZSelfProtocolLockReturnDataForModCloseMode(char *data_buf,char* gw_mac,char* dev_mac);

/*����������ӹ���Ա�û��ɹ���������*/
int HSZSelfProtocolLockReturnDataForRemoteAddAdminUser(char *data_buf,char* gw_mac,char* dev_mac);

/*�������������ͨ�û��ɹ���������*/
int HSZSelfProtocolLockReturnDataForRemoteAddNormalUser(char *data_buf,char* gw_mac,char* dev_mac);

/*�������������ͨ�û��ɹ���������*/
int HSZSelfProtocolLockReturnDataForRemoteAddNormalUser(char *data_buf,char* gw_mac,char* dev_mac);

/*�����û�������֤��������*/
int HSZSelfProtocolLockReturnDataForUserPasswdAuthen(char *data_buf,char* gw_mac,char* dev_mac);


#endif
