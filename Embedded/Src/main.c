// main.c or a dedicated button_task.c file for STM32F769 Discovery Board

#include "main.h" // Includes HAL and other necessary headers
#include "cmsis_os.h" // FreeRTOS header

// Define the GPIO pin for the User Button (B1 on STM32F769 Discovery)
// Typically PA0 for the user button on Discovery boards
#define USER_BUTTON_GPIO_PORT GPIOA
#define USER_BUTTON_GPIO_PIN GPIO_PIN_0

// Global flag to indicate button press, accessed by Qt C++ backend
// In a real application, this would be protected by a mutex or semaphore
// and ideally communicated via a more robust IPC mechanism for Qt.
volatile uint8_t buttonPressedFlag = 0;

// FreeRTOS Task Handle
osThreadId_t buttonMonitorTaskHandle;

// Forward declarations
void MX_GPIO_Init(void);
void StartButtonMonitorTask(void *argument);

// --- HAL GPIO Initialization (Conceptual, typically done by CubeMX) ---
void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIOA clock
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure User Button pin PA0 as input with falling edge interrupt
    GPIO_InitStruct.Pin = USER_BUTTON_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // Trigger on button press (falling edge)
    GPIO_InitStruct.Pull = GPIO_PULLUP;          // Enable internal pull-up resistor
    HAL_GPIO_Init(USER_BUTTON_GPIO_PORT, &GPIO_InitStruct);

    // Enable and set EXTI Line0 Interrupt priority
    // EXTI0_IRQn corresponds to PA0, PB0, etc.
    HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0); // Adjust priority as needed
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

// --- EXTI Line0 Interrupt Service Routine (ISR) ---
// This function is called when the button is pressed (falling edge detected)
void EXTI0_IRQHandler(void)
{
    // Check if the interrupt source is EXTI Line0
    if (__HAL_GPIO_EXTI_GET_IT(USER_BUTTON_GPIO_PIN) != RESET)
    {
        // Clear the EXTI line pending bit
        __HAL_GPIO_EXTI_CLEAR_IT(USER_BUTTON_GPIO_PIN);

        // Set the flag to indicate button press
        // This is a critical section if accessed by multiple tasks/ISRs
        // For simplicity, using a volatile flag here. In FreeRTOS, you might
        // use a binary semaphore or a queue to signal a task.
        buttonPressedFlag = 1;

        // Optionally, if signaling a FreeRTOS task directly from ISR:
        // BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);
        // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// --- FreeRTOS Button Monitoring Task ---
void StartButtonMonitorTask(void *argument)
{
    // Initialize GPIO for the user button
    MX_GPIO_Init();

    // Infinite loop for the FreeRTOS task
    for (;;)
    {
        // Check the button press flag
        if (buttonPressedFlag == 1)
        {
            // Button was pressed, clear the flag
            buttonPressedFlag = 0;

            // --- Here you would signal the Qt application ---
            // This is conceptual. In a real system, this might involve:
            // 1. Updating a shared memory region that Qt polls.
            // 2. Sending a message over a communication interface (e.g., UART, SPI, custom IPC).
            // 3. A custom event mechanism if Qt for MCUs provides one for inter-process/thread comms.
            // For this example, assume Qt's C++ backend can read `buttonPressedFlag`
            // and react to its change.
            //
            // Example: Print to a debug console (if available)
            // printf("User button pressed!\r\n");
        }

        // Delay to prevent busy-waiting and allow other tasks to run
        osDelay(10); // Check every 10ms
    }
}

// --- FreeRTOS Initialization (Conceptual, typically in main.c) ---
// This would be called from main() after HAL_Init()
void FreeRTOS_Init(void)
{
    // Create the button monitoring task
    const osThreadAttr_t buttonMonitorTask_attributes = {
        .name = "ButtonMonitorTask",
        .stack_size = 128 * 4, // Adjust stack size as needed
        .priority = (osPriority_t)osPriorityNormal,
    };
    buttonMonitorTaskHandle = osThreadNew(StartButtonMonitorTask, NULL, &buttonMonitorTask_attributes);

    // Start the scheduler (if not already started)
    // osKernelStart();
}

/*
// Example main function structure (conceptual)
int main(void)
{
    HAL_Init();
    SystemClock_Config(); // Configure system clock

    FreeRTOS_Init(); // Initialize and start FreeRTOS tasks

    // Start scheduler if not started in FreeRTOS_Init
    osKernelStart();

    // Should not reach here if scheduler starts successfully
    while (1)
    {
        // Background loop or low-priority tasks if scheduler allows
    }
}
*/
