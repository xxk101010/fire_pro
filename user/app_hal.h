/********************************************************************************

 **** Copyright (C), 2017, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     :   app_hal.h
 * Author        :    xukun
 * Date          :     2017-11-18
 * Description   : usb_hot_check.c header file
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        :  2017-11-18
 *   Author      : xukun
 *   Modification: Created file

*************************************************************************************************************/

#ifndef __APP_HAL_H__
#define __APP_HAL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*   GPIO CONFIG   */
typedef enum
{
  HAL_GPIO_MODE_IN_FL_NO_IT      = (unsigned char)0x00,  /*!< Input floating, no external interrupt */
  HAL_GPIO_MODE_IN_PU_NO_IT      = (unsigned char)0x40,  /*!< Input pull-up, no external interrupt */
  HAL_GPIO_MODE_IN_FL_IT         = (unsigned char)0x20,  /*!< Input floating, external interrupt */
  HAL_GPIO_MODE_IN_PU_IT         = (unsigned char)0x60,  /*!< Input pull-up, external interrupt */
  HAL_GPIO_MODE_OUT_OD_LOW_FAST  = (unsigned char)0xA0,  /*!< Output open-drain, low level, 10MHz */
  HAL_GPIO_MODE_OUT_PP_LOW_FAST  = (unsigned char)0xE0,  /*!< Output push-pull, low level, 10MHz */
  HAL_GPIO_MODE_OUT_OD_LOW_SLOW  = (unsigned char)0x80,  /*!< Output open-drain, low level, 2MHz */
  HAL_GPIO_MODE_OUT_PP_LOW_SLOW  = (unsigned char)0xC0,  /*!< Output push-pull, low level, 2MHz */
  HAL_GPIO_MODE_OUT_OD_HIZ_FAST  = (unsigned char)0xB0,  /*!< Output open-drain, high-impedance level,10MHz */
  HAL_GPIO_MODE_OUT_PP_HIGH_FAST = (unsigned char)0xF0,  /*!< Output push-pull, high level, 10MHz */
  HALGPIO_MODE_OUT_OD_HIZ_SLOW  = (unsigned char)0x90,  /*!< Output open-drain, high-impedance level, 2MHz */
  HAL_GPIO_MODE_OUT_PP_HIGH_SLOW = (unsigned char)0xD0   /*!< Output push-pull, high level, 2MHz */
}HAL_GPIO_Mode_TypeDef;

typedef enum
{
  HAL_GPIO_PIN_0    = ((unsigned char)0x01),  /*!< Pin 0 selected */
  HAL_GPIO_PIN_1    = ((unsigned char)0x02),  /*!< Pin 1 selected */
  HAL_GPIO_PIN_2    = ((unsigned char)0x04),  /*!< Pin 2 selected */
  HAL_GPIO_PIN_3    = ((unsigned char)0x08),   /*!< Pin 3 selected */
  HAL_GPIO_PIN_4    = ((unsigned char)0x10),  /*!< Pin 4 selected */
  HAL_GPIO_PIN_5    = ((unsigned char)0x20),  /*!< Pin 5 selected */
  HAL_GPIO_PIN_6    = ((unsigned char)0x40),  /*!< Pin 6 selected */
  HAL_GPIO_PIN_7    = ((unsigned char)0x80),  /*!< Pin 7 selected */
  HAL_GPIO_PIN_LNIB = ((unsigned char)0x0F),  /*!< Low nibble pins selected */
  HAL_GPIO_PIN_HNIB = ((unsigned char)0xF0),  /*!< High nibble pins selected */
  HAL_GPIO_PIN_ALL  = ((unsigned char)0xFF)   /*!< All pins selected */
}HAL_GPIO_Pin_TypeDef;

typedef enum
{
  HAL_GPIOA          = 0,
  HAL_GPIOB,
  HAL_GPIOC,
  HAL_GPIOD,
  HAL_GPIOE,
}HAL_GPIO_Port_TypeDef;

typedef enum
{
    HAL_GPIO_LOW           = 0,
    HAL_GPIO_HIGH          = 1,
}HAL_GPIO_VAL_TypeDef;

typedef enum _sysClk_val
{
    CLK_2MHZ   =0 ,
    CLK_4MHZ,
    CLK_6MHZ,
    CLK_8MHZ,
    CLK_12MHZ,
    CLK_14MHZ,
    CLK_16MHZ
}sysClk_val_t;

typedef enum _sysTim_val
{
    IT_500ms   =0 ,
    IT_1s,
    IT_2s,
}sysTim_val_t;

#define FS_IN_Pin                                        HAL_GPIO_PIN_3
#define FS_IN_Port                                      HAL_GPIOD
#define FS_START_Pin                                 HAL_GPIO_PIN_3
#define FS_START_Port                               HAL_GPIOC
#define BEEP_Pin                                          HAL_GPIO_PIN_5
#define BEEP_Port                                        HAL_GPIOC
#define FAN_O_Pin                                       HAL_GPIO_PIN_4
#define FAN_O_Port                                     HAL_GPIOC
#define KEY_SMALL_VALVE_Pin                 HAL_GPIO_PIN_6
#define KEY_SMALL_VALVE_Port               HAL_GPIOD
#define KEY_LARGE_VALVE_Pin                 HAL_GPIO_PIN_5
#define KEY_LARGE_VALVE_Port               HAL_GPIOD
#define KEY_FAN_Pin                                  HAL_GPIO_PIN_4
#define KEY_FAN_Port                                HAL_GPIOD
#define LARGE_VALVE_Pin                          HAL_GPIO_PIN_2
#define LARGE_VALVE_Port                        HAL_GPIOA
#define SMALL_VALVE_Pin                          HAL_GPIO_PIN_1
#define SMALL_VALVE_Port                        HAL_GPIOA
#define LED_R_Pin                                        HAL_GPIO_PIN_7
#define LED_R_Port                                      HAL_GPIOC
#define LED_G_Pin                                        HAL_GPIO_PIN_6
#define LED_G_Port                                      HAL_GPIOC
#define LED_SYS_Pin                                    HAL_GPIO_PIN_3
#define LED_SYS_Port                                  HAL_GPIOA

int   HAL_SysClk_set(sysClk_val_t etSysClk_val);
int  HAL_TimIT_set(sysTim_val_t etSysClk_val);
//int  HAL_Delay100us( unsigned long   val);
int  HAL_Delayms(unsigned long  val);
void HAL_GPIO_DeInit(HAL_GPIO_Port_TypeDef HAL_GPIOx);
void HAL_GPIO_Init(HAL_GPIO_Port_TypeDef HAL_GPIOx, HAL_GPIO_Pin_TypeDef GPIO_Pin, HAL_GPIO_Mode_TypeDef GPIO_Mode);
unsigned char  HAL_GPIO_ReadInputPin(HAL_GPIO_Port_TypeDef HAL_GPIOx,  HAL_GPIO_Pin_TypeDef GPIO_Pin);
void  HAL_GPIO_OutputSet(HAL_GPIO_Port_TypeDef HAL_GPIOx,  HAL_GPIO_Pin_TypeDef PortPins, HAL_GPIO_VAL_TypeDef val);
void  HAL_GPIO_OutputReverse(HAL_GPIO_Port_TypeDef HAL_GPIOx,  HAL_GPIO_Pin_TypeDef PortPins);
unsigned char  HAL_GPIO_ReadOutputData(HAL_GPIO_Port_TypeDef HAL_GPIOx,  HAL_GPIO_Pin_TypeDef PortPins);
void  HAL_enableInterrupts(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __USB_HOT_CHECK_H__ */
