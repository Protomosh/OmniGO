#ifndef ANIMATIONS_H
#define ANIMATIONS_H

int LED_pin     = 12;
int numPixels   = 20; // LED strip pixel count

// NeoPixel color format & data rate. See the strandtest example for
// information on possible values.
int pixelFormat = NEO_GRB + NEO_KHZ800;

// Rather than declaring the whole NeoPixel object here, we just create
// a pointer for one, which we'll then allocate later...
Adafruit_NeoPixel *pixels;

int32_t WHITE  = pixels->Color(255,255,255);
int32_t RED    = pixels->Color(255,0,0);
int32_t GREEN  = pixels->Color(0,255,0);
int32_t BLUE   = pixels->Color(0,0,255);
int32_t YELLOW = pixels->Color(255,255,0);


int32_t colors[5] = {YELLOW,RED,BLUE,GREEN,WHITE};

enum anim_state
{
    BOOT_ANIM,
    IDLE_ANIM,
    DRIVE_ANIM,
    ERROR_ANIM,
} anim_state;

void anim_pulse(int32_t color,int speed);
void anim_fill(int32_t color,int speed,int brightness);
void anim_knight(int32_t color,int speed,int brightness);


///@brief Animation pulse, pulses whole strip with set color.
///@par color = use pixel->color(0,0,0) function.
///@par speed = delay value
void anim_pulse(int32_t color,int speed){
    for(int i=0; i<100; i++) { // For each pixel...
    pixels->fill(color,0, numPixels);
    pixels->setBrightness(i);
    pixels->show();   // Send the updated pixel colors to the hardware.
    vTaskDelay(speed / portTICK_PERIOD_MS);
  }
    for(int i=100; i>0; i--) { // For each pixel...
    pixels->fill(color,0, numPixels);
    pixels->setBrightness(i);
    pixels->show();   // Send the updated pixel colors to the hardware.

    vTaskDelay(speed / portTICK_PERIOD_MS);
  }
  pixels->clear();
}

///@brief Animation fill, fills strip pixel by pixel from each end.
///@par color = use pixel->color(0,0,0) function.
///@par speed = delay value
///@par brightness = 0-255 value
void anim_fill(int32_t color,int speed,int brightness){

  for(int i=0; i<numPixels/2; i++) { // For each pixel...
    pixels->setPixelColor(i, color);
    pixels->setPixelColor((numPixels-1)-i, color); 
    pixels->setBrightness(brightness);
    pixels->show();   // Send the updated pixel colors to the hardware.

    vTaskDelay(speed / portTICK_PERIOD_MS);
  }
}

///@brief Animation fill, fills strip pixel by pixel from each end.
///@par color = use pixel->color(0,0,0) function.
///@par speed = delay value
///@par brightness = 0-255 value
void anim_knight(int32_t color,int speed,int brightness){
  int second_index = numPixels-1;

  for(int i=0; i<=(numPixels/2)-1; i++) { // For each pixel...
  int first_index = i;
    pixels->clear();

    pixels->setPixelColor(i,color);
    pixels->setPixelColor(i+1,color);

    pixels->setPixelColor((second_index)-i,color);
    pixels->setPixelColor((second_index -1)-i,color);

    pixels->setBrightness(brightness);

    pixels->show();   // Send the updated pixel colors to the hardware.
    vTaskDelay(speed / portTICK_PERIOD_MS);
  }
    for(int i=(numPixels/2)-1; i>=0; i--) { // For each pixel...
    pixels->clear();

    pixels->setPixelColor(i,color);
    pixels->setPixelColor(i+1,color);

    pixels->setPixelColor((second_index)-i,color);
    pixels->setPixelColor((second_index -1)-i,color);

    pixels->setBrightness(brightness);
    pixels->show();   // Send the updated pixel colors to the hardware.
    vTaskDelay(speed / portTICK_PERIOD_MS);
  }
}
#endif