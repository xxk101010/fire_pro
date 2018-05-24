
#include<stdlib.h>
#include "stm8s.h"
#include "app.h"

app_macheine_sta_t  g_appStatus;
valve_macheine_sta_t  g_valve_status = VAVLE_TURNOFF;
app_IO_event_mgmt_t  g_IO_BEEP;
app_IO_event_mgmt_t  g_IO_SYSLED;
app_IO_event_mgmt_t  g_IO_KEY_SVALVE;
app_IO_event_mgmt_t  g_IO_KEY_LVALVE;
app_IO_event_mgmt_t  g_IO_FS_IN;
app_IO_event_mgmt_t  g_IO_SVALVE;
app_IO_event_mgmt_t  g_IO_LVALVE;
char IO_val;
char IO_val1;

/************************************************************
  Function   :  int  System_Init(void)

  Description:  system hardware init
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
 int  APP_GPIO_Init(void)
{
    int ret = 0;
    HAL_GPIO_DeInit(HAL_GPIOA);
    HAL_GPIO_DeInit(HAL_GPIOB);
    HAL_GPIO_DeInit(HAL_GPIOC);
    HAL_GPIO_DeInit(HAL_GPIOD);

    HAL_GPIO_Init(FS_IN_Port, FS_IN_Pin, HAL_GPIO_MODE_IN_FL_NO_IT);
    HAL_GPIO_Init(BEEP_Port, BEEP_Pin, HAL_GPIO_MODE_OUT_PP_LOW_FAST);
    HAL_GPIO_Init(FAN_O_Port, FAN_O_Pin, HAL_GPIO_MODE_OUT_PP_LOW_FAST);
    HAL_GPIO_Init(KEY_SMALL_VALVE_Port, KEY_SMALL_VALVE_Pin, HAL_GPIO_MODE_IN_FL_NO_IT);
    HAL_GPIO_Init(KEY_LARGE_VALVE_Port, KEY_LARGE_VALVE_Pin, HAL_GPIO_MODE_IN_FL_NO_IT);
    HAL_GPIO_Init(KEY_FAN_Port, KEY_FAN_Pin, HAL_GPIO_MODE_IN_FL_NO_IT);
    HAL_GPIO_Init(LARGE_VALVE_Port, LARGE_VALVE_Pin, HAL_GPIO_MODE_OUT_PP_LOW_FAST);
    HAL_GPIO_Init(SMALL_VALVE_Port, SMALL_VALVE_Pin, HAL_GPIO_MODE_OUT_PP_LOW_FAST);
    HAL_GPIO_Init(LED_R_Port, LED_R_Pin, HAL_GPIO_MODE_OUT_PP_LOW_FAST);
    HAL_GPIO_Init(LED_G_Port, LED_G_Pin, HAL_GPIO_MODE_OUT_PP_LOW_FAST);
    HAL_GPIO_Init(FS_START_Port, FS_START_Pin, HAL_GPIO_MODE_OUT_PP_LOW_FAST);
    HAL_GPIO_Init(LED_SYS_Port, LED_SYS_Pin, HAL_GPIO_MODE_OUT_PP_LOW_FAST);

    return ret;
}


/************************************************************
  Function   :  int  System_Init(void)

  Description:  system hardware init
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
 int  app_IO_event_register(app_IO_event_mgmt_t *pstIOEvent, HAL_GPIO_Port_TypeDef port, HAL_GPIO_Pin_TypeDef pin, app_IO_switch_t  status, unsigned  long  delay, app_IO_mode_t  mode)
{
    int ret = 0;

    if(pstIOEvent == NULL)
    {
        ret = -1;
        goto END;
    }
    pstIOEvent->app_IO_delay = delay;
    pstIOEvent->app_IO_delay_switch = 0;
    pstIOEvent->app_IO_pin = pin;
    pstIOEvent->app_IO_port = port;
    pstIOEvent->app_IO_switch = status;
    pstIOEvent->app_IO_mode = mode;
    pstIOEvent->app_IO_pin_status = RESET;

 END:
    return ret;
}

/************************************************************
  Function   :  int  System_Init(void)

  Description:  system hardware init
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
 int  app_IO_event_process(app_IO_event_mgmt_t *pstIOEvent)
 {
    int ret = 0;

    if(pstIOEvent == NULL)
    {
        ret = -1;
        goto END;
    }
    if(pstIOEvent->app_IO_mode == app_IO_OUTPUT)
    {

        if(pstIOEvent->app_IO_delay)
        {
            pstIOEvent->app_IO_delay--;
            ret = 0;
            goto END;
        }

        switch(pstIOEvent->app_IO_switch)
        {
            case app_IO_SWITCH_ON:
            {
                break;
            }
            case app_IO_SWITCH_OFF:
            {
                HAL_GPIO_OutputSet(pstIOEvent->app_IO_port,  pstIOEvent->app_IO_pin, HAL_GPIO_LOW);
                break;
            }
            case app_IO_SWITCH_ON2OFF:
            {
                pstIOEvent->app_IO_switch = app_IO_SWITCH_OFF;
                HAL_GPIO_OutputSet(pstIOEvent->app_IO_port,  pstIOEvent->app_IO_pin, HAL_GPIO_LOW);
                pstIOEvent->app_IO_pin_status = HAL_GPIO_ReadOutputData(pstIOEvent->app_IO_port,   pstIOEvent->app_IO_pin);
                break;
            }
            case app_IO_SWITCH_OFF2ON:
            {
                pstIOEvent->app_IO_switch = app_IO_SWITCH_ON;
                HAL_GPIO_OutputSet(pstIOEvent->app_IO_port,  pstIOEvent->app_IO_pin, HAL_GPIO_LOW);
                pstIOEvent->app_IO_pin_status = HAL_GPIO_ReadOutputData(pstIOEvent->app_IO_port,   pstIOEvent->app_IO_pin);
                break;
            }
            case app_IO_SWITCH_FLASH_SLOW:
            {
                HAL_GPIO_OutputReverse(pstIOEvent->app_IO_port,  pstIOEvent->app_IO_pin);
                pstIOEvent->app_IO_pin_status = HAL_GPIO_ReadOutputData(pstIOEvent->app_IO_port,   pstIOEvent->app_IO_pin);
                //pstIOEvent->app_IO_delay = 500;
                pstIOEvent->app_IO_delay_switch++;
                if( (pstIOEvent->app_IO_delay_switch%6==0) || ((pstIOEvent->app_IO_delay_switch - 1)%6)==0)
                {
                    pstIOEvent->app_IO_delay=1000;
                }
                else
                {
                    pstIOEvent->app_IO_delay = 300;
                }
                break;
            }
            case app_IO_SWITCH_FLASH_FAST:
            {
                HAL_GPIO_OutputReverse(pstIOEvent->app_IO_port,  pstIOEvent->app_IO_pin);
                pstIOEvent->app_IO_pin_status = HAL_GPIO_ReadOutputData(pstIOEvent->app_IO_port,   pstIOEvent->app_IO_pin);
                pstIOEvent->app_IO_delay = APP_SLOW_PWM;
                break;
            }
            case  app_IO_SWITCH_FLASH_FAST_1:
            {
                 HAL_GPIO_OutputReverse(pstIOEvent->app_IO_port,  pstIOEvent->app_IO_pin);
                 pstIOEvent->app_IO_pin_status = HAL_GPIO_ReadOutputData(pstIOEvent->app_IO_port,   pstIOEvent->app_IO_pin);
                  pstIOEvent->app_IO_delay = APP_FAST_PWM;
                 break;
            }
            default:
            {
                break;
            }
        }
    }
    else if(pstIOEvent->app_IO_mode == app_IO_INPUT)
    {
         if(HAL_GPIO_ReadInputPin(pstIOEvent->app_IO_port,  pstIOEvent->app_IO_pin)  != pstIOEvent->app_IO_pin_status)
         {
             HAL_Delayms(pstIOEvent->app_IO_delay);
             if(HAL_GPIO_ReadInputPin(pstIOEvent->app_IO_port,  pstIOEvent->app_IO_pin) != pstIOEvent->app_IO_pin_status)
             {
                 pstIOEvent->app_IO_pin_status = HAL_GPIO_ReadInputPin(pstIOEvent->app_IO_port,  pstIOEvent->app_IO_pin);
             }
         }
    }
 END:
    return ret;
}

/************************************************************
  Function   :   void  app_IO_event_polling(app_IO_event_mgmt_t *pstIOEvent)

  Description:  system hardware init
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
 void  app_IO_event_set(app_IO_event_mgmt_t *pstIOEvent, app_IO_switch_t  status, unsigned  long  delay)
{
     if(status == app_IO_SWITCH_ON2OFF || status == app_IO_SWITCH_ON)
     {
         HAL_GPIO_OutputSet(pstIOEvent->app_IO_port,  pstIOEvent->app_IO_pin, HAL_GPIO_HIGH);
     }
     else if(status == app_IO_SWITCH_OFF2ON || status == app_IO_SWITCH_OFF)
     {
         HAL_GPIO_OutputSet(pstIOEvent->app_IO_port,  pstIOEvent->app_IO_pin, HAL_GPIO_LOW);
     }
     pstIOEvent->app_IO_switch = status;
     pstIOEvent->app_IO_delay = delay;
     pstIOEvent->app_IO_pin_status = HAL_GPIO_ReadOutputData(pstIOEvent->app_IO_port,   pstIOEvent->app_IO_pin);
}

/************************************************************
  Function   :   void  app_IO_event_polling(app_IO_event_mgmt_t *pstIOEvent)

  Description:  system hardware init
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
char  app_IO_event_get(app_IO_event_mgmt_t *pstIOEvent)
{
     return pstIOEvent->app_IO_pin_status;
}

/************************************************************
  Function   :   void  app_IO_event_polling(app_IO_event_mgmt_t *pstIOEvent)

  Description:  system hardware init
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
 void  app_IO_event_polling(void)
{
    app_IO_event_process(&g_IO_BEEP);
    app_IO_event_process(&g_IO_SYSLED);
    app_IO_event_process(&g_IO_KEY_SVALVE);
    app_IO_event_process(&g_IO_KEY_LVALVE);
    app_IO_event_process(&g_IO_FS_IN);
    app_IO_event_process(&g_IO_SVALVE);
    app_IO_event_process(&g_IO_LVALVE);
}

/************************************************************
  Function   :  int  System_Init(void)

  Description:  system hardware init
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
 int  System_Init(void)
{
    int ret = 0;

    HAL_Delayms(POWER_ON_DELAY_TIME);
    ret =  HAL_SysClk_set(CLK_2MHZ);
    if(ret)
    {
        goto END;
    }
    HAL_Delayms(1000);
    APP_GPIO_Init();
    HAL_TimIT_set(IT_500ms);
    HAL_enableInterrupts();

 END:
    return ret;
}

/************************************************************
  Function   :  int  System_Init(void)

  Description:  system hardware init
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2017/09/20, kun.xu create

************************************************************/
 int  app_Init(void)
{
    int ret = 0;

    ret = System_Init();
    if(ret)
    {
        ret = -1;
        goto END;
    }
    app_IO_event_register(&g_IO_BEEP, BEEP_Port, BEEP_Pin, app_IO_SWITCH_OFF, 0,  app_IO_OUTPUT);
    app_IO_event_register(&g_IO_SYSLED, LED_SYS_Port, LED_SYS_Pin, app_IO_SWITCH_FLASH_SLOW, 0, app_IO_OUTPUT);
    app_IO_event_register(&g_IO_KEY_SVALVE, KEY_SMALL_VALVE_Port, KEY_SMALL_VALVE_Pin, app_IO_SWITCH_OFF, KEY_SVALVE_DELAY_TIME, app_IO_INPUT);
    app_IO_event_register(&g_IO_KEY_LVALVE, KEY_LARGE_VALVE_Port, KEY_LARGE_VALVE_Pin, app_IO_SWITCH_OFF, KEY_LVALVE_DELAY_TIME, app_IO_INPUT);
    app_IO_event_register(&g_IO_FS_IN, FS_IN_Port, FS_IN_Pin, app_IO_SWITCH_OFF, FS_DELAY_TIME, app_IO_INPUT);

 END:
    return ret;
}

