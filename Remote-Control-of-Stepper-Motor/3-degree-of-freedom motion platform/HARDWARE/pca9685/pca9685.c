#include "pca9685.h"
#include "myiic.h"
#include "delay.h"
#include "math.h"

void pca_write(u8 adrr,u8 data)//向PCA写数据,adrrd地址,data数据
{ 
        IIC_Start();
        
        IIC_Send_Byte(pca_adrr);
        IIC_Wait_Ack();
        
        IIC_Send_Byte(adrr);
        IIC_Wait_Ack();
        
        IIC_Send_Byte(data);
        IIC_Wait_Ack();
        
        IIC_Stop();
}

u8 pca_read(u8 adrr)//从PCA读数据
{
        u8 data;
        IIC_Start();
        
        IIC_Send_Byte(pca_adrr);
        IIC_Wait_Ack();
        
        IIC_Send_Byte(adrr);
        IIC_Wait_Ack();
        
        IIC_Start();
        
        IIC_Send_Byte(pca_adrr|0x01);
        IIC_Wait_Ack();
        
        data=IIC_Read_Byte(0);
        IIC_Stop();
        
        return data;
}


void pca_setfreq(float freq)//设置PWM频率
{
                u8 prescale,oldmode,newmode;
                double prescaleval;
                freq *= 0.92; 
                prescaleval = 25000000;
                prescaleval /= 4096;
                prescaleval /= freq;
                prescaleval -= 1;
                prescale =floor(prescaleval + 0.5f);

                oldmode = pca_read(pca_mode1);//pca_mode1 0x0
        
                newmode = (oldmode&0x7F) | 0x10; // sleep  0x0&0x7f=0x0  0x0|0x10=0x10
        
                pca_write(pca_mode1, newmode); // go to sleep  D4sleep写1
        
                pca_write(pca_pre, prescale); // set the prescaler 
								//pca_pre 0xFE控制周期的寄存器，将计算过的预装载值放入
        
                pca_write(pca_mode1, oldmode);//oldmode 0x0,D4写0退出sleep模式
                delay_ms(2);
        
                pca_write(pca_mode1, oldmode | 0xa1); 
								//0x0|0xa1=10100001 D6使用内部时钟，D5内部地址读写后自动增加，D0响应0x70通用i2c地址
}

void pca_setpwm(u8 num, u32 on, u32 off)
{
                pca_write(LED0_ON_L+4*num,on);//
                pca_write(LED0_ON_H+4*num,on>>8);
                pca_write(LED0_OFF_L+4*num,off);
                pca_write(LED0_OFF_H+4*num,off>>8);
}
/*num:舵机PWM输出引脚0~15，on:PWM上升计数值0~4096,off:PWM下降计数值0~4096
一个PWM周期分成4096份，由0开始+1计数，计到on时跳变为高电平，继续计数到off时
跳变为低电平，直到计满4096重新开始。所以当on不等于0时可作延时,当on等于0时，
off/4096的值就是PWM的占空比。*/

void Thumb(void)//拇指伸展，缓慢增长，易于看到效果
{
	u16 i=0;
	for(i=800;i<=1622;i++)
	{
		pca_setpwm(1,0,i);
					delay_ms(3);
	}
}

void Index(void)//食指伸展
	{
	u16 i=0;
	for(i=675;i<=2200;i++)
	{
		pca_setpwm(2,0,i);
					delay_ms(3);
	}
}
	
	void Middle(void)//中指伸展
{
	u16 i=0;
	for(i=675;i<=2800;i++)
	{
		pca_setpwm(15,0,i);
					delay_ms(3);
	}
}

