#define LGFX_AUTODETECT // Autodetect board
#define LGFX_USE_V1     // set to use new version of library

#include <LovyanGFX.hpp> // main library
static LGFX lcd;         // declare display variable

#include <lvgl.h>
#include "lv_conf.h"
#include "assets/ui.h"
#include "freertos/semphr.h"
#include <esp_now.h>
#include <WiFi.h>
#include "data_struct.h"
#include <string.h>
#include <math.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x30, 0xAE, 0xA4, 0xFB, 0x07, 0x00};

// Variable to store if sending data was successful
String success;

incoming_msg_struct incomingMsg;
outgoing_msg_struct outputMsg;

esp_now_peer_info_t peerInfo;

/*** Setup screen resolution for LVGL ***/
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 480;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];
static lv_color_t buf2[screenWidth * 10];
char msg[256];
char boot_text[256];

#define joyLX     32
#define joyLY     33
#define joyRX     34
#define joyRY     35
#define left_btn  26
#define right_btn 27

// Variables for touch x,y
#ifdef DRAW_ON_SCREEN
static int32_t x, y;
#endif

/*** Function declaration ***/
static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);
static void Task1(void *pvParameter);
static void send_Command_task(void *pvParameters);

void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void init_wireless_protocol(void);
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void update_battery_level(void);
void read_inputs(void);


/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
SemaphoreHandle_t xGuiSemaphore;

void setup(void)
{
  extern bool boot_loading_ready;
  lgfx::Bus_SPI _bus_instance;
  auto cfg = _bus_instance.config();
  cfg.freq_write = 80000000; // SPI-speed
  _bus_instance.config(cfg); // Apply the setting value to the bus.

  pinMode(joyLX, INPUT);
  pinMode(joyLY, INPUT);
  pinMode(joyRX, INPUT);
  pinMode(joyRY, INPUT);
  pinMode(right_btn,  INPUT_PULLUP);
  pinMode(left_btn,   INPUT_PULLUP);

  Serial.begin(115200);
  lcd.init(); // Initialize LovyanGFX
  lv_init();  // Initialize lvgl
  xTaskCreatePinnedToCore(guiTask, "gui", 4096 * 2, NULL, 3, NULL, 1);
  delay(2000);
  xTaskCreatePinnedToCore(Task1, "Task1", 1024 * 2, NULL, 5, NULL, 1);
  init_wireless_protocol();
  xTaskCreatePinnedToCore(send_Command_task, "Send command", 1024 * 2, NULL, 1, NULL, 0);
  LV_LOG_USER("*** boot_loading_ready ***");
  lv_textarea_add_text(ui_BootScreenLoadingTextBox, "Loading done.\n");
  boot_loading_ready = true;
  delay(2000);
}

void loop()
{
  // Keep this empty
}

/*** Display callback to flush the buffer to screen ***/
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  lcd.startWrite();
  lcd.setAddrWindow(area->x1, area->y1, w, h);
  lcd.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  lcd.endWrite();

  lv_disp_flush_ready(disp);
}

/*** Touchpad callback to read the touchpad ***/
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t touchX, touchY;
  bool touched = lcd.getTouch(&touchX, &touchY);

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;
    // Serial.printf("Touch (x,y): (%03d,%03d)\n",touchX,touchY );
  }
}

/// @brief Task for Grafical User Interface
static void guiTask(void *pvParameter)
{
  (void)pvParameter;
  xGuiSemaphore = xSemaphoreCreateMutex();
  Serial.println("*GuiTask* Started");
  lcd.setRotation(2);
  /* LVGL : Setting up buffer to use for display */
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

  /*** LVGL : Setup & Initialize the display device driver ***/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = display_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*** LVGL : Setup & Initialize the input device driver ***/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpad_read;
  lv_indev_drv_register(&indev_drv);

  ui_init();
  lv_textarea_add_text(ui_BootScreenLoadingTextBox, "GUI task started \n");
  while (1)
  {
    /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
    vTaskDelay(pdMS_TO_TICKS(10));

    /* Try to take the semaphore, call lvgl related function on success */
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {
      lv_task_handler();
      xSemaphoreGive(xGuiSemaphore);
    }
  }

  /* A task should NEVER return */
  free(buf);
  vTaskDelete(NULL);
}

