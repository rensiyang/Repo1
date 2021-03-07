/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZBindDevice.h
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2017年10月23日
  最近修改   :
  功能描述   : 处理cluster绑定信息
  函数列表   :
  修改历史   :
  1.日    期   : 2017年10月23日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

******************************************************************************/
#ifndef _HSZBINDDEVICE_H_
#define _HSZBINDDEVICE_H_

/*请求读取设备属性*/
int HSZRequestAttribute(char* dev_mac,int dev_ep,int cluid,int arrtibutestr);

/*网关绑定设备（主要指ON_OFF类）*/
int HSZBindNewDevice(int epcount,char* dev_mac,int clusterid);

/*新设备入网消息上报配置*/
int HSZNewDeviceConfigReport(int epcount,char* dev_mac,int clusterid,int arrtibutestr,int arrtibute_type,\
								int report_max_time,char *max_change_amount);
								
/*有源新设备入网消息上报配置*/								
int HSZNewDeviceConfigReportWithPower(int epcount,char* dev_mac,int clusterid,int arrtibutestr,int arrtibute_type,\
int report_max_time,char *max_change_amount);
								
/*根据入网设备Cluster 绑定设备类型，读取设备属性，配置上报Report*/
int HSZBindNewDeviceReadAndConfigReport(char* dev_mac,int epcount,int clusterid);

#endif
