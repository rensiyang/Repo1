/******************************************************************************

                  ��Ȩ���� (C), 2017-2025, �ൺ�����ǻۼҾ�ϵͳ�ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : HSZSDKTest.c
  �� �� ��   : ����
  ��    ��   : Zigbee Project Team
  ��������   : 2017��6��12��
  ����޸�   :
  ��������   : ���Ժ������

  �޸���ʷ   :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ļ�

  2.��    ��   : 2017��6��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����˲��ֲ��Խӿڣ������˶Խӿں�����ʵ���������ڶ�ֻ����
                 ʱ��ӡ��ʾ����

******************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <stdarg.h>

#if !defined(WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "HSZInterface.h"
#include "HSZSDKTest.h"

#define GATEWAY_PID_MIN   	0x0100
#define GATEWAY_PID_MAX   	0x7FFF

#define NCP_OTA_FIRMWARE_NAME  "ncp-uart-rts-cts-use-with-serial-uart-btl-5.10.1.ebl"

#define DEVID_XU "00124B0016027E09"
#define DEVID_RSY "00124B0014655AC1"
//#define DEVID_XU "00124B0016027B08" 

/* APP��������ص����� */
void AppCallbackOneerror(char* gw_mac,int errtype);
/*APP����״̬�ص����� */
void AppCallbackGatewayState(char* gw_mac,int gw_state);

/*APP�����豸�����Ļص� */
void AppCallbackOnDeviceJoin(char* gw_mac,char* dev_type,char* dev_mac,int endpoint_count);
/*APP�豸�����ص�*/
void AppCallbackOnDeviceLeave(char* gw_mac,char* dev_mac);

/*APP�豸��������״̬�ص� */
void AppCallbackOnDeviceLineState(char* gw_mac,char* device_mac,int dev_state);
/* �豸�Ƿ����������ص� */
void AppCallbackGatewayPermitJoinState(char* gw_mac,int permit_state);

/* ��ȡ�豸���� */
void AppCallbackDeviceData(char* gw_mac,char* device_mac,char* data);

/*��ȡ����ִ��Commandʧ��*/
void AppCallbackCommandFailed(char* gw_mac,char* device_mac,int data);

void AppCallbackNcpUpdateResult(char* gw_mac,int data);

/* �����������PID */
int APPGetGatewayPanid(int min,int max);

char device_eui[17]= {0};
extern char* HSZRemoveMacAddr0X(char*mac);

int device_nodeid = 0; 
int device_endpoint = 0;

int test_temp = 0;

extern char app_value[512];
extern int HSZGetNcpOtaStatus(char* gw_mac);

char app_datestr[16];
char app_timestr[16];

static int test_index = 0;

char get_gateway_mac[17];			/*������ȡ����MAC��ַ*/

char* AppGatewayMac(void);
/*****************************************************************************
 �� �� ��  : HSZAppSetCallback
 ��������  : ���ûص�������ʼ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : static
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��23��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

  2.��    ��   : 2017��7��5��
    ��    ��   : Zigbee Project Team
    �޸�����   : �޸�Ϊͨ������Callback�����ӿ�

*****************************************************************************/
static void HSZAppSetCallback(void)
{
		HSZSetCallbackOnCommand(APP_CALLBACK_ON_ERROR,(void*)AppCallbackOneerror);
		
		HSZSetCallbackOnCommand(APP_CALLBACK_GATEWAY_STATE,(void*)AppCallbackGatewayState);
		
		HSZSetCallbackOnCommand(APP_CALLBACK_DEVICE_JOIN,(void*)AppCallbackOnDeviceJoin);
		HSZSetCallbackOnCommand(APP_CALLBACK_DEVICE_LEAVE,(void*)AppCallbackOnDeviceLeave);
		
		HSZSetCallbackOnCommand(APP_CALLBACK_DEVICE_LINE_STATE,(void*)AppCallbackOnDeviceLineState);
		
		HSZSetCallbackOnCommand(APP_CALLBACK_GATEWAY_PERMIT_JOIN_STATE,(void*)AppCallbackGatewayPermitJoinState);
			
		HSZSetCallbackOnCommand(APP_CALLBACK_DEV_DATA,(void*)AppCallbackDeviceData);
		HSZSetCallbackOnCommand(APP_CALLBACK_COMMAND_FAILED,(void*)AppCallbackCommandFailed);
		HSZSetCallbackOnCommand(APP_CALLBACK_UPDATE_NCP,(void*)AppCallbackNcpUpdateResult);
} 

