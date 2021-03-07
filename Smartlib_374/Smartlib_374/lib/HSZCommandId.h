/******************************************************************************

                  版权所有 (C), 2017-2025, 青岛海信智慧家居系统股份有限公司

 ******************************************************************************
  文 件 名   : command-id.h
  版 本 号   : 初稿
  作    者   : Zigbee Project Team
  生成日期   : 2017年8月31日
  最近修改   :
  功能描述   : Command ID 文档描述
  函数列表   :
  修改历史   :
  1.日    期   : 2017年8月31日
    作    者   : Zigbee Project Team
    修改内容   : 创建文件

******************************************************************************/
#ifndef _COMMAND_ID_
#define _COMMAND_ID_


/* Global commands --Either direction */

#define ZCL_READ_ATTRIBUTES_COMMAND_ID                       0x00    
#define ZCL_READ_ATTRIBUTES_RESPONSE_COMMAND_ID              0x01    
#define ZCL_WRITE_ATTRIBUTES_COMMAND_ID                      0x02    
#define ZCL_WRITE_ATTRIBUTES_UNDIVIDED_COMMAND_ID            0x03    
#define ZCL_WRITE_ATTRIBUTES_RESPONSE_COMMAND_ID             0x04    
#define ZCL_WRITE_ATTRIBUTES_NO_RESPONSE_COMMAND_ID          0x05    
#define ZCL_CONFIGURE_REPORTING_COMMAND_ID                   0x06    
#define ZCL_CONFIGURE_REPORTING_RESPONSE_COMMAND_ID          0x07    
#define ZCL_READ_REPORTING_CONFIGURATION_COMMAND_ID          0x08    
#define ZCL_READ_REPORTING_CONFIGURATION_RESPONSE_COMMAND_ID 0x09    
#define ZCL_REPORT_ATTRIBUTES_COMMAND_ID                     0x0A    
#define ZCL_DEFAULT_RESPONSE_COMMAND_ID                      0x0B    
#define ZCL_DISCOVER_ATTRIBUTES_COMMAND_ID                   0x0C    
#define ZCL_DISCOVER_ATTRIBUTES_RESPONSE_COMMAND_ID          0x0D    
#define ZCL_READ_ATTRIBUTES_STRUCTURED_COMMAND_ID            0x0E    
#define ZCL_WRITE_ATTRIBUTES_STRUCTURED_COMMAND_ID           0x0F    
#define ZCL_WRITE_ATTRIBUTES_STRUCTURED_RESPONSE_COMMAND_ID  0x10    
#define ZCL_DISCOVER_COMMANDS_RECEIVED_COMMAND_ID            0x11    
#define ZCL_DISCOVER_COMMANDS_RECEIVED_RESPONSE_COMMAND_ID   0x12    
#define ZCL_DISCOVER_COMMANDS_GENERATED_COMMAND_ID           0x13    
#define ZCL_DISCOVER_COMMANDS_GENERATED_RESPONSE_COMMAND_ID  0x14    
#define ZCL_DISCOVER_ATTRIBUTES_EXTENDED_COMMAND_ID          0x15    
#define ZCL_DISCOVER_ATTRIBUTES_EXTENDED_RESPONSE_COMMAND_ID 0x16  


/*  
 * Command types for cluster: Basic
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Client to server */
#define ZCL_RESET_TO_FACTORY_DEFAULTS_COMMAND_ID			 0x00	 


/*  
 * Command types for cluster: Identify
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_IDENTIFY_QUERY_RESPONSE_COMMAND_ID				 0x00	 


#define ZCL_IDENTIFY_COMMAND_ID								 0x00	 
#define ZCL_IDENTIFY_QUERY_COMMAND_ID						 0x01	 
#define ZCL_E_Z_MODE_INVOKE_COMMAND_ID						 0x02	 
#define ZCL_UPDATE_COMMISSION_STATE_COMMAND_ID				 0x03	 
#define ZCL_TRIGGER_EFFECT_COMMAND_ID						 0x40	


/*  
 * Command types for cluster: Groups
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_ADD_GROUP_RESPONSE_COMMAND_ID					 0x00	 
#define ZCL_VIEW_GROUP_RESPONSE_COMMAND_ID					 0x01	 
#define ZCL_GET_GROUP_MEMBERSHIP_RESPONSE_COMMAND_ID		 0x02	 
#define ZCL_REMOVE_GROUP_RESPONSE_COMMAND_ID				 0x03	 

/* Client to server */
#define ZCL_ADD_GROUP_COMMAND_ID							 0x00	 
#define ZCL_VIEW_GROUP_COMMAND_ID							 0x01	 
#define ZCL_GET_GROUP_MEMBERSHIP_COMMAND_ID					 0x02	 
#define ZCL_REMOVE_GROUP_COMMAND_ID							 0x03	 
#define ZCL_REMOVE_ALL_GROUPS_COMMAND_ID					 0x04	 
#define ZCL_ADD_GROUP_IF_IDENTIFYING_COMMAND_ID				 0x05	 


/*  
 * Command types for cluster: Scenes
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_ADD_SCENE_RESPONSE_COMMAND_ID					 0x00	 
#define ZCL_VIEW_SCENE_RESPONSE_COMMAND_ID					 0x01	 
#define ZCL_REMOVE_SCENE_RESPONSE_COMMAND_ID				 0x02	 
#define ZCL_REMOVE_ALL_SCENES_RESPONSE_COMMAND_ID			 0x03	 
#define ZCL_STORE_SCENE_RESPONSE_COMMAND_ID					 0x04	 
#define ZCL_GET_SCENE_MEMBERSHIP_RESPONSE_COMMAND_ID		 0x06	 
#define ZCL_ENHANCED_ADD_SCENE_RESPONSE_COMMAND_ID			 0x40	 
#define ZCL_ENHANCED_VIEW_SCENE_RESPONSE_COMMAND_ID			 0x41	
#define ZCL_COPY_SCENE_RESPONSE_COMMAND_ID					 0x42	 

/* Client to server */
#define ZCL_ADD_SCENE_COMMAND_ID							 0x00	 
#define ZCL_VIEW_SCENE_COMMAND_ID							 0x01	 
#define ZCL_REMOVE_SCENE_COMMAND_ID							 0x02	 
#define ZCL_REMOVE_ALL_SCENES_COMMAND_ID					 0x03	 
#define ZCL_STORE_SCENE_COMMAND_ID							 0x04	 
#define ZCL_RECALL_SCENE_COMMAND_ID							 0x05	 
#define ZCL_GET_SCENE_MEMBERSHIP_COMMAND_ID					 0x06	 
#define ZCL_ENHANCED_ADD_SCENE_COMMAND_ID					 0x40	 
#define ZCL_ENHANCED_VIEW_SCENE_COMMAND_ID					 0x41	
#define ZCL_COPY_SCENE_COMMAND_ID							 0x42	 


