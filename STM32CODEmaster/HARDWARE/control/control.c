#include "control.h"
#include "motor.h"
//#include "BPSim.h"
#include "uart_handler.h"
#include "gpio.h"

extern void Turn_Right(void);
extern void Turn_Left(void);

float Position_KP=2,Position_KI=0,Position_KD=0;  			//X����PID����
float Position_Kp=2,Position_Ki=-0,Position_Kd=0;  			//Y����PID����
extern uint8_t Temperature_limit;
extern uint16_t Distance;
extern float Temperature,skin_temp;
extern double data1[],data[];
extern unsigned char ucRxFinish;
extern float BP_in[2],BP_out[1];
extern float	Temperature;
int ci=5;
int di=5;

void Control_Loop_A(void)													//ģʽһ�����²���
{ 
	int i;
	
	MLX90614_ReadObjectTemperature(&Temperature);      
//	BP_in[0] = Temperature;
//	BP_in[1] = Distance;
//	sim(BP_in, BP_out);
	skin_temp = human_temp(Temperature);
	if(skin_temp > Temperature_limit && Temperature_limit != 0)
	{
		Beep();
	}
	Laser_on();
	Get_Value(&ucRxFinish);
	Screnn_trans_float(0,skin_temp);		//��Ļ��ʾ�¶�
	uartx_printf(huart1,"����%.2f   ���� %dmm\n",skin_temp,Distance);
}

void Control_Loop_B(void)     													//ģʽ������������¶� ����              
{
	int i;

	Laser_on();
	Get_Value(&ucRxFinish);
	MLX90614_ReadObjectTemperature(&Temperature);
//	BP_in[0] = Temperature;
//	BP_in[1] = Distance;
//	sim(BP_in, BP_out);
	if(BP_out[0] > Temperature_limit && Temperature_limit != 0)
	{
		Beep();
	}
	Screnn_trans_float(0,BP_out[0]);
//	Screnn_trans_int(0,BP_out[0]);		//��Ļ��ʾ�¶�
//	uartx_printf(huart1,"temp%.2f   temp_pre:%.1f\n",Temperature,BP_out[0]);
//	uartx_printf(huart1,"��������¶�%.2f   ���� %dmm\n",Temperature,Distance);
}

void Control_Loop_C(void)											//ģʽ��������ʶ��⣬��ʾ��ӦID
{
	extern int ID,learn;
	Screnn_trans_int(6,ID);
	if(ci!=0)
	{
	    Camera_entering();
		ci--;
	}
	//Camera_entering();
	if(learn == 1)
	{
		learn = 0;
		Camera_Learn();
	}
	if(ID == 1 )
	{
		//Beep();
		ID = 0;
	}
	Laser_off();
	di=5;
	//Camera_temperature(skin_temp);
	MLX90614_ReadObjectTemperature(&Temperature);
//	BP_in[0] = Temperature;
//	BP_in[1] = Distance;
//	sim(BP_in, BP_out);
	skin_temp = human_temp(BP_out[0]);
	skin_temp = human_temp(Temperature);
	Get_Value(&ucRxFinish);
}

void Control_Loop_D(void)											//ģʽ�ģ�ʶ���Ƿ��������
{
	extern uint16_t mask;
	if(di!=0)
	{
	    Camera_normal();
		di--;
	}
	//Camera_normal();
	MLX90614_ReadObjectTemperature(&Temperature);
	BP_in[0] = Temperature;
	BP_in[1] = Distance;
//	sim(BP_in, BP_out);
//	skin_temp = human_temp(BP_out[0]);
	skin_temp = human_temp(Temperature);
	//Camera_temperature(skin_temp);
	Get_Value(&ucRxFinish);
	Screnn_trans_float(0,skin_temp);		//��Ļ��ʾ�¶�
	Laser_off();
	ci=5;
	if(mask == 1)
	{
		Screnn_trans_char(8,"��");
		mask = 0;
	}
	else Screnn_trans_char(8,"��");
}

float human_temp(float temp)
{
	float body_temp;
	if(temp <= 32)
	{
		body_temp = temp + 2.4;
	}
	else if(temp > 32 && temp <= 35)
	{
		body_temp = temp * 0.85 + 7.266;
	}
	else if(temp > 35 && temp <= 36.4)
	{
		body_temp = temp * 0.75 + 10.815;
	}
	else if(temp > 36.4)
	{
		body_temp = temp + 1.5;
	}
	return body_temp;
}
