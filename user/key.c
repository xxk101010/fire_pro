


#include "key.h"
#include  "string.h"



//----------------------------------全局变量--------------------------------
struct Key_Info_Obj        key_info_obj_buf[KEY_Num];        /* 定义按键结构体数组 */

void read_key(void);



/*******************************************
函数名称：init_key_task
功    能：初始化按键结构体
参    数：无
返 回 值：无
描    述：
********************************************/
void init_key_task(void)
{
    unsigned int i;

    for(i = 0; i < KEY_Num ; i++)
    {
        key_info_obj_buf[i].key_check_mode = KEY_LOOP_CHECK;
        key_info_obj_buf[i].key_state = FSM_NOKEY;
        key_info_obj_buf[i].key_value=0;
        key_info_obj_buf[i].key_dly_cnt = 0;
        key_info_obj_buf[i].key_hold_cnt = 0;
        key_info_obj_buf[i].key_repeat_cnt = 0;
        key_info_obj_buf[i].event = KEY_UP_NULL;

    }

}


/*******************************************
函数名称：delay_key
功    能：抖动延时
参    数：struct_KeyInfo * pKey_Info: 按键组信息
返 回 值：无
描    述：
********************************************/
void delay_key(struct Key_Info_Obj * pKey_Info)
{
    unsigned char i, j;


    if (pKey_Info->key_check_mode)                                //定时器循环延时
    {
        if (pKey_Info->key_dly_cnt < KEY_DITHERLESS_DELAY)
        {
            pKey_Info->key_dly_cnt ++;
        }
    }
    else
    {
        pKey_Info->key_dly_cnt = 0;
        j = KEY_DITHERLESS_DELAY;
        while (j-- > 0)
            for (i = KEY_DELAY_BASE; i > 0; i--);
    }
}



/*******************************************
函数名称：key_callback
功    能：按键回调函数，用于长按和重复计数
参    数：    unsigned char key_holdtime: 保持计数次数
            struct_KeyInfo * pKey_Info: 按键组信息
返 回 值：无
描    述：
********************************************/
static void key_callback(unsigned short key_holdtime, struct Key_Info_Obj * pKey_Info)
{
     pKey_Info->key_hold_cnt++;

   if ((pKey_Info->key_hold_cnt >= key_holdtime) && (pKey_Info->key_hold_cnt <= (key_holdtime+2)))//&&(pKey_Info->key_hold_cnt <= (key_holdtime+2)
    {
                //pKey_Info->event=KEY_LONG_PRESSED;
            pKey_Info->key_repeat_cnt=0;

            //    Device_Switch_Fuc_Process(0xFF,i,0xFF,0,RESET,0XFF);
      // pKey_Info->key_hold_cnt++;
    }


    if(pKey_Info->key_hold_cnt >= KEY_LONGHOLD_TIME)
    {
        pKey_Info->event = KEY_LONG_PRESSING;
    }
    else if(pKey_Info->key_hold_cnt>=KEY_SHORTLONGHOLD_TIME)
    {
        pKey_Info->event = KEY_SHORT_LONG_PRESSING;
    }

}


