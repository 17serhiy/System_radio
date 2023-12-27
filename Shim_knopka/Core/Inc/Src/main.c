#include "main.h"

TIM_HandleTypeDef htim2;  // TIM2 handle structure

GPIO_InitTypeDef GPIO_InitStruct;  // GPIO initialization structure

int buttonState = 0;  // Variable to store the current state of the button
int previousButtonState = 0;  // Variable to store the previous state of the button
int servoPosition = 0;  // Variable to store the servo position (0 - fully retracted, 1 - 90 degrees)

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
void Error_Handler(void);

int main(void) {
  HAL_Init();  // Initialize the Hardware Abstraction Layer
  SystemClock_Config();  // Configure the system clock
  MX_GPIO_Init();  // Initialize GPIO pins
  MX_TIM2_Init();  // Initialize TIM2 for PWM
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);  // Start PWM on TIM2, Channel 1
  
  while (1) {
    buttonState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);  // Read the state of the button
    
    if (buttonState != previousButtonState) {
      if (buttonState == GPIO_PIN_SET) {
        // If the button is pressed
        if (servoPosition == 0) {
          // Smoothly move to 90 degrees
          for (int i = 60; i <= 210; i++) {
            TIM2->CCR1 = i;  // Adjust PWM duty cycle
            HAL_Delay(10);  // Delay for smooth movement
          }
          servoPosition = 1;  // Update the servo position
        } else {
          // Smoothly move to 0 degrees
          for (int i = 210; i >= 60; i--) {
            TIM2->CCR1 = i;  // Adjust PWM duty cycle
            HAL_Delay(10);  // Delay for smooth movement
          }
          servoPosition = 0;  // Update the servo position
        }
      }
    }
    
    previousButtonState = buttonState;  // Update the previous button state
  }
}

// System Clock Configuration
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  HAL_RCC_OscConfig(&RCC_OscInitStruct);  // Configure the system oscillator
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);  // Configure the system clock
}

// TIM2 Initialization
static void MX_TIM2_Init(void) {
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;  // Set the prescaler for TIM2 to achieve a 1MHz clock
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1439;  // Set the period for a 50Hz PWM signal
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  HAL_TIM_Base_Init(&htim2);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

  HAL_TIM_PWM_Init(&htim2);
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
  
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
  HAL_TIM_MspPostInit(&htim2);
}

// GPIO Initialization
static void MX_GPIO_Init(void) {
  __HAL_RCC_GPIOA_CLK_ENABLE();  // Enable the clock for GPIOA
  
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// Error Handler
void Error_Handler(void) {
  __disable_irq();
  while (1) {
    // Error handling routine
  }
}

#ifdef  USE_FULL_ASSERT
// Assert Failed Handler
void assert_failed(uint8_t *file, uint32_t line) {
  // Assert failed handling routine
}
#endif /* USE_FULL_ASSERT */