int16_t read_joy_LY()
{
  const int deadRangeLow = 1980 - 50;  // arbitrary range
  const int deadRangeHigh = 1980 + 50; // arbitrary range
  int value = analogRead(joyLX);
  // Serial.println(value);
  int Joystick_value;

  if (value < deadRangeLow)
  {
    Joystick_value = map(value, 0, deadRangeLow, -1000, 0);
    return Joystick_value;
  }
  else if (value > deadRangeHigh)
  {
    Joystick_value = map(value, deadRangeHigh, 4095, 0, 1000);
    return Joystick_value;
  }
  else
  {
    return 0; // deadzone around center value
  }
}

int16_t read_joy_LX()
{
    const int deadRangeLow  = 1900 - 50;  // arbitrary range
    const int deadRangeHigh = 1900 + 50; // arbitrary range
    int value = analogRead(joyLY);
    // Serial.println(value);
    int Joystick_value;

    if (value < deadRangeLow)
    {
      Joystick_value = map(value, 0, deadRangeLow, -1000, 0);
      return Joystick_value;
    }
    else if (value > deadRangeHigh)
    {
      Joystick_value = map(value, deadRangeHigh, 4095, 0, 1000);
      return Joystick_value;
    }
    else
    {
      return 0; // deadzone around center value
    }
}

int16_t read_joy_RX()
{
    const int deadRangeLow = 1810 - 50;  // arbitrary range
    const int deadRangeHigh = 1810 + 50; // arbitrary range
    int value = analogRead(joyRX);
    // Serial.println(value);
    int Joystick_value;

    if (value < deadRangeLow)
    {
      Joystick_value = map(value, 0, deadRangeLow, -1000, 0);
      return Joystick_value;
    }
    else if (value > deadRangeHigh)
    {
      Joystick_value = map(value, deadRangeHigh, 4095, 0, 1000);
      return Joystick_value;
    }
    else
    {
      return 0; // deadzone around center value
    }
}

int16_t read_joy_RY()
{
    const int deadRangeLow = 1905 - 50;  // arbitrary range
    const int deadRangeHigh = 1905 + 50; // arbitrary range
    int value = analogRead(joyRY);
    // Serial.println(value);
    int Joystick_value;

    if (value < deadRangeLow)
    {
      Joystick_value = map(value, 0, deadRangeLow, -1000, 0);
      return Joystick_value;
    }
    else if (value > deadRangeHigh)
    {
      Joystick_value = map(value, deadRangeHigh, 4095, 0, 1000);
      return Joystick_value;
    }
    else
    {
      return 0; // deadzone around center value
    }
}

// Function for commanding the platform to move based on the position of the joysticks.
void Drive()
{
  int X_value   = read_joy_LX();      // returns Left joystick  X value -1000 - 1000
  int Y_value   = read_joy_LY();      // returns Left joystick  Y value -1000 - 1000
  int rot_spd   = read_joy_RX();      // returns Right joystick X value -1000 - 1000
  int lin_spd   = read_joy_RY();      // returns Right joystick Y value -1000 - 1000

  int speed = sqrt(pow(abs(X_value), 2) + pow(abs(Y_value), 2));      // calc speed value 
  speed = constrain(speed, 0, 1000);

  double angle = atan2((double(X_value)/1000),(double(Y_value)/1000)); // Left joystick angle in radians

  double vx = round(sin((angle * PI) / PI) * speed);
  double vy = round(cos((angle * PI) / PI) * speed);

  // calculate the target speed for each motor pair
  int M1 = constrain(((vy - vx)+ rot_spd + lin_spd),-1000,1000);
  int M2 = constrain(((vy + vx)- rot_spd + lin_spd),-1000,1000);
  int M3 = constrain(((vy + vx)+ rot_spd + lin_spd),-1000,1000);
  int M4 = constrain(((vy - vx)- rot_spd + lin_spd),-1000,1000);

  outputMsg.uFrontLeft  = M1;
  outputMsg.uFrontRight = M2;
  outputMsg.uRearLeft   = M3;
  outputMsg.uRearRight  = M4;

/* print commands for debugging
  char buff[255];
  sprintf(buff, "FrontLeft: \t%i \tFrontRight: \t%i\n"
                "RearLeft: \t%i \tRearRight: \t%i\n",
          Wheel_velocity_A,Wheel_velocity_B,Wheel_velocity_C,Wheel_velocity_D);
  Serial.println(buff);
*/

  // Set values for displaying Steer and throttle bars in display 
  lv_bar_set_value(ui_SteerBar, X_value, LV_ANIM_OFF);
  lv_bar_set_value(ui_throttleBar, speed, LV_ANIM_OFF);
}

