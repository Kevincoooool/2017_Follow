#include "MedianFiler.h"


unsigned int measure_num = 4;
static int Filter_data[10] = {0};
static int Filterdata1[5] = {0};
static int Filterdata2[5] = {0};
/*************中位值滤波***************/
int Median_filer1(int data)
{
  unsigned int i = 0;
	unsigned int j = 0;

	int temp;
  Filterdata1[measure_num-1] = data;
	for(i=0;i<measure_num-1;i++)
	{
	 Filterdata1[i]=Filterdata1[i+1];
	}
	for(i = 0 ; i < measure_num-1 ; i++)
	{
			for(j = 0 ; j < measure_num-1-i; j++)
			{
					if(Filterdata1[j] > Filterdata1[j+1] )
					{
							temp = Filterdata1[j];
							Filterdata1[j] =  Filterdata1[j+1];
							Filterdata1[j+1] = temp;
					}
			}
	}
	return Filterdata1[(measure_num-1)/2];
}

int Median_filer2(int data)
{
  unsigned int i = 0;
	unsigned int j = 0;

	int temp;
  Filterdata2[measure_num-1] = data;
	for(i=0;i<measure_num-1;i++)
	{
	 Filterdata2[i]=Filterdata2[i+1];
	}
	for(i = 0 ; i < measure_num-1 ; i++)
	{
			for(j = 0 ; j < measure_num-1-i; j++)
			{
					if(Filterdata2[j] > Filterdata2[j+1] )
					{
							temp = Filterdata2[j];
							Filterdata2[j] =  Filterdata2[j+1];
							Filterdata2[j+1] = temp;
					}
			}
	}
	return Filterdata2[(measure_num-1)/2];
}
/*************中位值平均滤波***************/
float MedianAverage_filer(int data)
{
  unsigned int i = 0;
	unsigned int j = 0;
	unsigned int MAX_error_targe = 0;
	int MAX_error1;
	int temp;
  Filter_data[measure_num-1] = data;
	for(i=0;i<measure_num-1;i++)
	{
	 Filter_data[i]=Filter_data[i+1];
	}
	for(i = 0 ; i < measure_num-1 ; i++)
	{
			for(j = 0 ; j < measure_num-1-i; j++)
			{
					if(Filter_data[j] > Filter_data[j+1] )
					{
							temp = Filter_data[j];
							Filter_data[j] =  Filter_data[j+1];
							Filter_data[j+1] = temp;
					}
			}
	}
	MAX_error1 = Filter_data[1] - Filter_data[0];
	for(i = 1 ; i < measure_num-1 ; i++)
	{
			if(MAX_error1 < Filter_data[i+1] - Filter_data[i] )
			{
					MAX_error1 =  Filter_data[i+1] - Filter_data[i];
					MAX_error_targe = i; 
			}
	}
	float Average_data=0;
	if(MAX_error_targe+1 > (measure_num+1)/2)
	{
			for(i = 0 ; i <= MAX_error_targe ; i++)
			{
					Average_data += Filter_data[i];
			}
			Average_data /= (MAX_error_targe+1);
	}
	else
	{
			for(i = MAX_error_targe + 1 ; i < measure_num ; i++)
			{
					Average_data += Filter_data[i];
			}
			Average_data /= (measure_num - MAX_error_targe -1);
	}
	return Average_data;
}





