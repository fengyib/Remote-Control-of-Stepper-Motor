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
 
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(9600);	 //串口初始化为9600
	LED_Init();		  	 //初始化与LED连接的硬件接口 
  KEY_Init();	
	
			IIC_Init();//IIC初始化
      pca_write(pca_mode1,0x0);//写地址
			pca_setfreq(330);
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n");
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				c=USART_RX_BUF[t];
				printf("\r\n res接受数据为: %d\r\n", c);
				while((USART1->SR&0X40)==0);//等待发送结束
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
				printf("\r\nALIENTEK MiniSTM32开发板 串口实验\r\n");
			}
			if(times%200==0)printf("请输入数据,以回车键结束\r\n");  
			delay_ms(10);   
		}
	}	 
	
}


