#include "adc_drv.h"

u16 data[2]; // data[0] �������� ADC1_CH11 ���ݣ����棩��data[1] �������� ADC1_CH12 ���ݣ�����

//int ADC_Init(void)
//{
//    //ʹ���ⲿʱ��
//    rcu_periph_clock_enable(RCU_GPIOC);
//    rcu_periph_clock_enable(RCU_AF);
//    rcu_periph_clock_enable(RCU_ADC0);
//
//    //����ADCʱ�ӷ�Ƶ
//    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);
//
//    //GPIO Init
//    gpio_init(GPIOC,GPIO_MODE_AIN,GPIO_OSPEED_50MHZ,GPIO_PIN_0);
//
//    //ADC Init
//    adc_deinit(ADC0);
//    adc_mode_config(ADC_MODE_FREE);//ADC����ģʽ
//    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);//���ݶ��뷽ʽ �Ҷ���
//    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE); // ����ת��
//    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1); // ͨ����
//    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); // �������
//
//    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE); // ʹ���ⲿ����
//
//    adc_enable(ADC0); // ʹ��ADC
//
//    Delay_ms(1); // �ȴ��ȶ�
//
//    adc_calibration_enable(ADC0); // ʹ�ܸ�λУ׼��ADCУ׼
//
//}
//
//
//uint16_t GetADC_Val(void)
//{
//    uint16_t data = 0;
//    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_10, ADC_SAMPLETIME_239POINT5); // ���ò�������, ����˳���ͨ��
//    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // ʹ���������ת��
//
//    // �ȴ�ת�����
//    while (adc_flag_get(ADC0, ADC_FLAG_EOC) != SET);
//    adc_flag_clear(ADC0, ADC_FLAG_EOC);
//
//    // ��ȡת����ɵ�����
//    data = adc_regular_data_read(ADC0);
//
//    return data;
//}

/*
 *@berief:adc��ʼ������
 *@param:��
 *@retval:��
 */

// ADC ����
void ADC_MyConfig(void)
{
    // GPIO ��ʼ��
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_1 | GPIO_PIN_2);  // ���� PC1 �� PC2 Ϊģ�����룬���ڻ������������

    // ���� ADC �� GPIO ʱ��
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_ADC1);

    // ���� ADC ��Ƶ
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);  // ���� ADC ʱ�ӷ�Ƶϵ��Ϊ PCLK2/6

    // ADC ��ʼ��
    adc_deinit(ADC1);
    adc_mode_config(ADC_MODE_FREE);
    adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);
    adc_channel_length_config(ADC1, ADC_REGULAR_CHANNEL, 2);  // ͨ������Ϊ2
    adc_external_trigger_source_config(ADC1, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, ENABLE);

    // ��������ת��ģʽ
    adc_special_function_config(ADC1, ADC_CONTINUOUS_MODE, ENABLE);

    // ����ɨ��ģʽ
    adc_special_function_config(ADC1, ADC_SCAN_MODE, ENABLE);

    // ���� ADC ����ʱ���ͨ��
    adc_regular_channel_config(ADC1, 0, ADC_CHANNEL_11, ADC_SAMPLETIME_239POINT5);  // ���洫����
    adc_regular_channel_config(ADC1, 1, ADC_CHANNEL_12, ADC_SAMPLETIME_239POINT5);  // ��������

    // ʹ�� ADC
    adc_enable(ADC1);
    delay_ms(1);  // �ȴ� ADC �ȶ�

    // ��λУ׼
    adc_calibration_enable(ADC1);

    // ������� ADC ת��
    adc_software_trigger_enable(ADC1, ADC_REGULAR_CHANNEL);

    // DMA ��ʼ��
    DMA_ADC_Config();

    // ���� ADC DMA ����
    adc_dma_mode_enable(ADC1);
}