/************************************************************
  Function   : static VOID_T *fespa_bfHandler(VOID_T *ptr)

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
int  app_process(void)
{
    int ret = 0;
    int  fs_try_cnt = 0;
    long fan_delay = 0;
    long fire_delay = 0;
    long before_fire_delay = 0;
    long fire_check_delay = 0;
    long small_valve_delay = 0;
    //long large_valve_delay = 0;
    long small_valve_check_delay = 0;

    HAL_Delayms(2000);
    g_valve_status = VAVLE_TURNOFF;
    g_appStatus = APP_POWERON_CHECK;
    while(g_appStatus != APP_ABNORMAL_STATUS)
    {
         app_IO_event_polling();
         switch(g_appStatus)
         {
             case APP_POWEROFF:
             {
                 //g_valve_status = VAVLE_TURNOFF;
                 HAL_GPIO_OutputSet(FS_START_Port,  FS_START_Pin, HAL_GPIO_LOW);
                 HAL_GPIO_OutputSet(LARGE_VALVE_Port,  LARGE_VALVE_Pin, HAL_GPIO_LOW);
                 HAL_Delayms(100);
                 //HAL_GPIO_OutputSet(SMALL_VALVE_Port,  SMALL_VALVE_Pin, HAL_GPIO_LOW);
                 app_IO_event_register(&g_IO_SVALVE, SMALL_VALVE_Port, SMALL_VALVE_Pin, app_IO_SWITCH_OFF, 0, app_IO_OUTPUT);
                 app_IO_event_register(&g_IO_LVALVE, LARGE_VALVE_Port, LARGE_VALVE_Pin, app_IO_SWITCH_OFF, 0, app_IO_OUTPUT);
                 HAL_GPIO_OutputSet(LED_R_Port,  LED_R_Pin, HAL_GPIO_LOW);
                 HAL_GPIO_OutputSet(LED_G_Port,  LED_G_Pin, HAL_GPIO_LOW);
                 HAL_Delayms(100);
                 HAL_GPIO_OutputSet(FAN_O_Port,  FAN_O_Pin, HAL_GPIO_LOW);
                 HAL_Delayms(100);
                 g_appStatus =    APP_POWERON_CHECK;
                 break;
             }
             case APP_POWERON_CHECK:
             {
                 if(app_IO_event_get(&g_IO_FS_IN))
                 {
                     g_appStatus = APP_FAN_TURNON;
                     app_IO_event_set(&g_IO_BEEP, app_IO_SWITCH_ON2OFF, 100);
                 }
                 else
                 {
                     app_IO_event_set(&g_IO_BEEP, app_IO_SWITCH_ON2OFF, BEEP_WARNING_TIME);
                     g_appStatus = APP_DEVICE_SHUTDOWN;
                 }
                 break;
             }
             case APP_FAN_TURNOFF:
             {
                 break;
             }
             case APP_FAN_TURNON:
             {
                 HAL_GPIO_OutputSet(FAN_O_Port,  FAN_O_Pin, HAL_GPIO_HIGH);
                 fan_delay =  1200;
                 g_appStatus = APP_FAN_TURNON_DELAY;
                 break;
             }
             case APP_FAN_TURNON_DELAY:
             {
                 if(!fan_delay)
                 {
                     g_appStatus = APP_SMALL_VALVE_KEY_CHECK;
                     small_valve_check_delay = 7000;
                     break;
                 }
                 fan_delay--;
                 break;
             }
             case APP_SMALL_VALVE_KEY_CHECK:
             {
                 if(!app_IO_event_get(&g_IO_KEY_SVALVE))
                 {
                      fs_try_cnt = 0;
                      before_fire_delay = 100;
                      g_appStatus = APP_BEFORE_FIRE_LIGHT_DELAY;
                 }
                 if(!small_valve_check_delay)
                 {
                     g_appStatus  = APP_DEVICE_SHUTDOWN;
                 }
                 small_valve_check_delay--;
                 break;
             }
             case APP_BEFORE_FIRE_LIGHT_DELAY:
             {
                 if(app_IO_event_get(&g_IO_KEY_SVALVE))
                 {
                     g_appStatus =  APP_DEVICE_RESET;
                     break;
                 }
                 if(!before_fire_delay)
                 {
                     g_appStatus = APP_FIRE_LIGHT;
                     break;
                 }
                 before_fire_delay--;
                 break;
             }
             case APP_FIRE_LIGHT:
             {
                 HAL_GPIO_OutputSet(FS_START_Port,  FS_START_Pin, HAL_GPIO_HIGH);
                 fire_delay = 1500;
                 g_appStatus = APP_FIRE_LIGHT_DELAY;
                 break;
             }
             case APP_FIRE_LIGHT_DELAY:
             {
                 if(app_IO_event_get(&g_IO_KEY_SVALVE))
                 {
                      g_appStatus =    APP_DEVICE_RESET;
                      break;
                 }
                 if(!fire_delay)
                 {
                     g_appStatus = APP_FIRE_CHECK;
                     fire_check_delay = 5000;
                     //HAL_GPIO_OutputSet(SMALL_VALVE_Port,  SMALL_VALVE_Pin, HAL_GPIO_HIGH);
                     app_IO_event_register(&g_IO_SVALVE, SMALL_VALVE_Port, SMALL_VALVE_Pin, app_IO_SWITCH_FLASH_FAST, 0, app_IO_OUTPUT);
                     break;
                 }
                 fire_delay--;
                 break;
             }
             case  APP_FIRE_CHECK:
             {
                if(fire_check_delay)
                {
                      if(app_IO_event_get(&g_IO_KEY_SVALVE))
                      {
                          g_appStatus =    APP_DEVICE_RESET;
                          break;
                      }
                      if(!app_IO_event_get(&g_IO_FS_IN))
                      {
                          HAL_GPIO_OutputSet(LED_R_Port,  LED_R_Pin, HAL_GPIO_HIGH);
                          HAL_GPIO_OutputSet(FS_START_Port,  FS_START_Pin, HAL_GPIO_LOW);
                          g_appStatus = APP_SMALL_VALVE_KEY_DELAY;
                          small_valve_delay = 400;
                      }
                      fire_check_delay--;
                      break;
                }
                if(g_appStatus == APP_SMALL_VALVE_KEY_DELAY)
                {
                    break;
                }
                else
                {
                    g_appStatus =   APP_BEFORE_FIRE_LIGHT_DELAY;
                    before_fire_delay = 3000;
                }
                HAL_GPIO_OutputSet(FS_START_Port,  FS_START_Pin, HAL_GPIO_LOW);      // failed : turn off val and fire delay
                HAL_Delayms(1);
                //HAL_GPIO_OutputSet(SMALL_VALVE_Port,  SMALL_VALVE_Pin, HAL_GPIO_LOW);
                  app_IO_event_register(&g_IO_SVALVE, SMALL_VALVE_Port, SMALL_VALVE_Pin, app_IO_SWITCH_OFF, 0, app_IO_OUTPUT);
                HAL_Delayms(1);
                 fs_try_cnt++;
                 if(fs_try_cnt >= 3)
                 {
                     app_IO_event_set(&g_IO_BEEP, app_IO_SWITCH_ON2OFF, BEEP_WARNING_TIME);
                     //g_appStatus = APP_DEVICE_RESET;
                     //g_appStatus = APP_FAN_TURNON_DELAY;
                     g_appStatus = APP_DEVICE_SHUTDOWN;
                     fan_delay = 2000;

                 }
                 break;
             }
             case APP_SMALL_VALVE_KEY_DELAY:
             {
                 if(app_IO_event_get(&g_IO_KEY_SVALVE))
                 {
                      g_appStatus =    APP_DEVICE_RESET;
                      break;
                 }
                 if(!small_valve_delay)
                 {
                     app_IO_event_register(&g_IO_SVALVE, SMALL_VALVE_Port, SMALL_VALVE_Pin, app_IO_SWITCH_FLASH_FAST_1, 0, app_IO_OUTPUT);
                     g_appStatus = APP_NORMAL_CHECK;
                     break;
                 }
                 small_valve_delay--;
                 break;
             }
             case APP_NORMAL_CHECK:
             {
                  if(app_IO_event_get(&g_IO_KEY_SVALVE))
                  {
                      g_appStatus =    APP_DEVICE_RESET;
                      break;
                  }
                  if(app_IO_event_get(&g_IO_FS_IN))
                  {
                         g_appStatus =  APP_DEVICE_RESET;
                         break;
                  }
                  if((!(!g_valve_status))  !=  (!app_IO_event_get(&g_IO_KEY_LVALVE)))  //
                  {
                     if( !app_IO_event_get(&g_IO_KEY_LVALVE))
                     {
                          g_valve_status = VAVLE_TURNON;
                          //large_valve_delay = 450;
                          app_IO_event_register(&g_IO_LVALVE, LARGE_VALVE_Port, LARGE_VALVE_Pin, app_IO_SWITCH_FLASH_FAST_1, 0, app_IO_OUTPUT);
                          //HAL_GPIO_OutputSet(LARGE_VALVE_Port,  LARGE_VALVE_Pin, HAL_GPIO_HIGH);
                          HAL_GPIO_OutputSet(LED_G_Port,  LED_G_Pin, HAL_GPIO_HIGH);
                          HAL_Delayms(1);
                     }
                     else
                     {
                          g_valve_status = VAVLE_TURNOFF;
                          app_IO_event_register(&g_IO_LVALVE, LARGE_VALVE_Port, LARGE_VALVE_Pin, app_IO_SWITCH_OFF, 0, app_IO_OUTPUT);
                          //HAL_GPIO_OutputSet(LARGE_VALVE_Port,  LARGE_VALVE_Pin, HAL_GPIO_LOW);
                          HAL_GPIO_OutputSet(LED_G_Port,  LED_G_Pin, HAL_GPIO_LOW);
                          HAL_Delayms(1);
                     }
                  }
                  /* if(g_valve_status == VAVLE_TURNON) */
                  /* { */
                       /* if(large_valve_delay) */
                       /* { */
                            /* large_valve_delay--; */
                       /* } */
                       /* else */
                       /* { */
                           /* g_valve_status = VAVLE_TURNON_PWM; */
                       /* } */
                  /* } */
                  /* else if(g_valve_status == VAVLE_TURNON_PWM) */
                  /* { */
                      /* HAL_GPIO_OutputReverse(LARGE_VALVE_Port,  LARGE_VA-
                      LVE_Pin); */
                  /* } */
                  //HAL_GPIO_OutputReverse(SMALL_VALVE_Port,  SMALL_VALVE_Pin);
                  break;
             }
             case APP_DEVICE_RESET:
             {
                 g_valve_status = VAVLE_TURNOFF;
                 HAL_GPIO_OutputSet(FS_START_Port,  FS_START_Pin, HAL_GPIO_LOW);
                 HAL_Delayms(50);
                 //HAL_GPIO_OutputSet(LARGE_VALVE_Port,  LARGE_VALVE_Pin, HAL_GPIO_LOW);
                 app_IO_event_register(&g_IO_LVALVE, LARGE_VALVE_Port, LARGE_VALVE_Pin, app_IO_SWITCH_OFF, 0, app_IO_OUTPUT);
                 HAL_Delayms(50);
                 //HAL_GPIO_OutputSet(SMALL_VALVE_Port,  SMALL_VALVE_Pin, HAL_GPIO_LOW);
                 app_IO_event_register(&g_IO_SVALVE, SMALL_VALVE_Port, SMALL_VALVE_Pin, app_IO_SWITCH_OFF, 0, app_IO_OUTPUT);
                 HAL_Delayms(50);
                 HAL_GPIO_OutputSet(LED_R_Port,  LED_R_Pin, HAL_GPIO_LOW);
                 HAL_GPIO_OutputSet(LED_G_Port,  LED_G_Pin, HAL_GPIO_LOW);
                 g_appStatus   = APP_SMALL_VALVE_KEY_CHECK;
                 small_valve_check_delay = 7000;
                 break;
             }
             case APP_DEVICE_SHUTDOWN:
             {
                 g_valve_status = VAVLE_TURNOFF;
                 HAL_GPIO_OutputSet(FS_START_Port,  FS_START_Pin, HAL_GPIO_LOW);
                 HAL_Delayms(150);
                 //HAL_GPIO_OutputSet(LARGE_VALVE_Port,  LARGE_VALVE_Pin, HAL_GPIO_LOW);
                 app_IO_event_register(&g_IO_LVALVE, LARGE_VALVE_Port, LARGE_VALVE_Pin, app_IO_SWITCH_OFF, 0, app_IO_OUTPUT);
                 HAL_GPIO_OutputSet(LED_G_Port,  LED_G_Pin, HAL_GPIO_LOW);
                 HAL_Delayms(150);
                 //HAL_GPIO_OutputSet(SMALL_VALVE_Port,  SMALL_VALVE_Pin, HAL_GPIO_LOW);
                 app_IO_event_register(&g_IO_SVALVE, SMALL_VALVE_Port, SMALL_VALVE_Pin, app_IO_SWITCH_OFF, 0, app_IO_OUTPUT);
                 HAL_GPIO_OutputSet(LED_R_Port,  LED_R_Pin, HAL_GPIO_LOW);
                 HAL_Delayms(APP_FAN_SHUTDOWN_DELAY);
                 HAL_GPIO_OutputSet(FAN_O_Port,  FAN_O_Pin, HAL_GPIO_LOW);
                 g_appStatus   = APP_DEVICE_SHUTDOWN_NULL;
                 break;
             }
             case APP_DEVICE_SHUTDOWN_NULL:
             {
                 break;
             }
             default:
             {
                  break;
             }
         }
         HAL_Delayms(1);
    }
    return ret;
}


