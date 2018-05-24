

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


#define KEY_NULL                0                   // �ް���
#define KEY_DELAY_BASE          10                  // �����ӳ�ʱ�����
#define KEY_DITHERLESS_DELAY    2                 	// ���������ӳ�ʱ��20ms
#define KEY_HOLD_TIME           10                  // ������ס����ʱ�俪ʼ�ظ�
#define KEY_REPEAT_TIME         3                  // ������������ظ�ʱ��
#define KEY_LONGHOLD_TIME       400                // ������ס����ʱ�俪ʼ�㳤��2000ms

#define KEY_SHORTLONGHOLD_TIME     110                // ������ס����ʱ�俪ʼ�㳤��2000ms

#define KEY_NO_HOLD_TIME        200                 // ������ס����ʱ�俪ʼ�㳤��2000ms


//#define KEY_HOLD_MAX            250                 // ������ס�㳬����5000

#define KEY_HOLD_MAX            250                 // ������ס�㳬����5000
#define KEY_TIME_MAX            8000                // �ܼ�������


#define KEY_DELAY_CHECK         0
#define KEY_LOOP_CHECK          1


// ����״̬����
#define FSM_NOKEY           0   // ����״̬���ް���
#define FSM_KEY_DOWN        1   // ����״̬������
#define FSM_KEY_SURE_HOLD   2   // ����״̬��һֱ��ס
#define FSM_KEY_UP          3   // ����״̬���ɿ�




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

// ��ʹ�ö�ʱ����ⰴ������֧�ְ������º��ɿ������������ȹ���
struct Key_Info_Obj
{
    unsigned char   key_check_mode;  // �Ƿ���ѭ���м�ⰴ���ı�־KEY_DELAY_CHECK��KEY_LOOP_CHECK
    unsigned char   key_value;   //
    unsigned char    Key_switch_flag;

    unsigned char key_state;       // ����״̬��״̬
    unsigned int  key_dly_cnt;  // ���������ӳټ���
    unsigned int  key_hold_cnt;  // �������º󱣳ּ���
    unsigned int  key_no_hold_cnt;    // �������º󲻱��ּ���
    unsigned int  key_repeat_cnt; // �������º��ظ���������
    key_status_t  event;                //!< key event type
};


// ---------------------------- end of __KEY_USE_TIMER ---------------


void init_key_task(void);
void key_scan_task(void);
void Key_Event_Peocess(void);

#endif