void Task1(void *pvParameters) // This is a task.
{
  (void)pvParameters;
  xGuiSemaphore = xSemaphoreCreateMutex();
  Serial.println("*Task1* Started");
  lv_textarea_add_text(ui_BootScreenLoadingTextBox, "Task1 started \n");

  extern outgoing_msg_struct outputMsg;

  while (1) // A Task shall never return or exit.
  {
    vTaskDelay(100);
    /* Try to take the semaphore, call lvgl related function on success */
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {
      update_battery_level();
      read_inputs();
      xSemaphoreGive(xGuiSemaphore);
    }
  }
}

/// @brief Task for sending commands to platform
void send_Command_task(void *pvParameters) // task for wireless connection
{
  (void)pvParameters;
  xGuiSemaphore = xSemaphoreCreateMutex();
  Serial.println("*Send_command_task* Started");
  lv_textarea_add_text(ui_BootScreenLoadingTextBox, "Send command task started \n");

  //  Check if esp-now is working
  while (esp_now_init() != ESP_OK)
  {
    Serial.println("ESP-NOW is not working. Retrying...");
    init_wireless_protocol();
    vTaskDelay(1000);
  }

  while (1) // A Task shall never return or exit.
  {
    vTaskDelay(100);
    /* Try to take the semaphore, call lvgl related function on success */
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
    {

      Drive();
      outputMsg.uMode = speed_mode;

      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&outputMsg, sizeof(outputMsg));
      if (result == ESP_OK)
      {
        // lv_label_set_text(ui_MainScreen_Label2,"Command sent succesfully" );
      }
      else
      {
        // Serial.println("Error sending the data");
      }
      xSemaphoreGive(xGuiSemaphore);
    }
  }
}

/// @brief Initialise wireless protocol (ESP-NOW)
void init_wireless_protocol(void)
{
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP-NOW initialised succesfully");
}


/// @brief Read and update user input sensor data
void read_inputs(void)
{
  uint8_t left_btn_pressed  = digitalRead(left_btn);     // Check button state
  uint8_t right_btn_pressed = digitalRead(right_btn);    // Check button state

  //if button2 changed state
  if (right_btn_pressed == HIGH && right_btn_prev == LOW)
    {
      speed_mode++;
      if (speed_mode > 3){
        speed_mode = 0;
      }
      Serial.print("Speed_Mode: ");
      Serial.println(speed_mode);
    }

    // save button states
    left_btn_prev   = digitalRead(left_btn);
    right_btn_prev  = digitalRead(right_btn);

}

/// @brief Update battery charge level from incoming information
void update_battery_level(void)
{
  int16_t batLevel_1 = constrain(map(incomingMsg.uBat1,3370,4200,0,100),0,100);
  int16_t batLevel_2 = constrain(map(incomingMsg.uBat2,3370,4200,0,100),0,100);

  lv_bar_set_value(ui_Batbar1, batLevel_1, LV_ANIM_OFF);
  lv_bar_set_value(ui_Batbar2, batLevel_2, LV_ANIM_OFF);
}

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

  /* Try to take the semaphore, call lvgl related function on success */
  if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY))
  {

    if (status == 0)
    {
      success = "Delivery Success";
      lv_label_set_text(ui_MainScreen_Label2, "Command delivery Success");
    }
    else
    {
      success = "Delivery Fail";
      lv_label_set_text(ui_MainScreen_Label2, "Command delivery Fail");
    }

    xSemaphoreGive(xGuiSemaphore);
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&incomingMsg, incomingData, sizeof(incomingMsg));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
}
