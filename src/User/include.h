/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�MK66FX1M0VLQ18���İ�
����    д��CHIUSIR
����    ע��
�������汾��V1.0
�������¡�2016��08��20��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
���������䡿chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef INCLUDE_H_
#define INCLUDE_H_

//ͨ��ͷ�ļ�
    #include    <stdio.h>                       //printf
    #include    <string.h>                      //memcpy
    #include    <stdlib.h>                      //malloc

//Cortex-M�ں�MCU�Ĵ���ͷ�ļ�
    #include "MK66F18.h"   //�Ĵ���ӳ��ͷ�ļ�
    #include "arm_math.h "
    #include "Systick.h"

//MCU�ڲ�ģ��������ͷ�ļ�
    #include "GPIO.h"
    #include "GPIO_Cfg.h"
    #include "PLL.h"
    #include "FTM.h"    
    #include "UART.h"
    #include "ADC.h"
    #include "PLL.h"    
    #include "PIT.h"
    #include "I2C.h"
    //#include "SPI.h"
    #include "DMA.h"
    #include "Lptmr.h"    
    #include "RTC.h"

//�ж����������жϺ�������
    #include "vectors.h"

//�������ͼ��˿������ض���
    #include "common.h"

//�ⲿ�豸���Զ��幦��������ͷ�ļ�
    //#include "LQ9AX.h"
    #include "LQLED.h"
    #include "LQKEY.h"
    #include "OV7620.h"
    //#include "LQSERVO.h"
    #include "LQ12864.h"
    //#include "LQnRF24L01.h"
    #include "CCD.h"


#endif