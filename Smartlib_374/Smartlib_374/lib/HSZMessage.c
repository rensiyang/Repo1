/******************************************************************************

                  ��Ȩ���� (C), 2017-2025, �ൺ�����ǻۼҾ�ϵͳ�ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : HSZMessage.c
  �� �� ��   : ����
  ��    ��   : Zigbee Project Team
  ��������   : 2017��6��12��
  ����޸�   :
  ��������   : ��Ϣ��������Ϣ��װ��������

  �޸���ʷ   :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ļ�

  2.��    ��   : 2017��6��21��
    ��    ��   : Zigbee Project Team
    �޸�����   : �������ϲ�sdk���ƽӿں�������
******************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "HSZMessage.h"
#include "HSZCommon.h"
#include "cJSON.h"

#include "HSZDeviceData.h"
#include "HSZCommandId.h"
#include "HSZClusterId.h"
#include "HSZAttributeId.h"
#include "HSZAttributeType.h"
#include "HSZBindDevice.h"

#include "HSZHandleZclData.h"



HSZCallbackSubscribeMsg* hsbk_top = 0;
HSZCallbackOnError* hsbk_onerror = 0;

HSZCallbackGatewayState* hsbk_gatewaystate = 0;

HSZCallbackOnDeviceJoin* hsbk_devicejoin = 0;
HSZCallbackOnDeviceLeave* hsbk_deviceleave = 0;

HSZCallbackGetDeviceList* hsbk_getdevicelist = 0;
HSZCallbackOnCommandSend* hsbk_commandsend = 0;

HSZCallbackOnDeviceLineState* hsbk_devicelineon = 0;

HSZCallbackGatewayPermitJoinState* hsbk_gatewaypermitjoinstate = 0;

HSZCallbackCommandExecuted* hsbk_cmdexcuted = 0;
HSZCallbackDevSignalValue* hsbk_devsignalvalue = 0;

HSZCallbackReportOnlineDeviceList* hsbk_devonlinelist = 0;
HSZCallbackDeviceData* hsbk_devicedata = 0;

HSZCallbackCommandFailed* hsbk_commandfailed = 0;
HSZCallbackNcpUpdateResult* hsbk_ncpupdateresult = 0;

extern int gateway_network_mode;

int cJSON_HasObjectItem(cJSON *object,const char *string)		{return cJSON_GetObjectItem(object,string)?1:0;}

char Zigbee_stack[30] = {0};

struct line_on_dev_info LINE_ON_DEV_INFO[PERMIT_MAX_LINE_DEV];		/*��¼�����豸��Ϣ�Ľṹ��*/

static int line_on_device_index;                       	            /*��¼�����豸����*/


struct join_on_status_info JOIN_ON_STAT_INFO[PERMIT_MAX_LINE_DEV];  /*��¼�����豸״̬�Ƿ��Ѿ��ϱ��ṹ��*/ 

static int Join_on_status_index;									/*��¼�����豸����*/

/*****************************************************************************
 �� �� ��  : HSZInitJoinOnstatusInfoBuffer
 ��������  : �����豸״̬�б��ʼ��
 �������  : ��    
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��2��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void HSZInitJoinOnstatusInfoBuffer(void)
{
	DEBUG_ZONE(LOG_ZONE_MY,"HSZInitJoinOnstatusInfoBuffer.\n");

	memset(JOIN_ON_STAT_INFO,0,PERMIT_MAX_LINE_DEV*sizeof(struct join_on_status_info));/*��ջ����buffer*/

	Join_on_status_index = 0x00;/*ͳ�Ƹ�������*/
}
/*****************************************************************************
 �� �� ��  : HSZPrintJoinOnstatusInfo
 ��������  : ��ӡ�����豸�豸�б����ڵ�����Ϣ
 �������  : ��    
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��2��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void HSZPrintJoinOnstatusInfo(void)
{
	int tmp;
	
	for(tmp = 0;tmp <Join_on_status_index;tmp++)
	{
		if(strcmp(JOIN_ON_STAT_INFO[tmp].dev_eui,"")!=0x00)
		{
			DEBUG_ZONE(LOG_ZONE_MY,"*****************************************************************\n");
			
			DEBUG_ZONE(LOG_ZONE_MY,"Join on Mac Address:%s,Update status:0x%02x.\n",JOIN_ON_STAT_INFO[tmp].dev_eui,JOIN_ON_STAT_INFO[tmp].join_on_statue);

			DEBUG_ZONE(LOG_ZONE_MY,"*****************************************************************\n");
		}
	}
}

/*****************************************************************************
 �� �� ��  : HSZSetJoinOnstatusInfo
 ��������  : �������豸��Ϣ���뻺��buffer
 �������  : 
			char* dev_mac������MAC��ַ 
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��2��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZSetJoinOnstatusInfo(char* dev_mac)
{
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZSetLineOnInfoBuffer.\n");
	
	int find_index = -10;
	
	
	if(Join_on_status_index < PERMIT_MAX_LINE_DEV)/*С���趨�Ĵ洢�豸����*/
	{		
        find_index = HSZFindEuiFromJoinOnInfoBuffer(dev_mac,Join_on_status_index+1);	
		
		if(-1 != find_index)/*�����¼�¼������*/
		{
			JOIN_ON_STAT_INFO[find_index].join_on_statue = DEVICE_JOIN_ON_INIT;/*��ʼ״̬*/
			
			DEBUG_ZONE(LOG_ZONE_MY,"Device eui:%s has joined,and set status:0x%02x.\n",dev_mac,JOIN_ON_STAT_INFO[find_index].join_on_statue);
		}
		else
		{
			strncpy(JOIN_ON_STAT_INFO[Join_on_status_index].dev_eui,dev_mac,strlen(dev_mac));

			JOIN_ON_STAT_INFO[Join_on_status_index].join_on_statue = DEVICE_JOIN_ON_INIT;/*��ʼ״̬*/

			Join_on_status_index++;
			
			DEBUG_ZONE(LOG_ZONE_MY,"Join on device eui:%s,status:0x%02x.\n",dev_mac,JOIN_ON_STAT_INFO[Join_on_status_index].join_on_statue);
		}
		
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Device number out of range,Current EUI:%s,Total dev number:%d.\n",dev_mac,Join_on_status_index+1);
		HSZDeviceError(HSZ_ERR_RECORD_JOIN_DEV,"Device number out of range.",NULL);
	}
	
	
	HSZPrintJoinOnstatusInfo();
	
	return 0;
}
/*****************************************************************************
 �� �� ��  : HSZUpdateDeviceJoinStatus
 ��������  : ���������豸״̬�Ƿ��Ѿ��ϱ���Ϣ
 �������  : 
			char* dev_mac������MAC��ַ
			int status�� ״̬
 �������  : 
			���ض�Ӧ��Cluster ID
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��2��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZUpdateDeviceJoinStatus(char* dev_mac,int status)
{
	int tmp;
	
	for(tmp = 0;tmp <PERMIT_MAX_LINE_DEV;tmp++)
	{
		if(0x00 == strcmp(dev_mac,JOIN_ON_STAT_INFO[tmp].dev_eui))
		{	
			if(status!= JOIN_ON_STAT_INFO[tmp].join_on_statue)
			{
				JOIN_ON_STAT_INFO[tmp].join_on_statue = status;
				
				DEBUG_ZONE(LOG_ZONE_MY,"Device Mac:%s,change status to:0x%02x.\n",JOIN_ON_STAT_INFO[tmp].dev_eui,JOIN_ON_STAT_INFO[tmp].join_on_statue);
			}
		}
	}
	
	return 0x00;
}

/*****************************************************************************
 �� �� ��  : HSZAskDeviceJoinStatusInfo
 ��������  : ���������豸״̬�Ƿ��Ѿ��ϱ���Ϣ
 �������  : void
			
 �������  : void
			
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��2��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void HSZAskDeviceJoinStatusInfo(void)
{
	int tmp;
	
	for(tmp = 0;tmp <PERMIT_MAX_LINE_DEV;tmp++)
	{
		if(DEVICE_JOIN_ON_INIT == JOIN_ON_STAT_INFO[tmp].join_on_statue)
		{
			HSZRequestAttribute(JOIN_ON_STAT_INFO[tmp].dev_eui,0x01,ZCL_IAS_ZONE_CLUSTER_ID,ZCL_ZONE_TYPE_ATTRIBUTE_ID);
			
			DEBUG_ZONE(LOG_ZONE_MY,"&&&&&&&&Request the <%s> device join status info.\n",LINE_ON_DEV_INFO[tmp].dev_eui);
		}
	}
}

/*****************************************************************************
 �� �� ��  : HSZFindEuiFromJoinOnInfoBuffer
 ��������  : ���������豸��Ӧ��EUI��Index
 �������  : 
			char* dev_ma:�����豸MAC��ַ 
			int max_index�����ҵ�������
 �������  : 
			���ض�Ӧ��EUI��Index
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZFindEuiFromJoinOnInfoBuffer(char* dev_mac,int max_index)
{
	int tmp;
	int find_index = -1;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZFindEuiFromJoinOnInfoBuffer.\n");
	
	for(tmp = 0;tmp < max_index;tmp++)
	{
		if(0x00 == strcmp(dev_mac,JOIN_ON_STAT_INFO[tmp].dev_eui))
		{
			find_index = tmp;
			
			DEBUG_ZONE(LOG_ZONE_MY,"HSZFindEuiFromJoinOnInfoBuffer,Index:%d.\n",find_index);
			
			break;
		}
	}
	
	return find_index;
}

/*****************************************************************************
 �� �� ��  : HSZFindJoinDevStatusInfo
 ��������  : ��ȡ��ӦMAC��ַ�豸������״̬�ϱ�״̬
 �������  : 
			char* dev_mac
			
 �������  : void
			
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��2��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZFindJoinDevStatusInfo(char* dev_mac)
{
	int tmp;
	
	int find_join_info = -1;

	DEBUG_ZONE(LOG_ZONE_TEST,"HSZFindJoinDevStatusInfo.\n");

	for(tmp = 0;tmp < PERMIT_MAX_LINE_DEV;tmp++)
	{
		if(0x00 == strcmp(dev_mac,JOIN_ON_STAT_INFO[tmp].dev_eui))
		{
			find_join_info = JOIN_ON_STAT_INFO[tmp].join_on_statue;
			
			DEBUG_ZONE(LOG_ZONE_MY,"HSZFindJoinDevStatusInfo:Mac:%s,Status:%d.\n",dev_mac,find_join_info);
			
			break;
		}
	}

	if(-1 == find_join_info)
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Not find the join device info:%s.\n",dev_mac);
		HSZDeviceError(HSZ_ERR_FIND_NODEID,"Not find the join device info.",NULL);	
	}

	return find_join_info;
}
/*****************************************************************************
 �� �� ��  : HSZInitLineOnDeviceInfoBuffer
 ��������  : �����豸�б��ʼ��
 �������  : ��    
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��2��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void HSZInitLineOnDeviceInfoBuffer(void)
{
	DEBUG_ZONE(LOG_ZONE_MY,"HSZInitLineOnDeviceInfoBuffer.\n");
	
	memset(LINE_ON_DEV_INFO,0,PERMIT_MAX_LINE_DEV*sizeof(struct line_on_dev_info));/*��ջ����buffer*/
	
	line_on_device_index = 0x00;/*ͳ�Ƹ�������*/
}