/*  
 * Command types for cluster: On/off
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Client to server */
#define ZCL_OFF_COMMAND_ID									 0x00	 
#define ZCL_ON_COMMAND_ID									 0x01	 
#define ZCL_TOGGLE_COMMAND_ID								 0x02	 
#define ZCL_OFF_WITH_EFFECT_COMMAND_ID						 0x40	
#define ZCL_ON_WITH_RECALL_GLOBAL_SCENE_COMMAND_ID			 0x41	 
#define ZCL_ON_WITH_TIMED_OFF_COMMAND_ID					 0x42	 
#define ZCL_SAMPLE_MFG_SPECIFIC_OFF_WITH_TRANSITION_COMMAND_ID		 0x00   mfgCode: 0x1002
#define ZCL_SAMPLE_MFG_SPECIFIC_OFF_WITH_TRANSITION_COMMAND_ID		 0x00   mfgCode: 0x1002
#define ZCL_SAMPLE_MFG_SPECIFIC_ON_WITH_TRANSITION_COMMAND_ID		 0x01   mfgCode: 0x1002
#define ZCL_SAMPLE_MFG_SPECIFIC_TOGGLE_WITH_TRANSITION_COMMAND_ID	 0x02   mfgCode: 0x1002


/*  
 * Command types for cluster: Level Control
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Client to server */
#define ZCL_MOVE_TO_LEVEL_COMMAND_ID						 0x00	 
#define ZCL_MOVE_COMMAND_ID									 0x01	 
#define ZCL_STEP_COMMAND_ID									 0x02	 
#define ZCL_STOP_COMMAND_ID									 0x03	 
#define ZCL_MOVE_TO_LEVEL_WITH_ON_OFF_COMMAND_ID			 0x04	 
#define ZCL_MOVE_WITH_ON_OFF_COMMAND_ID						 0x05	 
#define ZCL_STEP_WITH_ON_OFF_COMMAND_ID						 0x06	 
#define ZCL_STOP_WITH_ON_OFF_COMMAND_ID						 0x07	 


/*  
 * Command types for cluster: Alarms
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_ALARM_COMMAND_ID								 0x00	 
#define ZCL_GET_ALARM_RESPONSE_COMMAND_ID					 0x01	 

/* Client to server */
#define ZCL_RESET_ALARM_COMMAND_ID							 0x00	 
#define ZCL_RESET_ALL_ALARMS_COMMAND_ID						 0x01	 
#define ZCL_GET_ALARM_COMMAND_ID							 0x02	 
#define ZCL_RESET_ALARM_LOG_COMMAND_ID						 0x03	 


/*  
 * Command types for cluster: RSSI Location
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_DEVICE_CONFIGURATION_RESPONSE_COMMAND_ID		 0x00	 
#define ZCL_LOCATION_DATA_RESPONSE_COMMAND_ID				 0x01	 
#define ZCL_LOCATION_DATA_NOTIFICATION_COMMAND_ID			 0x02	 
#define ZCL_COMPACT_LOCATION_DATA_NOTIFICATION_COMMAND_ID	 0x03	 
#define ZCL_RSSI_PING_COMMAND_ID							 0x04	 
#define ZCL_RSSI_REQUEST_COMMAND_ID							 0x05	 
#define ZCL_REPORT_RSSI_MEASUREMENTS_COMMAND_ID				 0x06	 
#define ZCL_REQUEST_OWN_LOCATION_COMMAND_ID					 0x07	 

/* Client to server */
#define ZCL_SET_ABSOLUTE_LOCATION_COMMAND_ID				 0x00	 
#define ZCL_SET_DEVICE_CONFIGURATION_COMMAND_ID				 0x01	 
#define ZCL_GET_DEVICE_CONFIGURATION_COMMAND_ID				 0x02	 
#define ZCL_GET_LOCATION_DATA_COMMAND_ID					 0x03	 
#define ZCL_RSSI_RESPONSE_COMMAND_ID						 0x04	 
#define ZCL_SEND_PINGS_COMMAND_ID							 0x05	 
#define ZCL_ANCHOR_NODE_ANNOUNCE_COMMAND_ID					 0x06	 


/*  
 * Command types for cluster: Commissioning
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_RESTART_DEVICE_RESPONSE_COMMAND_ID				 0x00	 
#define ZCL_SAVE_STARTUP_PARAMETERS_RESPONSE_COMMAND_ID		 0x01	 
#define ZCL_RESTORE_STARTUP_PARAMETERS_RESPONSE_COMMAND_ID	 0x02	 
#define ZCL_RESET_STARTUP_PARAMETERS_RESPONSE_COMMAND_ID	 0x03	 

/* Client to server */
#define ZCL_RESTART_DEVICE_COMMAND_ID						 0x00	 
#define ZCL_SAVE_STARTUP_PARAMETERS_COMMAND_ID				 0x01	 
#define ZCL_RESTORE_STARTUP_PARAMETERS_COMMAND_ID			 0x02	 
#define ZCL_RESET_STARTUP_PARAMETERS_COMMAND_ID				 0x03	 



/*  
 * Command types for cluster: Partition
 * Cluster specification level: zcl-1.0-07-5123-03
*/


/* Server to client */

#define ZCL_MULTIPLE_ACK_COMMAND_ID							 0x00	 
#define ZCL_READ_HANDSHAKE_PARAM_RESPONSE_COMMAND_ID		 0x01	 

/* Client to server */
#define ZCL_TRANSFER_PARTITIONED_FRAME_COMMAND_ID			 0x00	 
#define ZCL_READ_HANDSHAKE_PARAM_COMMAND_ID					 0x01	 
#define ZCL_WRITE_HANDSHAKE_PARAM_COMMAND_ID				 0x02	 




/*  
 * Command types for cluster: Over the Air Bootloading
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_IMAGE_NOTIFY_COMMAND_ID							 0x00	 
#define ZCL_QUERY_NEXT_IMAGE_RESPONSE_COMMAND_ID			 0x02	 
#define ZCL_IMAGE_BLOCK_RESPONSE_COMMAND_ID					 0x05	 
#define ZCL_UPGRADE_END_RESPONSE_COMMAND_ID					 0x07	 
#define ZCL_QUERY_SPECIFIC_FILE_RESPONSE_COMMAND_ID			 0x09	 

/* Client to server */
#define ZCL_QUERY_NEXT_IMAGE_REQUEST_COMMAND_ID				 0x01	 
#define ZCL_IMAGE_BLOCK_REQUEST_COMMAND_ID					 0x03	 
#define ZCL_IMAGE_PAGE_REQUEST_COMMAND_ID					 0x04	 
#define ZCL_UPGRADE_END_REQUEST_COMMAND_ID					 0x06	 
#define ZCL_QUERY_SPECIFIC_FILE_REQUEST_COMMAND_ID			 0x08	 



