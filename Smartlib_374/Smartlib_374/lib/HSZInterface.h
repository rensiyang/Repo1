/******************************************************************************

                  ��Ȩ���� (C), 2017-2025, �ൺ�����ǻۼҾ�ϵͳ�ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : HSZInterface.h
  �� �� ��   : ����
  ����  ��   : Zigbee Project Team
  ��������   : 2017��6��12��
  ����޸�   :
  ��������   : ���ϲ�SDK�ӿں�������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ļ�
	
	��������
		1.�ص����� :HSZCallbak_****		
		2.�ӿں���	:HSZ_****		
		3.ö��	:HSZN_***	

	��������		
		1.mac��ַ��ȫ��ʹ�ô�д������0Xǰ׺������16���ַ�������	

  2.��    ��   : 2017��6��23��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����˻ص������Ϳ��ƽӿ�

  3.��    ��   : 2017��7��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : ���ݻ������������޸�
		
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
*��������
*
********************************************************************/

	/* �����ϲ�ص����� */
	#define APP_CALLBACK_SUBSCRIBE_MSG  		    1     	
	#define APP_CALLBACK_ON_ERROR     	            2	 	/* ������ϢCallback     		*/
	#define APP_CALLBACK_GATEWAY_STATE    	        3	 	/* ����״̬Callback     		*/

	#define APP_CALLBACK_DEVICE_JOIN      			4   	/* �豸����Callback     		*/
	#define APP_CALLBACK_DEVICE_LEAVE				5   	/* �豸����Callback     		*/
	#define APP_CALLBACK_DEVICE_LIST                6   	/* �豸�б�Callback     		*/
	#define APP_CALLBACK_CMD_SEND                	7   	/* �����Callback     		*/
	#define APP_CALLBACK_DEVICE_LINE_STATE          8   	/* �豸��������״̬Callback     */

	#define APP_CALLBACK_GATEWAY_PERMIT_JOIN_STATE  9		/* �����Ƿ���������Callback     */

	#define APP_CALLBACK_COMMAND_EXECUTE            10		/* ����ִ��Callback     		*/
	#define APP_CALLBACK_DEV_SIGNAL_VALUE           11		/* ��ȡ�豸�ź�ǿ��Callback		*/
	#define APP_CALLBACK_DEV_ONLINE_LIST            12      /* �����豸�б�Callback			*/
	#define APP_CALLBACK_DEV_DATA                   13      /* �ϱ��豸����Callback 		*/
	#define APP_CALLBACK_COMMAND_FAILED             14      /* �ϲ��������ִ��ʧ��Callback */
	#define APP_CALLBACK_UPDATE_NCP                 15      /*  NCP�����������             */


	/* ������Ϣ���� */
	#define	APP_TYPE_TOPIC_ERR                      0       /* ����������趨�Ļ�Ĭ��Ϊ������ */
	#define APP_TYPE_TOPIC_HEARTBEAT                1     	/* HEARTBEAT���� */
	#define	APP_TYPE_TOPIC_DEVICES                  2		/* DEVICES��������� */
	#define	APP_TYPE_TOPIC_ZCLRESPONSE              3		/* ZCLRESPONSE��������� */
	#define APP_TYPE_TOPIC_DEVICESJOINED            4		/* DEVICESJOINED��������� */
	#define	APP_TYPE_TOPIC_DEVICELEFT               5		/* DEVICELEFT��������� */
	#define	APP_TYPE_TOPIC_DEVEICE_STATE_CHANGE     6		/* DEVEICE_STATE_CHANGE��������� */
	#define	APP_TYPE_TOPIC_RELAYS                   7		/* RELAYS��������� */
	#define	APP_TYPE_TOPIC_SETTINGS                 8		/* SETTINGS��������� */
	#define	APP_TYPE_TOPIC_EXECUTED                 9		/* EXECUTED��������� */
	#define	APP_TYPE_TOPIC_OTAEVENT                 10		/* OTAEVENT��������� */
	#define	APP_TYPE_TOPIC_APSRESPONSE              11		/* APSRESPONSE��������� */
	#define	APP_TYPE_TOPIC_DEVLINESTCH              12		/* DEVLINESTCH��������� */
	
	/* HSZReformNetwork�趨����*/
    #define GATEWAY_MAX_POWER           			20
	#define GATEWAY_MID_POWER           			14
	#define GATEWAY_MIN_POWER           			8

/*******************************************************************
*
*��ʼ�������ӿ�
*
********************************************************************/
	
	/* 
        ��������  :ϵͳ��ʼ���������ڵ���֮ǰ�������ûص�����
        �������  : void  
        �������  : ��
        �� �� ֵ  : int�� �ɹ�����0�����򷵻�-1
	*/
	int HSZInit(void);
	
	/* 
        ��������  :ϵͳ�����˳����Ͽ���MQTT Broker����
        �������  : void  
        �������  : ��
        �� �� ֵ  : int�� �ɹ�����0�����򷵻�-1
	*/
	int HSZDeinit(void);

	
