#include "adc_drv.h"

u16 data[2]; // data[0] 用来保存 ADC1_CH11 数据（火焰），data[1] 用来保存 ADC1_CH12 数据（烟雾）

//int ADC_Init(void)
//{
//    //使能外部时钟
//    rcu_periph_clock_enable(RCU_GPIOC);
//    rcu_periph_clock_enable(RCU_AF);
//    rcu_periph_clock_enable(RCU_ADC0);
//
//    //设置ADC时钟分频
//    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);
//
//    //GPIO Init
//    gpio_init(GPIOC,GPIO_MODE_AIN,GPIO_OSPEED_50MHZ,GPIO_PIN_0);
//
//    //ADC Init
//    adc_deinit(ADC0);
//    adc_mode_config(ADC_MODE_FREE);//ADC工作模式
//    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);//数据对齐方式 右对齐
//    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE); // 连续转换
//    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1); // 通道数
//    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); // 软件触发
//
//    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE); // 使能外部触发
//
//    adc_enable(ADC0); // 使能ADC
//
//    Delay_ms(1); // 等待稳定
//
//    adc_calibration_enable(ADC0); // 使能复位校准和ADC校准
//
//}
//
//
//uint16_t GetADC_Val(void)
//{
//    uint16_t data = 0;
//    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_10, ADC_SAMPLETIME_239POINT5); // 设置采样周期, 采样顺序和通道
//    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // 使能软件触发转换
//
//    // 等待转换完成
//    while (adc_flag_get(ADC0, ADC_FLAG_EOC) != SET);
//    adc_flag_clear(ADC0, ADC_FLAG_EOC);
//
//    // 获取转换完成的数据
//    data = adc_regular_data_read(ADC0);
//
//    return data;
//}

/*
 *@berief:adc初始化配置
 *@param:无
 *@retval:无
 */

// ADC 配置
void ADC_MyConfig(void)
{
    // GPIO 初始化
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_1 | GPIO_PIN_2);  // 配置 PC1 和 PC2 为模拟输入，用于火焰和烟雾传感器

    // 启用 ADC 和 GPIO 时钟
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_ADC1);

    // 配置 ADC 分频
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);  // 设置 ADC 时钟分频系数为 PCLK2/6

    // ADC 初始化
    adc_deinit(ADC1);
    adc_mode_config(ADC_MODE_FREE);
    adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);
    adc_channel_length_config(ADC1, ADC_REGULAR_CHANNEL, 2);  // 通道数量为2
    adc_external_trigger_source_config(ADC1, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, ENABLE);

    // 启用连续转换模式
    adc_special_function_config(ADC1, ADC_CONTINUOUS_MODE, ENABLE);

    // 启用扫描模式
    adc_special_function_config(ADC1, ADC_SCAN_MODE, ENABLE);

    // 配置 ADC 采样时间和通道
    adc_regular_channel_config(ADC1, 0, ADC_CHANNEL_11, ADC_SAMPLETIME_239POINT5);  // 火焰传感器
    adc_regular_channel_config(ADC1, 1, ADC_CHANNEL_12, ADC_SAMPLETIME_239POINT5);  // 烟雾传感器

    // 使能 ADC
    adc_enable(ADC1);
    delay_ms(1);  // 等待 ADC 稳定

    // 复位校准
    adc_calibration_enable(ADC1);

    // 软件启动 ADC 转换
    adc_software_trigger_enable(ADC1, ADC_REGULAR_CHANNEL);

    // DMA 初始化
    DMA_ADC_Config();

    // 启用 ADC DMA 请求
    adc_dma_mode_enable(ADC1);
}


// DMA 初始化
void DMA_ADC_Config(void)
{
    dma_parameter_struct dma_init_struct;

    // 启用 DMA 时钟
    rcu_periph_clock_enable(RCU_DMA0);

    // 初始化 DMA
    dma_deinit(DMA0, DMA_CH0);
    dma_init_struct.periph_addr = (uint32_t)&ADC_RDATA(ADC1);  // ADC 数据寄存器地址
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;  // 外设地址不自增
    dma_init_struct.memory_addr = (uint32_t)data;  // 存储数据的内存地址
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;  // 内存地址自增
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;  // 外设数据宽度16位
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;  // 内存数据宽度16位
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;  // 数据方向：外设到内存
    dma_init_struct.number = 2;  // 传输数据个数
    dma_init_struct.priority = DMA_PRIORITY_MEDIUM;  // DMA优先级

    // 设置 DMA 模式为循环模式
    dma_init(DMA0, DMA_CH0, &dma_init_struct);

    // 启用 DMA 通道
    dma_channel_enable(DMA0, DMA_CH0);
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



