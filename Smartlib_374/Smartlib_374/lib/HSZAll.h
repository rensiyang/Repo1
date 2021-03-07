/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : HSZSelfProtocolSL500Lock.c
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2019年03月07日
  最近修改   :
  功能描述   : 门锁工程中所有头文件
  函数列表   :
  修改历史   :
  1.日    期   : 
    作    者   : Zigbee Project Team
    修改内容   : 创建文件
******************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include "HSZCommon.h"
#include "cJSON.h"
#include "HSZMessage.h"
#include "HSZCommandId.h"
#include "HSZClusterId.h"
#include "HSZAttributeId.h"
#include "HSZAttributeType.h"
#include "HSZDeviceData.h"
#include "HSZSelfProtocolSL500Lock.h"
#include "HSZSmartLockMessage.h"

/*****************************RSY*******************************/
#include "ha_client.h"
//#include "HSZAttributeSize.h"
#include "HSZBindDevice.h"
#include "HSZHandleZclData.h"
#include "HSZInterface.h"
#include "HSZSelfProtocolLock.h"
#include "ringbuffer.h"
#include <ctype.h>
#include "ha_client.h"
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include "ringbuffer.h"
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h> 
#include <netinet/tcp.h>
#include <netdb.h>

#if !defined(WIN32)
#include <unistd.h>
#else#include <windows.h>
#endif