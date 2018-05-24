


#include "key.h"
#include  "string.h"



//----------------------------------ȫ�ֱ���--------------------------------
struct Key_Info_Obj        key_info_obj_buf[KEY_Num];        /* ���尴���ṹ������ */

void read_key(void);



/*******************************************
�������ƣ�init_key_task
��    �ܣ���ʼ�������ṹ��
��    ������
�� �� ֵ����
��    ����
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
�������ƣ�delay_key
��    �ܣ�������ʱ
��    ����struct_KeyInfo * pKey_Info: ��������Ϣ
�� �� ֵ����
��    ����
********************************************/
void delay_key(struct Key_Info_Obj * pKey_Info)
{
    unsigned char i, j;


    if (pKey_Info->key_check_mode)                                //��ʱ��ѭ����ʱ
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
�������ƣ�key_callback
��    �ܣ������ص����������ڳ������ظ�����
��    ����    unsigned char key_holdtime: ���ּ�������
            struct_KeyInfo * pKey_Info: ��������Ϣ
�� �� ֵ����
��    ����
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
�������ƣ�key_fsm
��    �ܣ���ⰴ��״̬ת����ת������
��    ����    unsigned char key_value: �����ļ�ֵ
            unsigned char key_holdtime: ���ּ�������
            struct_KeyInfo * pKey_Info: ��������Ϣ
�� �� ֵ����
��    ����
********************************************/
static void key_fsm(unsigned char key_value, unsigned short key_holdtime, struct Key_Info_Obj * pKey_Info)
{
    switch (pKey_Info->key_state) // �����»�õļ�ֵת������״̬�����������ּ�ⷽʽ
     {
        case FSM_NOKEY:           // �ް���
        {

            if (key_value != KEY_NULL)                      // key_value��ֵ
            {
                if(pKey_Info->key_check_mode)            // ѭ�����ģʽ����������
                {
                    pKey_Info->key_dly_cnt=0;
                    pKey_Info->key_state = FSM_KEY_DOWN;  // ��������
                }
                else                                      // �ӳټ��ģʽ���Ѿ�������
                {
                    pKey_Info->key_hold_cnt = 0;
                    //pKey_Info->key_repeat_cnt = 0;
                    pKey_Info->key_state = FSM_KEY_SURE_HOLD; // �ӳټ��ģʽ��������Ч
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

        case FSM_KEY_DOWN:        // ѭ�����ģʽ�������ܰ��£���������
        {
            if (!pKey_Info->key_check_mode ||                   // �ӳټ��ģʽ����ѭ�������ӳ�
                (pKey_Info->key_dly_cnt == KEY_DITHERLESS_DELAY))
            {
                if (key_value != KEY_NULL)                         // key_value��ֵ
                {
                    //pKey_Info->key_hold_cnt++;
                //    pKey_Info->key_repeat_cnt = 0;
                    pKey_Info->key_state = FSM_KEY_SURE_HOLD;     // ������Ч
                    pKey_Info->event = KEY_DOWN;
                }
                else
                {
                    pKey_Info->key_state = FSM_NOKEY;             // ������Ч
                }
            }
            else ;
                    // ѭ�����ģʽδ�����ӳ٣�����״̬
            break;
        }

        case FSM_KEY_SURE_HOLD:                                   // ����ȷ�����£�������ɼ��������Ч����
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

        case FSM_KEY_UP:          // �ͷŰ���˲�䣬�����ڰ�����Чһ�ε����
        {

               pKey_Info->key_repeat_cnt++;

                pKey_Info->event = KEY_PRESSED;

              pKey_Info->key_state = FSM_NOKEY;



//          if(pKey_Info->key_repeat_cnt == KEY_REPEAT_TIME) // �����ظ�
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

    if (key_value != KEY_NULL)              // ����м�����
    {
        delay_key(pKey_Info);             // ��������
    }
    else
      {
        pKey_Info->key_dly_cnt = 0;
    }

    key_fsm(key_value, key_holdtime, pKey_Info);
}



/*******************************************
�������ƣ�key_app
��    �ܣ����ڷ��Ͱ�����Ϣ
��    ����������Ϣָ��
�� �� ֵ����
��    ����
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
�������ƣ�read_key
��    �ܣ���ⰴ��
��    ������
�� �� ֵ����
��    ����0:�ް��� 1:�а���
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

   //k1Ϊ���ã�k2Ϊ������k3Ϊ�½���k4���У�k5ֹͣ
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