/*****************************************************************************
 �� �� ��  : HSZPrintLineOnDeviceStatusInfo
 ��������  : ��ӡ�����豸�б����ڵ�����Ϣ
 �������  : ��    
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��2��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
void HSZPrintLineOnDeviceStatusInfo(void)
{
	int tmp;
	int tmp_index;
	
	for(tmp = 0;tmp <line_on_device_index;tmp++)
	{                     
		if(strcmp(LINE_ON_DEV_INFO[tmp].dev_eui,"")!=0x00)
		{
			DEBUG_ZONE(LOG_ZONE_MY,"*****************************************************************\n");
			
			DEBUG_ZONE(LOG_ZONE_MY,"The Line on device Mac:%s.\n",LINE_ON_DEV_INFO[tmp].dev_eui);
			DEBUG_ZONE(LOG_ZONE_MY,"The Line on device cluster id:0x%04x.\n",LINE_ON_DEV_INFO[tmp].cluster_id);
						
			for(tmp_index = 0;tmp_index <LINE_ON_DEV_INFO[tmp].endpoint_total;tmp_index++)
			{
				DEBUG_ZONE(LOG_ZONE_MY,"The <%d> line on device endpoint:0x%02x,status:0x%02x.\n",tmp_index+1,LINE_ON_DEV_INFO[tmp].endpoint[tmp_index],LINE_ON_DEV_INFO[tmp].line_on_statue[tmp_index]);
			}
			
			DEBUG_ZONE(LOG_ZONE_MY,"*****************************************************************\n");
		}
	}
}
/*****************************************************************************
 �� �� ��  : HSZFindClusteridFromInfoBuffer
 ��������  : ���������豸��Ӧ��Cluster ID
 �������  : 
			char* dev_mac:�����豸MAC��ַ     
 �������  : 
			���ض�Ӧ��Cluster ID
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��2��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZFindClusteridFromInfoBuffer(char* dev_mac)
{
	int tmp;
	int find_clusterid = -1;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZFindClusteridFromInfoBuffer.\n");
	
	for(tmp = 0;tmp < PERMIT_MAX_LINE_DEV;tmp++)
	{
		if(0x00 == strcmp(dev_mac,LINE_ON_DEV_INFO[tmp].dev_eui))
		{
			find_clusterid = LINE_ON_DEV_INFO[tmp].cluster_id;
			
			DEBUG_ZONE(LOG_ZONE_MY,"Find the clusterid:0x%04x.\n",find_clusterid);
			
			break;
		}
	}
	
	if(-1 == find_clusterid)
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Not find the clusterid eui:%s.\n",dev_mac);
		HSZDeviceError(HSZ_ERR_FIND_NODEID,"Not find the clusterid.",NULL);	
	}
	
	return find_clusterid;
}

/*****************************************************************************
 �� �� ��  : HSZUpdateLineOnDeviceInfoBuffer
 ��������  : ���������豸״̬������Ϣ
 �������  : 
			int max_index:   
 �������  : 
			���ض�Ӧ��Cluster ID
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��2��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZUpdateLineOnDeviceInfoBuffer(int max_index)
{
	int tmp;
	int tmp_index;
	
	for(tmp = 0;tmp < max_index;tmp++)
	{		
		for(tmp_index = 0;tmp_index < LINE_ON_DEV_INFO[tmp].endpoint_total;tmp_index++)
		{
			if(DEVICE_LINE_ON_INIT == LINE_ON_DEV_INFO[tmp].line_on_statue[tmp_index])
			{
				HSZReadLineDeviceAttribute(LINE_ON_DEV_INFO[tmp].dev_eui,LINE_ON_DEV_INFO[tmp].endpoint[tmp_index],LINE_ON_DEV_INFO[tmp].cluster_id);
				
				// DEBUG_ZONE(LOG_ZONE_MY,"#####################Request the <%s> device endpoint:0x%02x,cluster id:0x%04x update status.#########################\n",LINE_ON_DEV_INFO[tmp].dev_eui,LINE_ON_DEV_INFO[tmp].endpoint[tmp_index],LINE_ON_DEV_INFO[tmp].cluster_id);
			}
		}		
	}
	
	return 0x00;
}

/*****************************************************************************
 �� �� ��  : HSZUpdateDeviceLineStatus
 ��������  : ���������豸״̬�Ƿ��Ѿ��ϱ���Ϣ
 �������  : 
			char* dev_mac:�豸mac��ַ
			int end_point��endpoint 
 �������  : 
			���ض�Ӧ��Cluster ID
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��2��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZUpdateDeviceLineStatus(char* dev_mac,int end_point,int status)
{
	int tmp;
	int tmp_index;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZUpdateDeviceLineStatus.\n");
	
	for(tmp = 0;tmp < PERMIT_MAX_LINE_DEV;tmp++)
	{	
        if(0x00 == strcmp(dev_mac,LINE_ON_DEV_INFO[tmp].dev_eui))
		{
			for(tmp_index = 0;tmp_index < LINE_ON_DEV_INFO[tmp].endpoint_total;tmp_index++)
			{
				if(end_point == LINE_ON_DEV_INFO[tmp].endpoint[tmp_index])
				{
					if(status != LINE_ON_DEV_INFO[tmp].line_on_statue[tmp_index])
					{
						LINE_ON_DEV_INFO[tmp].line_on_statue[tmp_index] = status;
						
						DEBUG_ZONE(LOG_ZONE_MY,"The <%s> device endpoint:0x%02x,change status to:0x%02x.\n",LINE_ON_DEV_INFO[tmp].dev_eui,LINE_ON_DEV_INFO[tmp].endpoint[tmp_index],LINE_ON_DEV_INFO[tmp].line_on_statue[tmp_index]);
					}
				}
			}
		}		
	}
	
	return 0x00;
}
/*****************************************************************************
 �� �� ��  : HSZFindEuiFromLineOnInfoBuffer
 ��������  : ���������豸��Ӧ��EUI��Index
 �������  : 
			char* dev_ma:�����豸MAC��ַ 
			int max_index�����ҵ�������
 �������  : 
			���ض�Ӧ��EUI��Index
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZFindEuiFromLineOnInfoBuffer(char* dev_mac,int max_index)
{
	int tmp;
	int find_index = -1;
	
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZFindEuiFromLineOnInfoBuffer.\n");
	
	for(tmp = 0;tmp < max_index;tmp++)
	{
		if(0x00 == strcmp(dev_mac,LINE_ON_DEV_INFO[tmp].dev_eui))
		{
			find_index = tmp;
			
			DEBUG_ZONE(LOG_ZONE_MY,"HSZFindEuiFromLineOnInfoBuffer,Index:%d.\n",find_index);
			
			break;
		}
	}
	
	return find_index;
}
/*****************************************************************************
 �� �� ��  : HSZSetLineOnInfoBuffer
 ��������  : �������豸���뻺��buffer
 �������  : 
			char* dev_mac�������豸��ַ��
			int cluster_id�������豸Cluster ID��
			int end_point������Endpoint��
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��8��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZSetLineOnInfoBuffer(char* dev_mac,int cluster_id,int end_point)
{
	DEBUG_ZONE(LOG_ZONE_TEST,"HSZSetLineOnInfoBuffer.\n");
    
	int find_index = -10;
	
	int temp_index = -1;
	
	if(line_on_device_index < PERMIT_MAX_LINE_DEV)/*С���趨�Ĵ洢�豸����*/
	{	
	    find_index = HSZFindEuiFromLineOnInfoBuffer(dev_mac,line_on_device_index+1);
	
	    if(-1 != find_index)/*�����¼�¼������*/
		{	
			/*����Ѿ�������˸�Endpoint������*/
			for(temp_index = 0;temp_index < LINE_ON_MAX_ENDPOINT-1; temp_index++)
			{
				if(end_point == LINE_ON_DEV_INFO[find_index].endpoint[temp_index])
				{
					LINE_ON_DEV_INFO[find_index].line_on_statue[temp_index] = DEVICE_LINE_ON_INIT;
					
					DEBUG_ZONE(LOG_ZONE_TEST,"Has saved the endpoint,and set to DEVICE_LINE_ON_INIT.\n");
					return 0;
				}
			}
			
			if(LINE_ON_DEV_INFO[find_index].endpoint_total<(LINE_ON_MAX_ENDPOINT-1))
			{
				LINE_ON_DEV_INFO[find_index].endpoint_total = LINE_ON_DEV_INFO[find_index].endpoint_total + 0x01;				
				
				LINE_ON_DEV_INFO[find_index].line_on_statue[LINE_ON_DEV_INFO[find_index].endpoint_total-1] =  DEVICE_LINE_ON_INIT;	
				
				LINE_ON_DEV_INFO[find_index].endpoint[LINE_ON_DEV_INFO[find_index].endpoint_total-1] =  end_point;
				
				/*��֮ǰ����endpoint ����*/		
				HSZSort(LINE_ON_DEV_INFO[find_index].endpoint,LINE_ON_DEV_INFO[find_index].endpoint_total);					
			}
			else
			{
				DEBUG_ZONE(LOG_ZONE_MY,"Device Total Endpoint out of range,Current EUI:%s,Total Endpoint:%d.\n",dev_mac,LINE_ON_DEV_INFO[find_index].endpoint_total+1);
				
				HSZDeviceError(HSZ_ERR_RECORD_JOIN_DEV,"Device Total Endpoint out of range.",NULL);
			}
		}
		else /*�״μ�¼��������*/
		{		
	        DEBUG_ZONE(LOG_ZONE_MY,"New Device EUI:%s.\n",dev_mac);
			strncpy(LINE_ON_DEV_INFO[line_on_device_index].dev_eui,dev_mac,strlen(dev_mac));
			LINE_ON_DEV_INFO[line_on_device_index].cluster_id = cluster_id;
			LINE_ON_DEV_INFO[line_on_device_index].endpoint_total += 0x01;
			LINE_ON_DEV_INFO[line_on_device_index].endpoint[0] =  end_point;	
		    LINE_ON_DEV_INFO[line_on_device_index].line_on_statue[0] = DEVICE_LINE_ON_INIT;/*��ʼ״̬*/
		
			line_on_device_index++;
		}
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_MY,"Device number out of range,Current EUI:%s,Total dev number:%d.\n",dev_mac,line_on_device_index+1);
		HSZDeviceError(HSZ_ERR_RECORD_JOIN_DEV,"Device number out of range.",NULL);
	}
	
	HSZPrintLineOnDeviceStatusInfo();
	
	return 0;
}



