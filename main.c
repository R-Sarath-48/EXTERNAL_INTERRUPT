

#include <stdint.h>

uint32_t volatile *AHB1ENR = (uint32_t *)0x40023830;
uint32_t volatile *APB2ENR = (uint32_t *)0x40023844;
uint32_t volatile *GPIOA_MODER = (uint32_t *)0x40020000;
uint32_t volatile *GPIOA_ODR = (uint32_t *)0x40020014;
uint32_t volatile *GPIOC_MODER = (uint32_t *)0x40020800;
uint32_t volatile *SYSCFG_EXTICR4 = (uint32_t *)0x40013814;
uint32_t volatile *EXTI_FTSR = (uint32_t *)0x40013c0c;
uint32_t volatile *EXTI_IMR = (uint32_t *)0x40013c00;
uint32_t volatile *NVIC_ISER1 = (uint32_t *)0xe000e104;
uint32_t volatile *EXTI_PR = (uint32_t *)0x40013c14;

void Button_init(void)
{

    *AHB1ENR |= (1 << 0) | (1 << 2); // PORT C & PORT A
    *GPIOA_MODER &= ~(3 << (5 * 2));
    *GPIOA_MODER |= (1 << (5 * 2));   // SET PORTA PIN 10,11 AS OUTPUT MODE
    *GPIOC_MODER &= ~(3 << (13 * 2)); // PORT C PIN 13 AND 14 AS INPUT MODE

    *APB2ENR |= (1 << 14);          // ENABLE SYSCNFG
    *SYSCFG_EXTICR4 &= ~(0xF << 4); // Clear bits 7:4
    *SYSCFG_EXTICR4 |= (0x2 << 4);  // Write 0010 (Port C)
    *EXTI_FTSR |= (1 << 13);        // ENBALE PIN13 FOR FALLING EDGE
    *EXTI_IMR |= (1 << 13);         // FOR EXTERNAL INTERRUPT
    *NVIC_ISER1 |= (1 << 8);        // ENABLING BIT 40 BECAUSE THE POSITION OF PORT-C
                             // PIN 10-15 IS IN IN VECTOR TABEL IS 40
}
int main()
{
    Button_init();
    while (1)
    {
    }
}

void EXTI15_10_IRQHandler(void)
{

    if (*EXTI_PR & (1 << 13))
    {

        *EXTI_PR |= (1 << 13);  // CLEARING THE PENDING FLAG
        *GPIOA_ODR ^= (1 << 5); // TOGGLE THE LED
    }
}
