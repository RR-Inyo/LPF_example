/*
   LPF_example.ino
   A low-pass filter example test, my first digital signal processing program
   (c) 2021 @RR_Inyo
   Released under the MIT lisence
   https://opensource.org/licenses/mit-license.php
*/

#include <M5Stack.h>
#include "dFilt.hpp"

// Constants (example)
const int T_SAMPLE_US = 8000;
const float OMEGA_N1 = 5;
const float ZETA2 = 0.2;
const float OMEGA_N2 = 12;

// Handlers for task and timer
TaskHandle_t taskHandle;
hw_timer_t *timer = NULL;

// Function prototypes
void IRAM_ATTR onTimer();
void taskPeriodical(void *pvParameters);
void plotGraph(float u, float y1, float y2);

// Define LPF instances, first- and second-order
FirstOrderLPF lpf1(T_SAMPLE_US / 1e6, OMEGA_N1);
SecondOrderLPF lpf2(T_SAMPLE_US / 1e6, ZETA2, OMEGA_N2);

/*
   The setup function, run once
*/
void setup() {
  M5.begin();
  M5.Power.begin();

  // Creat a task to be executed periodically by signal from ISR
  xTaskCreateUniversal(
    taskPeriodical,
    "taskPeriodical",
    8192,    
    NULL,
    1,
    &taskHandle,
    APP_CPU_NUM
  );

  // Set up timer and attach ISR
  timer = timerBegin(0, 80, true); // Count up every microsecond
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, T_SAMPLE_US, true);
  timerAlarmEnable(timer);
}

/*
   The loop function, run repeadedly
*/
void loop() {
  // Nothing to do
  delay(10);
}

// ISR, sending a signal to actual task
void IRAM_ATTR onTimer() {
  BaseType_t taskWoken;
  xTaskNotifyFromISR(taskHandle, 0, eIncrement, &taskWoken);
}

// A task periodically executed by signal from ISR
void taskPeriodical(void *pvParameters) {
  uint32_t ulNotifiedValue;

  float u, y1, y2;
  while (true) {
    xTaskNotifyWait(0, 0, &ulNotifiedValue, portMAX_DELAY);
    M5.update();
    if (M5.BtnA.read()) {
      u = 1.0;
    } else {
      u = 0.0;
    }

    // Apply LPFs, first-order and second-order
    y1 = lpf1.apply(u);
    y2 = lpf2.apply(u);
    plotGraph(u, y1, y2);
  }
}

// A function to plot graph on LCD
void plotGraph(float u, float y1, float y2) {

  // Variables for LCD control
  static int x = 0;
  static int x_old = 0;
  static int yu = 0;
  static int yu_old = 110;
  static int yy1 = 0;
  static int yy1_old = 219;
  static int yy2 = 0;
  static int yy2_old = 219;

  // Clear screen to begin plotting
  if (x == 0) {
    x_old = 0;
    M5.Lcd.fillScreen(BLACK);
  }

  // Map the y-axis values
  yu = map(int(u * 90), 0, 90, 110, 20);
  yy1 = map(int(y1 * 90), 0, 90, 219, 129);
  yy2 = map(int(y2 * 90), 0, 90, 219, 129);

  M5.Lcd.drawLine(x_old, yu_old, x, yu, YELLOW);
  M5.Lcd.drawLine(x_old, yy1_old, x, yy1, PINK);
  M5.Lcd.drawLine(x_old, yy2_old, x, yy2, CYAN);

  // Preserve current values
  x_old = x;
  yu_old = yu;
  yy1_old = yy1;
  yy2_old = yy2;

  // Increment x-axis
  x = (x + 1) % 319;
}
