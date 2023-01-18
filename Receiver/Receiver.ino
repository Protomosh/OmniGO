// Import required libraries
#include "ESPAsyncWebServer.h"
#include <Arduino_JSON.h>
#include <Adafruit_NeoPixel.h>
#include "global_variables.h"
#include "freertos/semphr.h"
#include "wireless_protocol.h"
#include <Wire.h>
#include "animations.h"
#include <HardwareSerial.h>

void printDetail(uint8_t type, int value);
void webserver_init(void);

// Create Uart instances for motor drivers // 
HardwareSerial motor_1_serial(1);
HardwareSerial motor_2_serial(2);

/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
SemaphoreHandle_t xGuiSemaphore;

extern incoming_msg_struct incomingMsg;
extern output_msg_struct feedbackMsg;

void setup() {
  pixels = new Adafruit_NeoPixel(numPixels, LED_pin, pixelFormat);
  xTaskCreatePinnedToCore(animation_task, "Animation task", 1024*2, NULL, 25, NULL, 1);
  delay(1000);
  anim_state = BOOT_ANIM;

  // Initialize Serial connections
  Serial.begin(115200);  
  motor_1_serial.begin(115200, SERIAL_8N1, 33, 32);
  motor_2_serial.begin(115200, SERIAL_8N1, 17, 16);

  init_wireless_protocol();  // Start wireless communication with transmiter
  webserver_init();          // Start webserver

  xTaskCreatePinnedToCore(send_Feedback_task, "Feedback", 1024 * 2, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(motor_communication, "Motor communication", 2048 * 4, NULL, 1, NULL, 1);

}

void loop() {
  /*
  static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 5000;

//send ping every 5s
  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
    events.send("ping",NULL,millis());
    lastEventTime = millis();
  }
*/
}

static void animation_task(void *pvParameters)  // task for wireless connection
{
  (void)pvParameters;
  xGuiSemaphore = xSemaphoreCreateMutex();
  Serial.println("*Animation task* Started");

  while (1)  // A Task shall never return or exit.
  {
    vTaskDelay(80 / portTICK_PERIOD_MS);
    if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
      // do something different depending on the animation state:
      switch (anim_state) {
        case BOOT_ANIM:
          //Serial.println("Boot animation");
          anim_fill(WHITE,100,100);
          break;

        case IDLE_ANIM:
          //Serial.println("Idle animation");
          anim_pulse(colors[incomingMsg.uMode],10);
          break;
          
        case DRIVE_ANIM:
          //Serial.println("Drive animation");
          anim_knight(colors[incomingMsg.uMode],80,150);
          break;
        case ERROR_ANIM:
          //Serial.println("Error animation");
          anim_pulse(RED,2);
          break;
      }
    xSemaphoreGive(xGuiSemaphore);
    }
  }
}

static void send_Feedback_task(void *pvParameters)  // task for wireless connection
{
  (void)pvParameters;
  Serial.println("*Send_Feedback_task* Started");

  // Check if esp-now is working
  while (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW is not working. Retrying...");
    init_wireless_protocol();
    anim_state = ERROR_ANIM;
    vTaskDelay(1000);
  }

  while (1)  // A Task shall never return or exit.
  {
    vTaskDelay(25 / portTICK_PERIOD_MS);
    Serial.print(Feedback_driver_1.batVoltage);
    Serial.print(" : ");
    Serial.println(Feedback_driver_2.batVoltage);
    if(sendFeedbackData(Feedback_driver_1.batVoltage, Feedback_driver_2.batVoltage,incomingMsg.uMode)){
      anim_state = ERROR_ANIM;
    };
  }
}

