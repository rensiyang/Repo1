/******************************************************************************

                  ��Ȩ���� (C), 2017-2025, �ൺ�����ǻۼҾ�ϵͳ�ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : HSZBindDevice.h
  �� �� ��   : ����
  ��    ��   : Zigbee Project Team
  ��������   : 2017��10��23��
  ����޸�   :
  ��������   : ����cluster����Ϣ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2017��10��23��
    ��    ��   : Zigbee Project Team
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _HSZBINDDEVICE_H_
#define _HSZBINDDEVICE_H_

/*�����ȡ�豸����*/
int HSZRequestAttribute(char* dev_mac,int dev_ep,int cluid,int arrtibutestr);

/*���ذ��豸����ҪָON_OFF�ࣩ*/
int HSZBindNewDevice(int epcount,char* dev_mac,int clusterid);

/*���豸������Ϣ�ϱ�����*/
int HSZNewDeviceConfigReport(int epcount,char* dev_mac,int clusterid,int arrtibutestr,int arrtibute_type,\
								int report_max_time,char *max_change_amount);
								
/*��Դ���豸������Ϣ�ϱ�����*/								
int HSZNewDeviceConfigReportWithPower(int epcount,char* dev_mac,int clusterid,int arrtibutestr,int arrtibute_type,\
int report_max_time,char *max_change_amount);
								
/*���������豸Cluster ���豸���ͣ���ȡ�豸���ԣ������ϱ�Report*/
int HSZBindNewDeviceReadAndConfigReport(char* dev_mac,int epcount,int clusterid);

#endif