/*******************************************
函数名称：key_fsm
功    能：检测按键状态转换机转换函数
参    数：    unsigned char key_value: 读到的键值
            unsigned char key_holdtime: 保持计数次数
            struct_KeyInfo * pKey_Info: 按键组信息
返 回 值：无
描    述：
********************************************/
static void key_fsm(unsigned char key_value, unsigned short key_holdtime, struct Key_Info_Obj * pKey_Info)
{
    switch (pKey_Info->key_state) // 根据新获得的键值转换按键状态，需区分两种检测方式
     {
        case FSM_NOKEY:           // 无按键
        {

            if (key_value != KEY_NULL)                      // key_value有值
            {
                if(pKey_Info->key_check_mode)            // 循环检测模式，进入消抖
                {
                    pKey_Info->key_dly_cnt=0;
                    pKey_Info->key_state = FSM_KEY_DOWN;  // 进入消抖
                }
                else                                      // 延迟检测模式（已经消抖）
                {
                    pKey_Info->key_hold_cnt = 0;
                    //pKey_Info->key_repeat_cnt = 0;
                    pKey_Info->key_state = FSM_KEY_SURE_HOLD; // 延迟检测模式，按键有效
                }
            }
            else
            {
                pKey_Info->event = KEY_UP_NULL;
                pKey_Info->key_state = FSM_NOKEY;
                pKey_Info->key_no_hold_cnt++;
                if(pKey_Info->key_no_hold_cnt>=KEY_NO_HOLD_TIME)
                {
                    pKey_Info->key_no_hold_cnt=0;
                    pKey_Info->key_repeat_cnt=0;
                }
            }
            break;
        }

        case FSM_KEY_DOWN:        // 循环检测模式按键可能按下，用于消抖
        {
            if (!pKey_Info->key_check_mode ||                   // 延迟检测模式或者循环满足延迟
                (pKey_Info->key_dly_cnt == KEY_DITHERLESS_DELAY))
            {
                if (key_value != KEY_NULL)                         // key_value有值
                {
                    //pKey_Info->key_hold_cnt++;
                //    pKey_Info->key_repeat_cnt = 0;
                    pKey_Info->key_state = FSM_KEY_SURE_HOLD;     // 按键有效
                    pKey_Info->event = KEY_DOWN;
                }
                else
                {
                    pKey_Info->key_state = FSM_NOKEY;             // 按键无效
                }
            }
            else ;
                    // 循环检测模式未满足延迟，保持状态
            break;
        }

        case FSM_KEY_SURE_HOLD:                                   // 按键确定按下，长按后可检测连续有效几次
        {
            if (key_value != KEY_NULL)
            {
                pKey_Info->key_state = FSM_KEY_SURE_HOLD;
                pKey_Info->event = KEY_DOWN_HOLD;
                if(!pKey_Info->key_check_mode ||
                    (pKey_Info->key_dly_cnt == KEY_DITHERLESS_DELAY))
                {
                  key_callback(key_holdtime, pKey_Info);
                }
            }
            else
            {
                pKey_Info->key_state = FSM_KEY_UP;
            }
            break;
        }

        case FSM_KEY_UP:          // 释放按键瞬间，可用于按键有效一次的情况
        {

               pKey_Info->key_repeat_cnt++;

                pKey_Info->event = KEY_PRESSED;

              pKey_Info->key_state = FSM_NOKEY;



//          if(pKey_Info->key_repeat_cnt == KEY_REPEAT_TIME) // 进入重复
//            {
//                    pKey_Info->event = KEY_REPEAT;
//                  pKey_Info->key_repeat_cnt=0;
//            }

//            if (pKey_Info->key_hold_cnt >= key_holdtime)
//        {
//                pKey_Info->event=KEY_LONG_PRESSED;
//            pKey_Info->key_repeat_cnt=0;
//        }

        pKey_Info->key_hold_cnt=0;


            pKey_Info->key_state =FSM_NOKEY;
            pKey_Info->key_no_hold_cnt=0;

            break;
        }

      default:
      {
            pKey_Info->key_state = FSM_NOKEY;

            break;
      }
    }
}


static void check_key_scan(unsigned char key_value, unsigned short key_holdtime, struct Key_Info_Obj * pKey_Info)
{

    if (key_value != KEY_NULL)              // 如果有键按下
    {
        delay_key(pKey_Info);             // 消除抖动
    }
    else
      {
        pKey_Info->key_dly_cnt = 0;
    }

    key_fsm(key_value, key_holdtime, pKey_Info);
}



/*******************************************
函数名称：key_app
功    能：串口发送按键信息
参    数：按键信息指针
返 回 值：无
描    述：
********************************************/
void key_scan_task(void)
{
    unsigned int i;

    read_key();
    for(i = 0; i < KEY_Num ; i++)
    {
        check_key_scan(key_info_obj_buf[i].key_value, KEY_HOLD_MAX , &key_info_obj_buf[i]);

    }
}

/*********************************************************************************************************

*********************************************************************************************************/


/*******************************************
函数名称：read_key
功    能：检测按键
参    数：无
返 回 值：无
描    述：0:无按键 1:有按键
********************************************/

 void read_key(void)
{
  key_info_obj_buf[0].key_value =  !(uint8_t)HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
    key_info_obj_buf[1].key_value =  !(uint8_t)HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
    key_info_obj_buf[2].key_value =  !(uint8_t)HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin);
    key_info_obj_buf[3].key_value =  !(uint8_t)HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin);
    key_info_obj_buf[4].key_value =  !(uint8_t)HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin);
    key_info_obj_buf[5].key_value =  !(uint8_t)HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin);
    key_info_obj_buf[6].key_value =  !(uint8_t)HAL_GPIO_ReadPin(KEY7_GPIO_Port, KEY7_Pin);
    key_info_obj_buf[7].key_value =  !(uint8_t)HAL_GPIO_ReadPin(KEY8_GPIO_Port, KEY8_Pin);
    key_info_obj_buf[8].key_value =  !(uint8_t)HAL_GPIO_ReadPin(KEY9_GPIO_Port, KEY9_Pin);
    key_info_obj_buf[9].key_value =  !(uint8_t)HAL_GPIO_ReadPin(KEY10_GPIO_Port, KEY10_Pin);

   //k1为设置，k2为上升，k3为下降，k4运行，k5停止
//     if(Key_mode==0)
//
      // key_info_obj_buf[0].key_value= !(uint8_t)HAL_GPIO_ReadPin(Service_GPIO_Port, Service_Pin);
//     }
    // key_info_obj_buf[1].key_value= !(uint8_t)HAL_GPIO_ReadPin(AUX_GPIO_Port, AUX_Pin);
}

void Key_Event_Peocess(void)
{
    uint8_t i=0;
    uint8_t * ptr_tmp;

    if(key_info_obj_buf[i].event==KEY_LONG_PRESSING)
    {
    }
    else if(key_info_obj_buf[i].event==KEY_UP_NULL)
    {

    }
    else
    {

    }
}


