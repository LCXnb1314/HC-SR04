#include "ultrasonic.h"
#include "delay.h"

/**
	*@brief	������ģ���ʼ��
	*@param	��
	*@retval	��
	*/
void Ultrasonic_Init(void)
{
	//ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	//PC7��ʼ����Trig�ܽţ�
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	
	//PC6��ʼ����Echo�ܽţ�
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	
	//��ʱ��4��ʼ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=1999;//���������ĵ�2000������1s��ÿ��һ��0.5ms
	TIM_TimeBaseInitStructure.TIM_Prescaler=35999;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	
}


/**
	*@brief	������ģ����
	*@param	��
	*@retval	����
	*���Ծ���=���ߵ�ƽʱ��*���٣�340m/s����/2
	*/
float Ultrasonic_Distance(void)
{
	u8 i;
	u16 Num=0,Temp;
	float Distance;
	for(i=0;i<5;i++)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		GPIO_ResetBits(GPIOC,GPIO_Pin_6);
		TIM_SetCounter(TIM4,0);
		
		GPIO_SetBits(GPIOC,GPIO_Pin_7);
		delay_us(40);
		GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)==0);
		TIM_Cmd(TIM4,ENABLE);
		while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)==1);
		Temp=TIM_GetCounter(TIM4);
		TIM_Cmd(TIM4,DISABLE);
		TIM_SetCounter(TIM4,0);
		
		Num+=Temp;	
	}
	Distance=((Num/5)*0.0005*Speed)/2;
	return Distance;
}

