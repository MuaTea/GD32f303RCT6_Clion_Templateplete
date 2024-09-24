#include "adc_drv.h"

u16 data[2]; // data[0] �������� ADC1_CH11 ���ݣ����棩��data[1] �������� ADC1_CH12 ���ݣ�����

/*
 *@berief:adc��ʼ������
 *@param:��
 *@retval:��
 */
void ADC_MyConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	ADC_InitTypeDef  ADC_InitStruct;
		
	// ʹ������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1, ENABLE);
	
	// ���� ADC �ķ�Ƶϵ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	// ���� GPIO ��ʼ����PC1 ���ڻ��洫������PC2 ��������������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN; // ģ������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2; // ���� PC1 �� PC2
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	// ���� ADC ��ʼ��
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; // ��������ת��ģʽʹ��
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; // ���ݶ��뷽ʽ 
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // �ⲿ�¼�����
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; // ��������ģʽ
	ADC_InitStruct.ADC_NbrOfChannel = 2; // ͨ��������
	ADC_InitStruct.ADC_ScanConvMode = ENABLE; // ɨ��ģʽ(��ͨ��ģʽ��Ҫɨ��)
	ADC_Init(ADC1, &ADC_InitStruct);

	// ʹ�� ADC
	ADC_Cmd(ADC1, ENABLE);

	// У׼
	ADC_ResetCalibration(ADC1); // ִ�и�λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1)); // �ȴ���λУ׼����
	ADC_StartCalibration(ADC1); // ��ʼָ�� ADC1 ��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1)); // �ȴ�У׼����

	// ���ù���ת��ͨ��������˳��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5); // ���û��洫����ͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_239Cycles5); // ������������ͨ��
	
	
	// ������� ADC
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	
 	DMA_ADC_Config(); // DMA ��ʼ��
	
	ADC_DMACmd(ADC1, ENABLE); // ADC1 �� DMA ����
}

/*
 *@berief:DMA��ʼ������
 *@param:��
 *@retval:��
 */
void DMA_ADC_Config(void)
{
	DMA_InitTypeDef  DMA_InitStruct;

	// ʹ�� DMA ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// ��ʼ�� DMA
	DMA_InitStruct.DMA_BufferSize = 2;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)data;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&ADC1-> DR;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);

	// ʹ�� DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
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



