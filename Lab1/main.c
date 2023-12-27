#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

// Variant 9: Output on pin A1, pulse width (tau) = 120/2, period (T) = 4
void delayL1(int millisL1);
void initL1(void);

int main(void)
{
    // Initialize the configuration for LED L1
    initL1();

    while (1)
    {
        // Turn on LED L1 (GPIO Pin A1)
        GPIOA->ODR |= GPIO_Pin_1; 
        // Delay for the specified pulse width
        delayL1(1200000);
        
        // Turn off LED L1 (GPIO Pin A1)
        GPIOA->ODR &= ~GPIO_Pin_1; 
        // Delay for the specified period
        delayL1(20000000);
    }
}

// Function to introduce a delay based on the specified milliseconds
void delayL1(int millisL1) {
    for(int i = 0; i < millisL1 * 0x010000; i++);
}

// Function to initialize LED L1 on GPIO Pin A1
void initL1(void)
{
    // Initialize GPIO configuration structure
    GPIO_InitTypeDef GPIO_InitS;
    
    // Enable clock for GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // Configure GPIO Pin A1 for output
    GPIO_InitS.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitS.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitS.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitS);
    
    // Initially set the pin high (LED off)
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}