/*  
 * Command types for cluster: Power Profile
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_POWER_PROFILE_NOTIFICATION_COMMAND_ID			 0x00	 
#define ZCL_POWER_PROFILE_RESPONSE_COMMAND_ID				 0x01	 
#define ZCL_POWER_PROFILE_STATE_RESPONSE_COMMAND_ID			 0x02	 
#define ZCL_GET_POWER_PROFILE_PRICE_COMMAND_ID				 0x03	 
#define ZCL_POWER_PROFILES_STATE_NOTIFICATION_COMMAND_ID	 0x04	 
#define ZCL_GET_OVERALL_SCHEDULE_PRICE_COMMAND_ID			 0x05	 
#define ZCL_ENERGY_PHASES_SCHEDULE_REQUEST_COMMAND_ID		 0x06	 
#define ZCL_ENERGY_PHASES_SCHEDULE_STATE_RESPONSE_COMMAND_ID 0x07    
#define ZCL_ENERGY_PHASES_SCHEDULE_STATE_NOTIFICATION_COMMAND_ID		 0x08	
#define ZCL_POWER_PROFILE_SCHEDULE_CONSTRAINTS_NOTIFICATION_COMMAND_ID	 0x09	
#define ZCL_POWER_PROFILE_SCHEDULE_CONSTRAINTS_RESPONSE_COMMAND_ID		 0x0A	
#define ZCL_GET_POWER_PROFILE_PRICE_EXTENDED_COMMAND_ID		 0x0B	 

/* Client to server */
#define ZCL_POWER_PROFILE_REQUEST_COMMAND_ID				 0x00	 
#define ZCL_POWER_PROFILE_STATE_REQUEST_COMMAND_ID			 0x01	 
#define ZCL_GET_POWER_PROFILE_PRICE_RESPONSE_COMMAND_ID		 0x02	 
#define ZCL_GET_OVERALL_SCHEDULE_PRICE_RESPONSE_COMMAND_ID	 0x03	 
#define ZCL_ENERGY_PHASES_SCHEDULE_NOTIFICATION_COMMAND_ID	 0x04	 
#define ZCL_ENERGY_PHASES_SCHEDULE_RESPONSE_COMMAND_ID		 0x05	 
#define ZCL_POWER_PROFILE_SCHEDULE_CONSTRAINTS_REQUEST_COMMAND_ID	 0x06   
#define ZCL_ENERGY_PHASES_SCHEDULE_STATE_REQUEST_COMMAND_ID			 0x07	
#define ZCL_GET_POWER_PROFILE_PRICE_EXTENDED_RESPONSE_COMMAND_ID	 0x08	



/*  
 * Command types for cluster: Appliance Control
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_SIGNAL_STATE_RESPONSE_COMMAND_ID				 0x00	 
#define ZCL_SIGNAL_STATE_NOTIFICATION_COMMAND_ID			 0x01	 

/* Client to server */
#define ZCL_EXECUTION_OF_A_COMMAND_COMMAND_ID				 0x00	 
#define ZCL_SIGNAL_STATE_COMMAND_ID							 0x01	 
#define ZCL_WRITE_FUNCTIONS_COMMAND_ID						 0x02	 
#define ZCL_OVERLOAD_PAUSE_RESUME_COMMAND_ID				 0x03	 
#define ZCL_OVERLOAD_PAUSE_COMMAND_ID						 0x04	 
#define ZCL_OVERLOAD_WARNING_COMMAND_ID						 0x05	 


/*  
 * Command types for cluster: Poll Control
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_CHECK_IN_COMMAND_ID								 0x00	 

/* Client to server */
#define ZCL_CHECK_IN_RESPONSE_COMMAND_ID					 0x00	 
#define ZCL_FAST_POLL_STOP_COMMAND_ID						 0x01	 
#define ZCL_SET_LONG_POLL_INTERVAL_COMMAND_ID				 0x02	 
#define ZCL_SET_SHORT_POLL_INTERVAL_COMMAND_ID				 0x03	 




/*  
 * Command types for cluster: Door Lock
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_LOCK_DOOR_RESPONSE_COMMAND_ID					 0x00	 
#define ZCL_UNLOCK_DOOR_RESPONSE_COMMAND_ID					 0x01	 
#define ZCL_TOGGLE_RESPONSE_COMMAND_ID						 0x02	 
#define ZCL_UNLOCK_WITH_TIMEOUT_RESPONSE_COMMAND_ID			 0x03	 
#define ZCL_GET_LOG_RECORD_RESPONSE_COMMAND_ID				 0x04	 
#define ZCL_SET_PIN_RESPONSE_COMMAND_ID						 0x05	 
#define ZCL_GET_PIN_RESPONSE_COMMAND_ID						 0x06	 
#define ZCL_CLEAR_PIN_RESPONSE_COMMAND_ID					 0x07	 
#define ZCL_CLEAR_ALL_PINS_RESPONSE_COMMAND_ID				 0x08	 
#define ZCL_SET_USER_STATUS_RESPONSE_COMMAND_ID				 0x09	 
#define ZCL_GET_USER_STATUS_RESPONSE_COMMAND_ID				 0x0A	 
#define ZCL_SET_WEEKDAY_SCHEDULE_RESPONSE_COMMAND_ID		 0x0B	 
#define ZCL_GET_WEEKDAY_SCHEDULE_RESPONSE_COMMAND_ID		 0x0C	 
#define ZCL_CLEAR_WEEKDAY_SCHEDULE_RESPONSE_COMMAND_ID		 0x0D	 
#define ZCL_SET_YEARDAY_SCHEDULE_RESPONSE_COMMAND_ID		 0x0E	 
#define ZCL_GET_YEARDAY_SCHEDULE_RESPONSE_COMMAND_ID		 0x0F	 
#define ZCL_CLEAR_YEARDAY_SCHEDULE_RESPONSE_COMMAND_ID		 0x10	 
#define ZCL_SET_HOLIDAY_SCHEDULE_RESPONSE_COMMAND_ID		 0x11	 
#define ZCL_GET_HOLIDAY_SCHEDULE_RESPONSE_COMMAND_ID		 0x12	 
#define ZCL_CLEAR_HOLIDAY_SCHEDULE_RESPONSE_COMMAND_ID		 0x13	 
#define ZCL_SET_USER_TYPE_RESPONSE_COMMAND_ID				 0x14	 
#define ZCL_GET_USER_TYPE_RESPONSE_COMMAND_ID				 0x15	 
#define ZCL_SET_RFID_RESPONSE_COMMAND_ID					 0x16	 
#define ZCL_GET_RFID_RESPONSE_COMMAND_ID					 0x17	 
#define ZCL_CLEAR_RFID_RESPONSE_COMMAND_ID					 0x18	 
#define ZCL_CLEAR_ALL_RFIDS_RESPONSE_COMMAND_ID				 0x19	 
#define ZCL_OPERATION_EVENT_NOTIFICATION_COMMAND_ID			 0x20	 
#define ZCL_PROGRAMMING_EVENT_NOTIFICATION_COMMAND_ID		 0x21	 

/* Client to server */
#define ZCL_LOCK_DOOR_COMMAND_ID							 0x00	 
#define ZCL_UNLOCK_DOOR_COMMAND_ID							 0x01	 
#define ZCL_TOGGLE_COMMAND_ID								 0x02	 
#define ZCL_UNLOCK_WITH_TIMEOUT_COMMAND_ID					 0x03	 
#define ZCL_GET_LOG_RECORD_COMMAND_ID						 0x04	 
#define ZCL_SET_PIN_COMMAND_ID								 0x05	 
#define ZCL_GET_PIN_COMMAND_ID								 0x06	 
#define ZCL_CLEAR_PIN_COMMAND_ID							 0x07	 
#define ZCL_CLEAR_ALL_PINS_COMMAND_ID						 0x08	 
#define ZCL_SET_USER_STATUS_COMMAND_ID						 0x09	 
#define ZCL_GET_USER_STATUS_COMMAND_ID						 0x0A	 
#define ZCL_SET_WEEKDAY_SCHEDULE_COMMAND_ID					 0x0B	 
#define ZCL_GET_WEEKDAY_SCHEDULE_COMMAND_ID					 0x0C	 
#define ZCL_CLEAR_WEEKDAY_SCHEDULE_COMMAND_ID				 0x0D	 
#define ZCL_SET_YEARDAY_SCHEDULE_COMMAND_ID					 0x0E	 
#define ZCL_GET_YEARDAY_SCHEDULE_COMMAND_ID					 0x0F	 
#define ZCL_CLEAR_YEARDAY_SCHEDULE_COMMAND_ID				 0x10	 
#define ZCL_SET_HOLIDAY_SCHEDULE_COMMAND_ID					 0x11	 
#define ZCL_GET_HOLIDAY_SCHEDULE_COMMAND_ID					 0x12	 
#define ZCL_CLEAR_HOLIDAY_SCHEDULE_COMMAND_ID				 0x13	 
#define ZCL_SET_USER_TYPE_COMMAND_ID						 0x14	 
#define ZCL_GET_USER_TYPE_COMMAND_ID						 0x15	 
#define ZCL_SET_RFID_COMMAND_ID								 0x16	 
#define ZCL_GET_RFID_COMMAND_ID								 0x17	 
#define ZCL_CLEAR_RFID_COMMAND_ID							 0x18	 
#define ZCL_CLEAR_ALL_RFIDS_COMMAND_ID						 0x19	 



