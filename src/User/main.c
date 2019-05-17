/****************************************************************************************************
【平    台】龙邱K66FX智能车VC母板
【编    写】CHIUSIR
【E-mail  】chiusir@aliyun.com
【软件版本】V1.0
【最后更新】2016年08月24日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR7.3
【Target  】K66FX1M0VLQ18
【Crystal 】 50.000Mhz
【busclock】 40.000MHz
【pllclock】200.000MHz
=============================================================
呼吸灯实现，PWM占空比连续变化  
-------------------------------
LED测试GPIO
LED1--PTA17
LED2--PTC0
LED3--PTD15 
LED4--PTE26
------------------------------------------------------------------------------------------------------
PWM口的定义FTM.H中
#define FTM3_CH6    PTC10       // PTE11、PTC10
#define FTM3_CH7    PTC11       // PTE12、PTC11
=============================================================
修改历史：
******************************************************************************************************/

#include "include.h" 

//float Kp,Ki,Kd;
//主函数



void main(void)
{
   int chose=0;
   u16 duty=7500;
   char  txt[16]="X:";
   float d;
   DisableInterrupts;        //关闭总中断
   PLL_Init(PLL200);         //初始化PLL为200M，总线为40MHZ   
   LED_Init();               //LED初始化
   LED_Ctrl(LEDALL, OFF);    //LED全亮  
   LCD_Init();               
   Draw_LQLogo();            //OLED显示LOGO
   LCD_CLS();
   Init_TSL1401(); 
   KEY_Init();               //按键初始化
   //频率：40M/(2^3)/50000=100HZ,每个脉冲为0.2us 
   FTM_PWM_Init(FTM3,FTM_CH6,50000,7500);//
   FTM_PWM_Init(FTM3,FTM_CH7,50000,7500);//
   while(1)
   {    
     Read_TSL1401();         
     d=Show_TSL1401();
     
     duty-=d;
     
    if(!KEY_Read(KEY1))           //如果KEY1按下
    {
      if(chose==1){
        Kp+=0.05;
      }else if(chose==2){
        Ki+=0.05;
      }else if(chose==3){
        Kd+=0.05;    
      }
      //if(duty<12500) duty+=100;       
    }
    else if(!KEY_Read(KEY2))     //如果KEY2按下
    {
      if(chose==1){
        Kp-=0.05;
      }else if(chose==2){
        Ki-=0.05;
      }else if(chose==3){
        Kd-=0.05;    
      }
      //if(duty>2500)  duty-=100;       
    }
    else if(!KEY_Read(KEY0))     //如果KEY0按下
    {
      chose++;
      if(chose==4){
        chose=1;
      };
      if(chose==1){
        sprintf(txt," ");
        LCD_P6x8Str(64,4,(uint8*)txt); 
        LCD_P6x8Str(0,5,(uint8*)txt);
        sprintf(txt,"*");
        LCD_P6x8Str(0,4,(uint8*)txt);
      }else if(chose==2){
        sprintf(txt," ");
        LCD_P6x8Str(0,4,(uint8*)txt); 
        LCD_P6x8Str(0,5,(uint8*)txt);
        sprintf(txt,"*");
        LCD_P6x8Str(64,4,(uint8*)txt);
      }else if(chose==3){
        sprintf(txt," ");
        LCD_P6x8Str(64,4,(uint8*)txt); 
        LCD_P6x8Str(0,4,(uint8*)txt);
        sprintf(txt,"*");
        LCD_P6x8Str(0,5,(uint8*)txt);      
      }
      //duty=7500;       
    }
     FTM_PWM_Duty(FTM3,FTM_CH6,duty);
     FTM_PWM_Duty(FTM3,FTM_CH7,duty);
    
     sprintf(txt,"duty:%5.2f ",duty);
     LCD_P6x8Str(5,0,(uint8*)txt);

     sprintf(txt,"kp:%5.2f ",Kp);
     LCD_P6x8Str(9,4,(uint8*)txt);

     sprintf(txt,"ki:%5.2f ",Ki);
     LCD_P6x8Str(73,4,(uint8*)txt);

     sprintf(txt,"kd:%5.2f ",Kd);
     LCD_P6x8Str(9,5,(uint8*)txt);
   }
}

