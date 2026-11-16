#include <Arduino.h>
#include <DShotRMT.h>

static constexpr dshot_mode_t DSHOT_MODE = DSHOT300;
static constexpr uint32_t MOTOR_UPDATE_RATE = 1000;
static constexpr int NUMBER_OF_MOTORS = 4;

DShotRMT motors[NUMBER_OF_MOTORS] = {
    DShotRMT(GPIO_NUM_27, DSHOT_MODE),
    DShotRMT(GPIO_NUM_26, DSHOT_MODE),
    DShotRMT(GPIO_NUM_25, DSHOT_MODE),
    DShotRMT(GPIO_NUM_33, DSHOT_MODE)
};

//no lock needed as 32-bit floats are atomic on ESP32
volatile float throttle[NUMBER_OF_MOTORS] = {0, 0, 0, 0};

hw_timer_t *motor_timer = nullptr;
TaskHandle_t motor_task_handle = nullptr;

void IRAM_ATTR onMotorTimer() {
    BaseType_t higher_prio_woken = pdFALSE;
    vTaskNotifyGiveFromISR(motor_task_handle, &higher_prio_woken);
    portYIELD_FROM_ISR(higher_prio_woken);
}

void sendMotor(DShotRMT &motor, float throttle_percent) {
    if (throttle_percent <= 0.0f) {
        motor.sendThrottle(0);
    }
    else {
        throttle_percent = constrain(throttle_percent, 0.0f, 100.0f);
        motor.sendThrottlePercent(throttle_percent);
    }
}

void motorTask(void *) {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        for (int i = 0; i < NUMBER_OF_MOTORS; i++) {
            sendMotor(motors[i], throttle[i]);
        }
    }
}


void setup() {
    Serial.begin(115200);
    delay(200);

    for (int i = 0; i < NUMBER_OF_MOTORS; i++) {
        dshot_result_t result = motors[i].begin();
        if (result.success == DSHOT_OK) {
            Serial.println("Failed to initialize motor");
        }
    }

    //motor task on core 1
    xTaskCreatePinnedToCore(motorTask, "Motor Task", 4096, nullptr, configMAX_PRIORITIES - 1, &motor_task_handle, 1);

    motor_timer = timerBegin(1000000);
    timerAttachInterrupt(motor_timer, &onMotorTimer);
    timerAlarm(motor_timer, 1000000 / MOTOR_UPDATE_RATE, true, 0);
}

void loop() {
    delay(4000);
    throttle[0] = 50.0f;
    delay(4000);
    throttle[1] = 50.0f;
    delay(4000);
    throttle[2] = 50.0f;
    delay(4000);
    throttle[3] = 50.0f;

}