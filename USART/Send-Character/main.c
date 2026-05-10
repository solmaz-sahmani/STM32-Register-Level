#include "stm32g431xx.h"

#define SYSCLK      16000000
#define PCLK1       SYSCLK

static void Brr_Compute (uint32_t baudrate)
{
    USART2->BRR = (PCLK1 + (baudrate/2U))/baudrate;
}

void USART2_SendChar(char c)
{
    while (!(USART2->ISR & (1U << 7))); // WHILE TXE IS 0 WAITE, WHEN TXE IS 1 WRITE c IN TDR
    
    USART2->TDR = c;
}

void USART2_Init(void)
{
    //ENABLE CLOCK FOR GPIOA & USART2
    RCC->AHB2ENR |= (1U << 0);
    RCC->APB1ENR1 |= (1U << 17);

    //SELECT MODE FOR PA2
    GPIOA->MODER &= ~(0x3 << 4); // CLEAR PIN
    GPIOA->MODER |= (0x2 << 4); // AF MODE

    // SELECT AF7
    GPIOA->AFR[0] &= ~(0xF << 8); //CLEAR PIN
    GPIOA->AFR[0] |= (0x7 << 8); // SELECT AF 7 FOR PA2

        //PUSH PULL
    GPIOA->OTYPER &= ~(0x1 << 2);

    //GPIO SPEED
    GPIOA->OSPEEDR |= (0x3 << 4);

    //PULLUP
    GPIOA->PUPDR &= ~(0x3 << 4); // CLEAR BITS
    GPIOA->PUPDR |= (0x1 << 4);

    //SET USART BAUDRATE
    Brr_Compute(9600);

    //ENABLE TRANSMITTER
    USART2->CR1 |= (0x1 << 3);

    //ENABLE RECEIVER
    USART2->CR1 |= (0x1 << 2);

    //ENABLE USART
    USART2->CR1 |= (0x1 << 0);
}

int main (void)
{
    USART2_Init();

    while (1)
    {
        USART2_SendChar('S');

        for (volatile int i = 0; i < 100000; i++);
        
    }
    
}