/*****************************************************************************
 �� �� ��  : AppLock
 ��������  : дlog����
 �������  : CRITICAL_SECTION *l  
 �������  : ��
 �� �� ֵ  :  
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void AppLock(CRITICAL_SECTION *l) 
{
    pthread_mutex_lock(l);
}
/*****************************************************************************
 �� �� ��  : AppUnlock
 ��������  : дlog����
 �������  : CRITICAL_SECTION *l  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void AppUnlock(CRITICAL_SECTION *l) 
{
    pthread_mutex_unlock(l);
}

/*****************************************************************************
 �� �� ��  : APP_DEBUG
 ��������  : дlog�����ӿں���
 �������  : int zone            
             const char *file    
             const char *func    
             int line            
             const char *pszFmt  
             ...                 
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void APP_DEBUG(int zone,const char *file,const char *func, int line,const char *pszFmt,...) 
{
		struct tm *now;
	    struct timeb tb;
	    va_list argp;
		char output[1024] = {0};
		
		if(HSZSDKDebugZone(zone))
		{
			ftime(&tb);
			now=localtime(&tb.time);
			sprintf(app_datestr,"%04d-%02d-%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday);
			sprintf(app_timestr,"%02d:%02d:%02d",now->tm_hour     ,now->tm_min  ,now->tm_sec );
			
			AppLock(&app_cs_log);
			va_start(argp,pszFmt);
			
			if(0x02 == HSZGetAppDebugOnOFF(app_value))
			{
					if (NULL!=App_fp) 
					{
						fprintf(App_fp,"%s %s",app_datestr,app_timestr);
						snprintf(output,1023," <%s> [%s] %05d: ",file,func,line);
						fprintf(App_fp,"%s",output);
						vfprintf(App_fp,pszFmt,argp);
						
						if (ftell(App_fp)>MAXLOGSIZE) 
						{
								fclose(App_fp);
								if (rename(logfilename1,logfilename2)) 
								{
										remove(logfilename2);
										rename(logfilename1,logfilename2);
								}
								App_fp=fopen(logfilename1,"a");
								if (NULL==App_fp)
								{
										return;
								}
						}
					}
			}
		    else if(0x01 == HSZGetAppDebugOnOFF(app_value))
			{	
					printf("%s %s",app_datestr,app_timestr);
					snprintf(output,1023," <%s> [%s] %05d: ",file,func,line);
					printf("%s",output);
					vprintf(pszFmt,argp);
			}
			else
			{
				;
			}
			
			va_end(argp);
			AppUnlock(&app_cs_log);
		}
}

/*****************************************************************************
 �� �� ��  : HSZSetLogFileInit
 ��������  : APP log��Ϣ��ʼ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : static
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
static void HSZSetLogFileInit(void)
{	
		/*���ļ� */
		App_fp = fopen(logfilename1,"a");
		
		getlogset();
		
		pthread_mutex_init(&app_cs_log,NULL);
}
/*****************************************************************************
 �� �� ��  : HSZSetLogFileClose
 ��������  : APP log��Ϣ�ر�
 �������  : void  
 �������  : ��
 �� �� ֵ  : static 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��9��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
static void HSZSetLogFileClose(void)
{
		/*�ر��ļ� */
		fclose(App_fp);
}



char* AppGatewayMac(void)
{
		if (NULL != get_gateway_mac)
		{
				DEBUG_APP_ZONE(LOG_APP_TEST,"The gatway id :%s.\n",get_gateway_mac);
		}
		return get_gateway_mac;
}