/*  
 * Command types for cluster: Window Covering
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Client to server */
#define ZCL_WINDOW_COVERING_UP_OPEN_COMMAND_ID				 0x00	 
#define ZCL_WINDOW_COVERING_DOWN_CLOSE_COMMAND_ID			 0x01	 
#define ZCL_WINDOW_COVERING_STOP_COMMAND_ID					 0x02	 
#define ZCL_WINDOW_COVERING_GO_TO_LIFT_VALUE_COMMAND_ID		 0x04	 
#define ZCL_WINDOW_COVERING_GO_TO_LIFT_PERCENTAGE_COMMAND_ID 0x05    
#define ZCL_WINDOW_COVERING_GO_TO_TILT_VALUE_COMMAND_ID		 0x07	 
#define ZCL_WINDOW_COVERING_GO_TO_TILT_PERCENTAGE_COMMAND_ID 0x08    


/*  
 * Command types for cluster: Thermostat
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_CURRENT_WEEKLY_SCHEDULE_COMMAND_ID				 0x00	 
#define ZCL_RELAY_STATUS_LOG_COMMAND_ID						 0x01	 

/* Client to server */
#define ZCL_SETPOINT_RAISE_LOWER_COMMAND_ID					 0x00	 
#define ZCL_SET_WEEKLY_SCHEDULE_COMMAND_ID					 0x01	 
#define ZCL_GET_WEEKLY_SCHEDULE_COMMAND_ID					 0x02	 
#define ZCL_CLEAR_WEEKLY_SCHEDULE_COMMAND_ID				 0x03	 
#define ZCL_GET_RELAY_STATUS_LOG_COMMAND_ID					 0x04	 




/*  
 * Command types for cluster: Color Control
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Client to server */
#define ZCL_MOVE_TO_HUE_COMMAND_ID							 0x00	 
#define ZCL_MOVE_HUE_COMMAND_ID								 0x01	 
#define ZCL_STEP_HUE_COMMAND_ID								 0x02	 
#define ZCL_MOVE_TO_SATURATION_COMMAND_ID					 0x03	 
#define ZCL_MOVE_SATURATION_COMMAND_ID						 0x04	 
#define ZCL_STEP_SATURATION_COMMAND_ID						 0x05	 
#define ZCL_MOVE_TO_HUE_AND_SATURATION_COMMAND_ID			 0x06	 
#define ZCL_MOVE_TO_COLOR_COMMAND_ID						 0x07	 
#define ZCL_MOVE_COLOR_COMMAND_ID							 0x08	 
#define ZCL_STEP_COLOR_COMMAND_ID							 0x09	 
#define ZCL_MOVE_TO_COLOR_TEMPERATURE_COMMAND_ID			 0x0A	 
#define ZCL_ENHANCED_MOVE_TO_HUE_COMMAND_ID					 0x40	 
#define ZCL_ENHANCED_MOVE_HUE_COMMAND_ID					 0x41	 
#define ZCL_ENHANCED_STEP_HUE_COMMAND_ID					 0x42	 
#define ZCL_ENHANCED_MOVE_TO_HUE_AND_SATURATION_COMMAND_ID	 0x43	 
#define ZCL_COLOR_LOOP_SET_COMMAND_ID						 0x44	 
#define ZCL_STOP_MOVE_STEP_COMMAND_ID						 0x47	 
#define ZCL_MOVE_COLOR_TEMPERATURE_COMMAND_ID				 0x4B	 
#define ZCL_STEP_COLOR_TEMPERATUE_COMMAND_ID				 0x4C	 




/*  
 * Command types for cluster: IAS Zone
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_ZONE_STATUS_CHANGE_NOTIFICATION_COMMAND_ID		 0x00	 
#define ZCL_ZONE_ENROLL_REQUEST_COMMAND_ID					 0x01	 

/* Client to server */
#define ZCL_ZONE_ENROLL_RESPONSE_COMMAND_ID					 0x00	 


 

/*  
 * Command types for cluster: IAS ACE
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Server to client */
#define ZCL_ARM_RESPONSE_COMMAND_ID							 0x00	 
#define ZCL_GET_ZONE_ID_MAP_RESPONSE_COMMAND_ID				 0x01	 
#define ZCL_GET_ZONE_INFORMATION_RESPONSE_COMMAND_ID		 0x02	 
#define ZCL_ZONE_STATUS_CHANGED_COMMAND_ID					 0x03	 
#define ZCL_PANEL_STATUS_CHANGED_COMMAND_ID					 0x04	 

/* Client to server */
#define ZCL_ARM_COMMAND_ID									 0x00	 
#define ZCL_BYPASS_COMMAND_ID								 0x01	 
#define ZCL_EMERGENCY_COMMAND_ID							 0x02	 
#define ZCL_FIRE_COMMAND_ID									 0x03	 
#define ZCL_PANIC_COMMAND_ID								 0x04	 
#define ZCL_GET_ZONE_ID_MAP_COMMAND_ID						 0x05	 
#define ZCL_GET_ZONE_INFORMATION_COMMAND_ID					 0x06	 



/*  
 * Command types for cluster: IAS WD
 * Cluster specification level: zcl-1.0-07-5123-03
*/

/* Client to server */
#define ZCL_START_WARNING_COMMAND_ID						 0x00	 
#define ZCL_SQUAWK_COMMAND_ID								 0x01	 



/*  
 * Command types for cluster: Generic Tunnel
 * Cluster specification level: cba-1.0-05-3516-12
*/

/* Server to client */
#define ZCL_MATCH_PROTOCOL_ADDRESS_RESPONSE_COMMAND_ID		 0x00	 
#define ZCL_ADVERTISE_PROTOCOL_ADDRESS_COMMAND_ID			 0x01	 

