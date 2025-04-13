#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LED_PIN 25

void print_task_1(void *params) {
    while (1) {
        printf("Hello from Task 1 on core %u\n", get_core_num());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void print_task_2(void *params) {
    while (1) {
        printf("Hello from Task 2 on core %u\n", get_core_num());
        vTaskDelay(pdMS_TO_TICKS(1300));
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

    // Handles to set core affinity
    TaskHandle_t handle1, handle2, blink_handle;

    // Create tasks
    xTaskCreate(print_task_1, "Print1", 256, NULL, 1, &handle1);
    xTaskCreate(print_task_2, "Print2", 256, NULL, 1, &handle2);
    xTaskCreate(led_blink_task, "Blink", 256, NULL, 1, &blink_handle);

    // Pin print_task_1 to core 0
    vTaskCoreAffinitySet(handle1, (1 << 0));
    // Pin print_task_2 to core 1
    vTaskCoreAffinitySet(handle2, (1 << 1));
    // Let the LED task run on either core
    vTaskCoreAffinitySet(blink_handle, (1 << 0) | (1 << 1));

    vTaskStartScheduler();

    while (1); // should never reach here
    return 0;
}