void DoTestCommand(int      ch)
{
				//ch = getchar();
		        DEBUG_APP_ZONE(LOG_APP_TEST,"-------Receive date;%d.\n",ch);

					switch(ch)
					{
							case 'a':/* �����豸���� */
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"PermitJopinOn.\n");
									HSZPermitJoinOn(AppGatewayMac(),127);
							}
							break;

							case 'b':/* ֹͣ�豸���� */
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"PermitJopinOff.\n");
									HSZPermitJoinOff(AppGatewayMac());
							}
							break;	
							
							case 'c':/* ���½������� */
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Reform Network using APPGetGatewayPanid.\n");
									HSZReformNetwork(AppGatewayMac(),25,2,APPGetGatewayPanid(GATEWAY_PID_MIN,GATEWAY_PID_MAX));
							}
							break;
							
							case 'd':/* ���½������磨�Զ��� */
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"HSZReformNetwork using network find unused.\n");
									HSZReformNetworkAuto(AppGatewayMac());
							}
							break;
									
						/*
							case 'e':// ��ȡ�豸�б� 
							case 'E':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"HSZPrintDeviceBufInfo.\n");
									HSZPrintDeviceBufInfo();
							}
							break;
							
							case 'f':// ��ȡ�豸״̬ 
							case 'F':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"HSZGetGatewayState.\n");
									HSZPrintLineOnDeviceStatusInfo();
							}
							break;
						
							case 'h':// ɾ���豸 
							case 'H':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"HSZRemoveDevice.\n");
									HSZRemoveDevice(AppGatewayMac(),"00124B0014C6258E");
							}
							break;
							
							case 'i':// ���ػָ��������� 
							case 'I':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"HSZRemoveDevice.\n");
									HSZRemoveDevice(AppGatewayMac(),"00124B000FA4ACC2");
							}
							break;
							
							case 'j':// ��������OTAģʽ 
							case 'J':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"HSZSetNCPOTAMode:%s.\n",NCP_OTA_FIRMWARE_NAME);
									DEBUG_APP_ZONE(LOG_APP_TEST,"Waring Device Control Test.\n");
									
									char cmd[40] = {0};
									char tmp[10] = {0};
									sprintf(cmd,"%04x",0x0403);
									sprintf(tmp,"%02x",0x01);
									strcat(cmd,tmp);
									sprintf(tmp,"%02x",0x08);//*8���ֽڳ���
									strcat(cmd,tmp);
									
									sprintf(tmp,"%02x",0x01);//Index
									strcat(cmd,tmp);
									
									sprintf(tmp,"%02x",0x02);//Warning Mode,
									strcat(cmd,tmp);
									
									sprintf(tmp,"%02x",0x01);//Strobe
									strcat(cmd,tmp);
									
									sprintf(tmp,"%02x",0x01);//Strobe Level
									strcat(cmd,tmp);
									
									sprintf(tmp,"%04x",0x001E);//Warning Duration(in seconds)
									strcat(cmd,tmp);
									
									sprintf(tmp,"%02x",0x50);//Strobe Duty Cycle
									strcat(cmd,tmp);
									
									sprintf(tmp,"%02x",0x01);//Strobe Level
									strcat(cmd,tmp);
									
									HSZCommandsSend(AppGatewayMac(),"00124B00170EF079",cmd);//000D6F000DD4A8F9 -00124B00061EC850  
							}
							break;
							
							case 'k':// ��������Zigbee����
							case 'K':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"HSZRestartZigBeeService.\n");
									DEBUG_APP_ZONE(LOG_APP_TEST,"HSZSetNCPOTAMode:%s.\n",NCP_OTA_FIRMWARE_NAME);
									DEBUG_APP_ZONE(LOG_APP_TEST,"Waring Device Control Test.\n");
									
									char cmd[40] = {0};
									char tmp[10] = {0};
									sprintf(cmd,"%04x",0x0403);
									sprintf(tmp,"%02x",0x01);
									strcat(cmd,tmp);
									sprintf(tmp,"%02x",0x08);//8���ֽڳ���
									strcat(cmd,tmp);
									
									sprintf(tmp,"%02x",0x01);//Index
									strcat(cmd,tmp);
									
									sprintf(tmp,"%02x",0x00);//Warning Mode,
									strcat(cmd,tmp);
									
									sprintf(tmp,"%02x",0x00);//Strobe
									strcat(cmd,tmp);
									
									sprintf(tmp,"%02x",0x00);//Strobe Level
									strcat(cmd,tmp);
									
									sprintf(tmp,"%04x",0x0000);//Warning Duration(in seconds)
									strcat(cmd,tmp);
									
									sprintf(tmp,"%02x",0x00);//Strobe Duty Cycle
									strcat(cmd,tmp);
									
									sprintf(tmp,"%02x",0x00);//Strobe Level
									strcat(cmd,tmp);
									
									HSZCommandsSend(AppGatewayMac(),"00124B00170EF079",cmd);//000D6F000DD4A8F9 -00124B00170EF079
							}
							break;
							
							case 'l'://��ȡZigbeeЭ��ջ�汾  
							case 'L':
							{
									// DEBUG_APP_ZONE(LOG_APP_TEST,"The nodeid:0x%04x.\n",HSZFindNodeidFromInfoBuffer("000B57FFFE6E3D4C"));
									HSZGetNcpVersion(AppGatewayMac());
							}
							break;
							
							case 'm'://����ͨ���豸����ָ��  
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Toggle.\n");
									
									char cmd[30] = {0};
									char tmp[10] = {0};
									sprintf(cmd,"%04x",0x000C);
									sprintf(tmp,"%02x",0x01);
									strcat(cmd,tmp);
									sprintf(tmp,"%02x",0x01);//һ���ֽڳ���
									strcat(cmd,tmp);
									sprintf(tmp,"%02x",0x01);//����Data
									strcat(cmd,tmp);
									HSZCommandsSend(AppGatewayMac(),"00124B001709A005",cmd);
							}
							break;
							
							case 'n'://��ȡ�豸�źŷ��书��  
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Set switch Off.\n");	
									char cmd[30] = {0};
									char tmp[10] = {0};
									sprintf(cmd,"%04x",0x000C);
									sprintf(tmp,"%02x",0x02);
									strcat(cmd,tmp);
									sprintf(tmp,"%02x",0x01);//һ���ֽڳ���
									strcat(cmd,tmp);
									sprintf(tmp,"%02x",0x01);//����Data
									strcat(cmd,tmp);
								   
									HSZCommandsSend(AppGatewayMac(),"00124B001709A005",cmd);//00124B0008ED6679
							}
							break;
							
							case 'o'://��ȡ�豸�źŷ��书��  
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Set switch On.\n");
									char cmd[30] = {0};
									char tmp[10] = {0};
									sprintf(cmd,"%04x",0x0203);
									sprintf(tmp,"%02x",0x03);
									strcat(cmd,tmp);
									sprintf(tmp,"%02x",0x01);//һ���ֽڳ���
									strcat(cmd,tmp);
									sprintf(tmp,"%02x",0x01);//����Data
									strcat(cmd,tmp);
								   
									HSZCommandsSend(AppGatewayMac(),"00124B0008EB3F43",cmd);
							}
							break;
							
							case 'p'://��ȡ�翪�� 
							{
					                DEBUG_APP_ZONE(LOG_APP_TEST,"1-Open.\n");
									char cmd[30] = {0};
					                strcpy(cmd,"0000010101");
					                HSZCommandsSend(AppGatewayMac(),"00124B0014C6258E",cmd);           
							}
							break;
						
							case 'r'://�ر�ȡ�翪��
							{
					                DEBUG_APP_ZONE(LOG_APP_TEST,"1-Close.\n");
									char cmd[32] = {0};
					                strcpy(cmd,"0000020101");
					                HSZCommandsSend(AppGatewayMac(),"00124B0014C6258E",cmd);           
							}
							break;
							
							case 'M':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"2-Open.\n");
									char cmd[32] = {0};
					                strcpy(cmd,"0000010102");
					                HSZCommandsSend(AppGatewayMac(),"00124B0014C6258E",cmd);    
							}
							break;
							
							case 'N':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"2-Close.\n");
									char cmd[32] = {0};
					                strcpy(cmd,"0000020102");
					                HSZCommandsSend(AppGatewayMac(),"00124B0014C6258E",cmd);   
								
							}
							break;
							
							case 'O':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"3-Open.\n");
									char cmd[32] = {0};
					                strcpy(cmd,"0000010103");
					                HSZCommandsSend(AppGatewayMac(),"00124B0014C6258E",cmd);    
								
							}
							break;
							
							
							case 'P':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"3-Close.\n");
									char cmd[32] = {0};
					                strcpy(cmd,"0000020103");
					                HSZCommandsSend(AppGatewayMac(),"00124B0014C6258E",cmd);   
							}
							break;
							
							
						/*
							case 's'://����ͼ��
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"add user.\n");
					      
									HSZSetHuayiDeviceIcon(AppGatewayMac(),"00124B0008EB3F43",0x01,0x12);//�ƴ�
							}
							break;

							case 't'://�����û���
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"del user.\n");
									
									HSZSetHuayiDeviceName(AppGatewayMac(),"00124B0008EB3F44",0x01,"084e8c8def706f5149");     
							}
							break;

							case 'u'://ɨ������
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"request temp user.\n");
									
									HSZJoinDeviceByScanCode(AppGatewayMac(),"00124B0008EB3F44",120);
								
							}
							break;

							case 'v'://����Ա������֤
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"admin passwd auth.\n");
									char cmd[30] = {0};
									strcpy(cmd,"000B156123456");
									//HSZSendSmartLockData("00124B001477BA09",cmd);
									HSZCommandsSend(AppGatewayMac(),DEVID_XU,cmd);        //xushuya?��?????����??��
							}
							break;
							
							case 'w'://��ѯ����ʱ��
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"requst time.\n");
									char cmd[30] = {0};
									strcpy(cmd,"000B35");
									//HSZSendSmartLockData("00124B001477BA09",cmd);
									HSZCommandsSend(AppGatewayMac(),DEVID_XU,cmd);        //xushuya?��?????����??��                
							}
							break;

							case 'x'://�������Ա������֤
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"error admin passwd auth .\n");
									char cmd[30] = {0};
									strcpy(cmd,"000B150006123457");
									HSZCommandsSend(AppGatewayMac(),DEVID_XU,cmd);        //xushuya?��?????����??��
							}
							break;
							case 'y':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"request normal user.\n");
									char cmd[30] = {0};
									strcpy(cmd,"000B32");
									HSZCommandsSend(AppGatewayMac(),DEVID_XU,cmd);        //xushuya?��?????����??��
							}
							break;
							case 'z':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"request admin user.\n");
									char cmd[30] = {0};
									strcpy(cmd,"000B31");
									HSZCommandsSend(AppGatewayMac(),DEVID_XU,cmd);        //xushuya?��?????����??��
							}
							break;
				
							case 'A':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"request lock info.\n");
									char cmd[30] = {0};
									strcpy(cmd,"000B3401");
									//HSZSendSmartLockData("00124B001477BA09",cmd);
									HSZCommandsSend(AppGatewayMac(),DEVID_XU,cmd);        //xushuya?��?????����??��
							}
							break;

							case 'B':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"request opendoor history.\n");
									char cmd[30] = {0};
									strcpy(cmd,"000B36");
									HSZCommandsSend(AppGatewayMac(),DEVID_XU,cmd);        //xushuya?��?????����??��
							}
							break;
							case 'C':
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"remote open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"000B113C181112163944");
									HSZCommandsSend(AppGatewayMac(),DEVID_XU,cmd);        //xushuya?��?????����??��
							}
							break;
							
							case 'D':												      //RSY �ͳɱ����»�
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"open.\n");
									char cmd[32] = {0};
					                strcpy(cmd,"FE1E01050181000000");
					                HSZCommandsSend(AppGatewayMac(),"00124B0019FCF9FA",cmd);   
							}
							break;
							case 'g':												  	  //RSY �ͳɱ����»�
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"open light.\n");
									char cmd[32] = {0};
					                strcpy(cmd,"FE1E01050101000000");
					                HSZCommandsSend(AppGatewayMac(),"00124B0019FCF9FA",cmd);   
							}
							break;			
							case 'G':												  	  //RSY �ͳɱ����»�
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"stop.\n");
									char cmd[32] = {0};
					                strcpy(cmd,"FE1E01050100000000");
					                HSZCommandsSend(AppGatewayMac(),"00124B0019FCF9FA",cmd);   
							}
							break;
				*/

					//00124B00146579EF
							case 'A':													 //RSY SL500���� ����������֤����
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C11106999999");								//11:���λ+������֤ 2�����뿪�� 06�����볤�� 123456������
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;
				
							case 'B':													
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C11206777777");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'C':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door active return commands.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C35");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);      
							}
							break;
				
							case 'D':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C12190320104530");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'E':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C1506999999");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'F':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C2120643445519032010001903202200");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'G':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C2161044702717C00309B819032010001903202200");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'H':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C31");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'I':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C32");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'J':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C33");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'K':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C37");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'L':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C3401");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'M':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C3402");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'N':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C3403");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'O':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C3404");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'P':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C36");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'Q':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C42");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'R':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C2301");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'S':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C2302");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'T':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C2303");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'U':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C2401");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'V':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C2402");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'W':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C25000005AABBCCDDEE0006332234");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'X':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C25010003BBCCDDEEFF021042702717C00309B8");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'Y':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C1306777777");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'Z':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C25010008CCEEFFAADD0006245891");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'e':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C2120644433319031910001903192200");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'f':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C25000004EEDDCCBBAA0006333222");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'g':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C25010009DDDDDEEEEE0006222333");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'h':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C36");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'i':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C410135");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'j':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C32");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;

							case 'k':													 
							{
									DEBUG_APP_ZONE(LOG_APP_TEST,"Wireless password open door.\n");
									char cmd[30] = {0};
									strcpy(cmd,"100C2210008");								
									HSZCommandsSend(AppGatewayMac(),DEVID_RSY,cmd);       
							}
							break;
							
							default:
									DEBUG_APP_ZONE(LOG_APP_TEST,"Error??Check input:%d.\n",ch);
							break;
					}
				usleep(1000);
} 

