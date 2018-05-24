
#include<stdlib.h>
#include "app_hal.h"
#include "stm8s.h"
#include "stm8s_conf.h"

/************************************************************
  Function   : int  SysClk_set(sysClk_val_t etSysClk_val)

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
int  HAL_SysClk_set(sysClk_val_t etSysClk_val)
{
    return 0;
}

/************************************************************
  Function   : int  SysClk_set(sysClk_val_t etSysClk_val)

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
int  HAL_TimIT_set(sysTim_val_t etSysClk_val)
{
    TIM1_DeInit();
    TIM1_TimeBaseInit(2, TIM1_COUNTERMODE_UP, 1200, 200);
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
    TIM1_Cmd(ENABLE);
    return 0;
}

/************************************************************
  Function   :void HAL_GPIO_DeInit(void* GPIOx)

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
void HAL_GPIO_DeInit(HAL_GPIO_Port_TypeDef HAL_GPIOx)
{
    GPIO_TypeDef *GPIOx;
    switch(HAL_GPIOx)
    {
        case HAL_GPIOA:
        {
            GPIOx = GPIOA;
            break;
        }
        case HAL_GPIOB:
        {
            GPIOx = GPIOB;
            break;
        }
        case HAL_GPIOC:
        {
            GPIOx = GPIOC;
            break;
        }
        case HAL_GPIOD:
        {
            GPIOx = GPIOD;
            break;
        }
        default:
        {
            GPIOx = GPIOA;
            break;
        }
    }
    GPIO_DeInit(GPIOx);
}

/************************************************************
  Function   :void HAL_GPIO_DeInit(void* GPIOx)

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
void HAL_GPIO_Init(HAL_GPIO_Port_TypeDef HAL_GPIOx, HAL_GPIO_Pin_TypeDef GPIO_Pin, HAL_GPIO_Mode_TypeDef GPIO_Mode)
{
     GPIO_TypeDef *GPIOx;
     switch(HAL_GPIOx)
    {
        case HAL_GPIOA:
        {
            GPIOx = GPIOA;
            break;
        }
        case HAL_GPIOB:
        {
            GPIOx = GPIOB;
            break;
        }
        case HAL_GPIOC:
        {
            GPIOx = GPIOC;
            break;
        }
        case HAL_GPIOD:
        {
            GPIOx = GPIOD;
            break;
        }
        default:
        {
            GPIOx = GPIOA;
            break;
        }
    }
    GPIO_Init(GPIOx, (GPIO_Pin_TypeDef)GPIO_Pin, (GPIO_Mode_TypeDef)GPIO_Mode);
}

/************************************************************
  Function   :void HAL_GPIO_ReadInputData(void* GPIOx)

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
unsigned char  HAL_GPIO_ReadInputPin(HAL_GPIO_Port_TypeDef HAL_GPIOx,  HAL_GPIO_Pin_TypeDef GPIO_Pin)
{
     GPIO_TypeDef *GPIOx;
     switch(HAL_GPIOx)
    {
        case HAL_GPIOA:
        {
            GPIOx = GPIOA;
            break;
        }
        case HAL_GPIOB:
        {
            GPIOx = GPIOB;
            break;
        }
        case HAL_GPIOC:
        {
            GPIOx = GPIOC;
            break;
        }
        case HAL_GPIOD:
        {
            GPIOx = GPIOD;
            break;
        }
        default:
        {
            GPIOx = GPIOA;
            break;
        }
    }
    return ((char)GPIO_ReadInputPin(GPIOx, (GPIO_Pin_TypeDef)GPIO_Pin));
}

/************************************************************
  Function   :void HAL_GPIO_ReadInputData(void* GPIOx)

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
void  HAL_GPIO_OutputSet(HAL_GPIO_Port_TypeDef HAL_GPIOx,  HAL_GPIO_Pin_TypeDef PortPins, HAL_GPIO_VAL_TypeDef val)
{
     GPIO_TypeDef *GPIOx;
     switch(HAL_GPIOx)
    {
        case HAL_GPIOA:
        {
            GPIOx = GPIOA;
            break;
        }
        case HAL_GPIOB:
        {
            GPIOx = GPIOB;
            break;
        }
        case HAL_GPIOC:
        {
            GPIOx = GPIOC;
            break;
        }
        case HAL_GPIOD:
        {
            GPIOx = GPIOD;
            break;
        }
        default:
        {
            GPIOx = GPIOA;
            break;
        }
    }
    if(val == HAL_GPIO_LOW)
    {
         GPIO_WriteLow(GPIOx, (GPIO_Pin_TypeDef)PortPins);
    }
    else if(val == HAL_GPIO_HIGH)
    {
        GPIO_WriteHigh(GPIOx, (GPIO_Pin_TypeDef)PortPins);
    }
}


/************************************************************
  Function   :void HAL_GPIO_ReadInputData(void* GPIOx)

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
void  HAL_GPIO_OutputReverse(HAL_GPIO_Port_TypeDef HAL_GPIOx,  HAL_GPIO_Pin_TypeDef PortPins)
{
     GPIO_TypeDef *GPIOx;
     switch(HAL_GPIOx)
    {
        case HAL_GPIOA:
        {
            GPIOx = GPIOA;
            break;
        }
        case HAL_GPIOB:
        {
            GPIOx = GPIOB;
            break;
        }
        case HAL_GPIOC:
        {
            GPIOx = GPIOC;
            break;
        }
        case HAL_GPIOD:
        {
            GPIOx = GPIOD;
            break;
        }
        default:
        {
            GPIOx = GPIOA;
            break;
        }
    }
    GPIO_WriteReverse(GPIOx, (GPIO_Pin_TypeDef)PortPins);
}

/************************************************************
  Function   :void  HAL_GPIO_OutputReverse(HAL_GPIO_Port_TypeDef HAL_GPIOx,  HAL_GPIO_Pin_TypeDef PortPins)

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
unsigned char  HAL_GPIO_ReadOutputData(HAL_GPIO_Port_TypeDef HAL_GPIOx,  HAL_GPIO_Pin_TypeDef PortPins)
{
     GPIO_TypeDef *GPIOx;
     switch(HAL_GPIOx)
    {
        case HAL_GPIOA:
        {
            GPIOx = GPIOA;
            break;
        }
        case HAL_GPIOB:
        {
            GPIOx = GPIOB;
            break;
        }
        case HAL_GPIOC:
        {
            GPIOx = GPIOC;
            break;
        }
        case HAL_GPIOD:
        {
            GPIOx = GPIOD;
            break;
        }
        default:
        {
            GPIOx = GPIOA;
            break;
        }
    }
    return  (GPIO_ReadOutputData(GPIOx)&PortPins);
}

/************************************************************
  Function   : int  SysClk_set(sysClk_val_t etSysClk_val)

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
int  HAL_SysGpio_init(sysClk_val_t etSysClk_val)
{
    return 0;
}

/************************************************************
  Function   : int  Sys_usdelay(sysClk_val_t etSysClk_val)

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
int  HAL_Delayms(unsigned long  val)
{
    unsigned long  i =0;
    val =  (val <<3);
    for(i=0; i < val; i++)
    {
         nop();
    }
    return 0;
}

/************************************************************
  Function   : void  HAL_enableInterrupts(void)

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
void  HAL_enableInterrupts(void)
{
    enableInterrupts();
}
