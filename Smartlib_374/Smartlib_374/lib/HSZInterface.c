/******************************************************************************

                  ��Ȩ���� (C), 2017-2025, �ൺ�����ǻۼҾ�ϵͳ�ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : HSZInterface.c
  �� �� ��   : ����
  ��    ��   : Zigbee Project Team
  ��������   : 2017��6��12��
  ����޸�   :
  ��������   : ���ϲ�SDK�ӿں�������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ļ�

  2.��    ��   : 2017��6��23��
    ��    ��   : Zigbee Project Team
    �޸�����   : �޸��ļ��ܹ�����Common�е����ŷ��������Ƶ��˲���

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
 �� �� ��  : HSZSetCallbackOnCommand
 ��������  : ���ûص�ͨ�ô�����
 �������  : 
			char* gw_mac:����MAC��ַ
			int callbacktype���첽�ص�����
			void* p���첽�ص�����ָ��         
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��5��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZDeviceError
 ��������  : �����ϲ������Ϣ
 �������  : int errtype        
             const char* reason  
             const char*others  
 �������  : ��
 �� �� ֵ  :  
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZRemoveOneDevice
 ��������  : ɾ���豸
 �������  : 
			char* gw_mac�� ����MAC��ַ
			int nnodeid:�豸nodeid
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZPermitPJoinGatewayOn
 ��������  : �����豸����
 �������  : int seconds����������ʱ��
			 char* gw_mac�� ����MAC��ַ
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��13��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZPermitPJoinGatewayOff
 ��������  : �ر��豸����
 �������  : 
			char* gw_mac ����MAC��ַ
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��13��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZReformNetwork
 ��������  : Reform network
 �������  : 
			char* gw_mac: ����MAC��ַ
			int channel  :�㲥�ŵ�
			int tx_power :���书��
			int panid    :pan id
 �������  : ��
 �� �� ֵ  : 
 ����˵��  : ���panid Ϊ0�������network find unused ��������
             ������ô���Ĳ�����ʹ��network form ��������
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��13��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

  2.��    ��   : 2017��7��5��
    ��    ��   : Zigbee Project Team
    �޸�����   : ȥ���Զ���������ӿ�

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
 �� �� ��  : HSZReformNetworkAuto
 ��������  : �Զ��������磬����Ҫ�趨����
 �������  : 
			char* gw_mac�� ����MAC��ַ  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZGetDeviceList
 ��������  : ��ȡ�豸�б�
 �������  : 
			char* gw_mac�� ����MAC��ַ
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��20��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZGetGatewayState
 ��������  : ������ȡ�豸��ǰ״̬
 �������  : 
			char* gw_mac�� ����MAC��ַ
 �������  : ��
 �� �� ֵ  :  
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��20��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZGetGatewayNetworkInfo
 ��������  : ������ȡ��ǰ�����networkid��channel���ź�ǿ�ȵ���Ϣ
 �������  :
			char* gw_mac�� ����MAC��ַ
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��20��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZSetGatewayFactory
 ��������  : �������ع�����λ
 �������  : 
			char* gw_mac�� ����MAC��ַ
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��28��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

  2.��    ��   : 2017��6��29��
    ��    ��   : Zigbee Project Team
    �޸�����   : �޸Ļָ�������λģʽΪ���Զ��������磬���ǲ���������

  3.��    ��   : 2017��7��6��
    ��    ��   : Zigbee Project Team
    �޸�����   : �������ۣ��޸�Zigbee����״̬Ϊ��Ĭ������

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
 �� �� ��  : HSZGetNcpOtaStatus
 ��������  : ��ȡNCP OTA״̬
 �������  : char* gw_mac  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZGetNcpOtaStatus(char* gw_mac)
{
	return ncp_on_ota_mode;
}


/*****************************************************************************
 �� �� ��  : HSZSetNCPOTAMode
 ��������  : ����NCP OTAģʽ
 �������  : 
			char* gw_mac�� ����MAC��ַ
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��28��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZSetNCPOTAMode(char* gw_mac,char * ncp_firmware_name)
{
	int rc = -1;
	
	char cmd[300] = {0};
	
	ncp_on_ota_mode = NCP_ON_OTA_MODE;
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"NCP OTA Mode On.\n");
	
	system("./lib/hagateway/Restart_HaGatewayReferenceHost");/*ֹͣHost����*/
	
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
 �� �� ��  : HSZRestartZigBeeService
 ��������  : ����Zigbee����
 �������  : 
			char* gw_mac�� ����MAC��ַ
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��4��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZGetZigbeeStackVersion
 ��������  : ��ȡZigbeeЭ��ջ�汾��Ϣ
 �������  : 
			char* gw_mac�� ����MAC��ַ
 �������  : ��
 �� �� ֵ  :  
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��4��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZCommandsSend
 ��������  : ͨ�ÿ�������:��Ҫ���ݲ�ͬ�Ĵ���������������ָ����Ͷ�Ӧ�豸
 �������  :
			char* gw_mac������MAC��ַ
			char* device_id���豸ID
			char* cmd������ָ��    
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��6��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
	
	/*��ȡcommmad device type value*/
	strncpy(device_type_buf,cmd,4);
	
	device_type = strtol(device_type_buf,NULL,16);

	DEBUG_ZONE(LOG_ZONE_TEST,"device_type:0x%04x.\n",device_type);
	

	/*��ȡcommmad cmd type value*/	
	strncpy(cmd_buf,&cmd[4],2);
	
	cmd_type = strtol(cmd_buf,NULL,16);
	
	DEBUG_ZONE(LOG_ZONE_TEST,"cmd_buf:0x%02x.\n",cmd_type);
	
	/*��ȡMQTT Topic*/	
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
 �� �� ��  : HSZCommandsActiveReturn
 ��������  : ͨ�����ݶη���:��Ҫ���ݲ�ͬ�Ĵ������������������������ӡ����
 �������  :
 			char* device_mac: �豸MAC
			char* cmd��ģ���豸�������"�豸����"+"CMD2"+"���ݶ�����"��    
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��1��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZCommandsPassiveReturn
 ��������  : ͨ�����ݶη���:��Ҫ���ݲ�ͬ�Ĵ������������������������ӡ����
 �������  :
 			char* device_mac: �豸MAC
			char* cmd��ģ���豸�������"�豸����"+"CMD2"+"���ݶ�����"��    
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��3��1��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
					//�������غ���
			}
			break;

			default:
			break;
	}
	
	return 0;
}
*/