/*****************************************************************************
 �� �� ��  : HSZRemoveMacAddr0X
 ��������  : ȥ��MIC��ַǰ��'0x'����'0X��
 �������  : char*mac  
 �������  : ��ȡ��MIz��ַ
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
char* HSZRemoveMacAddr0X(char*mac)
{
	if (mac!=NULL && strlen(mac)>=16)
	{
		if ((mac[0]='0') && ((mac[1]=='x') ||( mac[1]=='X')))
		{
			return mac + 2;
		}
		else
		{
			return mac;
		}
	}
	else
	{
		return NULL;
	}
}

/*****************************************************************************
 �� �� ��  : strupr_c
 ��������  : ���ַ����е�Сд��ĸת��Ϊ��д��ĸ��C�����޴�ϵͳ����������Լ�
             ��װ
 �������  : char* src  
 �������  : ��
 �� �� ֵ  : �ַ�����д��ĸ

 
 �޸���ʷ      :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
char* strupr_c(char* src)
{
	while(*src != '\0')
	{
		if (*src >= 'a' && *src <= 'z')
			*src -=32;
		
		src++;
	}
	
	return src;
}

/*****************************************************************************
 �� �� ��  : HSZHostHeartBeatMsgHandle
 ��������  : ���ĵ�Heartbeat��Ϣ�󣬽���������Ϣ
 �������  : cJSON * jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��07��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHostHeartBeatMsgHandle(cJSON *jsonroot)
{
	/* 
	{"znet":true,"PanId":"0x4003","TxPwr":20,"Channel":14
	{"znet":true,"no":406,"PanId":"0xD414","TxPwr":25,"Channel":25}
	*/
	cJSON *networkup = NULL;
	cJSON *networkpid = NULL;
	cJSON *number = NULL;
	cJSON *txpower = NULL;
	cJSON *channel = NULL;
	
	int number_index = -1;
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"znet"))
	{
		networkup = cJSON_GetObjectItem(jsonroot,"znet");		
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"NetworkUp is:%d.\n",networkup->valueint);
		
		/*��������*/
		if(0x01 == networkup->valueint)
		{
			gateway_network_mode = GATEWAY_NETWORK_UP_MODE;
			
			if (hsbk_gatewaystate!=NULL)
			{
				//hsbk_gatewaystate(HSZGatewayMac(),0x01);
			}
		}
		else if(0x00 == networkup->valueint)
		{
			gateway_network_mode = GATEWAY_NETWORK_DOWN_MODE;
			
			if (hsbk_gatewaystate!=NULL)
			{
				hsbk_gatewaystate(HSZGatewayMac(),0x00);
			}
		}
	}

	if(0x01 == cJSON_HasObjectItem(jsonroot,"PanId"))
	{
		networkpid = cJSON_GetObjectItem(jsonroot,"PanId");
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"NetworkPanId is:%s.\n",networkpid->valuestring);
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"no"))
	{
		number = cJSON_GetObjectItem(jsonroot,"no");
		number_index  = number->valueint;
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"number index is:%s.\n",number->valueint);
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"TxPwr"))
	{
		txpower = cJSON_GetObjectItem(jsonroot,"TxPwr");
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"RadioTxPower is:%d.\n",txpower->valueint);
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"Channel"))
	{
		channel = cJSON_GetObjectItem(jsonroot,"Channel");
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"RadioChannel is:%d.\n",channel->valueint);
	}
	
	if((number_index!=-1)&&(0x00 == (number_index%3)))
	{
		// HSZUpdateLineOnDeviceInfoBuffer(line_on_device_index+1);
		
		HSZAskDeviceJoinStatusInfo();
	}
	
	return 0;

}

/*****************************************************************************
 �� �� ��  : HSZHandleDeviceList
 ��������  : ���ĵ�Device list��Ϣ�󣬽���������Ϣ
 �������  : cJSON * jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleDeviceList(cJSON * jsonroot)
{
   /*
   {"devices":
   [
		{
			"nodeId":"0x71B4",
			"working":"0",
			"deviceState":16,
			"deviceType":"0x0101",
			"timeSinceLastMessage":523534,
			"deviceEndpoint":
			{
				"eui64":"0x000D6F000CAAE12C",
				"endpoint":1,
		        "clusterInfo":[
								{"clusterId":"0x0000","clusterType":"In"},
								{"clusterId":"0x0003","clusterType":"In"},
								{"clusterId":"0x0004","clusterType":"In"},
								{"clusterId":"0x0005","clusterType":"In"},
								{"clusterId":"0x0006","clusterType":"In"},
								{"clusterId":"0x0008","clusterType":"In"},
								{"clusterId":"0x0003","clusterType":"Out"}
							  ]	
		    }
		},
		
		{
			"nodeId":"0x2890",
			"working":"0",
			"deviceState":16,
			"deviceType":"0x0102",
			"timeSinceLastMessage":523534,
			"deviceEndpoint":
			{
				"eui64":"0x000B57FFFE47464A",
				"endpoint":1,
		        "clusterInfo":[
								{"clusterId":"0x0000","clusterType":"In"},
								{"clusterId":"0x0003","clusterType":"In"},
								{"clusterId":"0x0004","clusterType":"In"},
								{"clusterId":"0x0005","clusterType":"In"},
								{"clusterId":"0x0006","clusterType":"In"},
								{"clusterId":"0x0008","clusterType":"In"},
								{"clusterId":"0x0300","clusterType":"In"},
								{"clusterId":"0x0B05","clusterType":"In"},
								{"clusterId":"0xFC01","clusterType":"In"},
								{"clusterId":"0xFC02","clusterType":"In"},
								{"clusterId":"0x0019","clusterType":"In"}
								]
			}
		}		
	]
}
	
	
	*/
	cJSON *devep = NULL;
	cJSON *nodeid = NULL;
	cJSON *workstate = NULL;
	cJSON *devtype = NULL;
	cJSON *devstate = NULL;
	cJSON *devep_ep = NULL;
	cJSON *devep_eui = NULL;
	cJSON *taskClustArry = NULL;
	cJSON *clusterid = NULL;
	cJSON *clustertype = NULL;
	
	cJSON *taskArry = NULL;
	
	int devlinestate = -1;
	int device_type = -1;
	int clusterArrySize;
	
	int nnodeid = -1;
	cJSON *clusterlist = NULL;
		
	/* �����豸�б� */

	taskArry = cJSON_GetObjectItem(jsonroot,"devices");/* ��ȡ���� */
	
	int arrySize;
	cJSON *tasklist = NULL;
	
	if (taskArry!=NULL)
	{
		tasklist = taskArry->child;/* �Ӷ��� */
		arrySize = cJSON_GetArraySize(taskArry);/* �����С */
		
		DEBUG_ZONE(LOG_ZONE_TEST,"Devices ArrySize:%d\n",arrySize);  
	}	
	
	HSZInitDeviceInfoBuffer();
	
	while(tasklist!=NULL)  
	{
		if((cJSON_HasObjectItem(tasklist,"nodeId"))&&(cJSON_HasObjectItem(tasklist,"working"))\
		&&(cJSON_HasObjectItem(tasklist,"deviceType"))&&(cJSON_HasObjectItem(tasklist,"deviceState"))&&(cJSON_HasObjectItem(tasklist,"deviceEndpoint")))
		{
			nodeid = cJSON_GetObjectItem(tasklist,"nodeId");
			workstate = cJSON_GetObjectItem(tasklist,"working");
			devtype = cJSON_GetObjectItem(tasklist,"deviceType");
			devstate = cJSON_GetObjectItem(tasklist,"deviceState");
			devep = cJSON_GetObjectItem(tasklist,"deviceEndpoint");
			
			nnodeid = strtol(nodeid->valuestring,NULL,16);
		}
		else
		{
			HSZDeviceError(HSZ_ERR_DEVICE_LIST,NULL,NULL);
			return -1;
		}
		
		devlinestate = strtol(workstate->valuestring,NULL,16);
		device_type = strtol(devtype->valuestring,NULL,16);
		
		if (devep)
		{
			if((cJSON_HasObjectItem(devep,"clusterInfo"))&&(cJSON_HasObjectItem(devep,"endpoint"))&&(cJSON_HasObjectItem(devep,"eui64")))
			{
				devep_ep = cJSON_GetObjectItem(devep,"endpoint");
				devep_eui = cJSON_GetObjectItem(devep,"eui64");
				taskClustArry = cJSON_GetObjectItem(devep,"clusterInfo");/* ��ȡCluster Info ���� */
			}
			else
			{
				HSZDeviceError(HSZ_ERR_DEVICE_LIST,NULL,NULL);
				return -1;
			}
		}
		
		if(taskClustArry!=NULL)
		{
			
			clusterlist = taskClustArry->child;/* Cluster Info �Ӷ��� */
			clusterArrySize = cJSON_GetArraySize(taskClustArry);/*Cluster Info �����С */

			DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"clusterArrySize:%d\n",clusterArrySize);  
		}
		
		while(clusterlist!=NULL)
		{
			if((cJSON_HasObjectItem(clusterlist,"clusterId"))&&(cJSON_HasObjectItem(clusterlist,"clusterType")))
			{
				clusterid = cJSON_GetObjectItem(clusterlist,"clusterId");
				clustertype = cJSON_GetObjectItem(clusterlist,"clusterType");
			}
			else
			{
				HSZDeviceError(HSZ_ERR_DEVICE_LIST,NULL,NULL);
				return -1;
			}
			
			DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"clusterId:%s,clusterType:%s\n",clusterid->valuestring,clustertype->valuestring);  
			
			
			clusterlist = clusterlist->next;
		}
		
		if (nodeid && devtype && devstate && devep && devep_ep && devep_eui && strlen(devep_eui->valuestring)>=16)
		{

			strupr_c(devep_eui->valuestring);
				
			DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"%s\n",nodeid->valuestring);  
			DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"%d\n",devstate->valueint);  
			DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"%s\n",devtype->valuestring); 
			
			HSZSetInfoBuffer(HSZRemoveMacAddr0X(devep_eui->valuestring),nnodeid,devep_ep->valueint,device_type);
			
			/*�ϴ�SDK�ӿ�*/
			if (hsbk_getdevicelist!=NULL)
			{
				hsbk_getdevicelist(HSZGatewayMac(),nodeid->valuestring,devlinestate,devtype->valuestring,\
								   HSZRemoveMacAddr0X(devep_eui->valuestring),devep_ep->valueint);
			}
		}
		else
		{
			HSZDeviceError(-1,NULL,NULL);
		}
		
		tasklist = tasklist->next; 
	}
	
	HSZPrintDeviceBufInfo();
	
	return 0;
}

