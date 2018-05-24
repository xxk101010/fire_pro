

#include "stm32f0xx_hal.h"

#ifndef __KEY_NEW_H
#define __KEY_NEW_H


#ifndef  FALSE
#define  FALSE                  0
#endif


#ifndef  TRUE
#define  TRUE                   1
#endif

#define KEY_Num                 3


#define KEY_NULL                0                   // 无按键
#define KEY_DELAY_BASE          10                  // 消抖延迟时间基数
#define KEY_DITHERLESS_DELAY    2                 	// 按键消抖延迟时间20ms
#define KEY_HOLD_TIME           10                  // 按键按住多少时间开始重复
#define KEY_REPEAT_TIME         3                  // 按键长按后的重复时间
#define KEY_LONGHOLD_TIME       400                // 按键按住多少时间开始算长按2000ms

#define KEY_SHORTLONGHOLD_TIME     110                // 按键按住多少时间开始算长按2000ms

#define KEY_NO_HOLD_TIME        200                 // 按键按住多少时间开始算长按2000ms


//#define KEY_HOLD_MAX            250                 // 按键按住算超长按5000

#define KEY_HOLD_MAX            250                 // 按键按住算超长按5000
#define KEY_TIME_MAX            8000                // 总计数次数


#define KEY_DELAY_CHECK         0
#define KEY_LOOP_CHECK          1


// 按键状态定义
#define FSM_NOKEY           0   // 按键状态，无按键
#define FSM_KEY_DOWN        1   // 按键状态，按下
#define FSM_KEY_SURE_HOLD   2   // 按键状态，一直按住
#define FSM_KEY_UP          3   // 按键状态，松开




typedef enum{
        KEY_DOWN = 0,                       //!< key press down
        KEY_DOWN_HOLD,                      //!<key down hold
        KEY_SHORT_LONG_PRESSING,
        KEY_LONG_PRESSING,                  //!< key long pressing
        //KEY_UP,                             //!< key release
        KEY_PRESSED,                        //!< key pressed
        KEY_LONG_PRESSED,                   //!< key long pressed
        KEY_REPEAT,                         //!< key repeat
        KEY_UP_NULL,
}key_status_t;




typedef struct{
     unsigned int flag;
     unsigned int chKeyValue;           //!< key value and the value 0 is reserve
     key_status_t tEvent;                //!< key event type
}key_event_t;

extern  key_event_t  key_event_buf[KEY_Num];

// 不使用定时器检测按键，但支持按键按下和松开，按键长按等功能
struct Key_Info_Obj
{
    unsigned char   key_check_mode;  // 是否在循环中检测按键的标志KEY_DELAY_CHECK或KEY_LOOP_CHECK
    unsigned char   key_value;   //
    unsigned char    Key_switch_flag;

    unsigned char key_state;       // 按键状态机状态
    unsigned int  key_dly_cnt;  // 按键消抖延迟计数
    unsigned int  key_hold_cnt;  // 按键按下后保持计数
    unsigned int  key_no_hold_cnt;    // 按键按下后不保持计数
    unsigned int  key_repeat_cnt; // 按键按下后重复次数计数
    key_status_t  event;                //!< key event type
};


// ---------------------------- end of __KEY_USE_TIMER ---------------


void init_key_task(void);
void key_scan_task(void);
void Key_Event_Peocess(void);

#endif

