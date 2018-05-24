/********************************************************************************

 **** Copyright (C), 2017, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : usb_hot_check.h
 * Author        : xukun
 * Date          : 2017-09-30
 * Description   : usb_hot_check.c header file
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2017-09-30
 *   Author      : xukun
 *   Modification: Created file

*************************************************************************************************************/

#ifndef __APP_H__
#define __APP_H__

#include "app_hal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define FS_DELAY_TIME  10
#define KEY_SVALVE_DELAY_TIME    2
#define KEY_LVALVE_DELAY_TIME    2
#define BEEP_WARNING_TIME          1500
#define POWER_ON_DELAY_TIME     4000



#ifndef FALSE
#define  FALSE     0
#endif
#ifndef TRUE
#define  TRUE       1
#endif

#ifndef RESET
#define  RESET     0
#endif
#ifndef SET
#define  SET       1
#endif

#define  APP_FAST_PWM        0
#define  APP_SLOW_PWM     50

#define  APP_FAN_SHUTDOWN_DELAY     5000


typedef enum _app_macheine_sta
{
    APP_POWEROFF                                          = 0,
    APP_POWERON_CHECK,
    APP_FAN_TURNOFF,
    APP_FAN_TURNON,
    APP_FAN_TURNON_DELAY,
    APP_SMALL_VALVE_KEY_CHECK,
    APP_BEFORE_FIRE_LIGHT_DELAY,
    APP_FIRE_LIGHT,
    APP_FIRE_LIGHT_DELAY,
    APP_FIRE_CHECK,
    APP_SMALL_VALVE_KEY_DELAY,
    APP_NORMAL_CHECK,
    APP_ABNORMAL_STATUS,
    APP_DEVICE_RESET,
    APP_DEVICE_SHUTDOWN,
    APP_DEVICE_SHUTDOWN_NULL,
    APP_STATUS_SUM,
}app_macheine_sta_t;


typedef enum _valve_macheine_sta
{
    VAVLE_TURNOFF                                        = 0,
    VAVLE_TURNON,
    VAVLE_TURNON_PWM,
    VAVLE_STATUS_SUM,
}valve_macheine_sta_t;

typedef enum _app_IO_switch
{
    app_IO_SWITCH_ON                                        = 0,
    app_IO_SWITCH_OFF,
    app_IO_SWITCH_ON2OFF,
    app_IO_SWITCH_OFF2ON,
    app_IO_SWITCH_FLASH_SLOW,
    app_IO_SWITCH_FLASH_FAST,
    app_IO_SWITCH_FLASH_FAST_1,
}app_IO_switch_t;

typedef enum _app_IO_mode
{
    app_IO_INPUT                                       = 0,
    app_IO_OUTPUT,

}app_IO_mode_t;

typedef struct  _app_IO_event_mgmt
{
    app_IO_mode_t                app_IO_mode;
    HAL_GPIO_Port_TypeDef  app_IO_port ;
    HAL_GPIO_Pin_TypeDef    app_IO_pin;
    app_IO_switch_t               app_IO_switch;
    unsigned  long                   app_IO_delay;
    unsigned  long                   app_IO_delay_switch;
    unsigned  char                   app_IO_pin_status;

}app_IO_event_mgmt_t;

int  app_process(void);
 int  app_Init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __USB_HOT_CHECK_H__ */