/*****************************************************************************
 �� �� ��  : HSZHandleDeviceJoined
 ��������  : ���ĵ��豸��������Ϣ�󣬽���������Ϣ
 �������  : cJSON *jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int	HSZHandleDeviceJoined(cJSON *jsonroot)
{
	/*
	{
		"devicejoined":
		[
			{
				"nodeId":"0x710A",
				"working":"1",
				"deviceState":16,
				"deviceType":"0x0100",
				"timeSinceLastMessage":507596,
				"deviceEndpoint":
				{
					"eui64":"0x000D6F000D578BBB",
					"endpoint":1,
					"clusterInfo":
					[
						{"clusterId":"0x0000","clusterType":"In"},
						{"clusterId":"0x0003","clusterType":"In"},
						{"clusterId":"0x0004","clusterType":"In"},
						{"clusterId":"0x0005","clusterType":"In"},
						{"clusterId":"0x0006","clusterType":"In"},
						{"clusterId":"0x0003","clusterType":"Out"}
					]
				}
			},
			
			{
				"nodeId":"0x710A",
				"working":"1",
				"deviceState":16,
				"deviceType":"0x0100",
				"timeSinceLastMessage":507596,
				"deviceEndpoint":
				{
					"eui64":"0x000D6F000D578BBB",
					"endpoint":2,
					"clusterInfo":
					[
						{"clusterId":"0x0000","clusterType":"In"},
						{"clusterId":"0x0003","clusterType":"In"},
						{"clusterId":"0x0004","clusterType":"In"},
						{"clusterId":"0x0005","clusterType":"In"},
						{"clusterId":"0x0006","clusterType":"In"},
						{"clusterId":"0x0003","clusterType":"Out"}
					]
				}
			},
			
			{
				"nodeId":"0x710A",
				"working":"1",
				"deviceState":16,
				"deviceType":"0x0100",
				"timeSinceLastMessage":507596,
				"deviceEndpoint":
				{
					"eui64":"0x000D6F000D578BBB",
					"endpoint":3,"clusterInfo":
					[
					{"clusterId":"0x0000","clusterType":"In"},
					{"clusterId":"0x0003","clusterType":"In"},
					{"clusterId":"0x0004","clusterType":"In"},
					{"clusterId":"0x0005","clusterType":"In"},
					{"clusterId":"0x0006","clusterType":"In"},
					{"clusterId":"0x0003","clusterType":"Out"}
					]
				}
			}
		]
	}.
	
	*/
	
	cJSON *devep = NULL;
	cJSON *nodeid = NULL;
	cJSON *workstate = NULL;
	cJSON *devtype = NULL;
	cJSON *devstate = NULL;
	cJSON *devep_ep = NULL;
	cJSON *devep_eui = NULL;
	cJSON *taskClustArry = NULL;
	cJSON *clusterid = NULL;
	cJSON *clustertype = NULL;
	
	cJSON *clusterlist = NULL;
	
	cJSON *taskArry = NULL;
	int clusterArrySize;
	int nclusterid = 0;
	int nworkstate = 0;
	int nnodeid = 0;
	int nendpoint = 0;
	int ndevtype = 0;
		
	/* �����豸�б� */

	taskArry = cJSON_GetObjectItem(jsonroot,"devicejoined");/* ��ȡ���� */
	
	int arrySize;
	cJSON *tasklist = NULL;
	
	if (taskArry!=NULL)
	{
		tasklist = taskArry->child;/* �Ӷ��� */
		arrySize = cJSON_GetArraySize(taskArry);/* �����С */
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"Joined ArrySize:%d\n",arrySize);  
	}
	
	while(tasklist!=NULL)  
	{
		if((cJSON_HasObjectItem(tasklist,"nodeId"))&&(cJSON_HasObjectItem(tasklist,"working"))\
		&&(cJSON_HasObjectItem(tasklist,"deviceType"))&&(cJSON_HasObjectItem(tasklist,"deviceState"))&&(cJSON_HasObjectItem(tasklist,"deviceEndpoint")))
		{
			nodeid = cJSON_GetObjectItem(tasklist,"nodeId");
			workstate = cJSON_GetObjectItem(tasklist,"working");
			devtype = cJSON_GetObjectItem(tasklist,"deviceType");
			devstate = cJSON_GetObjectItem(tasklist,"deviceState");
			devep = cJSON_GetObjectItem(tasklist,"deviceEndpoint");
		}
		else
		{
			HSZDeviceError(HSZ_ERR_DEVICE_LIST,NULL,NULL);
			
			return -1;
		}
		
		nnodeid = strtol(nodeid->valuestring,NULL,16);
		ndevtype = strtol(devtype->valuestring,NULL,16);
		nworkstate = strtol(workstate->valuestring,NULL,16);
		
		DEBUG_ZONE(LOG_ZONE_MY,"nnodeid:0x%04x,ndevtype:0x%04x,nworkstate:0x%04x.\n",nnodeid,ndevtype,nworkstate); 
		
		if (devep)
		{
			if((cJSON_HasObjectItem(devep,"clusterInfo"))&&(cJSON_HasObjectItem(devep,"endpoint"))&&(cJSON_HasObjectItem(devep,"eui64")))
			{
				devep_ep = cJSON_GetObjectItem(devep,"endpoint");
				devep_eui = cJSON_GetObjectItem(devep,"eui64");
				
				nendpoint = devep_ep->valueint;
				
				DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"nendpoint:0x%04x.\n",nendpoint); 
				
				taskClustArry = cJSON_GetObjectItem(devep,"clusterInfo");/* ��ȡCluster Info ���� */				
				
			}
			else
			{
				HSZDeviceError(HSZ_ERR_DEVICE_LIST,NULL,NULL);
			
				return -1;
			}
		}
		
		if(taskClustArry!=NULL)
		{
			clusterlist = taskClustArry->child;/* Cluster Info�Ӷ��� */
			clusterArrySize = cJSON_GetArraySize(taskClustArry);/*Cluster Info�����С */

			DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"clusterArrySize:%d\n",clusterArrySize);  
		}
		
		while(clusterlist!=NULL)
		{
			if((cJSON_HasObjectItem(clusterlist,"clusterId"))&&(cJSON_HasObjectItem(clusterlist,"clusterType")))
			{
				
				clusterid = cJSON_GetObjectItem(clusterlist,"clusterId");
				clustertype = cJSON_GetObjectItem(clusterlist,"clusterType");
			}
			else
			{
				HSZDeviceError(HSZ_ERR_DEVICE_LIST,NULL,NULL);
			
				return -1;
			}
			
			DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"clusterId:%s,clusterType:%s\n",clusterid->valuestring,clustertype->valuestring);  
			
			nclusterid = strtol(clusterid->valuestring,NULL,16);
			
			DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"nclusterid:0x%04x.\n",nclusterid);
			
			if(0x00 == strcmp(clustertype->valuestring,"In"))
			{
#if 0
				if((ZCL_ON_OFF_CLUSTER_ID == nclusterid)||(ZCL_IAS_ZONE_CLUSTER_ID == nclusterid)||(ZCL_WINDOW_COVERING_CLUSTER_ID == nclusterid))
				{
					DEBUG_ZONE(LOG_ZONE_MY,"Device Join on,Mac addr:%s,cluster id:0x%0x,endpoint:0x%02x.\n",devep_eui->valuestring,nclusterid,devep_ep->valueint);
					HSZSetLineOnInfoBuffer(HSZRemoveMacAddr0X(devep_eui->valuestring),nclusterid,devep_ep->valueint);
				}
#endif 				
				HSZBindNewDeviceReadAndConfigReport(HSZRemoveMacAddr0X(devep_eui->valuestring),nendpoint,nclusterid);	
			}
			
			clusterlist = clusterlist->next;
		}
	
		tasklist = tasklist->next;  
	}
	
	DEBUG_ZONE(LOG_ZONE_MY,"HSZSetJoinOnstatusInfo.\n"); 
	
	HSZSetJoinOnstatusInfo(HSZRemoveMacAddr0X(devep_eui->valuestring));
	
	
	if (nodeid && devtype && devstate && devep && devep_ep && devep_eui && strlen(devep_eui->valuestring)>=16)
	{
		strupr_c(devep_eui->valuestring);
		
		/*����豸���Ͳ��ǣ�Contact sensor/Smart plug/Occupancy Sensor/Device �������ͣ������ϱ��豸������Ϣ*/
		if((ndevtype!=DEVICE_TYPE_SENSOR)&&(ndevtype!=DEVICE_TYPE_CONTACT_SENSOR)&&(ndevtype!=DEVICE_TYPE_OCCUPANCY_SENSOR))
		{
			/* ֪ͨ�ϲ� */
			if (hsbk_devicejoin!=NULL)
			{
				hsbk_devicejoin(HSZGatewayMac(),devtype->valuestring,HSZRemoveMacAddr0X(devep_eui->valuestring),arrySize);
			}
			
			HSZUpdateDeviceJoinStatus(HSZRemoveMacAddr0X(devep_eui->valuestring),DEVICE_JOIN_ON_FINISH);
		}
	}
	else
	{
		HSZDeviceError(-1,NULL,NULL);
	}
	
	/* �����豸�б� */
	HSZGetDeviceList(HSZGatewayMac());
	
	return 0;
}