/*****************************************************************************
 �� �� ��  : HSZGetDevSignalValue
 ��������  : ��ȡ�豸�ķ����ź�ǿ��
 �������  : 
			char* gw_mac������MAC��ַ
			char* device_mac���豸ID
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��6��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZGetDevSignalValue(char* gw_mac,char* device_mac)
{
	int rc = -1;
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"Get device signal value.\n");

	return rc;
}
/*****************************************************************************
 �� �� ��  : HSZGetOnlineDeviceList
 ��������  : ��ȡ���������豸�б�
 �������  : 
			char* gw_mac������MAC��ַ
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��6��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZGetOnlineDeviceList(char* gw_mac)
{
	int rc = -1;
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"HSZGetOnlineDeviceList.\n");
	
	return rc;
}

/*****************************************************************************
 �� �� ��  : 
 ��������  : ���ö���Topic����
 �������  : 
			char* gw_mac������MAC��ַ
			int topic_index�����ö���Topic����
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��10��25��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZSetSubTopic(char* gw_mac,int topic_index)
{
	sub_topic = topic_index;
	
	return sub_topic;
}
/*****************************************************************************
 �� �� ��  : HSZFindDevice
 ��������  : �����豸
 �������  : 
			char* gw_mac������MAC��ַ
			char* device_mac���豸ID
			int time�����ú��豸��˸������
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��6��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZFindDevice(char* gw_mac, char* device_mac, int time)
{
	int rc = -1;
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"HSZFindDevice.\n");
	
	return rc;
}

/*****************************************************************************
 �� �� ��  : HSZRefreshDevice
 ��������  : ˢ���豸
 �������  : 
			char* gw_mac������MAC��ַ
			char* device_mac���豸ID
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��6��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZRefreshDevice(char* gw_mac, char* device_mac)
{
	int rc = -1;
	
	DEBUG_ZONE(LOG_ZONE_PUBLIC,"HSZRefreshDevice.\n");
	
	return rc;
}

/*****************************************************************************
 �� �� ��  : HSZGetNcpVersion
 ��������  : ��ȡNCP�İ汾
 �������  : 
			char* gw_mac������MAC��ַ
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��3��13��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZSetHuayiDeviceIcon
 ��������  : ���û��࿪��ͼ��
 �������  : 
            char* gw_mac:  ����MAC ID
			char* dev_mac���豸ID
			int index:�豸 endpoint
			int icon_id   ͼ��ID
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��1��13��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZGetNcpVersion
 ��������  : ���û��࿪������
 �������  : 
            char* gw_mac:  ����MAC ID
			char* dev_mac���豸ID
			int index:�豸 endpoint
		    char* switch_name ����ͼ������(unicode����)��
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��1��13��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
 �� �� ��  : HSZGetNcpVersion
 ��������  : ���û��࿪������
 �������  : 
            char* gw_mac:  ����MAC ID
			char* dev_mac���豸ID
			int seconds���������ʱ�䣨0xFF�����ü�����0x00����ֹ������
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��1��13��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

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