/*****************************************************************************
 �� �� ��  : main
 ��������  : ����Main()���
 �������  : int argc      
             char *argv[]  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int main(int argc, char *argv[])
{
		int ch;
		int i=0;
		
	    printf("\n#################VERSION:1.0.4 2019-01-12:13:51####################\n");

		HSZSetLogFileInit();
		
		DEBUG_APP_ZONE(LOG_APP_TEST,"New_Mosquitto sdk test begin.\n");
		
		HSZAppSetCallback();
		
		HSZInit();

		do
		{
			ch=getchar();
			DEBUG_APP_ZONE(LOG_APP_TEST,"-------Receive date;%d.\n",ch);

			if(666==ch)
			{
				for(i=0;i<26;i++)
				{
					sprintf(ch,"%c",'A'+i);
					DoTestCommand(ch);
				}
			}
			else
			{
					DoTestCommand(ch);
			}
		}


		while (ch!='Q' && ch != 'q');
		
		// HSZDeinit();
		HSZSetLogFileClose();

		pthread_mutex_destroy(&app_cs_log);
		exit(0);
		
		return 0;
}


/*****************************************************************************
 �� �� ��  : HSZSDKDebugZone
 ��������  : ��ȡ�ɴ�ӡlog����
 �������  : int zone  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZSDKDebugZone(int zone)
{
		if(-1 == getcode(app_value))
		{
				return 0x00;
		}

		if(LOG_APP_ZONE_ANY == getcode(app_value))
		{
				return 0x01;
		}
		
		if((zone & getcode(app_value)) != 0x00)
		{
				return 0x01;
		}
		else
		{
				return 0x00;
		}
}

/*****************************************************************************
 �� �� ��  : getcode
 ��������  : ��ȡlog��Ϣ����
 �������  : char *path  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
LOGLEVEL getcode(char *path)
{ 
		int code = -1; 

		if(strstr(path,"LOG_APP_TOPIC")!=NULL)
		{
				if(code == -1)
				{
					code = LOG_APP_TOPIC;
				}
				else
				{
					code |= LOG_APP_TOPIC;
				}
		}		
		
		if(strstr(path,"LOG_APP_TEST")!=NULL)
		{
				if(code == -1)
				{
					code = LOG_APP_TEST;
				}
				else
				{
					code |= LOG_APP_TEST;
				}
		}		
		
		if(strstr(path,"LOG_APP_DEV_LIST")!=NULL)
		{
				if(code == -1)
				{
					code = LOG_APP_DEV_LIST;
				}
				else
				{
					code |= LOG_APP_DEV_LIST;
				}
		}
		
		if(strstr(path,"LOG_APP_JOIN_ON")!=NULL)
		{
				if(code == -1)
				{
					code = LOG_APP_JOIN_ON;
				}
				else
				{
					code |= LOG_APP_JOIN_ON;
				}
		}
		
		if(strstr(path,"LOG_APP_JOIN_OFF")!=NULL)
		{
				if(code == -1)
				{
					code = LOG_APP_JOIN_OFF;
				}
				else
				{
					code |= LOG_APP_JOIN_OFF;
				}
		}
		
		if(strstr(path,"LOG_APP_ERROR")!=NULL)
		{
				if(code == -1)
				{
					code = LOG_APP_ERROR;
				}
				else
				{
					code |= LOG_APP_ERROR;
				}
		}
		
		if(strstr(path,"LOG_APP_CMD")!=NULL)
		{
				if(code == -1)
				{
					code = LOG_APP_CMD;
				}
				else
				{
					code |= LOG_APP_CMD;
				}
		}
		
		if(strstr(path,"LOG_APP_BINDS")!=NULL)
		{
				if(code == -1)
				{
					code = LOG_APP_BINDS;
				}
				else
				{
					code |= LOG_APP_BINDS;
				}
		}
		
		if(strstr(path,"LOG_APP_DATA")!=NULL)
		{
				if(code == -1)
				{
					code = LOG_APP_DATA;
				}
				else
				{
					code |= LOG_APP_DATA;
				}
		}
		
		if(strstr(path,"LOG_APP_OTHER")!=NULL)
		{
				if(code == -1)
				{
					code = LOG_APP_OTHER;
				}
				else
				{
					code |= LOG_APP_OTHER;
				}
		}
		
		if(strstr(path,"LOG_APP_ZONE_ANY")!=NULL)
		{
				code = LOG_APP_ZONE_ANY;
		}

		return (LOGLEVEL)code; 
} 

/*****************************************************************************
 �� �� ��  : HSZGetAppDebugOnOFF
 �������  : char *path  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZGetAppDebugOnOFF(char *path)
{
		int debug_info = -1;
		
		if(strstr(path,"APP_DEBUG_ON_FILE")!=NULL)
		{
				debug_info  = 0x02;
		}
		else if((strstr(path,"APP_DEBUG_ON_PRINT")!=NULL))
		{
				debug_info  = 0x01;
		}
		else if((strstr(path,"APP_DEBUG_OFF")!=NULL))
		{
				debug_info = -1;
		}
		else
		{
			;
		}
		
		return debug_info;
}

/*****************************************************************************
 �� �� ��  : getlogset
 ��������  : ��ȡ��־������Ϣ
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void getlogset(void)
{ 
		char path[512]={0x0}; 
		
		getcwd(path,sizeof(path)); 
			
		strcat(path,"/log/log_app.conf"); 
		
		FILE *fpath = fopen(path,"r"); 
		
		if(fpath == NULL) 
		{
				printf("App log path in null.\n");
				return;
		}	
		fscanf(fpath,"path=%s.\n",app_value); 
		
		fclose(fpath); 
} 
/*****************************************************************************
 �� �� ��  : APPGetGatewayPanid
 ��������  : ��ȡ���������PID
 �������  : int min  
             int max  
 �������  : ��
 �� �� ֵ  :  
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��28��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int APPGetGatewayPanid(int min,int max)
{
		int get_rand = (rand()%(max-min+1) + min);
		
		DEBUG_APP_ZONE(LOG_APP_TEST,"APPGetGatewayPanid :0x%x\n",get_rand);
		
		return get_rand;
}

/*****************************************************************************
 �� �� ��  : AppCallbackOneerror
 ��������  : ��������Ļص�����
 �������  : int errtype        
 
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���
*****************************************************************************/