/*******************************************************************
*
*�ϲ㺯���ӿڶ���
*
********************************************************************/
   
    /* 
        ��������: ��������ص�����
        �������  : 
					char* gw_mac:����MAC��ַ
					int errtype :��������                         
        �������  : ��
        �� �� ֵ  : ��
    */
	typedef void HSZCallbackOnError(char* gw_mac,int errtype);

	/*
        ��������: �����豸����״̬
        �������  : 
					char* gateway_mac:����MAC��ַ
					int gateway_state:0��ʾ����NG��1��ʾ����OK��-1��ʾϵͳNG
					                  99 ��ʾNCP�����ɹ���98��ʾNCP����ʧ��
        �������  : ��
        �� �� ֵ  : ��
    */
    typedef void HSZCallbackGatewayState(char* gw_mac,int gw_state);
	
	/*
	    ��������: �����豸�����Ļص�
        �������  : 
					char* gw_mac:����MAC��ַ				               
                    char* dev_type���豸����
                    char* dev_mac���豸MAC��ַ
                    int dev_endpoint���豸�˵������ 
        �������  : ��
        �� �� ֵ  : ��
	*/
	typedef void HSZCallbackOnDeviceJoin(char* gw_mac,char* dev_type,char* dev_mac,int endpoint_count);

    /*
        �������� : �豸�����ص��������豸��������
        �������  : 
					char* gw_mac:����MAC��ַ 
                    char* dev_mac ���豸MAC��ַ
        �������  : ��
        �� �� ֵ  : ��
    */
	typedef void HSZCallbackOnDeviceLeave(char* gw_mac,char* dev_mac);


	 /*
        �������� : �豸��������״̬�ص�
        �������  : 
		            char* gw_mac:����MAC��ַ
					char* dev_mac: �豸nodeid   
					int dev_state���豸����,0x01�����豸���ߣ�0x00�����豸����
        �������  : ��
        �� �� ֵ  : ��
    */
	typedef void HSZCallbackOnDeviceLineState(char* gw_mac,char* device_mac,int dev_state);
	
	/*
		�������� : ����ֹͣ�豸�����ص�
		�������  : 
					char* gw_mac:����MAC��ַ
					permit_state: 0x00,ֹͣ����������0x01��������
		�������  : ��

		�� �� ֵ  : ��
	*/
	typedef void HSZCallbackGatewayPermitJoinState(char* gw_mac,int permit_state);

	/*
		�������� : �����е��豸�������ϱ�ʱ�ص��ú���
		�������  : 
					char* gw_mac:����MAC��ַ
					char* device_mac:�豸mac��ַ
					char* data:�豸����
		�������  : ��

		�� �� ֵ  : ��
	*/
	typedef void HSZCallbackDeviceData(char* gw_mac,char* device_mac,char* data);
	
	/*
		�������� : ��������ִ�г����ϱ��ص�
		�������  : 
		char* gw_mac:����MAC��ַ
		char* device_mac:�豸mac��ַ
		char* data:�豸����
		�������  : ��

		�� �� ֵ  : ��
	*/
	typedef void HSZCallbackCommandFailed(char* gw_mac,char* device_mac,int data);
	
	/*
		�������� : NCP�����������
		�������  : 
		char* gw_mac:����MAC��ַ
		char* data:�豸����
		�������  : ��

		�� �� ֵ  : ����0x66���������ɹ�������0x11��ʾ����ʧ�ܣ�
	*/
	typedef void HSZCallbackNcpUpdateResult(char* gw_mac,int data);
/*******************************************************************
*
*�ϲ�ص���������
*
********************************************************************/
	
	/*
	    ��������  : ���û�ȡ����ִ�еĻص�����
        �������  : 
					int callbacktype���첽�ص����ͣ��ο������ϲ�ص�����
					void* p���첽�ص�����ָ��
        �������  : ��
        �� �� ֵ  : int�ͣ�����0
	*/
	
	int HSZSetCallbackOnCommand(int callbacktype,void* p);
	