/*****************************************************************************
 �� �� ��  : HSZHandleZCLResponse
 ��������  : ���ĵ���Ϣ�󣬴���ZCL Response
 �������  : cJSON *jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��12��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleZCLResponse(cJSON *jsonroot)
{
	int nclusterid = -1;
	
	cJSON *clu = NULL;
	cJSON *cmd = NULL;
	cJSON *devep = NULL;
	cJSON *cmddata = NULL;
	cJSON *eui64 = NULL;
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"clusterid"))
	{
		clu = cJSON_GetObjectItem(jsonroot,"clusterid");
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"commandid"))
	{
		cmd = cJSON_GetObjectItem(jsonroot,"commandid");
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"commanddata"))
	{
		cmddata = cJSON_GetObjectItem(jsonroot,"commanddata");
	}
	
	if (clu)
	{
		nclusterid = strtol(clu->valuestring,NULL,16);
	}
	
	if (clu && cmd && cmddata)
	{
		int ncmdid = -1;
		int dev_endpoint = 0x00;
		int nep = -1;
		
		ncmdid = strtol(cmd->valuestring,NULL,16);
		
		if(0x01 == cJSON_HasObjectItem(jsonroot,"deviceendpoint"))
		{
			devep = cJSON_GetObjectItem(jsonroot,"deviceendpoint");
		}
		
		if (devep!=NULL)
		{
			if(0x01 == cJSON_HasObjectItem(devep,"eui64"))
			{
				eui64 = cJSON_GetObjectItem(devep,"eui64");
			}
			
			if(0x01 == cJSON_HasObjectItem(devep,"endpoint"))
			{
				dev_endpoint = cJSON_GetObjectItem(devep,"endpoint")->valueint;
			}
			
			DEBUG_ZONE(LOG_ZONE_TEST,"eui:%s,endpoint:%d.\r\n",eui64->valuestring,nep);
		}
		
		switch(nclusterid)
		{
			case ZCL_IAS_ZONE_CLUSTER_ID:
			{		
				switch(ncmdid)
				{
					case ZCL_ZONE_STATUS_CHANGE_NOTIFICATION_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_IAS_ZONE_CLUSTER_ID:ZCL_ZONE_STATUS_CHANGE_NOTIFICATION_COMMAND_ID.\n");
						HSZProcessZoneStatusChangeNotifyCmdData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_ZONE_ENROLL_REQUEST_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_IAS_ZONE_CLUSTER_ID:ZCL_ZONE_ENROLL_REQUEST_COMMAND_ID.\n");
						HSZProcessZoneEnrollRequestCmdData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
						
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_IAS_ZONE_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessZoneReportAttributeCmdData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
				
			}
			break;
			
			case ZCL_ON_OFF_CLUSTER_ID:
			{			
				switch(ncmdid)
				{
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{	
						/*		
						{"clusterid":"0x0006","commandid":"0x0a","commandData":"0x00001001","clusterspecific":false,"deviceendpoint":{"eui64":"0x000b57fffe07ab16","endpoint":1}}
						
						*/
					
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_ON_OFF_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");

						if (cmddata!=NULL)
						{
							HSZProcessOnOffDeviceReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
						}
						
					}
					break;
					
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						/*
						{"clusterId":"0x0006","commandId":"0x01","commandData":"0x0000001000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000D6F000D578BBB","endpoint":1}}
						
						*/
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_ON_OFF_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						
						if (cmddata!=NULL)
						{
							HSZProcessOnOffDeviceReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
						}
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case ZCL_OCCUPANCY_SENSING_CLUSTER_ID:
			{				
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_OCCUPANCY_SENSING_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						/*						{"clusterId":"0x0406","commandId":"0x01","commandData":"0x0000001801","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE623488","endpoint":1}}.
						
						{"clusterId":"0x0406","commandId":"0x01","commandData":"0x0100003000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE623488","endpoint":1}}.

						*/				                       						
						HSZProcessOccupancySensorReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						/*						{"clusterId":"0x0406","commandId":"0x0A","commandData":"0x00001801","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE623488","endpoint":1}}.
						
						{"clusterId":"0x0406","commandId":"0x0A","commandData":"0x01003000","clusterSpecific":false,"deviceEndpoint":{"eui64":"0x000B57FFFE623488","endpoint":1}}.
						*/
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_OCCUPANCY_SENSING_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						
						HSZProcessOccupancySensorReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case ZCL_LEVEL_CONTROL_CLUSTER_ID:
			{
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_LEVEL_CONTROL_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessLevelControlDeviceReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_LEVEL_CONTROL_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessLevelControlDeviceReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case ZCL_COLOR_CONTROL_CLUSTER_ID:
			{
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_COLOR_CONTROL_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessColorControlDeviceReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_COLOR_CONTROL_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessColorControlDeviceReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case ZCL_OTA_BOOTLOAD_CLUSTER_ID:
			{
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_OTA_BOOTLOAD_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessOtaAndBootloadReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_OTA_BOOTLOAD_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessOtaAndBootloadReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case ZCL_POWER_CONFIG_CLUSTER_ID:
			{
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_POWER_CONFIG_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessPowerConfigReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_POWER_CONFIG_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessPowerConfigReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case ZCL_ALARM_CLUSTER_ID:
			{
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_ALARM_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessAlarmReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_ALARM_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessAlarmReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case ZCL_SIMPLE_METERING_CLUSTER_ID:
			{
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_SIMPLE_METERING_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessSimpleMeterReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_SIMPLE_METERING_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessSimpleMeterReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID:
			{
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessElectricalMeasurementReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_ELECTRICAL_MEASUREMENT_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessElectricalMeasurementReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case ZCL_ILLUM_MEASUREMENT_CLUSTER_ID:
			{
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_ILLUM_MEASUREMENT_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessIlluminanceMeasurementReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_ILLUM_MEASUREMENT_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessIlluminanceMeasurementReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case ZCL_WINDOW_COVERING_CLUSTER_ID:
			{
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_WINDOW_COVERING_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessWindowCoveringReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_WINDOW_COVERING_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessWindowCoveringReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			
			case ZCL_GROUPS_CLUSTER_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_GROUPS_CLUSTER_ID.\n");
			}
			break;
			
			case ZCL_SCENES_CLUSTER_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_SCENES_CLUSTER_ID.\n");
			}
			break;
			
			
			case OWNERSHIP_ONE_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"OWNERSHIP_ONE_ID.\n");
				
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"OWNERSHIP_ONE_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessOwnshipOneReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"OWNERSHIP_ONE_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessOwnshipOneReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case OWNERSHIP_TWO_ID:
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"OWNERSHIP_TWO_ID.\n");
				
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"OWNERSHIP_TWO_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessOwnshipTwoReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"OWNERSHIP_TWO_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessOwnshipTwoReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case ZCL_IAS_WD_CLUSTER_ID:
			{
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_IAS_WD_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessIASWDReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_IAS_WD_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessIASWDReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case ZCL_DIAGNOSTICS_CLUSTER_ID:/*���մ����ź�ǿ��*/
			{
				switch(ncmdid)
				{
					case ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_DIAGNOSTICS_CLUSTER_ID:ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID.\n");
						HSZProcessDiagnosticsReadData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_DIAGNOSTICS_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessDiagnosticsReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			
			case ZCL_DOOR_LOCK_CLUSTER_ID:/*�������ݽ��� */
			{
				switch(ncmdid)
				{
					case ZCL_OPERATION_EVENT_NOTIFICATION_COMMAND_ID:/*�����ϱ� */
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_DOOR_LOCK_CLUSTER_ID:ZCL_OPERATION_EVENT_NOTIFICATION_COMMAND_ID.\n");
						// HSZProcessSmartLockEventData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:/*��������Report����*/
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_DOOR_LOCK_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case LOCK_CLUSTER_ID:/*�������ݽ��� */
			{
				switch(ncmdid)
				{
					case ZCL_REPORT_ATTRIBUTES_COMMAND_ID:/*��������Report����*/
					{
						DEBUG_ZONE(LOG_ZONE_TEST,"ZCL_DOOR_LOCK_CLUSTER_ID:ZCL_REPORT_ATTRIBUTES_COMMAND_ID.\n");
						HSZProcessSmartLockReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
					}
					break;
					
					default:
					break;
				}
			}
			break;
			
			case THREE_IN_ONE_GAS1:
			{
				if(ZCL_REPORT_ATTRIBUTES_COMMAND_ID == ncmdid)  /*����һ���崫���� �豸����*/
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"THERE_IN_ONE_GAS1:Report device data.\n");
					HSZProcessThreeInOneDeviceData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
				}
			}
			break;
			case THREE_IN_ONE_GAS2:
			{
				if(ZCL_REPORT_ATTRIBUTES_COMMAND_ID == ncmdid)  /*����һ���崫���� �豸����*/
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"THERE_IN_ONE_GAS2:Report device parameter.\n");
					HSZProcessThreeInOneParameterData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
				}
			}
			break;
			
			case CLOTHES_HANGER_CLUSTER:						/*�������»� Report�豸����*/
			{
				if(ZCL_REPORT_ATTRIBUTES_COMMAND_ID == ncmdid)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"CLOTHES_HANGER_CLUSTER:Report device parameter.\n");
					HSZProcessClothesHangerReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);	
				}
			}
			break;
			case ZIGBEE_TO_485:									/*ZigBeeת485 Report�豸����*/
			{
				DEBUG_ZONE(LOG_ZONE_TEST,"ZIGBEE_TO_485:Report zigbee to 485 device parameter.\n");
				HSZProcessZigBeeTo485ReportData(HSZRemoveMacAddr0X(eui64->valuestring),dev_endpoint,cmddata->valuestring);
			}
			break;
			default:
			
			break;
			
		}
		
	}
	
	return 0;
}

/*****************************************************************************
 �� �� ��  : HSZHandleAPSResponse
 ��������  : ����APS ��Ϣ��Ӧ
 �������  : cJSON *jsonroot  
 �������  : ��
 �� �� ֵ  :  
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleAPSResponse(cJSON *jsonroot)
{
	char status_buf[2] = {0};
	int status = -1;
	
	char cmdid_buf[2] = {0};
	int commandid = -1;
	
	char cluster_buf[4] = {0};
	int clusterid = -1;
	
	char temp_buf[10] = {0};
	
	
	cJSON *apstype = NULL;
	cJSON *apseui = NULL;
	cJSON *apsstatus = NULL;
	cJSON *apsclusterid = NULL;
	cJSON *apscommandid = NULL;
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"statusType"))
	{
		apstype = cJSON_GetObjectItem(jsonroot,"statusType");
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"eui64"))
	{
		apseui = cJSON_GetObjectItem(jsonroot,"eui64");
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"status"))
	{
		apsstatus = cJSON_GetObjectItem(jsonroot,"status");
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"clusterId"))
	{
		apsclusterid = cJSON_GetObjectItem(jsonroot,"clusterId");
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"commandId"))
	{
		apscommandid = cJSON_GetObjectItem(jsonroot,"commandId");
	}
	
	DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"The left device EUI is:%s.\n",apstype->valuestring);
	DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"The left device EUI is:%s.\n",HSZRemoveMacAddr0X(apseui->valuestring));
	DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"The left device EUI is:%s.\n",apsstatus->valuestring);
	DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"The left device EUI is:%s.\n",apsclusterid->valuestring);
	DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"The left device EUI is:%s.\n",apscommandid->valuestring);
	
	sprintf(temp_buf,"%s",apsstatus->valuestring);
	strncpy(status_buf,&temp_buf[2],2);
	status = strtol(status_buf,NULL,16);
	
	sprintf(temp_buf,"%s",apscommandid->valuestring);
	strncpy(cmdid_buf,&temp_buf[2],2);
	commandid = strtol(cmdid_buf,NULL,16);
	
	sprintf(temp_buf,"%s",apsclusterid->valuestring);
	strncpy(cluster_buf,&temp_buf[2],2);/* ȡcluster���ֽ�*/
	clusterid = strtol(cluster_buf,NULL,16);
	
	strncpy(cluster_buf,&temp_buf[4],2);/* ȡcluster���ֽ�*/
	clusterid = (clusterid<<8)|(strtol(cluster_buf,NULL,16));
	
	switch(clusterid)
	{
		case ZCL_ON_OFF_CLUSTER_ID:
		{
			/*
				On-Off light

				Success��{"statusType":"apsAck","eui64":"0x000B57FFFE389B6E","status":"0x66","clusterId":"0x0006","commandId":"0x02"}

				Fail��{"statusType":"apsAck","eui64":"0x000B57FFFE389B6E","status":"0x00","clusterId":"0x0006","commandId":"0x02"}

				{"statusType":"apsAck","eui64":"0x000B57FFFE6E3D4C","status":"0x00","clusterId":"0x0006","commandId":"0x0B"}.

			*/
			
			if(0x02 == commandid)
			{
				if(0x66 == status)
				{
					DEBUG_ZONE(LOG_ZONE_MY,"Fail.\n");

					if (hsbk_commandfailed!=NULL)
					{
						hsbk_commandfailed(HSZGatewayMac(),HSZRemoveMacAddr0X(apseui->valuestring),status);
					}
				}
				else if(0x00 == status)
				{				

					DEBUG_ZONE(LOG_ZONE_TEST,"Success.\n");         
					
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"No-0x02.\n");
				}
			}
			else if(0x0B == commandid)
			{
				if(0x66 == status)
				{								
					DEBUG_ZONE(LOG_ZONE_MY,"No-receive.\n");
					
					if (hsbk_commandfailed!=NULL)
					{
						hsbk_commandfailed(HSZGatewayMac(),HSZRemoveMacAddr0X(apseui->valuestring),status);
					}
				}
				else if(0x00 == status)
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"Receive.\n");				
				}
				else
				{
					DEBUG_ZONE(LOG_ZONE_TEST,"No-cmd.\n");
				}
			}
		}
		break;
		
		
		case ZCL_IAS_ZONE_CLUSTER_ID:
		{
			/*
			DEVICE_TYPE_CONTACT_SENSOR
			
			{"statusType":"apsAck","eui64":"0x00124B000B7B1156","status":"0x00","clusterId":"0x0500","commandId":"0x02"}.
			
			{"statusType":"apsAck","eui64":"0x00124B000B7B1156","status":"0x00","clusterId":"0x0500","commandId":"0x0B"}.
			*/
		}
		break;
		
		
		default:
		break;
	}

	return 0;
}