// DMA ��ʼ��
void DMA_ADC_Config(void)
{
    dma_parameter_struct dma_init_struct;

    // ���� DMA ʱ��
    rcu_periph_clock_enable(RCU_DMA0);

    // ��ʼ�� DMA
    dma_deinit(DMA0, DMA_CH0);
    dma_init_struct.periph_addr = (uint32_t)&ADC_RDATA(ADC1);  // ADC ���ݼĴ�����ַ
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;  // �����ַ������
    dma_init_struct.memory_addr = (uint32_t)data;  // �洢���ݵ��ڴ��ַ
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;  // �ڴ��ַ����
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;  // �������ݿ��16λ
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;  // �ڴ����ݿ��16λ
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;  // ���ݷ������赽�ڴ�
    dma_init_struct.number = 2;  // �������ݸ���
    dma_init_struct.priority = DMA_PRIORITY_MEDIUM;  // DMA���ȼ�

    // ���� DMA ģʽΪѭ��ģʽ
    dma_init(DMA0, DMA_CH0, &dma_init_struct);

    // ���� DMA ͨ��
    dma_channel_enable(DMA0, DMA_CH0);
}



/*
 *@berief:�������ǿ��
 *@param:��
 *@retval:����ǿ�ȣ����跶ΧΪ0��100��
 */
float Calculate_Flame_Intensity(void)
{
    float voltage = (data[0] / 4096.0) * 3.3;  // �� ADC ֵת��Ϊ��ѹ������ο���ѹΪ 3.3V
    float intensity;

    // �������ǿ�����ѹ�Ĺ�ϵΪ���ԣ����ݴ������ֲ�ȷ����
    // ����ǿ�� = (��ѹ - V_min) / (V_max - V_min) * ǿ�����ֵ
    float V_min = 0.1;  // ��͵�ѹ��û�л��棩����Ҫ����ʵ�ʴ������趨
    float V_max = 3.3;  // ����ѹ������ǿ����󣩣���Ҫ����ʵ�ʴ������趨
    float intensity_max = 100.0;  // ����ǿ�����ֵ������0��100�İٷֱȣ�

    if (voltage < V_min) {
        intensity = 0.0;  // �����ѹС����Сֵ����ʾû�л���
    } else if (voltage > V_max) {
        intensity = intensity_max;  // �����ѹ�������ֵ������ǿ�ȴﵽ���
    } else {
        intensity = ((voltage - V_min) / (V_max - V_min)) * intensity_max;  // ���Բ�ֵ����
    }

    return intensity;
}

/*
 *@berief:��������Ũ��
 *@param:��
 *@retval:����Ũ�ȣ�ppm��
 */
float Calculate_Smoke_Concentration(void)
{
    float voltage = (data[1] / 4096.0) * 3.3;  // �� ADC ֵת��Ϊ��ѹ������ο���ѹΪ 3.3V
    float concentration;

    // ���贫������Ũ�����ѹ�Ĺ�ϵΪ����
    // ���������ֲ�ȷ�����ʵĹ�ʽ��������һ����ʾ��
    // ���磺Ũ��(ppm) = (��ѹ - V_min) / (V_max - V_min) * Ũ�����ֵ
    // V_min �� V_max �����ڴ����������ֲᣬppm_max �����Ũ��
    
    float V_min = 0.1;  // ��������ѹ������0.1Vʱ��Ӧ0ppm����Ҫ����ʵ�ʴ������趨��
    float V_max = 3.3;  // ��������ѹ������2.5Vʱ��Ӧ���Ũ��
    float ppm_max = 1000.0;  // �����������Ũ��Ϊ 1000ppm
    
    if (voltage < V_min) {
        concentration = 0.0;  // �����ѹС����Сֵ��Ũ��Ϊ0
    } else if (voltage > V_max) {
        concentration = ppm_max;  // �����ѹ�������ֵ��Ũ�ȴﵽ���
    } else {
        concentration = ((voltage - V_min) / (V_max - V_min)) * ppm_max;  // ���Բ�ֵ����
    }

    return concentration;
}