void AppCallbackOneerror(char* gw_mac,int errtype)
{ 
		switch (errtype)
		{
			case APP_ERR_APPNOTDEALSTATECHANGED1:
			{
					DEBUG_APP_ZONE(LOG_APP_ERROR,"ERROR,err = %d.\n",errtype);
			}
			break;
			
			default:
					DEBUG_APP_ZONE(LOG_APP_ERROR,"ERROR,err = %d.\n",errtype);
			break;
		}
}
/*****************************************************************************
 �� �� ��  : AppCallbackGatewayState
 ��������  : ����״̬�ص�
 �������  : int errtype        
 
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���
*****************************************************************************/
void AppCallbackGatewayState(char* gw_mac,int gw_state)
{	
		if(NULL!=gw_mac)
		{
				sprintf(get_gateway_mac,"%s",gw_mac);
		}
		else
		{
				DEBUG_APP_ZONE(LOG_APP_TEST,"Get_gateway_mac error.\n");
				return ;
		}
		
		if(0x01 == gw_state)
		{	
				DEBUG_APP_ZONE(LOG_APP_TEST,"Get_gateway_mac:%s,gateway network online.\n",get_gateway_mac);
		}
		else if(0x00 == gw_state)
		{
				DEBUG_APP_ZONE(LOG_APP_TEST,"Get_gateway_mac:%s,gateway network offline.\n",get_gateway_mac);
		}
		else
		{
				DEBUG_APP_ZONE(LOG_APP_TEST,"Get_gateway_mac:%s,gateway network error.\n",get_gateway_mac);
		}
}
/*****************************************************************************
 �� �� ��  : AppCallbackOnDeviceJoin
 ��������  : APP�����豸�����ص�
 �������  : char* nodeid      
             int dev_state     
             char* dev_type    
             char* eui         
             int dev_endpoint  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��21��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void AppCallbackOnDeviceJoin(char* gw_mac,char* dev_type,char* dev_mac,int endpoint_count)
{
		int device_type;
		
		DEBUG_APP_ZONE(LOG_APP_DEV_LIST,"AppCallbackOnDeviceJoin:\n");
		
		DEBUG_APP_ZONE(LOG_APP_DEV_LIST,"dev_type:%s.\n",dev_type);
		DEBUG_APP_ZONE(LOG_APP_DEV_LIST,"dev_mac:%s.\n",dev_mac);
		DEBUG_APP_ZONE(LOG_APP_DEV_LIST,"endpoint_count:%d.\n",endpoint_count);
		
		device_type = strtol(dev_type,NULL,16);
		
		switch (device_type)
		{
				case ON_OFF_LIGHT:/* On/Off Light*/
				{
						DEBUG_APP_ZONE(LOG_APP_DEV_LIST,"The Joined device is ON_OFF_LIGHT.\n");
				}
				break;
				
				case ON_OFF_SWITCH:/* On/Off Switch*/
				{
						DEBUG_APP_ZONE(LOG_APP_DEV_LIST,"The Joined device is ON_OFF_SWITCH.\n");
							
						test_temp = 0x01;
						
						sprintf(device_eui,"%s",HSZRemoveMacAddr0X(dev_mac));
						
						device_endpoint = endpoint_count;
						
						DEBUG_APP_ZONE(LOG_APP_DEV_LIST,"device_eui:%s,device_endpoint:%d.\n",device_eui,device_endpoint);
											
				}
				break;
				
				case CONTACT_SENSOR:
				{
						DEBUG_APP_ZONE(LOG_APP_DEV_LIST,"The Joined device is CONTACT_SENSOR.\n");
				}
				break;
				
				case LEVEL_CONTROL_SWITCH:
				{
						DEBUG_APP_ZONE(LOG_APP_DEV_LIST,"The Joined device is LEVEL_CONTROL_SWITCH.\n");
				}
				break;
					
				default:

				break;
		}
}