/* Client to server */
#define ZCL_MATCH_PROTOCOL_ADDRESS_COMMAND_ID				 0x00	 




/*  
 * Command types for cluster: BACnet Protocol Tunnel
 * Cluster specification level: cba-1.0-05-3516-12
*/

/* Client to server */
#define ZCL_TRANSFER_NPDU_COMMAND_ID						 0x00	 


/*  
 * Command types for cluster: 11073 Protocol Tunnel
 * Cluster specification level: hc-1.0-07-5360-15
*/

/* Client to server */
#define ZCL_TRANSFER_A_P_D_U_COMMAND_ID						 0x00	 
#define ZCL_CONNECT_REQUEST_COMMAND_ID						 0x01	 
#define ZCL_DISCONNECT_REQUEST_COMMAND_ID					 0x02	 
#define ZCL_CONNECT_STATUS_NOTIFICATION_COMMAND_ID			 0x03	 



/*  
 * Command types for cluster: ISO 7816 Protocol Tunnel
 * Cluster specification level: ta-1.0-07-5307-07
*/

/* Client to server */
#define ZCL_INSERT_SMART_CARD_COMMAND_ID					 0x01	 
#define ZCL_EXTRACT_SMART_CARD_COMMAND_ID					 0x02	 

/* Either direction */
#define ZCL_TRANSFER_APDU_COMMAND_ID						 0x00	 


/*  
 * Command types for cluster: Price
 * Cluster specification level: se-1.2-12-0517-08
*/

/* Server to client */
#define ZCL_PUBLISH_PRICE_COMMAND_ID						 0x00	 
#define ZCL_PUBLISH_CONVERSION_FACTOR_COMMAND_ID			 0x02	 
#define ZCL_PUBLISH_CALORIFIC_VALUE_COMMAND_ID				 0x03	 
#define ZCL_PUBLISH_BLOCK_PERIOD_COMMAND_ID					 0x01	 
#define ZCL_PUBLISH_TARIFF_INFORMATION_COMMAND_ID			 0x04	 
#define ZCL_PUBLISH_PRICE_MATRIX_COMMAND_ID					 0x11	 
#define ZCL_PUBLISH_BLOCK_THRESHOLD_COMMAND_ID				 0x12	 
#define ZCL_PUBLISH_C_O2_VALUE_COMMAND_ID					 0x15	 
#define ZCL_PUBLISH_CPP_EVENT_COMMAND_ID					 0x16	 
#define ZCL_PUBLISH_BILLING_PERIOD_COMMAND_ID				 0x20	 
#define ZCL_PUBLISH_CONSOLIDATED_BILL_COMMAND_ID			 0x21	 
#define ZCL_PUBLISH_CREDIT_PAYMENT_COMMAND_ID				 0x22	 
#define ZCL_PUBLISH_CURRENCY_CONVERSION_COMMAND_ID			 0x23	 

/* Client to server */
#define ZCL_GET_CURRENT_PRICE_COMMAND_ID					 0x00	 
#define ZCL_GET_SCHEDULED_PRICES_COMMAND_ID					 0x01	 
#define ZCL_PRICE_ACKNOWLEDGEMENT_COMMAND_ID				 0x02	 
#define ZCL_GET_BLOCK_PERIODS_COMMAND_ID					 0x03	 
#define ZCL_GET_CONVERSION_FACTOR_COMMAND_ID				 0x04	 
#define ZCL_GET_CALORIFIC_VALUE_COMMAND_ID					 0x05	 
#define ZCL_GET_TARIFF_INFORMATION_COMMAND_ID				 0x06	 
#define ZCL_GET_PRICE_MATRIX_COMMAND_ID						 0x07	 
#define ZCL_GET_BLOCK_THRESHOLDS_COMMAND_ID					 0x08	 
#define ZCL_GET_C_O2_VALUE_COMMAND_ID						 0x09	 
#define ZCL_GET_TIER_LABELS_COMMAND_ID						 0x0A	 
#define ZCL_GET_BILLING_PERIOD_COMMAND_ID					 0x0B	 
#define ZCL_GET_CONSOLIDATED_BILL_COMMAND_ID				 0x0C	 
#define ZCL_CPP_EVENT_RESPONSE_COMMAND_ID					 0x0D	 
#define ZCL_GET_CREDIT_PAYMENT_COMMAND_ID					 0x0E	 
#define ZCL_GET_CURRENCY_CONVERSION_COMMAND_COMMAND_ID		 0x0F	 




/*  
 * Command types for cluster: Demand Response and Load Control
 * Cluster specification level: se-1.2-12-0517-08
*/

/* Server to client */
#define ZCL_LOAD_CONTROL_EVENT_COMMAND_ID					 0x00	 
#define ZCL_CANCEL_LOAD_CONTROL_EVENT_COMMAND_ID			 0x01	 
#define ZCL_CANCEL_ALL_LOAD_CONTROL_EVENTS_COMMAND_ID		 0x02	 

/* Client to server */
#define ZCL_REPORT_EVENT_STATUS_COMMAND_ID					 0x00	 
#define ZCL_GET_SCHEDULED_EVENTS_COMMAND_ID					 0x01	 



/*  
 * Command types for cluster: Simple Metering
 * Cluster specification level: se-1.2-12-0517-08
*/

/* Server to client */
#define ZCL_GET_PROFILE_RESPONSE_COMMAND_ID					 0x00	 
#define ZCL_REQUEST_MIRROR_COMMAND_ID						 0x01	 
#define ZCL_REMOVE_MIRROR_COMMAND_ID						 0x02	 
#define ZCL_REQUEST_FAST_POLL_MODE_RESPONSE_COMMAND_ID		 0x03	 
#define ZCL_CONFIGURE_MIRROR_COMMAND_ID						 0x08	 
#define ZCL_CONFIGURE_NOTIFICATION_SCHEME_COMMAND_ID		 0x09	 
#define ZCL_CONFIGURE_NOTIFICATION_FLAGS_COMMAND_ID			 0x0A	 
#define ZCL_SCHEDULE_SNAPSHOT_RESPONSE_COMMAND_ID			 0x04	 
#define ZCL_TAKE_SNAPSHOT_RESPONSE_COMMAND_ID				 0x05	 
#define ZCL_PUBLISH_SNAPSHOT_COMMAND_ID						 0x06	 

/* Client to server */
#define ZCL_GET_PROFILE_COMMAND_ID							 0x00	 
#define ZCL_REQUEST_MIRROR_RESPONSE_COMMAND_ID				 0x01	 
#define ZCL_MIRROR_REMOVED_COMMAND_ID						 0x02	 
#define ZCL_REQUEST_FAST_POLL_MODE_COMMAND_ID				 0x03	 
#define ZCL_SCHEDULE_SNAPSHOT_COMMAND_ID					 0x04	 
#define ZCL_TAKE_SNAPSHOT_COMMAND_ID						 0x05	 
#define ZCL_GET_SNAPSHOT_COMMAND_ID							 0x06	 
#define ZCL_MIRROR_REPORT_ATTRIBUTE_RESPONSE_COMMAND_ID		 0x09	 



/*  
 * Command types for cluster: Messaging
 * Cluster specification level: se-1.2-12-0517-08
*/

