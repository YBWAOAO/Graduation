#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"
#include "mlx90614.h"
#include "stdio.h"
#include "motor.h"
#include "linefinder.h"

extern int speed_set;
void Control_Loop_A(void);     						//ģʽһ�����²���    
void Control_Loop_B(void);								//ģʽ����ˮ�²���
void Control_Loop_C(void);								//����ʶ�����
void Control_Loop_D(void);								//ʶ���Ƿ��������
float human_temp(float temp);

#endif 
