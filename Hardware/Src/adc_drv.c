#include "adc_drv.h"

u16 data[2]; // data[0] 用来保存 ADC1_CH11 数据（火焰），data[1] 用来保存 ADC1_CH12 数据（烟雾）

/*
 *@berief:adc初始化配置
 *@param:无
 *@retval:无
 */
void ADC_MyConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	ADC_InitTypeDef  ADC_InitStruct;
		
	// 使能外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1, ENABLE);
	
	// 设置 ADC 的分频系数
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	// 配置 GPIO 初始化（PC1 用于火焰传感器，PC2 用于烟雾传感器）
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2; // 配置 PC1 和 PC2
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	// 配置 ADC 初始化
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; // 配置连续转换模式使能
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; // 数据对齐方式 
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部事件触发
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; // 独立工作模式
	ADC_InitStruct.ADC_NbrOfChannel = 2; // 通道的数量
	ADC_InitStruct.ADC_ScanConvMode = ENABLE; // 扫描模式(多通道模式需要扫描)
	ADC_Init(ADC1, &ADC_InitStruct);

	// 使能 ADC
	ADC_Cmd(ADC1, ENABLE);

	// 校准
	ADC_ResetCalibration(ADC1); // 执行复位校准
	while(ADC_GetResetCalibrationStatus(ADC1)); // 等待复位校准结束
	ADC_StartCalibration(ADC1); // 开始指定 ADC1 的校准状态
	while(ADC_GetCalibrationStatus(ADC1)); // 等待校准结束

	// 配置规则转换通道，采样顺序
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5); // 配置火焰传感器通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_239Cycles5); // 配置烟雾传感器通道
	
	
	// 软件启动 ADC
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	
 	DMA_ADC_Config(); // DMA 初始化
	
	ADC_DMACmd(ADC1, ENABLE); // ADC1 的 DMA 请求
}

/*
 *@berief:DMA初始化配置
 *@param:无
 *@retval:无
 */
void DMA_ADC_Config(void)
{
	DMA_InitTypeDef  DMA_InitStruct;

	// 使能 DMA 时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// 初始化 DMA
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

	// 使能 DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

/*
 *@berief:计算火焰强度
 *@param:无
 *@retval:火焰强度（假设范围为0到100）
 */
float Calculate_Flame_Intensity(void)
{
    float voltage = (data[0] / 4096.0) * 3.3;  // 将 ADC 值转换为电压，假设参考电压为 3.3V
    float intensity;

    // 假设火焰强度与电压的关系为线性（根据传感器手册确定）
    // 火焰强度 = (电压 - V_min) / (V_max - V_min) * 强度最大值
    float V_min = 0.1;  // 最低电压（没有火焰），需要根据实际传感器设定
    float V_max = 3.3;  // 最大电压（火焰强度最大），需要根据实际传感器设定
    float intensity_max = 100.0;  // 火焰强度最大值（比如0到100的百分比）

    if (voltage < V_min) {
        intensity = 0.0;  // 如果电压小于最小值，表示没有火焰
    } else if (voltage > V_max) {
        intensity = intensity_max;  // 如果电压超过最大值，火焰强度达到最大
    } else {
        intensity = ((voltage - V_min) / (V_max - V_min)) * intensity_max;  // 线性插值计算
    }

    return intensity;
}

/*
 *@berief:计算烟雾浓度
 *@param:无
 *@retval:烟雾浓度（ppm）
 */
float Calculate_Smoke_Concentration(void)
{
    float voltage = (data[1] / 4096.0) * 3.3;  // 将 ADC 值转换为电压，假设参考电压为 3.3V
    float concentration;

    // 假设传感器的浓度与电压的关系为线性
    // 根据数据手册确定合适的公式，这里是一个简单示例
    // 例如：浓度(ppm) = (电压 - V_min) / (V_max - V_min) * 浓度最大值
    // V_min 和 V_max 来自于传感器数据手册，ppm_max 是最大浓度
    
    float V_min = 0.1;  // 最低输出电压，假设0.1V时对应0ppm（需要根据实际传感器设定）
    float V_max = 3.3;  // 最大输出电压，假设2.5V时对应最大浓度
    float ppm_max = 1000.0;  // 假设最大烟雾浓度为 1000ppm
    
    if (voltage < V_min) {
        concentration = 0.0;  // 如果电压小于最小值，浓度为0
    } else if (voltage > V_max) {
        concentration = ppm_max;  // 如果电压超过最大值，浓度达到最大
    } else {
        concentration = ((voltage - V_min) / (V_max - V_min)) * ppm_max;  // 线性插值计算
    }

    return concentration;
}