/* Server to client */
#define ZCL_DISPLAY_MESSAGE_COMMAND_ID						 0x00	 
#define ZCL_CANCEL_MESSAGE_COMMAND_ID						 0x01	 

/* Client to server */
#define ZCL_GET_LAST_MESSAGE_COMMAND_ID						 0x00	 
#define ZCL_MESSAGE_CONFIRMATION_COMMAND_ID					 0x01	 



/*  
 * Command types for cluster: Tunneling
 * Cluster specification level: se-1.2-12-0517-08
*/

/* Server to client */
#define ZCL_REQUEST_TUNNEL_RESPONSE_COMMAND_ID				 0x00	 
#define ZCL_TRANSFER_DATA_SERVER_TO_CLIENT_COMMAND_ID		 0x01	 
#define ZCL_TRANSFER_DATA_ERROR_SERVER_TO_CLIENT_COMMAND_ID	 0x02	 
#define ZCL_ACK_TRANSFER_DATA_SERVER_TO_CLIENT_COMMAND_ID	 0x03	 
#define ZCL_READY_DATA_SERVER_TO_CLIENT_COMMAND_ID			 0x04	 
#define ZCL_SUPPORTED_TUNNEL_PROTOCOLS_RESPONSE_COMMAND_ID	 0x05	 
#define ZCL_TUNNEL_CLOSURE_NOTIFICATION_COMMAND_ID			 0x06	 

/* Client to server */
#define ZCL_REQUEST_TUNNEL_COMMAND_ID						 0x00	 
#define ZCL_CLOSE_TUNNEL_COMMAND_ID							 0x01	 
#define ZCL_TRANSFER_DATA_CLIENT_TO_SERVER_COMMAND_ID		 0x02	 
#define ZCL_TRANSFER_DATA_ERROR_CLIENT_TO_SERVER_COMMAND_ID	 0x03	 
#define ZCL_ACK_TRANSFER_DATA_CLIENT_TO_SERVER_COMMAND_ID	 0x04	 
#define ZCL_READY_DATA_CLIENT_TO_SERVER_COMMAND_ID			 0x05	 
#define ZCL_GET_SUPPORTED_TUNNEL_PROTOCOLS_COMMAND_ID		 0x06	 


/*  
 * Command types for cluster: Prepayment
 * Cluster specification level: se-1.2-12-0517-08
*/

/* Server to client */
#define ZCL_GET_PREPAY_SNAPSHOT_RESPONSE_COMMAND_ID			 0x01	 
#define ZCL_CHANGE_PAYMENT_MODE_RESPONSE_COMMAND_ID			 0x02	 
#define ZCL_CONSUMER_TOP_UP_RESPONSE_COMMAND_ID				 0x03	 
#define ZCL_GET_COMMAND_COMMAND_ID							 0x04	 
#define ZCL_PUBLISH_TOP_UP_LOG_COMMAND_ID					 0x05	 
#define ZCL_PUBLISH_DEBT_LOG_COMMAND_ID						 0x06	 

/* Client to server */
#define ZCL_SELECT_AVAILABLE_EMERGENCY_CREDIT_COMMAND_ID	 0x00	 
#define ZCL_CHANGE_DEBT_COMMAND_ID							 0x02	 
#define ZCL_EMERGENCY_CREDIT_SETUP_COMMAND_ID				 0x03	 
#define ZCL_CONSUMER_TOP_UP_COMMAND_ID						 0x04	 
#define ZCL_CREDIT_ADJUSTMENT_COMMAND_ID					 0x05	 
#define ZCL_CHANGE_PAYMENT_MODE_COMMAND_ID					 0x06	 
#define ZCL_GET_PREPAY_SNAPSHOT_COMMAND_ID					 0x07	 
#define ZCL_GET_TOP_UP_LOG_COMMAND_ID						 0x08	 
#define ZCL_SET_LOW_CREDIT_WARNING_LEVEL_COMMAND_ID			 0x09	 
#define ZCL_GET_DEBT_REPAYMENTY_LOG_COMMAND_ID				 0x0A	 
#define ZCL_SET_MAXIMUM_CREDIT_LIMIT_COMMAND_ID				 0x0B	 
#define ZCL_SET_OVERALL_DEBT_CAP_COMMAND_ID					 0x0C	 


/*  
 * Command types for cluster: TOU Calendar
 * Cluster specification level: se-1.2-12-0517-08
*/

/* Server to client */
#define ZCL_PUBLISH_CALENDAR_COMMAND_ID						 0x00	 
#define ZCL_PUBLISH_DAY_PROFILE_COMMAND_ID					 0x01	 
#define ZCL_PUBLISH_WEEK_PROFILE_COMMAND_ID					 0x02	 
#define ZCL_PUBLISH_SEASONS_COMMAND_ID						 0x03	 
#define ZCL_PUBLISH_SPECIAL_DAYS_COMMAND_ID					 0x04	 
#define ZCL_CANCEL_CALENDAR_COMMAND_ID						 0x05	 

/* Client to server */
#define ZCL_GET_CALENDAR_COMMAND_ID							 0x00	 
#define ZCL_GET_DAY_PROFILES_COMMAND_ID						 0x01	 
#define ZCL_GET_WEEK_PROFILES_COMMAND_ID					 0x02	 
#define ZCL_GET_SEASONS_COMMAND_ID							 0x03	 
#define ZCL_GET_SPECIAL_DAYS_COMMAND_ID						 0x04	 



/*  
 * Command types for cluster: Device Management
 * Cluster specification level: se-1.2-12-0517-08
*/

/* Server to client */
#define ZCL_PUBLISH_CHANGE_OF_TENANCY_COMMAND_ID			 0x00	 
#define ZCL_PUBLISH_CHANGE_OF_SUPPLIER_COMMAND_ID			 0x01	 
#define ZCL_CHANGE_SUPPLY_COMMAND_ID						 0x02	 
#define ZCL_CHANGE_PASSWORD_COMMAND_ID						 0x03	 
#define ZCL_LOCAL_CHANGE_SUPPLY_COMMAND_ID					 0x04	 
#define ZCL_UPDATE_SITE_ID_COMMAND_ID						 0x05	 
#define ZCL_SET_SUPPLY_STATUS_COMMAND_ID					 0x06	 
#define ZCL_SET_EVENT_CONFIGURATION_COMMAND_ID				 0x07	 
#define ZCL_GET_EVENT_CONFIGURATION_COMMAND_ID				 0x08	 
#define ZCL_PUBLISH_UNCONTROLLED_FLOW_THRESHOLD_COMMAND_ID	 0x09	 

/* Client to server */
#define ZCL_GET_CHANGE_OF_TENANCY_COMMAND_ID				 0x00	 
#define ZCL_GET_CHANGE_OF_SUPPLIER_COMMAND_ID				 0x01	 
#define ZCL_GET_CHANGE_SUPPLY_COMMAND_ID					 0x02	 
#define ZCL_SUPPLY_STATUS_RESPONSE_COMMAND_ID				 0x03	 
#define ZCL_GET_PASSWORD_COMMAND_ID							 0x04	 
#define ZCL_GET_SITE_ID_COMMAND_ID							 0x05	 
#define ZCL_REPORT_EVENT_CONFIGURATION_COMMAND_ID			 0x06	 
#define ZCL_GET_UNCONTROLLED_THRESHOLD_COMMAND_COMMAND_ID	 0x07	 