/*****************************************************************************
 �� �� ��  : HSZHandleDeviceLeft
 ��������  : �������ߵ��豸��Ϣ
 �������  : cJSON *jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleDeviceLeft(cJSON *jsonroot)
{
	 /*{"eui64":"0x000B57FFFE389B6E"}*/
	 
	cJSON *euidevice = NULL;
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"eui64"))
	{
		euidevice = cJSON_GetObjectItem(jsonroot,"eui64");
	}
	
	/*�ϱ��ϲ�ӿ�*/
	if (hsbk_deviceleave!=NULL)
	{
		hsbk_deviceleave(HSZGatewayMac(),HSZRemoveMacAddr0X(euidevice->valuestring));
	}
	
	DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"The left device EUI is:%s.\n",HSZRemoveMacAddr0X(euidevice->valuestring));
	
	/* �����豸�б� */
	HSZGetDeviceList(HSZGatewayMac());

	return 0;
}
/*****************************************************************************
 �� �� ��  : HSZHandleDeveiceStateChange
 ��������  : �����豸״̬�����ı�
 �������  : cJSON *jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleDeveiceStateChange(cJSON *jsonroot)
{
	/* 
	{"eui64":"0x000B57FFFE389B6E","deviceState":16}.
	*/

	cJSON *deviceeui = NULL;
	cJSON *devicestate = NULL;
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"eui64"))
	{
		deviceeui = cJSON_GetObjectItem(jsonroot,"eui64");
	}
	
    if(0x01 == cJSON_HasObjectItem(jsonroot,"deviceState"))
	{
		devicestate = cJSON_GetObjectItem(jsonroot,"deviceState");
	}
	
	DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"The device nodeEui is:%s.\n",HSZRemoveMacAddr0X(deviceeui->valuestring));
	DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"The device devicestate is:%d.\n",devicestate->valueint);
	
	return 0;
}

/*****************************************************************************
 �� �� ��  : HSZHandleRelays
 ��������  : ��������Relay Topic
 �������  : cJSON *jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleRelays(cJSON *jsonroot)
{
	/* 
	mqtt message format: 
	{
		"relays":
		[
			{
				"inDeviceEndpoint":	
				{
					"eui64":	"0x3030323241333030",
					"endpoint":	1
				},
				"outDeviceEndpoint":	
				{
					"eui64":	"0x3030323241333030",
					"endpoint":	2
				}
			}
		]
	}
	*/
	
	int arrySize;
	cJSON *tasklist = NULL;
	
	cJSON *indevice = NULL;
	cJSON *indevEui = NULL;
	cJSON *indevEndpoint = NULL;
	
	cJSON *outdevice = NULL;
	cJSON *outdevEui = NULL;
	cJSON *outdevEndpoint = NULL;
	
	cJSON *taskArry = NULL;
	
	/* ����Relays�б� */
	if(0x01 == cJSON_HasObjectItem(jsonroot,"relays"))
	{
		taskArry = cJSON_GetObjectItem(jsonroot,"relays");/* ��ȡ���� */
	}
	else
	{
		return -1;
	}

	if (taskArry!=NULL)
	{
		tasklist = taskArry->child;/* �Ӷ��� */
		arrySize = cJSON_GetArraySize(taskArry);/* �����С */
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"arrySize:%d \n",arrySize);
	}
	
	while(tasklist!= NULL)  
	{
		{
			indevice = cJSON_GetObjectItem(tasklist,"inDeviceEndpoint");
			indevEui = cJSON_GetObjectItem(indevice,"eui64");
			indevEndpoint = cJSON_GetObjectItem(indevice,"endpoint");
		}
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"The indevice EUI is:%s and the indevice endpoint is :%d��\n",HSZRemoveMacAddr0X(indevEui->valuestring),indevEndpoint->valueint);

		outdevice = cJSON_GetObjectItem(tasklist,"outDeviceEndpoint");
		outdevEui = cJSON_GetObjectItem(outdevice,"eui64");
		outdevEndpoint = cJSON_GetObjectItem(outdevice,"endpoint");
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"The outdevice EUI is:%s and the outdevice endpoint is :%d��\n",HSZRemoveMacAddr0X(outdevEui->valuestring),outdevEndpoint->valueint);
		
		tasklist=tasklist->next; 
	}
	
	return 0;
}

/*****************************************************************************
 �� �� ��  : HSZHandleSettings
 ��������  : ��������settings
 �������  : cJSON *jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleSettings(cJSON *jsonroot)
{
	/*
	{"ncpStackVersion":"5.9.0-103","networkUp":true,"networkPanId":"0xCBBA","radioTxPower":20,"radioChannel":14}
	
	{"ncpStackVersion":"5.9.2-145","znet":true,"networkPanId":"0x4003","PanId":"0x4003","TxPwr":20,"Channel":14}.
	*/
	
	/*
	{"ncpStackVersion":"5.9.0-103","networkUp":false}.
	*/
	
	cJSON *stackVersion = NULL;
	cJSON *networkup = NULL;
	cJSON *networkpid = NULL;
	cJSON *txpower = NULL;
	cJSON *channel = NULL;
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"ncpStackVersion"))
	{
		stackVersion = cJSON_GetObjectItem(jsonroot,"ncpStackVersion");
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"znet"))
	{
		networkup = cJSON_GetObjectItem(jsonroot,"znet");
	}

	DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"NcpStackVersion is:%s.\n",stackVersion->valuestring);
	DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"NetworkUp is:%d.\n",networkup->valueint);
	
	strncpy(Zigbee_stack,stackVersion->valuestring,strlen(stackVersion->valuestring));
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"PanId"))
	{
		networkpid = cJSON_GetObjectItem(jsonroot,"PanId");
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"NetworkPanId is:%s.\n",networkpid->valuestring);
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"TxPwr"))
	{
		txpower = cJSON_GetObjectItem(jsonroot,"TxPwr");
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"RadioTxPower is:%d.\n",txpower->valueint);
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"Channel"))
	{
		channel = cJSON_GetObjectItem(jsonroot,"Channel");
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"RadioChannel is:%d.\n",channel->valueint);
	}

	/*�������߻ص� */
	if(0x01 == networkup->valueint)
	{
		gateway_network_mode = GATEWAY_NETWORK_UP_MODE;
		
		if (hsbk_gatewaystate!=NULL)
		{
			// hsbk_gatewaystate(HSZGatewayMac(),0x01);
		}
	}
	else if(0x00 == networkup->valueint)
	{
		gateway_network_mode = GATEWAY_NETWORK_DOWN_MODE;
		
		if (hsbk_gatewaystate!=NULL)
		{
			//hsbk_gatewaystate(HSZGatewayMac(),0x00);
		}
	}
	
	return 0;
}