static void motor_communication(void *pvParameters)  // task for wireless connection
{
  (void)pvParameters;
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 20;

  Serial.println("*motor_communication* Started");
  anim_state = IDLE_ANIM;

  // Check if esp-now is working
  while (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW is not working. Retrying...");
    anim_state = ERROR_ANIM;
    init_wireless_protocol();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  while (1)  // A Task shall never return or exit.
  {
    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

      int16_t Front_throttle  = incomingMsg.uFrontLeft;
      int16_t Front_steer     = incomingMsg.uFrontRight;
      int16_t Rear_throttle   = incomingMsg.uRearLeft;
      int16_t Rear_steer      = incomingMsg.uRearRight;
      uint8_t mode            = incomingMsg.uMode;
      uint8_t powerState      = incomingMsg.uPowerState;
      uint8_t sensors = 0;
      uint8_t turbo = 0;

      if (powerState){
        Send_cmd(Front_steer, Front_throttle, mode, turbo, 1);    // Send commands to Front motor driver
        Send_cmd(Rear_throttle, Rear_steer, mode, turbo, 2);      // Send commands to Rear motor driver
        anim_state = DRIVE_ANIM;
        }
      else{
        Send_cmd(0,0, mode, turbo, 1);    // Send commands to Front motor driver
        Send_cmd(0,0, mode, turbo, 2);      // Send commands to Rear motor driver
        anim_state = IDLE_ANIM;
      }
      Receive(1);   // Read feedback data from motor drivers
      Receive(2);
  }
}

// ########################## SEND ##########################

void Send_cmd(int16_t uSteer, int16_t uSpeed, uint8_t uMode, int uTurbo, int driver_number) {
  int uSensors = 0;
  // Create command
  Command.start     = (uint16_t)START_FRAME;
  Command.cmd1      = (uint8_t)uMode;
  Command.cmd2      = (uint8_t)uTurbo;
  Command.steer     = (int16_t)uSteer;
  Command.speed     = (int16_t)uSpeed;
  Command.sensors   = (uint8_t)uSensors;
  Command.checksum  = (uint16_t)(Command.start ^ Command.cmd1 ^ Command.cmd2 ^ Command.steer ^ Command.speed ^ Command.sensors);
  /*
  Serial.print("startframe: ");   Serial.print(Command.start);
  Serial.print(" cmd1: ");        Serial.print(Command.cmd1);
  Serial.print(" cmd2: ");        Serial.print(Command.cmd2);
  Serial.print(" steer: ");       Serial.print(Command.steer);
  Serial.print(" speed: ");       Serial.print(Command.speed);
  Serial.print(" sensors: ");     Serial.print(Command.sensors);
  Serial.print(" checksum: ");    Serial.println(Command.checksum);
*/
  // Write to Motor 1 serial
  if (driver_number == 1) {
    motor_1_serial.write((uint8_t *)&Command, sizeof(Command));
  }
  // Write to Motor 2 serial
  else if (driver_number == 2) {
    motor_2_serial.write((uint8_t *)&Command, sizeof(Command));
  } else {
      return;
  }
}

// ########################## RECEIVE ##########################

void Receive(int driver_number)
    {
    int driver_available,read_driver;

    switch (driver_number) {
      case 1:
        driver_available = motor_1_serial.available();
        read_driver      = motor_1_serial.read();
        break;

      case 2:
        driver_available = motor_2_serial.available();
        read_driver      = motor_2_serial.read();
        break;

      default:
        return;
        break;
    }
    // Check for new data availability in the Serial buffer
    if (driver_available) {
        incomingByte 	  = read_driver;                                   // Read the incoming byte
        bufStartFrame	= ((uint16_t)(incomingByte) << 8) | incomingBytePrev;        // Construct the start frame
    }
    else {
        anim_state = ERROR_ANIM;
        Serial.println("Serial connection error with motor driver. Check driver connection.");
        return;
    }

  // If DEBUG_RX is defined print all incoming bytes
  #ifdef DEBUG_RX
        Serial.println(incomingByte);
        return;
  #endif

    // Copy received data
    if (bufStartFrame == START_FRAME) {	                    // Initialize if new data is detected
        p       = (byte *)&NewFeedback;
        *p++    = incomingBytePrev;
        *p++    = incomingByte;
        idx     = 2;	
    } else if (idx >= 2 && idx < sizeof(SerialFeedback)) {  // Save the new received data
        *p++    = incomingByte; 
        idx++;
    }	
    
    // Check if we reached the end of the package
    if (idx == sizeof(SerialFeedback)) {
        uint16_t checksum;
        checksum = (uint16_t)(NewFeedback.start ^ NewFeedback.cmd1 ^ NewFeedback.cmd2 ^ NewFeedback.speedR_meas ^ NewFeedback.speedL_meas
                            ^ NewFeedback.batVoltage ^ NewFeedback.boardTemp ^ NewFeedback.cmdLed);

        // Check validity of the new data
        if (NewFeedback.start == START_FRAME && checksum == NewFeedback.checksum) {
            // Copy the new data
            switch (driver_number) {
              case 1:
                memcpy(&Feedback_driver_1, &NewFeedback, sizeof(SerialFeedback));
/*
                Serial.print("1: ");   Serial.print(Feedback_driver_1.cmd1);
                Serial.print(" 2: ");  Serial.print(Feedback_driver_1.cmd2);
                Serial.print(" 3: ");  Serial.print(Feedback_driver_1.speedR_meas);
                Serial.print(" 4: ");  Serial.print(Feedback_driver_1.speedL_meas);
                Serial.print(" 5: ");  Serial.print(Feedback_driver_1.batVoltage);
                Serial.print(" 6: ");  Serial.print(Feedback_driver_1.boardTemp);
                Serial.print(" 7: ");  Serial.println(Feedback_driver_1.cmdLed);
                */
              break;
              case 2:
                memcpy(&Feedback_driver_2, &NewFeedback, sizeof(SerialFeedback));
                /*
                Serial.print("1: ");   Serial.print(Feedback_driver_2.cmd1);
                Serial.print(" 2: ");  Serial.print(Feedback_driver_2.cmd2);
                Serial.print(" 3: ");  Serial.print(Feedback_driver_2.speedR_meas);
                Serial.print(" 4: ");  Serial.print(Feedback_driver_2.speedL_meas);
                Serial.print(" 5: ");  Serial.print(Feedback_driver_2.batVoltage);
                Serial.print(" 6: ");  Serial.print(Feedback_driver_2.boardTemp);
                Serial.print(" 7: ");  Serial.println(Feedback_driver_2.cmdLed);
                */
              break;
            }
            //memcpy(Feedback, &NewFeedback, sizeof(SerialFeedback));

        } else {
          //Serial.println("Non-valid data skipped");
        }
        idx = 0;    // Reset the index (it prevents to enter in this if condition in the next cycle)
    }
    // Update previous states
    incomingBytePrev = incomingByte;
}

/*
// power On function  TODO
void powerOn(void) {

  digitalWrite(Motor_1_relay, incomingMsg.uMode);
  digitalWrite(Motor_2_relay, incomingMsg.uMode);

  //incomingMsg.mode = 1; // change ON/OFF mode to HIGH
}
*/
