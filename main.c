#include "stm32f7xx.h"                  // Device header
volatile uint16_t i;

void uart2_write(unsigned char x)
{

USART2->TDR =(x);
while(!((USART2->ISR)&0x40)){;}
}

int uart2_available(void)
{
if((USART2->ISR)&0x20){return 1;}
else {return 0;}

}
unsigned char uart2_read(void)
{
return USART2->RDR;
}


void delay(int delayms){
int i;
	for(; delayms>0;delayms--){
	for(i=0;i<3192;i++);
	}
}


void led_play(int value)
{
	value %=16; 
	for (;value>0;value--){
	GPIOA->BSRR|=(1<<5); //turn on led
	delay(100);
	GPIOA->BSRR|=(1<<(5+16)); //turn off the led 
	delay(100);
	}
	delay(400);
}

int main(void)
{
SystemCoreClockUpdate();
	
RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;  // clock to AHBENR Port A and PA2 and PA3
RCC->APB1ENR 	|= RCC_APB1ENR_USART2EN; // clock to USART2
GPIOA->MODER 	|=0xA0 ; // PA2 and PA3 AF - PA5 Normal output
GPIOA->MODER	|=0x400;
GPIOA->OSPEEDR =0xFFFFFFF;
GPIOA->AFR[0] |= 0x7700; // PA2 and PA3 Set AF7
USART2->BRR    = 0x008B; // baud rate 9600
USART2->CR1    = 0; // Enable Tx and Rx and Enable USART2
USART2->CR1|=USART_CR1_TE;
USART2->CR1|=USART_CR1_RE;	
USART2->CR1|=USART_CR1_UE;
 while(1)
{

if(uart2_available())
		{
		char c=uart2_read();
			led_play(c);
			uart2_write(c);
			uart2_write('\r');
			uart2_write('\n');
		}

}
}

