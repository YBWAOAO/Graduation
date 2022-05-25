#include "BPSim.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "string.h"
#include "usart.h"

//ȫ�ֱ�������
static double data[] = {4.00000000,2.00000000,8.00000000,3.00000000,1.00000000,29.60000038,48.20000076,29.60000038,490.00000000,29.89999962,47.00000000,0.14902520,-0.38118072,0.14290883,-0.60647868,-0.76311993,1.37861364,-0.51985786,0.38187296,-0.00789541,-0.34967933,-0.15219167,-0.39146336,-0.03188758,-0.34719482,-0.03089574,-0.28804445,-0.24869389,0.63852759,-0.32544158,0.57463111,-0.47877324,-0.68749310,-0.33816767,0.00951445,-0.28094045,-0.44546777,0.12186440,-0.34922082,-0.49269746,0.08405929,-0.39612356,-0.82871620,0.48879087,0.60926118,-0.94130017,-0.69048532,0.12271732,-0.18701233,-0.73291994,0.43759654,-0.46172317,-0.24725700,-0.54702679,-0.28349253,-0.23834119,-0.07690911,0.47333783,-0.25059352,0.19092557,0.69554302,1.88965516,-0.25769211,1.11505658,-0.23950928,-0.61961172,0.43137870};
int layer_all1 = 0;
int *nodes_all1 = NULL;
int num_all = 0;
struct simnetnode **simnetwork=NULL;
float simxminmax[SIMROW_X][2];
float simyminmax[SIMROW_Y][2];

//������ڵ㽨������
int simNetSetUp(void)
{
	int i = 0,j=0,k=0,n=0;
	//���������
	layer_all1 = (int)data[n];
	n++;

	//������ÿ��ڵ���
	nodes_all1 = (int *)malloc(sizeof(int)*layer_all1);
	for (i = 0; i < layer_all1; i++)
	{
		nodes_all1[i] = data[n];
		n++;
	}
	//�������
	simnetwork = (struct simnetnode**)malloc(sizeof(struct simnetnode*)*(layer_all1));
	//����������ÿ��ڵ�
	for (i = 0; i < layer_all1; i++)
	{
		simnetwork[i] = (struct simnetnode*)malloc(sizeof(struct simnetnode)*nodes_all1[i]);
	}

	//ӳ�����
	for (i = 0; i < SIMROW_X; i++)
	{
		simxminmax[i][0] = data[n];
		n++;
		simxminmax[i][1] = data[n];
		n++;
	}
	for (i = 0; i < SIMROW_Y; i++)
	{
		simyminmax[i][0] = data[n];
		n++;
		simyminmax[i][1] = data[n];
		n++;
	}

	//������ڵ������ʼ��
	for (i = 0; i < layer_all1; i++)
	{
		for (j = 0; j < nodes_all1[i]; j++)
		{
			if (i != layer_all1 - 1)
			{
				simnetwork[i][j].w = (double*)malloc(sizeof(double)*nodes_all1[i + 1]);
				for (k = 0; k < nodes_all1[i + 1]; k++)
				{
					simnetwork[i][j].w[k] = data[n];
					n++;
				}

				simnetwork[i][j].b = data[n];
				n++;
			}
		}
	}
	return 0;

}

int sim(float x[SIMROW_X], float y[SIMROW_Y])
{
	if ((SIMROW_X != nodes_all1[0]) && (SIMROW_Y != nodes_all1[layer_all1 - 1]))
	{
		printf("������������������ȷ\n");
		return 1;
	}
	int i = 0, j = 0, k = 0;
	for (i = 0; i < layer_all1; i++)
	{
		//����������
		if (i == 0)
		{
			for (j = 0; j < nodes_all1[i]; j++)
			{
				simnetwork[i][j].input = simDataChange(x[j],j,1);
				simnetwork[i][j].output = simInspirit(simnetwork[i][j].input, simnetwork[i][j].b, 1);
			}
		}
		else
		{
			float s = 0;
			for (j = 0; j < nodes_all1[i]; j++)
			{
				//����
				s = 0;
				for (k = 0; k < nodes_all1[i - 1]; k++)
				{
					s += (simnetwork[i-1][k].output*simnetwork[i-1][k].w[j]);
				}
				simnetwork[i][j].input = s;
				if (i<(layer_all1-1))
				    simnetwork[i][j].output = simInspirit(simnetwork[i][j].input, simnetwork[i][j].b, 1);
				else
				{
					simnetwork[i][j].output = simnetwork[i][j].input;
					y[j] = simDataRecover(simnetwork[i][j].output,j,1); 
				}
			}
		}
	}
}

//mode1==0,����������sig��
//y=1/(1+exp(x))
//mode1==1,��������(����tansig)
//y=2/(1+e(-2x))-1
double simInspirit(double x, double b, int mode1)
{
	double y = 0;
	x -= b;
	if (mode1 == 0)
	{
		y = 1 + exp(-x);
		y = 1 / y;
	}
	else if (mode1 == 1)
	{
		y = 1 + exp(-2 * x);
		y = 2 / y - 1;
	}
	return y;
}

//int readData(void)
//{
//	FILE *fpread = fopen("data.txt", "r");
//	if (fpread == NULL)
//	{
//		return 1;
//	}
//	double k = 0;
//	num_all = 0;
//	while (fscanf(fpread, "%lf", &k) != EOF)
//		num_all++;
//	fclose(fpread);

//	int i = 0;
//	data = (double*)malloc(num_all*sizeof(double));
//	fpread = fopen("data.txt", "r");
//	while (fscanf(fpread, "%lf", &data[i]) != EOF)
//	{
//		i++;
//	}
//	return 0;
//}

int simInit()
{

//	if (readData())
//	{
//		printf("��ȡ����\n");
//		return 1;
//	}
	if (simNetSetUp())
	{
		uartx_printf(huart1,"�����罨������\n");
		return 2;
	}
	return 0;
}

//����ӳ��
double simDataChange(double x,int j,int mode1)
{
	float dis = simxminmax[j][1] - simxminmax[j][0];
	x -= simxminmax[j][0];
	x /= dis;
	if (mode1 == 1)
	{
		x *= 2;
		x -= 1;
	}
	return x;
}

//���ݻָ�
double simDataRecover(double y, int j,int mode1)
{
	float dis = simyminmax[j][1] - simyminmax[j][0];
	if (mode1 == 1)
	//ѹ����0-1����
	{
		y += 1;
		y /= 2;
	}
	y *= dis;
	y += simyminmax[j][0];
	return y;
}

int simFree(void)
{
	int i = 0, j = 0;
	//�ͷ�data
	free(data);
	*data = NULL;

	for (i = 0; i < layer_all1; i++)
	{
		for (j = 0; j < nodes_all1[i]; j++)
		{
			if (i != layer_all1 - 1)
			{
					free(simnetwork[i][j].w);
					simnetwork[i][j].w = NULL;
			}

		}
		free(simnetwork[i]);
		simnetwork[i] = NULL;
	}
	free(simnetwork);
	simnetwork = NULL;
	free(nodes_all1);
	nodes_all1 = NULL;
	return 0;
}