/*******************************************************************
*
*���ƺ����ӿ�
*
********************************************************************/

    /*
        ��������  : �����豸����
        �������  : 
					char* gw_mac: ����MAC��ַ
					int seconds:��������������,�Ϊ254s��������ڴ�ֵ��ѡ�����ø�ֵΪ0xFF��Ϊ������������
					ͬʱ���ϲ㷢��HSZPermitJoinOff����ֹͣ����
        �������  : ��
        �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZPermitJoinOn(char* gw_mac,int seconds);

    
    /*
        ��������  : ��ֹ�豸����
        �������  : 
					char* gw_mac :����MAC��ַ
        �������  : ��
        �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZPermitJoinOff(char* gw_mac);

    /*
         ��������  : ���½�������
         �������  : 
					char* gw_mac: ����MAC��ַ
					int channel:�㲥Ƶ�������Ƽ� 
					int tx_power: TX���书��(��Ҫ�趨�Ƽ�ֵ)
					int panid: PID��0x0001-0x7FFF��
         �������  : ��
         �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZReformNetwork(char* gw_mac,int channel,int tx_power,int panid);

	 /*
         ��������  : �Զ����½������磨�������������
         �������  : 
					char* gw_mac ����MAC��ַ
         �������  : ��
         �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZReformNetworkAuto(char* gw_mac);
	
	/*
         ��������  : ������ȡ����״̬
         �������  : 
					char* gw_mac�� ����MAC��ַ
         �������  : ��
         �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZGetGatewayState(char* gw_mac);
	 
    /*
         ��������  :������ȡ��ǰ�����channel�ź�ǿ�ȵ���Ϣ
         �������  : 
					 char* gw_mac�� ����MAC��ַ
         �������  : ��
         �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZGetGatewayNetworkInfo(char* gw_mac);
    
    /*
         ��������  : ɾ���豸
         �������  : 
					char* gw_mac�� ����MAC��ַ
					char* dev_mac: ɾ���豸��MAC��ַ
         �������  : ��
         �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZRemoveDevice(char* gw_mac,char* dev_mac);

    
    /*
         ��������  : �ָ���������
         �������  : 
					char* gw_mac�� ����MAC��ַ
         �������  : ��
         �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZSetGatewayFactory(char* gw_mac);

	
    /*
         ��������  : OTAģʽ����
         �������  : 
					char* gw_mac�� ����MAC��ַ
					char* ncp_firmware_name ����NCP�������
         �������  : ��
         �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZSetNCPOTAMode(char* gw_mac,char * ncp_firmware_name);
	
	
	/* 
		��������  :��ȡ����ZigbeeЭ��ջ�汾������NCP����Host�ˣ���������ZigbeeЭ��ջҪƥ�䣩
		�������  : 
				   char* gw_mac�� ����MAC��ַ
        �������  : ��
        �� �� ֵ  : char�ַ����ͣ���������ZigbeeЭ��ջ�汾
	*/
	char* HSZGetZigbeeStackVersion(char* gw_mac);
	
	/* 
		��������  :����ͨ�ÿ���ָ��
		�������  :
					char* gw_mac������MAC��ַ
					char* dev_mac���豸ID
					char* cmd������ָ��
        �������  : ��
        �� �� ֵ  : int�ͣ�������-1������ֵ����-1
	*/
	int HSZCommandsSend(char* gw_mac,char* dev_mac,char* cmd);
	
	/* 
		��������  :��֤ͨ���ϱ�ָ��
		�������  :
					char* gw_mac������MAC��ַ
					char* dev_mac���豸ID
					char* cmd������ָ��
        �������  : ��
        �� �� ֵ  : int�ͣ�������-1������ֵ����-1
	*/

	int HSZCommandsActiveReturn(char* device_mac,char* cmd);
	
	/* 
		��������  :��ȡ�豸�ķ��书��ֵ,�첽�ص��ϱ�
		�������  : 
					char* gw_mac������MAC��ַ
					char* dev_mac���豸ID
        �������  : ��
        �� �� ֵ  :
	*/
	int HSZGetDevSignalValue(char* gw_mac,char* dev_mac);
	
    /* 
        ��������  :��ȡ����NCP OTA״̬
        �������  :
            		char* gw_mac������MAC��ַ,ͨ���ص�����OTA״̬
        �������  : ��
        �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZGetNcpOtaStatus(char* gw_mac);
	
	/* 
        ��������  :��ȡ����NCP�汾
        �������  :
            		char* gw_mac������MAC��ַ,ͨ���ص�����NCP�汾
        �������  : ��
        �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZGetNcpVersion(char* gw_mac);
	
	/* 
        ��������  :���û��࿪��ͼ��
        �������  :
					char* gw_mac:  ����MAC ID
            		char* dev_mac���豸ID
					int index:�豸 endpoint
					int icon_id    ͼ��ID
					
        �������  : ��
        �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZSetHuayiDeviceIcon(char* gw_mac,char* dev_mac,int index,int icon_id);
	
	
	/* 
        ��������  :���û��࿪������
        �������  :
					char* gw_mac�� ����MAC ID
            		char* dev_mac���豸ID
					int index:�豸 endpoint
					char* switch_name ����ͼ�����ƣ�
        �������  : ��
        �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZSetHuayiDeviceName(char* gw_mac,char* dev_mac,int index,char* switch_name);
	
	
	/* 
        ��������  :���û��࿪������
        �������  :
					char* gw_mac�� ����MAC ID
            		char* dev_mac���豸ID
					int seconds���������ʱ��
					
        �� �� ֵ  : int�ͣ�������-1������ֵ����-1
    */
	int HSZJoinDeviceByScanCode(char* gw_mac,char* dev_mac,int seconds);
	
	
	
	
	
#endif	