/*****************************************************************************
 �� �� ��  : AppCallbackOnDeviceLeave
 ��������  : APP�豸�����ص�
 �������  : char* eui     
 
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��21��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void AppCallbackOnDeviceLeave(char* gw_mac,char* dev_mac)
{
		DEBUG_APP_ZONE(LOG_APP_JOIN_OFF,"AppCallbackOnDeviceLeave:\n");
		
		DEBUG_APP_ZONE(LOG_APP_JOIN_OFF,"dev_mac:%s.\n",dev_mac);
}
/*****************************************************************************
 �� �� ��  : AppCallbackOnDeviceLineState
 ��������  : �豸���߻ص�����
 �������  : char* dev_mac  
             int reason     
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��5��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void AppCallbackOnDeviceLineState(char* gw_mac,char* dev_mac,int dev_state)
{
		DEBUG_APP_ZONE(LOG_APP_DATA,"gw_mac:%s.\n",gw_mac);
		
		if(0x01 == dev_state)
		{
				DEBUG_APP_ZONE(LOG_APP_DATA,"++++Device line On++++.\n");
				DEBUG_APP_ZONE(LOG_APP_DATA,"dev_mac:%s.\n",dev_mac);
		}
		else if(0x00 == dev_state)
		{
				DEBUG_APP_ZONE(LOG_APP_DATA,"---Device line Off---.\n");
				DEBUG_APP_ZONE(LOG_APP_DATA,"dev_mac:%s.\n",dev_mac);
		}	
}
/*****************************************************************************
 �� �� ��  : AppCallbackGatewayPermitJoinState
 ��������  : ����ֹͣ�����ص�
 �������  : time_t curtime  ��
             int permit_state ��
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��6��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void AppCallbackGatewayPermitJoinState(char* gw_mac,int permit_state)
{
		if(0x01 == permit_state)
		{
				DEBUG_APP_ZONE(LOG_APP_JOIN_ON,"Gateway Mac:%s,permit join on.\n",gw_mac);
		}
		else if(0x00 == permit_state )
		{
				DEBUG_APP_ZONE(LOG_APP_JOIN_ON,"Gateway Mac:%s,permit join off.\n",gw_mac);
		}
		else
		{
				DEBUG_APP_ZONE(LOG_APP_JOIN_ON,"Gateway Mac:%s,permit join state error.\n",gw_mac);
		}
}
/*****************************************************************************
 �� �� ��  : AppCallbackDeviceData
 ��������  : ��ȡ�豸���ݻص�
 �������  : char* gw_mac     
             char* device_mac  
             char* data       
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��7��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void AppCallbackDeviceData(char* gw_mac,char* device_mac,char* data)
{
		char device_type_buf[10];
		char cmd_buf[10];
		int  device_type;
		int  cmd_type;
		char len_c[10],endpoint_c[10];
		int len,endpoint;
		
		DEBUG_APP_ZONE(LOG_APP_DATA,"*********************************************\n");
		
		// DEBUG_APP_ZONE(LOG_APP_DATA,"AppCallbackDeviceData:\n");
		// DEBUG_APP_ZONE(LOG_APP_DATA,"gw_mac:%s.\n",gw_mac);
		// DEBUG_APP_ZONE(LOG_APP_DATA,"device_id:%s.\n",device_mac);
		
		DEBUG_APP_ZONE(LOG_APP_DATA,"data:%s.\n",data);
		
#if 1	
		/*��ȡcommmad device type value*/
		strncpy(device_type_buf,data,4);
		
		device_type = strtol(device_type_buf,NULL,16);

		DEBUG_APP_ZONE(LOG_APP_DATA,"device_type:0x%04x.\n",device_type);
		

		/*��ȡcommmad cmd type value*/	
		strncpy(cmd_buf,&data[4],4);
		
		cmd_type = strtol(cmd_buf,NULL,16);
		
		DEBUG_APP_ZONE(LOG_APP_DATA,"cmd_type:0x%02x.\n",cmd_type);
		
		switch(device_type)
		{
				case LEVEL_CONTROL_SWITCH:
				{
						char levle[4] = {0};
						int levle_value = 0;
						
						strncpy(len_c,&data[6],2);
						len = strtol(len_c,NULL,16);
						
						DEBUG_APP_ZONE(LOG_APP_DATA,"len_c:0x%02x.\n",len);
						
						strncpy(endpoint_c,&data[8],2);
						endpoint = strtol(endpoint_c,NULL,16);
						
						DEBUG_APP_ZONE(LOG_APP_DATA,"endpoint:0x%02x.\n",endpoint);

						strncpy(levle,&data[10],2);
						levle_value = strtol(levle,NULL,16);
						
						DEBUG_APP_ZONE(LOG_APP_DATA,"levle_value:0x%02x.\n",levle_value);
				}
				break;
			
				case CONTACT_SENSOR:
				{
						char sontact_buf[4] = {0};
						int contact_status = 0;
						
						strncpy(len_c,&data[6],2);
						len = strtol(len_c,NULL,16);
						
						DEBUG_APP_ZONE(LOG_APP_DATA,"len_c:0x%02x.\n",len);
						
						strncpy(endpoint_c,&data[8],2);
						endpoint = strtol(endpoint_c,NULL,16);
						
						DEBUG_APP_ZONE(LOG_APP_DATA,"endpoint:0x%02x.\n",endpoint);
						
						strncpy(sontact_buf,&data[10],2);
						contact_status = strtol(sontact_buf,NULL,16);
						
						DEBUG_APP_ZONE(LOG_APP_DATA,"contact_status:0x%02x.\n",contact_status);
						
						if(0x00 == contact_status)
						{
								DEBUG_APP_ZONE(LOG_APP_DATA,"---Contact Sensor: %s is Closed.\n",device_mac);
						}
						else if(0x01 == contact_status)
						{
								DEBUG_APP_ZONE(LOG_APP_DATA,"+++Contact Sensor: %s is Open.\n",device_mac);
						}
						else if(0x02 == contact_status)
						{
								DEBUG_APP_ZONE(LOG_APP_DATA,"****Contact Sensor: %s is Abnormal Open.\n",device_mac);
						}
						else if(0x03 == contact_status)
						{
								DEBUG_APP_ZONE(LOG_APP_DATA,"+++Contact Sensor: %s is Abnormal Open and open.\n",device_mac);
						}
						else if(0x04 == contact_status)
						{
								DEBUG_APP_ZONE(LOG_APP_DATA,"Contact Sensor: %s is low battery.\n",device_mac);
						}
						else if(0x05 == contact_status)
						{
								DEBUG_APP_ZONE(LOG_APP_DATA,"Contact Sensor: %s is low battery and Open.\n",device_mac);
						}
						else if(0x06 == contact_status)
						{
								DEBUG_APP_ZONE(LOG_APP_DATA,"Contact Sensor: %s is low battery and abnormal Open.\n",device_mac);
						}
						else
						{
								DEBUG_APP_ZONE(LOG_APP_DATA,"Contact Sensor status Error.\n");
						}
				}
				break;
				
				case ON_OFF_LIGHT:
				{
						char on_off_status_buf[4] = {0};
						int on_off_status = 0;
						
						strncpy(len_c,&data[6],2);
						len = strtol(len_c,NULL,16);
						
						DEBUG_APP_ZONE(LOG_APP_DATA,"len_c:0x%02x.\n",len);
						
						strncpy(endpoint_c,&data[8],2);
						endpoint = strtol(endpoint_c,NULL,16);
						
						DEBUG_APP_ZONE(LOG_APP_DATA,"endpoint:0x%02x.\n",endpoint);

						strncpy(on_off_status_buf,&data[10],2);
						on_off_status = strtol(on_off_status_buf,NULL,16);
						
						DEBUG_APP_ZONE(LOG_APP_DATA,"On Off light status:0x%02x.\n",on_off_status);
						
						if(0x01 == on_off_status)
						{
								DEBUG_APP_ZONE(LOG_APP_DATA,"On Off light:%s Power on.\n",device_mac);
						}
						else
						{
								DEBUG_APP_ZONE(LOG_APP_DATA,"On Off light:%s Power off.\n",device_mac);
						}
				} 
				break;
			
				case SMART_PLUG:
				{
					DEBUG_APP_ZONE(LOG_APP_DATA,"SMART_PLUG.\n");
				}
				break;
			
				case 0xF100:
				{
						DEBUG_APP_ZONE(LOG_APP_DATA,"Scene Light.\n");
						
						char on_off_status_buf[4] = {0};
						int on_off_status = 0;
						
						strncpy(len_c,&data[6],2);
						len = strtol(len_c,NULL,16);
						
						DEBUG_APP_ZONE(LOG_APP_DATA,"len_c:0x%02x.\n",len);
						
						strncpy(endpoint_c,&data[8],2);
						endpoint = strtol(endpoint_c,NULL,16);
						
						DEBUG_APP_ZONE(LOG_APP_DATA,"endpoint:0x%02x.\n",endpoint);

						strncpy(on_off_status_buf,&data[10],2);
						on_off_status = strtol(on_off_status_buf,NULL,16);
						
						DEBUG_APP_ZONE(LOG_APP_DATA,"Scene light status:0x%02x.\n",on_off_status);
						
						if(0x01 == on_off_status)
						{
								DEBUG_APP_ZONE(LOG_APP_DATA,"Scene light:%s,endpoint:%d,Power on.\n",device_mac,endpoint);
						}
						else
						{
								DEBUG_APP_ZONE(LOG_APP_DATA,"On Off light:%s,endpoint:%d,Power off.\n",device_mac,endpoint);
						}
				}
				break;
				
				
				default:
				break;
		}