/*  
 * Command types for cluster: Events
 * Cluster specification level: se-1.2-12-0517-08
*/

/* Server to client */
#define ZCL_PUBLISH_EVENT_COMMAND_ID						 0x00	 
#define ZCL_PUBLISH_EVENT_LOG_COMMAND_ID					 0x01	 

/* Client to server */
#define ZCL_GET_EVENT_LOG_COMMAND_ID						 0x00	 
#define ZCL_CLEAR_EVENT_LOG_COMMAND_ID						 0x01	 

 

/*  
 * Command types for cluster: Key establishment
 * Cluster specification level: se-1.2-12-0517-08
*/

/* Server to client */
#define ZCL_INITIATE_KEY_ESTABLISHMENT_RESPONSE_COMMAND_ID	 0x00	 
#define ZCL_EPHEMERAL_DATA_RESPONSE_COMMAND_ID				 0x01	 
#define ZCL_CONFIRM_KEY_DATA_RESPONSE_COMMAND_ID			 0x02	 

/* Client to server */
#define ZCL_INITIATE_KEY_ESTABLISHMENT_REQUEST_COMMAND_ID	 0x00	 
#define ZCL_EPHEMERAL_DATA_REQUEST_COMMAND_ID				 0x01	 
#define ZCL_CONFIRM_KEY_DATA_REQUEST_COMMAND_ID				 0x02	 

/* Either direction */
#define ZCL_TERMINATE_KEY_ESTABLISHMENT_COMMAND_ID			 0x03	 



/*  
 * Command types for cluster: Information
 * Cluster specification level: ta-1.0-07-5307-07
*/

/* Server to client */
#define ZCL_REQUEST_INFORMATION_RESPONSE_COMMAND_ID			 0x00	 
#define ZCL_PUSH_INFORMATION_COMMAND_ID						 0x01	 
#define ZCL_SEND_PREFERENCE_RESPONSE_COMMAND_ID				 0x02	 
#define ZCL_SERVER_REQUEST_PREFERENCE_COMMAND_ID			 0x03	 
#define ZCL_REQUEST_PREFERENCE_CONFIRMATION_COMMAND_ID		 0x04	 
#define ZCL_UPDATE_RESPONSE_COMMAND_ID						 0x05	 
#define ZCL_DELETE_RESPONSE_COMMAND_ID						 0x06	 

/* Client to server */
#define ZCL_REQUEST_INFORMATION_COMMAND_ID					 0x00	 
#define ZCL_PUSH_INFORMATION_RESPONSE_COMMAND_ID			 0x01	 
#define ZCL_SEND_PREFERENCE_COMMAND_ID						 0x02	 
#define ZCL_REQUEST_PREFERENCE_RESPONSE_COMMAND_ID			 0x03	 
#define ZCL_UPDATE_COMMAND_ID								 0x04	 
#define ZCL_DELETE_COMMAND_ID								 0x05	 
#define ZCL_CONFIGURE_NODE_DESCRIPTION_COMMAND_ID			 0x06	 
#define ZCL_CONFIGURE_DELIVERY_ENABLE_COMMAND_ID			 0x07	 
#define ZCL_CONFIGURE_PUSH_INFORMATION_TIMER_COMMAND_ID		 0x08	 
#define ZCL_CONFIGURE_SET_ROOT_ID_COMMAND_ID				 0x09	 



/*  
 * Command types for cluster: Data Sharing
 * Cluster specification level: ta-1.0-07-5307-07
*/

/* Server to client */
#define ZCL_WRITE_FILE_REQUEST_COMMAND_ID					 0x00	 
#define ZCL_MODIFY_FILE_REQUEST_COMMAND_ID					 0x01	 
#define ZCL_MODIFY_RECORD_REQUEST_COMMAND_ID				 0x02	 
#define ZCL_FILE_TRANSMISSION_COMMAND_ID					 0x03	 
#define ZCL_RECORD_TRANSMISSION_COMMAND_ID					 0x04	 

/* Client to server */
#define ZCL_READ_FILE_REQUEST_COMMAND_ID					 0x00	 
#define ZCL_READ_RECORD_REQUEST_COMMAND_ID					 0x01	 
#define ZCL_WRITE_FILE_RESPONSE_COMMAND_ID					 0x02	 


/*  
 * Command types for cluster: Gaming
 * Cluster specification level: ta-1.0-07-5307-07
*/

/* Server to client */
#define ZCL_GAME_ANNOUNCEMENT_COMMAND_ID					 0x00	 
#define ZCL_GENERAL_RESPONSE_COMMAND_ID						 0x01	 

/* Client to server */
#define ZCL_SEARCH_GAME_COMMAND_ID							 0x00	 
#define ZCL_JOIN_GAME_COMMAND_ID							 0x01	 
#define ZCL_START_GAME_COMMAND_ID							 0x02	 
#define ZCL_PAUSE_GAME_COMMAND_ID							 0x03	 
#define ZCL_RESUME_GAME_COMMAND_ID							 0x04	 
#define ZCL_QUIT_GAME_COMMAND_ID							 0x05	 
#define ZCL_END_GAME_COMMAND_ID								 0x06	 
#define ZCL_START_OVER_COMMAND_ID							 0x07	 
#define ZCL_ACTION_CONTROL_COMMAND_ID						 0x08	 
#define ZCL_DOWNLOAD_GAME_COMMAND_ID						 0x09	 


/*  
 * Command types for cluster: Data Rate Control
 * Cluster specification level: ta-1.0-07-5307-07
*/

/* Server to client */
#define ZCL_DATA_RATE_CONTROL_COMMAND_ID					 0x00	 

/* Client to server */
#define ZCL_PATH_CREATION_COMMAND_ID						 0x00	 
#define ZCL_DATA_RATE_NOTIFICATION_COMMAND_ID				 0x01	 
#define ZCL_PATH_DELETION_COMMAND_ID						 0x02	 




/*  
 * Command types for cluster: Voice over ZigBee
 * Cluster specification level: ta-1.0-07-5307-07
*/

/* Server to client */
#define ZCL_ESTABLISHMENT_RESPONSE_COMMAND_ID				 0x00	 
#define ZCL_VOICE_TRANSMISSION_RESPONSE_COMMAND_ID			 0x01	 
#define ZCL_CONTROL_COMMAND_ID								 0x02	 

/* Client to server */
#define ZCL_ESTABLISHMENT_REQUEST_COMMAND_ID				 0x00	 
#define ZCL_VOICE_TRANSMISSION_COMMAND_ID					 0x01	 
#define ZCL_VOICE_TRANSMISSION_COMPLETION_COMMAND_ID		 0x02	 
#define ZCL_CONTROL_RESPONSE_COMMAND_ID						 0x03	 



/*  
 * Command types for cluster: Chatting
 * Cluster specification level: ta-1.0-07-5307-07
*/

