#include "LPC17xx.h"
//通过外部中断实现LED等闪烁。
//通过文档我们发现外部中断主要有EINT0,EINT1,EINT2,EINT3,在模块连接寄存器中有描述。
//其中设置SEL01第二功能表示为中断输入、输出；他们分别对应的引脚为p2.10,p2.11,p2.12,p2.13
//外部中断过程，1.选择引脚为EINT中断模式，设置中断模式EXTMODE和中断位置EXTPOLAR，同事设置GPIO为
//什么中断，这里我们选择IO2IntEnF，即下降沿中断。最后通过NVIC打开使能中断，允许中断。
//在中断处理函数中，需要LPC_SC->EXTINT清除中断，然后NVIC挂起的中断清除，最后才是自身的业务代码。

uint32_t currpos = 0;

//具体函数，受限引脚初始化
void LEDInit(){//引脚初始化，我们需要将p20~p2.7设置为输出，p2.10设置为EINT0模式
	//选择并设置
	LPC_PINCON -> PINSEL4 = 0x00100000;	//设置了个引脚的功能，
	//选择中断模式
	LPC_SC -> EXTMODE = 1;//设置中断模式为边沿中断
	LPC_SC -> EXTPOLAR = 0;//设置为下降沿触发
	LPC_GPIOINT -> IO2IntEnF = 1<<10;//设置为使能下降沿触发中断。
	//打开中断
	NVIC_EnableIRQ(EINT0_IRQn);//通过NVIC打开使能中断
	//设置LED等为输出，并初始化为低电平
	LPC_GPIO2 -> FIODIR |= 0x000000FF;//设立必须使用|=，防止与上面冲突。
	LPC_GPIO2 -> FIOCLR |= 0x000000FF;
}
//处理函数
void EINT0_IRQHandler(){
	uint32_t pos = 1 << currpos;
//清除中断
	LPC_SC -> EXTINT = 1;
	//处理自己的逻辑
	//点亮选中的等
	LPC_GPIO2 -> FIOSET |= pos;
	LPC_GPIO2 -> FIOCLR &= ~pos;
	currpos++;
	currpos %= 8;
}
//主函数
int main(){
	SystemInit();//系统初始化
	LEDInit();//引脚功能初始化
	while(1);//等待外部中断输入。
}