#endif 
	
}
/*****************************************************************************
 �� �� ��  : AppCallbackCommandFailed
 ��������  : ��ȡ����ִ��Commandʧ��
 �������  : char* gw_mac     
             char* device_mac  
             int data       
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��7��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void AppCallbackCommandFailed(char* gw_mac,char* device_mac,int data)
{
		DEBUG_APP_ZONE(LOG_APP_DATA,"*********************************************\n");
		DEBUG_APP_ZONE(LOG_APP_DATA,"AppCallbackCommandFailed:\n");
		DEBUG_APP_ZONE(LOG_APP_DATA,"gw_mac:%s.\n",gw_mac);
		DEBUG_APP_ZONE(LOG_APP_DATA,"device_mac:%s.\n",device_mac);
		DEBUG_APP_ZONE(LOG_APP_DATA,"data:%d.\n",data);
		DEBUG_APP_ZONE(LOG_APP_DATA,"*********************************************\n");
}
/*****************************************************************************
 �� �� ��  : AppCallbackNcpUpdateResult
 ��������  : 
 �������  : char* gw_mac     
             char* device_mac  
             int data       
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��7��7��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void AppCallbackNcpUpdateResult(char* gw_mac,int data)
{
		DEBUG_APP_ZONE(LOG_APP_DATA,"*********************************************\n");
		DEBUG_APP_ZONE(LOG_APP_DATA,"AppCallbackNcpUpdateResult:\n");
		DEBUG_APP_ZONE(LOG_APP_DATA,"gw_mac:%s.\n",gw_mac);
		DEBUG_APP_ZONE(LOG_APP_DATA,"data:0x%04x.\n",data);
		DEBUG_APP_ZONE(LOG_APP_DATA,"*********************************************\n");
}
