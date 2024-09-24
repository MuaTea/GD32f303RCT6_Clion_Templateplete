#include "state_drv.h"
#include "system.h"


u16 breath_timer = 0;//������
u16 alarm_timer = 0;//�л�״̬ʱ��
u16 blink_timer = 0;//��˸
u8 led_state = 0;
u8 alarm_active = 0; // ȫ�ֱ�������״̬����
u8 state = STATE_0; // 0: ������˸, 1: ������

//ָʾ�ƿ����������֮���л�
void LED_Control(u16 arr)
{
    // ���ݱ���״̬���� LED ��Ϊģʽ
    if (alarm_active) // ����������ܱ�����
    {
    	//static u8 state = STATE_0;// 0: ������˸, 1: ������
        // ���ݵ�ǰģʽ������ LED
        switch (state)
        {
            case STATE_0: // ������˸ģʽ
                if (blink_timer >= 500) // �л�Ƶ��Ϊ 500ms
                {
                    blink_timer = 0;
                    led_state = (led_state == 0) ? 1 : 0; // �л� LED ״̬
                    TIM_SetCompare1(TIM1, (led_state == 0) ? 0 : arr / 2);
                }
                if (alarm_timer >= 5000) // 5 ���ת��Ϊ������ģʽ
                {                  
                    alarm_timer = 0; // ���ñ�����ʱ��
                    breath_timer = 0; // ���ú����Ƽ�ʱ��
                    state = STATE_1; // ת��Ϊ������ģʽ
                }
                break;

            case STATE_1: // ������ģʽ

                if (breath_timer >= 20) // ���������Ƶ��ٶ�
                {
                	alarm_timer = 0; // ���ñ�����ʱ��
                    breath_timer = 0;
                    static u16 pulse_width = 0;// ����һ����̬�������ڴ洢�����ȣ�������ֵ�ں�������֮��
                    static s16 direction = 1;// ����һ����̬�������ڿ��������ȵ����ӻ���ٷ��򣬳�ʼΪ����
                    pulse_width += direction;// ���ݵ�ǰ�������������

                    if (pulse_width >= 50) direction = -1;// ��������ȴﵽ���ֵ����ı䷽��Ϊ����
                    if (pulse_width <= 0) direction = 1;// ��������ȴﵽ��Сֵ����ı䷽��Ϊ����

                   	TIM_SetCompare1(TIM1, pulse_width);// ���¶�ʱ�� TIM1 �ıȽ�ֵ���Ե��������Ƶ����Ȼ�������
                }
                break;

            default:
                // ��ͬ״̬�������
                // ...
                break;
        }
    }
    else // ��ͨģʽ
    {
        // ������ͨģʽ�µ� LED ��Ϊ
        // ...
    }
}

//�ж��Ƿ񱨾�
void AlarmHandler(u16 value)
{

	//static u8 state = STATE_0;// 0: ������˸, 1: ������
    if (value != 0)
    {
        alarm_active = 1; // ��������ģʽ
		state = STATE_0;// ����LED���ƺ���
    }
    else
    {
        alarm_active = 0; // �رձ���ģʽ
        alarm_timer = 0; // ���ñ�����ʱ��
        TIM_SetCompare1(TIM1, 0);// ����رձ���ģʽ���߼�
        // ����ͨģʽ�� LED ��Ϊ�Ĵ���
    }
}


//ָʾ�ƿ����������֮���л�
void LED_Control1(u16 arr)
{
    // ���ݱ���״̬���� LED ��Ϊģʽ
    if (alarm_active) // ����������ܱ�����
    {
    	static u8 state1 = STATE1_0;// 0: ������˸, 1: ������
        // ���ݵ�ǰģʽ������ LED
        switch (state1)
        {
            case STATE1_0: // ������˸ģʽ
                if (blink_timer >= 500) // �л�Ƶ��Ϊ 500ms
                {	
                	
                    blink_timer = 0;
                    led_state = (led_state == 0) ? 1 : 0; // �л� LED ״̬
                    TIM_SetCompare1(TIM1, (led_state == 0) ? 0 : arr / 2);
					alarm_timer = 0; // ���ñ�����ʱ��
                }
				
                if (alarm_timer >= 5000) // 5 ���ת��Ϊ������ģʽ
                {                  
                    alarm_timer = 0; // ���ñ�����ʱ��
                    breath_timer = 0; // ���ú����Ƽ�ʱ��
                    state = STATE_1; // ת��Ϊ������ģʽ
                }
                break;

            case STATE1_1: // ������ģʽ

                if (breath_timer >= 20) // ���������Ƶ��ٶ�
                {
                	alarm_timer = 0; // ���ñ�����ʱ��
                    breath_timer = 0;
                    static u16 pulse_width = 0;
                    static s16 direction = 1;
                    pulse_width += direction;

                    if (pulse_width >= 50) direction = -1;
                    if (pulse_width <= 0) direction = 1;

                   	TIM_SetCompare1(TIM1, pulse_width);
                }
                break;

            default:
                // ��ͬ״̬�������
                // ...
                break;
        }
    }
    else // ��ͨģʽ
    {
        // ������ͨģʽ�µ� LED ��Ϊ
        // ...
    }
}

//�ж��Ƿ񱨾�
void AlarmHandler1(u16 value)
{

	//static u8 state1 = STATE1_0;// 0: ������˸, 1: ������
    if (value != 0)
    {
        alarm_active = 1; // ��������ģʽ
		LED_Control1(720);// ����LED���ƺ���
    }
    else
    {
        alarm_active = 0; // �رձ���ģʽ
        alarm_timer = 0; // ���ñ�����ʱ��
        TIM_SetCompare1(TIM1, 0);// ����رձ���ģʽ���߼�
        // ����ͨģʽ�� LED ��Ϊ�Ĵ���
    }
}

