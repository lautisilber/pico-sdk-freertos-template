#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pwm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LED_PIN 25
#define PWM_WRAP 255

// Struct to hold task parameters
typedef struct {
    const char* name;
    uint32_t delay_ms;
} print_task_params_t;

void print_task(void *params) {
    print_task_params_t *cfg = (print_task_params_t *)params;
    while (1) {
        printf("%s running on core %u\n", cfg->name, get_core_num());
        vTaskDelay(pdMS_TO_TICKS(cfg->delay_ms));
    }
}

void led_fade_task(void *params) {
    // Set up PWM
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LED_PIN);
    pwm_set_wrap(slice_num, PWM_WRAP);
    pwm_set_enabled(slice_num, true);

    int brightness = 0;
    int direction = 1; // 1 = increasing, -1 = decreasing

    while (1) {
        pwm_set_gpio_level(LED_PIN, brightness);
        brightness += direction;
        if (brightness >= PWM_WRAP || brightness <= 0) {
            direction = -direction;
        }
        vTaskDelay(pdMS_TO_TICKS(5)); // adjust for fade speed
    }
}

int main() {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Define task parameters
    static print_task_params_t print1_cfg = { .name = "Print Task 1", .delay_ms = 2000 };
    static print_task_params_t print2_cfg = { .name = "Print Task 2", .delay_ms = 1300 };

    // Task handles
    TaskHandle_t handle1, handle2, fade_handle;

    // Create tasks
    xTaskCreate(print_task, "Print1", 256, &print1_cfg, 2, &handle1);
    xTaskCreate(print_task, "Print2", 256, &print2_cfg, 2, &handle2);
    xTaskCreate(led_fade_task, "Fade", 256, NULL, 1, &fade_handle);

    // Pin print_task_1 to core 0
    vTaskCoreAffinitySet(handle1, (1 << 0));
    // Pin print_task_2 to core 1
    vTaskCoreAffinitySet(handle2, (1 << 1));
    // Let the LED task run on either core
    vTaskCoreAffinitySet(fade_handle, (1 << 0) | (1 << 1));

    vTaskStartScheduler();

    while (1); // should never reach here
    return 0;
}