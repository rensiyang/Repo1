/******************************************************************************

                  ��Ȩ���� (C), 2017-2025, �ൺ�����ǻۼҾ�ϵͳ�ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : HSZSmartLockMessage.h
  �� �� ��   : ����
  ��    ��   : Zigbee Project Team
  ��������   : 2017��12��13��
  ����޸�   :
  ��������   : ����˽����������Э��� Data����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��3��29�� 2019��03��05��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ļ�

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
		REGULAR_DATA_REPORT = 0x32,									/*�̶������ϱ� ��-ZigBee*/
		TIME_DATA_REPORT = 0x51,									/*ʱ��ͬ�� ��-ZigBee*/
		PRIVATE_DATA_REPORT = 0x55,									/*�Զ��������ϱ� ��-ZigBee*/
		CONTROL_TIME = 0x52,										/*ʱ��ͬ�� ZigBee-��*/
		CONTROL_OPENDOOR = 0x53,									/*Զ�̿��� ZigBee-��*/
		CONTROL_REGULAR = 0x54,										/*�Զ��幦�� ZigBee-��*/
};

enum ATTRIBUTE_ID
{
		ADD_TMPPASSWORD = 0x00,										/*�����ʱ�����������*/
		DELETE_TMPPASSWORD = 0x01,									/*ɾ����ʱ�����������*/
		QUERY_USER = 0x03,											/*��ѯ�û���Ϣ����*/
		ADMIN_AUTH = 0x04,											/*����Ա������֤��������*/
		RESPONSE_CONTROL = 0x10,									/*����������Ϣ����ָʾ*/
					
};

/*smartgateway->zigbee*/
enum SMARTLOOK_COMMMAND_ID
{
		SMARTLOCK_TIME_SYNCHRONIZATION_COMMAND = 0x12,				/*ʱ��ͬ��*/
		SMARTLOCK_PASSWD_AUTHEN_COMMAND = 0x15,						/*����Ա������֤*/	
		SMARTLOCK_QUERY_TEMUSER_INFO_COMMAND = 0x33,				/*��ѯ������ʱ�û���Ϣ*/
		SMARTLOCK_ADD_NEW_TEMUSER_COMMAND = 0x21,					/*������û�*/	
		APP_DELETE_TEMUSER_COMMAND = 0x22,							/*APPɾ���û�*/	
		APP_REMOTR_OPEN_DOOR_COMMAND = 0x11,						/*APP����*/
		APP_QUERY_SMARTLOCK_TIME = 0x35,							/*APP��ѯ����ʱ��*/
	    SMARTLOCK_QUERY_ADMINUSER_INFO_COMMAND = 0x31,          	/*��ѯ����Ա�û���Ϣ*/
	    SMARTLOCK_QUERY_NORMALUSER_INFO_COMMAND = 0x32,        	 	/*查询锁具普�?�用户信�?*/
	    SMARTLOCK_QUERY_LOCK_INFO_COMMAND = 0x34,               	/*��ѯ������Ϣ*/
	    SMARTLOCK_QUERY_OPENDOOR_HISTORY_COMMAND = 0x36,        	/*查询�?锁历史信�?*/
	    SMARTLOCK_ADMIN_USER = 0x00,            					/*管理员用户类�?*/
	    SMARTLOCK_NORMAL_USER = 0x01,        						/*普�?�用户类�?*/
	    SMARTLOCK_TEP_USER = 0x02,        							/*临时用户类型*/
	    SMARTLOCK_TEP_CARD_USER = 0x06,								/*��ʱ��Ƭ�û�*/
	    SMARTLOCK_RESET = 0x42,               						/*恢复出厂设置*/
	    SMARTLOCK_OPRN_HISTORY = 0x41,
	/*************************RSY***************************/
		SELFPROTOCOLSL500LOCK_QUERY_TEMCARDUSER_INFO = 0x37,    	//��ѯ��ʱ��Ƭ�û���Ϣ
		SELFPROTOCOLSL500LOCK_MOD_OPEN_MODE = 0x23,             	//�޸Ŀ�����ʽ
		SELFPROTOCOLSL500LOCK_MOD_CLOSE_MODE = 0x24,            	//�޸Ĺ�����ʽ
		SELFPROTOCOLSL500LOCK_ADD_ADMIN_OR_NORMAL_USER = 0x25,  	//������ӹ���Ա����ͨ�û�
		SELFPROTOCOLSL500LOCK_USER_PASSWD_AUTHEN = 0x13,			//�û�������֤
	/******************************************************/
};


enum SMART374LOOK_COMMMAND_ID
{
		SMARTLOCK_374_TIME_SYNC_CMD = 0x12,				/*ʱ��ͬ��*/
		SMARTLOCK_374_PASSWD_AUTHEN_CMD = 0x15,						/*����Ա������֤*/	
		SMARTLOCK_374_QUERY_TEMUSER_INFO_CMD = 0x33,				/*��ѯ������ʱ�û���Ϣ*/
		SMARTLOCK_374_ADD_NEWTEMUSER_CMD = 0x21,					/*������û�*/	
		APP_374_DELETE_TEMUSER_CMD = 0x22,							/*APPɾ���û�*/	
		APP_374_REMOTR_OPENDOOR_CMD = 0x11,						/*APP����*/
		APP_374_QUERY_SMARTLOCK_TIME = 0x35,							/*APP��ѯ����ʱ��*/
	    SMARTLOCK_374_QUERY_ADMINUSER_INFO_CMD = 0x31,          	/*��ѯ����Ա�û���Ϣ*/
	    SMARTLOCK_374_QUERY_NORMALUSER_INFO_CMD = 0x32,        	 	/*查询锁具普�?�用户信�?*/
	    SMARTLOCK_374_QUERY_LOCK_INFO_CMD = 0x34,               	/*��ѯ������Ϣ*/
	    SMARTLOCK_374_QUERY_OPENDOOR_HISTORY_CMD = 0x36,        	/*查询�?锁历史信�?*/
	    SMARTLOCK_374_ADMIN_USER = 0x00,            					/*管理员用户类�?*/
	    SMARTLOCK_374_NORMAL_USER = 0x01,        						/*普�?�用户类�?*/
	    SMARTLOCK_374_TEP_USER = 0x02,        							/*临时用户类型*/
//	    SMARTLOCK_TEP_CARD_USER = 0x06,								/*��ʱ��Ƭ�û�*/
	    SMARTLOCK_374_RESET = 0x42,               						/*恢复出厂设置*/
	    SMARTLOCK_374_OPRN_HISTORY = 0x41,

