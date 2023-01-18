#ifndef WIRELESS_PROTOCOL_H
#define WIRELESS_PROTOCOL_H

#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address

uint8_t broadcastAddress[] = {0x30,0x83,0x98,0x7B,0x49,0xCC};

// Variable to store if sending data was successful
String success;
esp_now_peer_info_t peerInfo;

void init_wireless_protocol(void);
bool sendFeedbackData(uint8_t,uint8_t,uint16_t,uint8_t);
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

// Send message via ESP-NOW
bool sendFeedbackData(int16_t Bat_1, int16_t Bat_2, uint8_t Status){
  // send bat etc. data to transmiter
  feedbackMsg.uBat1       = Bat_1;
  feedbackMsg.uBat2       = Bat_2;
  feedbackMsg.uStatus     = Status;

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &feedbackMsg, sizeof(feedbackMsg));

    if (result == ESP_OK) {
      //Serial.println("Sent with success");
      return 0;
    }
    else {
      Serial.println("Error sending the data");
      return 1;
    }
}

JSONVar board; // Create json stucture

AsyncWebServer   server(80);            // Init async webserver to display data
AsyncEventSource events("/events");   // Init event source

// Webserver html,CSS and javascript code
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP-Dashboard</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #2f4468; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .packet { color: #bebebe; }
    .card.uLeft { color: #fd7e14; }
    .card.uRight { color: #1b78e2; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>ESP-Dashboard</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card uLeft">
        <h4>Left</h4>
        <p><span class="reading"><span id="left_value"> </span> </span></p>
      </div>
      <div class="card uRight">
        <h4>Right</h4>
        <p><span class="reading"><span id="right_value"> </span> </span></p>
      </div>
      <div class="card uMode">
        <h4>Mode</h4>
        <p><span class="reading"><span id="mode_value"> </span> </span></p>
      </div>
      <div class="card uAudio">
        <h4>Audio</h4>
        <p><span class="reading"><span id="audio_value"> </span> </span></p>
      </div>
        <div class="card uLights">
        <h4>Audio</h4>
        <p><span class="reading"><span id="lights_value"> </span> </span></p>
      </div>
    </div>
  </div>

<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('new_readings', function(e) {
  console.log("new_readings", e.data);
  var obj = JSON.parse(e.data);
  document.getElementById("left_value").innerHTML = obj.Left;
  document.getElementById("right_value").innerHTML = obj.Right;
  document.getElementById("mode_value").innerHTML = obj.Mode;
  document.getElementById("audio_value").innerHTML = obj.Audio;
  document.getElementById("lights_value").innerHTML = obj.Lights;

 }, false);
}
</script>
</body>
</html>)rawliteral";

/// Webserver initialisation ///
void init_wireless_protocol(void){
  // Set device as a Wi-Fi AP/Station
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
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
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
  }

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

  if (status == 0) {
    success = "Delivery Success";
    //Serial.println("Delivery Success");

  } else {
    success = "Delivery Fail";
  }
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
  // Copies the sender mac address to a string
  memcpy(&incomingMsg, incomingData, sizeof(incomingMsg));

  board["Left"]   = incomingMsg.uFrontLeft;
  board["Right"]  = incomingMsg.uFrontRight;
  board["Mode"]   = incomingMsg.uMode;

  String jsonString = JSON.stringify(board);

  events.send(jsonString.c_str(), "new_readings", millis());
}

/// Webserver initialisation ///
void webserver_init(){

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  
  server.addHandler(&events);
  server.begin();       //start webserver
}

#endif