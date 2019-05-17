/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技MK66FX1M0VLQ18核心板
【编    写】CHIUSIR
【备    注】
【软件版本】V1.0
【最后更新】2016年08月20日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
vuint16 ADV[128]={0,0};         //声明数组，用于存放采集的线性数值
vuint16 LCDD[128]={0,0};        //转换为LCD显示的数值

vint16 piancha=0;

#define TSL_SI   PTE3_OUT    //定义线性传感器的端口 SI
#define TSL_CLK  PTE2_OUT    //定义线性传感器的端口 CLK

//---------------PID参数------------------------------------------------
float err=0.0; //定义偏差值  
float err_next=0.0; //定义上一个偏差值   
float err_last=0.0; //定义最上前的偏差值   
float Kp=14.55,Ki=6.00,Kd=2.0; //定义比例、积分、微分系数   ；；；27；；；6；；；


/*   
2m:17.55,6.00,2.0   
5m:14.55,6.00,2.0   
*/   
float incrementSpeed; //增长速度  
float ActualSpeed=0.0;//实际速度
float index;
float integral; //定义积分值
float voltage; //定义电压值（控制执行器的变量）
float pa;  
//----------------------------------------------------------------------
float E[100]={0};
//-------------------------------------------------------------------------*
//函数名: void Init_TSL1401(void)                                                     
//功  能: 初始化TSL1401                                                  
//参  数: 无                                                                               
//返  回: 无                                                              
//简  例: Init_TSL1401() ;              
//-------------------------------------------------------------------------*
void Init_TSL1401(void)
{
  GPIO_Init(PTE,3,GPO,0); //定义线性传感器的端口 SI
  GPIO_Init(PTE,2,GPO,0); //定义线性传感器的端口 CLK
  ADC_Init(ADC_0);        //AD口初始化               
  ADC_Start(ADC_0,ADC0_SE16,ADC_12bit);   //AD口初始化   
}

//-------------------------------------------------------------------------*
//函数名: void Dly_us(int us)                                                    
//功  能: 短暂延时                                                    
//参  数: us 相当于微秒级别的延时                                                                               
//返  回: 无                                                              
//简  例: Dly_us(1) ;              
//-------------------------------------------------------------------------*
//-------------------------------------------------------------------------*
void Dly_us(int us)
{   
   uint16 i,j;    
   for(i=0;i<us;i++)
   {
     for(j=0;j<500;j++);  
   } 
}
//-------------------------------------------------------------------------*
//函数名: void Read_TSL1401(void)                                                  
//功  能: 读取摄像头数据                                                     
//参  数: 无                                                                               
//返  回: 无                                                              
//简  例: Read_TSL1401() ;              
//-------------------------------------------------------------------------*
void Read_TSL1401(void) 
{
  u8 i=0,tslp=0;
  
  TSL_CLK=1;//起始电平高 
  TSL_SI=0; //起始电平低
  Dly_us(1); //合理的延时
      
  TSL_SI=1; //上升沿
  TSL_CLK=0;//下降沿
  Dly_us(1); //合理延时
      
  TSL_CLK=1;//上升沿
  TSL_SI=0; //下降沿
  Dly_us(1); //合理延时      
  for(i=0;i<64;i++)
  { 
    TSL_CLK=0;//下降沿    
    Dly_us(8-i/8+1); //合理延时    
    ADV[tslp]=ADC_Ave(ADC_0,ADC0_SE16,ADC_12bit,10);;  //AD采集
    ++tslp;
    TSL_CLK=1;//上升沿 
    Dly_us(8-i/8+1); //合理延时    
  }
  for(i=0;i<64;i++)
  { 
    TSL_CLK=0;//下降沿    
    Dly_us(i/8+1); //合理延时   
    ADV[tslp]=ADC_Ave(ADC_0,ADC0_SE16,ADC_12bit,10);;  //AD采集
    ++tslp;
    TSL_CLK=1;//上升沿 
    Dly_us(i/8+1); //合理延时    
  }    
}
//-------------------------------------------------------------------------*
//函数名: void Show_TSL1401(void)                                                       
//功  能: 在OLED屏幕上显示数值                                                     
//参  数: 无                                                                               
//返  回: 无                                                              
//简  例: Show_TSL1401() ;              
//-------------------------------------------------------------------------*
float Show_TSL1401(void) 
{    
  u16 i=0,maxv=0,minv=0xFFFF;
  u16 evv=0,tslp=0;
  u32 addall=0;
  int num_L=0,num_M=0,num_R,num=0;//左个数，右个数,白个数
  char txt[16]="";   
  for(i=0;i<128;i++)
  { 
    addall+=ADV[i];           
  }
  evv=addall/128;    

  for(i=0;i<128;i++)
  {     
    if(minv>ADV[i])  minv=ADV[i];
    if(maxv<ADV[i])  maxv=ADV[i];               
  }

  sprintf(txt,"%04d %04d %04d",maxv,minv,evv);
  LCD_P6x8Str(20,6,(uint8*)txt);
 
  LCD_Set_Pos(0,7);    
  for(i=0;i<128;i++)  //显示黑色边界
  {   
    if(ADV[i]<= evv-evv/5)
    {  
      tslp=0xfe; //白色   
      num++;//统计白的个数
    }
    else
    {
      num_L=num;
      tslp=0x80; //黑色
      num_M++;//统计黑的个数
    }      
    LCD_WrDat(tslp);           
  }
  num_R=128-num_L-num_M;

  err=num_L-num_R;//左右两边的误差
  incrementSpeed=Kp*(err-err_next)+Ki*err+Kd*(err-2*err_next+err_last);
  err_last=err_next;
  err_next=err;
  for (int i=100;i>0;i--)
  {
    E[i]=err;
  }
  if (E[99]>-3 && E[99]<3)
  {
    LED_Ctrl(LED0,ON);
  }
  else
  {
    LED_Ctrl(LEDALL,OFF);
  }
  
  sprintf(txt,"%04d,%04d,%04d",num_L,num_M,num_R);
  LCD_P6x8Str(5,3,(uint8*)txt);

  sprintf(txt,"err: %5.2f",err);//打印err
  LCD_P6x8Str(5,2,(uint8*)txt);

  sprintf(txt,"inS: %5.2f",incrementSpeed);//打印incrementSpeed
  LCD_P6x8Str(5,1,(uint8*)txt);

  return incrementSpeed;
  //return voltage;
}
