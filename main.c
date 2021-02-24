#include "stm32f7xx.h"                  // Device header
//send data ti TX 
void uart2_write(unsigned char x)
{

USART2->TDR =(x);
while(!((USART2->ISR)&0x40)){;}
}
//if there is data iin the buffer
int uart2_available(void)
{
if((USART2->ISR)&0x20){return 1;}
else {return 0;}

}
//read the RX buffer
unsigned char uart2_read(void) 
{
return USART2->RDR;
}

//inefficient delay
void delay(int delayms){
int i;
	for(; delayms>0;delayms--){
	for(i=0;i<3192;i++);
	}
}

//blinking led algorithm 
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
GPIOA->MODER	|=0x400; //PA5 as output
GPIOA->OSPEEDR =0xFFFFFFFF; //maximum speed for the entire GPIO port
GPIOA->AFR[0] |= 0x7700; // PA2 and PA3 Set AF7
USART2->BRR    = 0x008B; // baud rate 115200 @16MHz
USART2->CR1    = 0; // Enable Tx and Rx and Enable USART2
USART2->CR1|=USART_CR1_TE; //enbale TX
USART2->CR1|=USART_CR1_RE; //enable RX	
USART2->CR1|=USART_CR1_UE;
 while(1)
{

if(uart2_available()) //iif there is any data in RX-buffer
		{
		char c=uart2_read(); //read the data
			led_play(c);	//bliink the led
			// print the sent character
			uart2_write(c);
			uart2_write('\r');
			uart2_write('\n');
		}

}
}

