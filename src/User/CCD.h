#ifndef __CCD_H__
#define __CCD_H__
extern vuint16 ADV[128];         //�������飬���ڴ�Ųɼ���������ֵ
extern vuint16 LCDD[128];        //ת��ΪLCD��ʾ����ֵ
extern vint16  piancha;

extern void Init_TSL1401(void);
extern void Read_TSL1401(void);
extern float Show_TSL1401(void);
extern float Kp,Ki,Kd;
#endif