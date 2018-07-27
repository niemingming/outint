#include "LPC17xx.h"
//ͨ���ⲿ�ж�ʵ��LED����˸��
//ͨ���ĵ����Ƿ����ⲿ�ж���Ҫ��EINT0,EINT1,EINT2,EINT3,��ģ�����ӼĴ�������������
//��������SEL01�ڶ����ܱ�ʾΪ�ж����롢��������Ƿֱ��Ӧ������Ϊp2.10,p2.11,p2.12,p2.13
//�ⲿ�жϹ��̣�1.ѡ������ΪEINT�ж�ģʽ�������ж�ģʽEXTMODE���ж�λ��EXTPOLAR��ͬ������GPIOΪ
//ʲô�жϣ���������ѡ��IO2IntEnF�����½����жϡ����ͨ��NVIC��ʹ���жϣ������жϡ�
//���жϴ������У���ҪLPC_SC->EXTINT����жϣ�Ȼ��NVIC������ж�����������������ҵ����롣

uint32_t currpos = 0;

//���庯�����������ų�ʼ��
void LEDInit(){//���ų�ʼ����������Ҫ��p20~p2.7����Ϊ�����p2.10����ΪEINT0ģʽ
	//ѡ������
	LPC_PINCON -> PINSEL4 = 0x00100000;	//�����˸����ŵĹ��ܣ�
	//ѡ���ж�ģʽ
	LPC_SC -> EXTMODE = 1;//�����ж�ģʽΪ�����ж�
	LPC_SC -> EXTPOLAR = 0;//����Ϊ�½��ش���
	LPC_GPIOINT -> IO2IntEnF = 1<<10;//����Ϊʹ���½��ش����жϡ�
	//���ж�
	NVIC_EnableIRQ(EINT0_IRQn);//ͨ��NVIC��ʹ���ж�
	//����LED��Ϊ���������ʼ��Ϊ�͵�ƽ
	LPC_GPIO2 -> FIODIR |= 0x000000FF;//��������ʹ��|=����ֹ�������ͻ��
	LPC_GPIO2 -> FIOCLR |= 0x000000FF;
}
//������
void EINT0_IRQHandler(){
	uint32_t pos = 1 << currpos;
//����ж�
	LPC_SC -> EXTINT = 1;
	//�����Լ����߼�
	//����ѡ�еĵ�
	LPC_GPIO2 -> FIOSET |= pos;
	LPC_GPIO2 -> FIOCLR &= ~pos;
	currpos++;
	currpos %= 8;
}
//������
int main(){
	SystemInit();//ϵͳ��ʼ��
	LEDInit();//���Ź��ܳ�ʼ��
	while(1);//�ȴ��ⲿ�ж����롣
}