/* Server to client */
#define ZCL_START_CHAT_RESPONSE_COMMAND_ID					 0x00	 
#define ZCL_JOIN_CHAT_RESPONSE_COMMAND_ID					 0x01	 
#define ZCL_USER_LEFT_COMMAND_ID							 0x02	 
#define ZCL_USER_JOINED_COMMAND_ID							 0x03	 
#define ZCL_SEARCH_CHAT_RESPONSE_COMMAND_ID					 0x04	 
#define ZCL_SWITCH_CHAIRMAN_REQUEST_COMMAND_ID				 0x05	 
#define ZCL_SWITCH_CHAIRMAN_CONFIRM_COMMAND_ID				 0x06	 
#define ZCL_SWITCH_CHAIRMAN_NOTIFICATION_COMMAND_ID			 0x07	 
#define ZCL_GET_NODE_INFORMATION_RESPONSE_COMMAND_ID		 0x08	 

/* Client to server */
#define ZCL_JOIN_CHAT_REQUEST_COMMAND_ID					 0x00	 
#define ZCL_LEAVE_CHAT_REQUEST_COMMAND_ID					 0x01	 
#define ZCL_SEARCH_CHAT_REQUEST_COMMAND_ID					 0x02	 
#define ZCL_SWITCH_CHAIRMAN_RESPONSE_COMMAND_ID				 0x03	 
#define ZCL_START_CHAT_REQUEST_COMMAND_ID					 0x04	 
#define ZCL_CHAT_MESSAGE_COMMAND_ID							 0x05	 
#define ZCL_GET_NODE_INFORMATION_REQUEST_COMMAND_ID			 0x06	 



/*  
 * Command types for cluster: Payment
 * Cluster specification level: ta-1.0-07-5307-07
*/

/* Server to client */
#define ZCL_BUY_CONFIRM_COMMAND_ID							 0x00	 
#define ZCL_RECEIPT_DELIVERY_COMMAND_ID						 0x01	 
#define ZCL_TRANSACTION_END_COMMAND_ID						 0x02	 

/* Client to server */
#define ZCL_BUY_REQUEST_COMMAND_ID							 0x00	 
#define ZCL_ACCEPT_PAYMENT_COMMAND_ID						 0x01	 
#define ZCL_PAYMENT_CONFIRM_COMMAND_ID						 0x02	 



/*  
 * Command types for cluster: Billing
 * Cluster specification level: ta-1.0-07-5307-07
*/

/* Server to client */
#define ZCL_CHECK_BILL_STATUS_COMMAND_ID					 0x00	 
#define ZCL_SEND_BILL_RECORD_COMMAND_ID						 0x01	 

/* Client to server */
#define ZCL_SUBSCRIBE_COMMAND_ID							 0x00	 
#define ZCL_UNSUBSCRIBE_COMMAND_ID							 0x01	 
#define ZCL_START_BILLING_SESSION_COMMAND_ID				 0x02	 
#define ZCL_STOP_BILLING_SESSION_COMMAND_ID					 0x03	 
#define ZCL_BILL_STATUS_NOTIFICATION_COMMAND_ID				 0x04	 
#define ZCL_SESSION_KEEP_ALIVE_COMMAND_ID					 0x05	 


/*  
 * Command types for cluster: Appliance Events and Alert
 * Cluster specification level: UNKNOWN
*/

/* Server to client */
#define ZCL_GET_ALERTS_RESPONSE_COMMAND_ID					 0x00	 
#define ZCL_ALERTS_NOTIFICATION_COMMAND_ID					 0x01	 
#define ZCL_EVENTS_NOTIFICATION_COMMAND_ID					 0x02	 

/* Client to server */
#define ZCL_GET_ALERTS_COMMAND_ID							 0x00	 


/*  
 * Command types for cluster: Appliance Statistics
 * Cluster specification level: UNKNOWN
*/

/* Server to client */
#define ZCL_LOG_NOTIFICATION_COMMAND_ID						 0x00	 
#define ZCL_LOG_RESPONSE_COMMAND_ID							 0x01	 
#define ZCL_LOG_QUEUE_RESPONSE_COMMAND_ID					 0x02	 
#define ZCL_STATISTICS_AVAILABLE_COMMAND_ID					 0x03	 

/* Client to server */
#define ZCL_LOG_REQUEST_COMMAND_ID							 0x00	 
#define ZCL_LOG_QUEUE_REQUEST_COMMAND_ID					 0x01	 


/*  
 * Command types for cluster: Electrical Measurement
 * Cluster specification level: UNKNOWN
*/

/* Server to client */
#define ZCL_GET_PROFILE_INFO_RESPONSE_COMMAND_COMMAND_ID		 0x00	 
#define ZCL_GET_MEASUREMENT_PROFILE_RESPONSE_COMMAND_COMMAND_ID	 0x01	 

/* Client to server */
#define ZCL_GET_PROFILE_INFO_COMMAND_COMMAND_ID				 0x00	 
#define ZCL_GET_MEASUREMENT_PROFILE_COMMAND_COMMAND_ID		 0x01	 

/*  
 * Command types for cluster: ZLL Commissioning
 * Cluster specification level: zll-1.0-11-0037-10
*/

/* Server to client */
#define ZCL_SCAN_RESPONSE_COMMAND_ID						 0x01	 
#define ZCL_DEVICE_INFORMATION_RESPONSE_COMMAND_ID			 0x03	 
#define ZCL_NETWORK_START_RESPONSE_COMMAND_ID				 0x11	 
#define ZCL_NETWORK_JOIN_ROUTER_RESPONSE_COMMAND_ID			 0x13	 
#define ZCL_NETWORK_JOIN_END_DEVICE_RESPONSE_COMMAND_ID		 0x15	 
#define ZCL_ENDPOINT_INFORMATION_COMMAND_ID					 0x40	 
#define ZCL_GET_GROUP_IDENTIFIERS_RESPONSE_COMMAND_ID		 0x41	 
#define ZCL_GET_ENDPOINT_LIST_RESPONSE_COMMAND_ID			 0x42	 

/* Client to server */
#define ZCL_SCAN_REQUEST_COMMAND_ID							 0x00	 
#define ZCL_DEVICE_INFORMATION_REQUEST_COMMAND_ID			 0x02	 
#define ZCL_IDENTIFY_REQUEST_COMMAND_ID						 0x06	 
#define ZCL_RESET_TO_FACTORY_NEW_REQUEST_COMMAND_ID			 0x07	 
#define ZCL_NETWORK_START_REQUEST_COMMAND_ID				 0x10	 
#define ZCL_NETWORK_JOIN_ROUTER_REQUEST_COMMAND_ID			 0x12	 
#define ZCL_NETWORK_JOIN_END_DEVICE_REQUEST_COMMAND_ID		 0x14	 
#define ZCL_NETWORK_UPDATE_REQUEST_COMMAND_ID				 0x16	 
#define ZCL_GET_GROUP_IDENTIFIERS_REQUEST_COMMAND_ID		 0x41	 
#define ZCL_GET_ENDPOINT_LIST_REQUEST_COMMAND_ID			 0x42	 


/*  
 * Command types for cluster: Sample Mfg Specific Cluster
 * Cluster specification level: UNKNOWN
*/

/* Client to server */
#define ZCL_COMMAND_ONE_COMMAND_ID							 0x00	


#endif 