/*****************************************************************************
 �� �� ��  : HSZHandleExecuted
 ��������  : ��������Executed Topic
 �������  : cJSON *jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 
 �޸���ʷ      :
  1.��    ��   : 2017��6��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleExecuted(cJSON *jsonroot)
{
	/* {"command":"plugin device-table send {000B57FFFE389B6E} 1"}*/
	/* {"delayMs":100}*/
	
	cJSON *actionCommand = NULL;
	cJSON *delaytime = NULL;
	
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"command"))
	{
		
		actionCommand = cJSON_GetObjectItem(jsonroot,"command");
		
		/* �ϱ��ϲ�ӿ�*/
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"The executed actionCommand is:%s.\n",actionCommand->valuestring);
	}
	else if(0x01 == cJSON_HasObjectItem(jsonroot,"delayMs"))
	{
		char delay_time[50] ;
		
		delaytime = cJSON_GetObjectItem(jsonroot,"delayMs");
		
		/* �ϱ��ϲ�ӿ�*/
		sprintf(delay_time,"delayMs:%d",delaytime->valueint);
	
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"The Delaytime is:%d.\n",delaytime->valueint);
	}
	else
	{
		return -1;
	}

	
	
	return 0;
}

/*****************************************************************************
 �� �� ��  : HSZHandleOtaevent
 ��������  : �豸OTA����
 �������  : cJSON *jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��9��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleOtaevent(cJSON *jsonroot)
{
	cJSON *ota_messageType = NULL;
	cJSON *ota_eui64 = NULL;
	cJSON *ota_status = NULL;
	
	cJSON *ota_bytesSent = NULL;
	cJSON *ota_manufacturerId = NULL;
	
	cJSON *ota_imageTypeIdString = NULL;
	cJSON *ota_firmwareVersion = NULL;

	DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"----------------------------------------------------------.\n");
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"messageType"))
	{
		ota_messageType = cJSON_GetObjectItem(jsonroot,"messageType");
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"OTA Message Type is:%s.\n",ota_messageType->valuestring);
	}
		
	if(0x01 == cJSON_HasObjectItem(jsonroot,"eui64"))
	{
		ota_eui64 = cJSON_GetObjectItem(jsonroot,"eui64");
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"OTA Device eui is:%s.\n",HSZRemoveMacAddr0X(ota_eui64->valuestring));
	}
		
	if(0x01 == cJSON_HasObjectItem(jsonroot,"status"))
	{
		ota_status = cJSON_GetObjectItem(jsonroot,"status");
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"OTA Status is:%s.\n",ota_status->valuestring);
	}
		
	if(0x01 == cJSON_HasObjectItem(jsonroot,"bytesSent"))
	{
		ota_bytesSent = cJSON_GetObjectItem(jsonroot,"bytesSent");
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"OTA BytesSent is:0x%04X.\n",ota_bytesSent->valueint);
	}
		
	if(0x01 == cJSON_HasObjectItem(jsonroot,"manufacturerId"))
	{
		ota_manufacturerId = cJSON_GetObjectItem(jsonroot,"manufacturerId");
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"OTA ManufacturerId is:%s.\n",ota_manufacturerId->valuestring);
	}
		
	if(0x01 == cJSON_HasObjectItem(jsonroot,"imageTypeIdString"))
	{
		ota_imageTypeIdString = cJSON_GetObjectItem(jsonroot,"imageTypeIdString");
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"OTA ImageTypeId is:%s.\n",ota_imageTypeIdString->valuestring);
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"firmwareVersion"))
	{
		ota_firmwareVersion = cJSON_GetObjectItem(jsonroot,"firmwareVersion");
		
		DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"OTA FirmwareVersion is:%s.\n",ota_firmwareVersion->valuestring);
	}

	DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"----------------------------------------------------------.\n");
	
	return 0;
}

/*****************************************************************************
 �� �� ��  : HSZHandleDeviceLineStateChange
 ��������  : �豸��������״̬�ı�ӿ�
 �������  : cJSON * jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��9��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleDeviceLineStateChange(cJSON * jsonroot)
{
	/*{"eui64":"0x000B57FFFE6E3D4C","devst":1}*/
	/*{"eui64":"0x00124B000E4F862A","devst":0}.*/
	
	/*	{
		"eui64":"0x00124B001709854F",
		"devst":1,
		"devices":[
		{"nodeId":"0x0A8E",
		"working":"1",
		"deviceState":16,
		"deviceType":"0x0000",
		"deviceEndpoint":{
			"eui64":"0x00124B001709854F",
			"endpoint":2,
			"clusterInfo":
			[
			{"clusterId":"0x0000","clusterType":"In"},
			{"clusterId":"0x0003","clusterType":"In"},
			{"clusterId":"0x0004","clusterType":"In"},
			{"clusterId":"0x0005","clusterType":"In"},
			{"clusterId":"0x0006","clusterType":"In"},
			{"clusterId":"0x0000","clusterType":"Out"},
			{"clusterId":"0x0003","clusterType":"Out"},
			{"clusterId":"0x0004","clusterType":"Out"},
			{"clusterId":"0x0005","clusterType":"Out"}]}},
		{"nodeId":"0x0A8E",
		"working":"1",
		"deviceState":16,
		"deviceType":"0x0000",
		"deviceEndpoint":{
			"eui64":"0x00124B001709854F",
			"endpoint":1,
			"clusterInfo":[
			{"clusterId":"0x0000","clusterType":"In"},
			{"clusterId":"0x0003","clusterType":"In"},
			{"clusterId":"0x0004","clusterType":"In"},
			{"clusterId":"0x0005","clusterType":"In"},
			{"clusterId":"0x0006","clusterType":"In"},
			{"clusterId":"0x0000","clusterType":"Out"},
			{"clusterId":"0x0003","clusterType":"Out"},
			{"clusterId":"0x0004","clusterType":"Out"},
			{"clusterId":"0x0005","clusterType":"Out"}
			]}}]}
	*/
	
	cJSON *dev_eui = NULL;
	cJSON *dev_state = NULL;
	
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"eui64"))
	{
		dev_eui = cJSON_GetObjectItem(jsonroot,"eui64");
	}

	if(0x01 == cJSON_HasObjectItem(jsonroot,"devst"))
	{
		dev_state = cJSON_GetObjectItem(jsonroot,"devst");
				
		if (hsbk_devicelineon!=NULL)
		{
			DEBUG_ZONE(LOG_ZONE_MY,"-------Device LineState change Mac:%s,State:%d.\n",HSZRemoveMacAddr0X(dev_eui->valuestring),dev_state->valueint);
			hsbk_devicelineon(HSZGatewayMac(),HSZRemoveMacAddr0X(dev_eui->valuestring),dev_state->valueint);
		}
		else
		{
			DEBUG_ZONE(LOG_ZONE_ERROR,"hsbk_devicelineon Error.\n");
		}

	
		if(0x01 == dev_state->valueint)
		{
			cJSON *devep = NULL;
			cJSON *nodeid = NULL;
			cJSON *workstate = NULL;
			cJSON *devtype = NULL;
			cJSON *devstate = NULL;
			cJSON *devep_ep = NULL;
			cJSON *devep_eui = NULL;
			cJSON *taskClustArry = NULL;
			cJSON *clusterid = NULL;
			cJSON *clustertype = NULL;

			cJSON *taskArry = NULL;

			int devlinestate = -1;
			int device_type = -1;
			int clusterArrySize;
			int cluster_id_type = -1;

			int nnodeid = -1;
			cJSON *clusterlist = NULL;
			
			/* �����豸�б� */			
			taskArry = cJSON_GetObjectItem(jsonroot,"devices");/* ��ȡ���� */

			int arrySize;
			cJSON *tasklist = NULL;

			if (taskArry != NULL)
			{
				tasklist = taskArry->child;/* �Ӷ��� */
				arrySize = cJSON_GetArraySize(taskArry);/* �����С */

				DEBUG_ZONE(LOG_ZONE_TEST,"Devices ArrySize:%d\n",arrySize);  
			}	

			while(tasklist!=NULL)  
			{
				if((cJSON_HasObjectItem(tasklist,"nodeId"))&&(cJSON_HasObjectItem(tasklist,"working"))\
				&&(cJSON_HasObjectItem(tasklist,"deviceType"))&&(cJSON_HasObjectItem(tasklist,"deviceState"))&&(cJSON_HasObjectItem(tasklist,"deviceEndpoint")))
				{
					nodeid = cJSON_GetObjectItem(tasklist,"nodeId");
					workstate = cJSON_GetObjectItem(tasklist,"working");
					devtype = cJSON_GetObjectItem(tasklist,"deviceType");
					devstate = cJSON_GetObjectItem(tasklist,"deviceState");
					devep = cJSON_GetObjectItem(tasklist,"deviceEndpoint");

					nnodeid = strtol(nodeid->valuestring,NULL,16);
				}
				else
				{
					HSZDeviceError(HSZ_ERR_DEVICE_LIST,NULL,NULL);
					return -1;
				}

				devlinestate = strtol(workstate->valuestring,NULL,16);
				device_type = strtol(devtype->valuestring,NULL,16);

				if (devep)
				{
					if((cJSON_HasObjectItem(devep,"clusterInfo"))&&(cJSON_HasObjectItem(devep,"endpoint"))&&(cJSON_HasObjectItem(devep,"eui64")))
					{
						devep_ep = cJSON_GetObjectItem(devep,"endpoint");
						devep_eui = cJSON_GetObjectItem(devep,"eui64");
						taskClustArry = cJSON_GetObjectItem(devep,"clusterInfo");/* ��ȡCluster Info ���� */
					}
					else
					{
						HSZDeviceError(HSZ_ERR_DEVICE_LIST,NULL,NULL);
						return -1;
					}
				}

				if(taskClustArry!=NULL)
				{
					clusterlist = taskClustArry->child;/* Cluster Info �Ӷ��� */
					clusterArrySize = cJSON_GetArraySize(taskClustArry);/*Cluster Info �����С */

					DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"clusterArrySize:%d\n",clusterArrySize);  
				}

				if (devep_eui && strlen(devep_eui->valuestring)>=16)
				{
					strupr_c(devep_eui->valuestring);

					DEBUG_ZONE(LOG_ZONE_OUTPACK_MSG,"%s\n",devtype->valuestring); 
				}
				else
				{
					HSZDeviceError(-1,NULL,NULL);
				}
				
				while(clusterlist!=NULL)
				{
					if((cJSON_HasObjectItem(clusterlist,"clusterId"))&&(cJSON_HasObjectItem(clusterlist,"clusterType")))
					{
						clusterid = cJSON_GetObjectItem(clusterlist,"clusterId");
						clustertype = cJSON_GetObjectItem(clusterlist,"clusterType");
					}
					else
					{
						HSZDeviceError(HSZ_ERR_DEVICE_LIST,NULL,NULL);
						return -1;
					}

					// DEBUG_ZONE(LOG_ZONE_MY,"clusterId:%s,clusterType:%s\n",clusterid->valuestring,clustertype->valuestring);  
					
					cluster_id_type = strtol(clusterid->valuestring,NULL,16);
					
					if(0x00 == strcmp(clustertype->valuestring,"In"))
					{
						if((ZCL_ON_OFF_CLUSTER_ID == cluster_id_type)||(ZCL_IAS_ZONE_CLUSTER_ID == cluster_id_type)||(ZCL_WINDOW_COVERING_CLUSTER_ID == cluster_id_type))
						{
							DEBUG_ZONE(LOG_ZONE_MY,"change status to init,Mac:%s,devepment:0x%02x.\n",devep_eui->valuestring,devep_ep->valueint);
							
							HSZIsNeedUpdateStatus(HSZRemoveMacAddr0X(devep_eui->valuestring),devep_ep->valueint,DEVICE_INIT_STATUS);
						}
					}

					clusterlist = clusterlist->next;
				}
				
				tasklist = tasklist->next; 
			}

		}
		else if(0x00 == dev_state->valueint)
		{
			// HSZUpdateDeviceLineStatus(HSZRemoveMacAddr0X(dev_eui->valuestring),0x00,DEVICE_LINE_ON_INIT);
		}


	}

	return 0;
}