		SMARTLOCK_374_QUERY_NORMALUSER_TIMELIMITED = 0x38,			//��ѯ��ͨ�û�ʹ��ʱ��
		SMARTLOCK_374_MOD_NORMALUSER_TIMELIMITED = 0x39,			//�޸���ͨ�û�ʹ��ʱ��
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




/*Э���ʽ*/
typedef struct smartlock_protocol_info
{
		int sof;											/*��ͷ��һ���ֽ�*/
		
		int len;											/*���ȣ�0x06~0x33*/
		
		int cmd1;											/*������1:Э��汾��*/

		int cmd2;                                           /*������2*/

		int src_low;                                        /*Ԥ���ֽ�:0xFF*/

		int src_high;                                       /*Ԥ���ֽ�:0xFF*/

		int dest_low;                                       /*Ԥ���ֽ�:0xFF*/
		
		int dest_high;                                      /*Ԥ���ֽ�:0xFF*/

		unsigned char msg[PRIVATE_DATA_LEN];
		
		int deviceType;

		int dataNumber;                                     /*���Ƶ�Ԫ����*/

		int type;                                           /*��������: һ���ֽ�*/

		int len1;											/*ͨ�����ݳ���*/

		int val;
		
		int clusterID_low;

		int clusterID_high;

		int attributeID_low;

		int attributeID_high;
		
		unsigned char data[PRIVATE_DATA_LEN];				/*���ݶΣ�Ϊ���߻�����ģ����Ҫ���ݵ��������ݣ�*/

		int data_unit_len;
		
		unsigned char data_unit_val[PRIVATE_DATA_LEN];		/*�Զ������ݶ�*/
		
		int fcs;											/*У�飨��ͷ�����ݶ�β���ۼӺͣ�*/	
}SMARTLOCK_PROTOCOL;


#if 0
struct tm
{
		char year;											/*��*/	
		char month;											/*��*/	
		char days;											/*��*/	
		char hour;											/*ʱ*/		
		char minute;										/*��*/	
		char second;										/*��*/
};

#endif
/*��װ������������ָ������*/
int HSZSendSmartLockData(char* dev_mac,char *data_buf);

/* ���㴫�����ݵ�Check fcs��ֵ*/
int HSZCalculateSmartLockDataCheckSum(int sof,int len,unsigned char *data_buf);					

/*������������˽������*/
int HSZAnalysisSmartLockData(unsigned char *data,char* gw_mac,char* dev_mac);							

/*������������ʱ��ͬ������*/
int HSZSmartLockTimeSync(char *data_buf,char* gw_mac,char* dev_mac);		

/*���������̶��ϱ�����*/
int HSZAnalysisSmartLockRegularData(char *data_buf,char* gw_mac,char* dev_mac);

/*���������ʱ���������������*/
int HSZReturnDataForAddTemCode(char *data_buf,char* gw_mac,char* dev_mac);

/*������ѯ�û�������������*/
int HSZReturnDataForQueryUser(char *data_buf,char* gw_mac,char* dev_mac);

/*����ɾ����ʱ���������������*/
int HSZReturnDataForDeleteTemCode(char *data_buf,char* gw_mac,char* dev_mac);

/*��������Ա������֤��������*/
int HSZReturnDataForAdminAuth(char *data_buf,char* gw_mac,char* dev_mac);

/*��������������Ϣ��������*/
int HSZReturnDataForControl(char *data_buf,char* gw_mac,char* dev_mac);

/*�ж��Ƿ�������*/
int CheckLeapYear(unsigned int year);

/*ʱ�������Ϊʱ��*/
void Hal_Time_SecondToDate(unsigned long int second,char *TimeYear,char *TimeMonth,char *TimeDate,char *TimeHour,char *TimeMinute,char *TimeSecond);

/*���ڻ���Ϊʱ���*/
unsigned long int Hal_Time_DateToSecond(const int g_ucTimeYear,const int g_ucTimeMonth,const int g_ucTimeDate,const int g_ucTimeHour,const int g_ucTimeMinute,const int g_ucTimeSecond);

/*��ѯ��ʱ�û���Ϣ*/
int HSZQueryTemUser();

/*��ѯ�����ڲ�ʱ��*/
int HSZQueryLockTime();

/*ɾ����ʱ�û�*/
int HSZDeleteTemUser(char *data_buf);

/*��װ������������ָ������*/
int HSZSendSmartLockData(char* dev_mac,char *send_buf);

/*ʱ��ͬ��*/
int HSZSmartLockTimeSynchronize(char *data_buf);

/*Զ�̿���*/
int HSZSmartLockRemoteOpenDoor(char *data_buf);

/*����Ա������֤*/
int HSZSmartLockPasswdCheck(char *data_buf);

/*�����ʱ�û�*/
int HSZAddTemUser(char *data_buf);

/*����ת�ַ���*/
void HSZConvertUnCharToStr(char* str,unsigned char* UnChar, int ucLen);

#endif
