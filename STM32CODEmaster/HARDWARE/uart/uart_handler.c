#include "uart_handler.h"
#include "string.h"
#include "usart.h"
#include "BPSim.h"

extern double data1[],data2[],data[];

void Camera_Get(uint8_t	RxBuffer[])
{
	static unsigned char ucRxCnt = 0, i = 0;
	extern int recive_flag,mask,ID;
	extern char Uart_get[20];
	extern char mask_flag[5],face_flag[5];
	
	Uart_get[ucRxCnt++] = RxBuffer[0];
	
	if(Uart_get[0] == 'W')										//ͷУ��λW
	{
		
		if(Uart_get[ucRxCnt-1] == 'X')					//βУ��λX
		{
			recive_flag = 1;
				switch(Uart_get[1])
			{
				case 'M':														//֡ͷM:�Ƿ��������
					for(i = 2; i < ucRxCnt-1; i++)
				{
						mask_flag[i-2] = Uart_get[i];
				}
				mask = atoi(mask_flag);break;
				case 'F':														//֡ͷF:�Ƿ��⵽������ID=0��������ID=1���˵�δ¼�룬ID=2.....��¼��
					for(i = 2; i < ucRxCnt-1; i++)
				{
						face_flag[i-2] = Uart_get[i];
				}
				ID = atoi(face_flag);break;
			}
//			uartx_printf(huart1,"mask:%d  face:%d",mask,ID);
			ucRxCnt = 0;
		}
	}
	else ucRxCnt = 0;
}

void Screen_get(uint8_t	RxBuffer[])
{
	static int i = 0;
	extern uint8_t screen_get[10];
	extern char get_val[8],get_id[8],get_mode[8];
	extern unsigned char ucRxcnt;
	extern uint8_t Temperature_limit;
	extern int mode,learn;
	
	screen_get[ucRxcnt++] = RxBuffer[0];
		if(screen_get[0] == 'W')										//ͷУ��λW
		{
			if(screen_get[ucRxcnt-1] == 'X')					//βУ��λX
			{
				switch(screen_get[1])
			{
				case 'T':
					for(i = 2; i < ucRxcnt-1; i++)
					{
							get_val[i-2] = screen_get[i];
					}
					Temperature_limit = atoi(get_val);		//�¶���ֵ
					break;
					
				case 'L':
					learn = 1;break;
				case 'F':														//�������ID
					for(i = 2; i < ucRxcnt-1; i++)
					{
							get_id[i-2] = screen_get[i];
					}break;

				case 'M':														//ģʽѡ��
					
					for(i = 2; i < ucRxcnt-1; i++)
					{
						get_mode[i-2] = screen_get[i];
					}
					mode = atoi(get_mode);		//ģʽѡ��1Ϊ������ģʽ��2Ϊ�������¶�ģʽ��3Ϊ����ʶ��ģʽ��4Ϊ����ģʽ
					if( mode == 3)
					{
						
					}
					else if( mode == 4)
					{
						
					}
					break;
			}
			for(i = 0; i < 8; i++)
			{
				get_val[i] = 0;
				get_mode[i] = 0;
			}
			
			ucRxcnt = 0;
			}
		}
		else ucRxcnt = 0;
}

void Camera_entering(void)				//����ʶ��ģʽѡ��
{
	uartx_printf(huart4,"WFX\n");
}

void Camera_Learn(void)						//��ʼѧϰ
{
	uartx_printf(huart4,"WCX\n");
}

void Camera_normal(void)					//�л�������ģʽ
{
	uartx_printf(huart4,"WNX\n");
}

void Camera_temperature(float temp)	//��K210���¶�
{
	uartx_printf(huart4,"WT%dX\n",(int)temp);
}

void Screnn_trans_float(int value,float screen_tx)
{
	uartx_printf(huart3,"t%d.txt=\"%.1f\"",value,screen_tx);
	uartx_printf(huart3,"%c",'\xff');
	uartx_printf(huart3,"%c",'\xff');
	uartx_printf(huart3,"%c",'\xff');
}

void Screnn_trans_int(int value,int screen_tx)
{
	uartx_printf(huart3,"t%d.txt=\"%d\"",value,screen_tx);
	uartx_printf(huart3,"%c",'\xff');
	uartx_printf(huart3,"%c",'\xff');
	uartx_printf(huart3,"%c",'\xff');
}

void Screnn_trans_char(uint8_t value,char screen_tx[])
{
	uartx_printf(huart3,"t%d.txt=\"%s\"",value,screen_tx);
	uartx_printf(huart3,"%c",'\xff');
	uartx_printf(huart3,"%c",'\xff');
	uartx_printf(huart3,"%c",'\xff');
}

void Get_Value(unsigned char* ucFlag)//��ȡVL53L0����
{
	extern unsigned char ucRxData[100];
	char dis_rec[4];
	unsigned char *p;
	extern uint16_t Distance;
	int i=0;
	
		if(*ucFlag==1)
		{
			p=(unsigned char*)strstr((char*)ucRxData,"d:");
			
			while(*p!='m')
			{
				if(*p>='0'&&*p<='9')
				{
					
					dis_rec[i] = *p;
					i++;
				}
				p++;
			}
			Distance = atoi(dis_rec)*0.853-8.035;
			i = 0;
			*ucFlag=0;
	 }
}