/*****************************************************************************
 �� �� ��  : HSZHandleZdoResponse
 ��������  : ZDO Response��Ϣ����
 �������  : cJSON * jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��9��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleZdoResponse(cJSON *jsonroot)
{
	
	return 0;
}

/*****************************************************************************
 �� �� ��  : HSZHandleNodeIdChanged
 ��������  : NodeId ��Ϣ����
 �������  : cJSON * jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��9��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleNodeIdChanged(cJSON *jsonroot)
{
	/*{"eui64":"0x000B57FFFE6E3D4C","nodeId":1}*/
	
	cJSON *dev_eui = NULL;
	cJSON *dev_nodeid = NULL;
	int nnodeid = -10;
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"eui64"))
	{
		dev_eui = cJSON_GetObjectItem(jsonroot,"eui64");
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"nodeId"))
	{
		dev_nodeid = cJSON_GetObjectItem(jsonroot,"nodeId");
	}
	
    if((dev_eui!=NULL)&&(dev_nodeid!=NULL))
	{
		nnodeid = strtol(dev_nodeid->valuestring,NULL,16);
		
		if(HSZUpdateNodeIdFromBuffer(HSZRemoveMacAddr0X(dev_eui->valuestring),nnodeid)<0)
		{
			/* �����豸�б� */
			HSZGetDeviceList(HSZGatewayMac());
		}
	}
	
	return 0;
}

/*****************************************************************************
 �� �� ��  : HSZHandleNcpReset
 ��������  : NodeId ��Ϣ����
 �������  : cJSON * jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��9��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleNcpReset(cJSON *jsonroot)
{
	/*{"status":0,"index":2} */
	
	cJSON *ncp_status = NULL;
	cJSON *reset_index = NULL;
	
	int reset_sattus = 101010;
	int reset_counts = -1;
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"status"))
	{
		ncp_status = cJSON_GetObjectItem(jsonroot,"status");
		reset_sattus = ncp_status->valueint;
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"index"))
	{
		reset_index = cJSON_GetObjectItem(jsonroot,"index");
		reset_counts = reset_index->valueint; 
	}
	
	if(0x00 == reset_sattus)
	{
		DEBUG_ZONE(LOG_ZONE_MY,"NCP Reset and Reset Success.\n");
		
		if (hsbk_gatewaystate!=NULL)
		{
			hsbk_gatewaystate(HSZGatewayMac(),99);
		}
	}
	else if(101010 == reset_sattus)
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"NCP Reset but report info error.\n");
	}
	else
	{
		DEBUG_ZONE(LOG_ZONE_ERROR,"NCP Reset and Reset failed.\n");
		
		if (hsbk_gatewaystate!=NULL)
		{
			hsbk_gatewaystate(HSZGatewayMac(),98);
		}
	}
	
	return 0x00;
}

/*****************************************************************************
 �� �� ��  : HSZHandlePermitJoinState
 ��������  : ��������״̬�ϱ�����
 �������  : cJSON * jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��9��11��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandlePermitJoinState(cJSON *jsonroot)
{
	/*{"result":1,"times":127}*/
	
	/*
		result=1:ʧ��,ʧ�ܺ�ϵͳ����Ϊ��ֹ����״̬,led�ָ�����
		result=0:����ɹ�
		times:ָʾ������ʱ��(��),�����������ʧ��,��ֵû������
	*/
	
	cJSON *permit_join_result = NULL;
	cJSON *permit_join_times = NULL;
	
	int join_result = -1;
	int join_times = -1;
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"result"))
	{
		permit_join_result = cJSON_GetObjectItem(jsonroot,"result");
		join_result = permit_join_result->valueint;
	}
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"times"))
	{
		permit_join_times = cJSON_GetObjectItem(jsonroot,"times");
		join_times = permit_join_times->valueint;
	}
	
	if((-1!= join_result)&&(-1!= join_times))
	{
		switch(join_result)
		{
			case 0x00:
			{
				if(join_times>0)
				{
					hsbk_gatewaypermitjoinstate(HSZGatewayMac(),0x01);
				}
				else if(0x00 == join_times)
				{
					hsbk_gatewaypermitjoinstate(HSZGatewayMac(),0x00);
				}
				else
				{
					printf("Permit join time error.\n");
				}
			}
			break;
			
			case 0x01:
			{
				hsbk_gatewaypermitjoinstate(HSZGatewayMac(),0x00);
			}
			break;
			
			default:
			break;
		}
		
	}
	
	return 0x00;
}

/*****************************************************************************
 �� �� ��  : HSZHandleNcpVersion
 ��������  : ����NCP�汾��Ϣ
 �������  : cJSON * jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��3��13��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleNcpVersion(cJSON *jsonroot)
{
	/* {"result":1,"ncpver":2003} */
	
	/*
		result=1:ʧ��
		result=0:�ɹ�
		ncpver:���ذ汾��,����int,Ϊ16���� 2003����0x2003
	*/

	cJSON *ncp_result = NULL;
	cJSON *ncp_version = NULL;

	int zigbee_ncp_result = -1;
	int zigbee_ncp_version = -1;
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"result"))
	{
		ncp_result = cJSON_GetObjectItem(jsonroot,"result");
		zigbee_ncp_result = ncp_result->valueint;
		
		if(0x00 == zigbee_ncp_result)
		{
			if(0x01 == cJSON_HasObjectItem(jsonroot,"ncpver"))
			{
				ncp_version = cJSON_GetObjectItem(jsonroot,"ncpver");
				
				zigbee_ncp_version = ncp_version->valueint;
				
				DEBUG_ZONE(LOG_ZONE_MY,"NCP Version:0x%04x\n",zigbee_ncp_version);
				hsbk_ncpupdateresult(HSZGatewayMac(),zigbee_ncp_version);
			}
		}
	}
	
	return 0x00;
}

/*****************************************************************************
 �� �� ��  : HSZHandleZigbeeNetworkState
 ��������  : ����NCP����״̬��Ϣ
 �������  : cJSON * jsonroot  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��3��13��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZHandleZigbeeNetworkState(cJSON *jsonroot)
{
	/*{"znet":"OK"}.*/
	
	cJSON *zigbee_network_state = NULL;
	
	if(0x01 == cJSON_HasObjectItem(jsonroot,"znet"))
	{
		zigbee_network_state = cJSON_GetObjectItem(jsonroot,"znet");
		
		if(0x00 == strcmp(zigbee_network_state->valuestring,"OK"))
		{
			if (hsbk_gatewaystate!=NULL)
			{
				hsbk_gatewaystate(HSZGatewayMac(),0x01);
			}
		}
		else
		{
			if (hsbk_gatewaystate!=NULL)
			{
				hsbk_gatewaystate(HSZGatewayMac(),0x00);
			}
		}
	}
	
	return 0x00;
}
/*****************************************************************************
 �� �� ��  : HSZReadLineDeviceAttribute 
 ��������  : ��ȡ�����豸��״̬
 �������  : char* dev_mac :�����豸MAC��ַ        
             int epcount �������豸endpoint           
             int clusterid :�����豸clusterid 
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��2��22��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ɺ���

*****************************************************************************/
int HSZReadLineDeviceAttribute(char* dev_mac,int epcount,int clusterid)
{
	int rc = -1;
	
	switch(clusterid)
	{
		case ZCL_ON_OFF_CLUSTER_ID:					/*ZigBee On/off Cluster*/
		{
			/* Read ON_OFF Attribute */
			
			DEBUG_ZONE(LOG_ZONE_MY,"Read Attribute:Device Mac:%s,Endpoint:0x%04x,Cluster ID:0x%04x,Attribute ID:ZCL_ON_OFF_ATTRIBUTE_ID.\n",dev_mac,epcount,clusterid);
			
			rc = HSZRequestAttribute(dev_mac,epcount,ZCL_ON_OFF_CLUSTER_ID,ZCL_ON_OFF_ATTRIBUTE_ID);
		}
		break;
		
		case ZCL_IAS_ZONE_CLUSTER_ID:				/*ZigBee IAS Zone Cluster*/
		{
			/*Read Zone Status*/
			
			DEBUG_ZONE(LOG_ZONE_MY,"Read Attribute:Device Mac:%s,Endpoint:0x%04x,Cluster ID:0x%04x,Attribute ID:ZCL_ZONE_STATUS_ATTRIBUTE_ID.\n",dev_mac,epcount,clusterid);
			
			rc = HSZRequestAttribute(dev_mac,epcount,ZCL_IAS_ZONE_CLUSTER_ID,ZCL_ZONE_STATUS_ATTRIBUTE_ID);
		}
		break;
		
		case ZCL_WINDOW_COVERING_CLUSTER_ID:		/*Window covering*/
		{	
			DEBUG_ZONE(LOG_ZONE_MY,"Read Attribute:Device Mac:%s,Endpoint:0x%04x,Cluster ID:0x%04x,Attribute ID:ZCL_COVERING_TYPE_ATTRIBUTE_ID.\n",dev_mac,epcount,clusterid);
			
			rc = HSZRequestAttribute(dev_mac,epcount,ZCL_WINDOW_COVERING_CLUSTER_ID,ZCL_COVERING_TYPE_ATTRIBUTE_ID);
		}
		break;
		
		default:
		break;
	}
	
	return rc;
}
