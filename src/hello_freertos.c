#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#define LED_PIN 25

void hello_task(void *params) {
    while (1) {
        printf("Hello, from FreeRTOS\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void led_blink_task(void *params) {
    while (1) {
        gpio_put(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(375));
        gpio_put(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(375));
    }
}

int main() {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Give the tasks a core affinity if SMP is enabled
    #if ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 )
    TaskHandle_t hello_handle, led_handle;

    xTaskCreate(hello_task, "HelloTask", 256, NULL, 1, &hello_handle);
    xTaskCreate(led_blink_task, "BlinkTask", 256, NULL, 1, &led_handle);

    // Pin tasks to different cores
    vTaskCoreAffinitySet(hello_handle, (1 << 0)); // Core 0
    vTaskCoreAffinitySet(led_handle, (1 << 1));   // Core 1

    #else
    xTaskCreate(hello_task, "HelloTask", 256, NULL, 1, NULL);
    xTaskCreate(led_blink_task, "BlinkTask", 256, NULL, 1, NULL);
    #endif

    vTaskStartScheduler();

    while (1); // should never reach here
    return 0;
}