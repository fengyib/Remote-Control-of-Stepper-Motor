#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "pca9685.h"
#include <stdio.h>
 int main(void)
 {	
	u8 t;
	u16 len;	
	u8 res;
	char  b;
	 int c;
	u16 times=0; 
	 u16 receivedData;
	 u16 j;
 
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ� 
  KEY_Init();	
	
			IIC_Init();//IIC��ʼ��
      pca_write(pca_mode1,0x0);//д��ַ
			pca_setfreq(330);
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n");
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				c=USART_RX_BUF[t];
				printf("\r\n res��������Ϊ: %d\r\n", c);
				while((USART1->SR&0X40)==0);//�ȴ����ͽ���
			}
			
			if(c == 49)
			{
				LED0=!LED0;
				pca_setpwm(1,0,0);
				pca_setpwm(0,0,500);
				
			}
			else if(c==50)
			{
				pca_setpwm(1,0,4000);
				pca_setpwm(0,0,500);
			}
			else if(c==51)
			{
				LED1=!LED1;
				pca_setpwm(0,0,0);
			}
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				printf("\r\nALIENTEK MiniSTM32������ ����ʵ��\r\n");
			}
			if(times%200==0)printf("����������,�Իس�������\r\n");  
			delay_ms(10);   
		}
	}	 
	
}


