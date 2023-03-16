
// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"


#define UP 2
#define DOWN 8
#define LEFT 4
#define RIGHT 6
#define UP_LEFT 1
#define UP_RIGHT 3
#define DOWN_LEFT 7
#define DOWN_RIGHT 9
#define STOP 5

#define FRONT_RIGHT_MOTOR 0
#define BACK_RIGHT_MOTOR 1
#define FRONT_LEFT_MOTOR 2
#define BACK_LEFT_MOTOR 3

#define FORWARD 1
#define BACKWARD -1

int direction_state = STOP;

struct MOTOR_PINS
{
  int pinIN1;
  int pinIN2;    
};

std::vector<MOTOR_PINS> motorPins = 
{
  {16, 17},  //FRONT_RIGHT_MOTOR
  {18, 19},  //BACK_RIGHT_MOTOR
  {27, 26},  //FRONT_LEFT_MOTOR
  {25, 33},  //BACK_LEFT_MOTOR   
};



// Replace with your network credentials
const char* ssid = "ROG";
const char* password = "carlios123";

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

AsyncEventSource events("/events");

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "DIRECTION"){
    Serial.println("direction = " + String(direction_state));
    switch(direction_state){
      case UP:
        return "FRONT";
      case UP_LEFT:
        return "FRONT LEFT";
      case UP_RIGHT:
        return "FRONT RIGHT";
      case LEFT:
        return "LEFT";
      case STOP:
        return "STOP";
    }
  }
  return String();
}

uint8_t retries=0;
void initWiFi() {
//  WiFi.mode(WIFI_AP);
//  // Remove the password parameter, if you want the AP (Access Point) to be open
//  WiFi.softAP(ssid, password);
//
//
//  IPAddress IP = WiFi.softAPIP();
//  Serial.print("AP IP address: ");
//  Serial.println(IP);
 
  //Try and Connect to the Network
  WiFi.begin(ssid,password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("...");
 
  //Wait for WiFi to connect for a maximum timeout of 20 seconds
  while(WiFi.status()!=WL_CONNECTED && retries<20)
  {
    Serial.print(".");
    retries++;
    delay(1000);
  }
 
  Serial.println();
  //Inform the user whether the timeout has occured, or the ESP8266 is connected to the internet
  if(retries==20)//Timeout has occured
  {
    Serial.print("Unable to Connect to ");
    Serial.println(ssid);
  }
   
  if(WiFi.status()==WL_CONNECTED)//WiFi has succesfully Connected
  {
    Serial.print("Successfully connected to ");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
}

void setUpPinModes()
{
  for (int i = 0; i < motorPins.size(); i++)
  {
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);  
    rotateMotor(i, STOP);  
  }
}


void setup(){
  setUpPinModes();
  // Serial port for debugging purposes
  Serial.begin(115200);

  initWiFi();

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }


  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/button1", HTTP_GET, [](AsyncWebServerRequest *request){
    direction_state = UP_LEFT;
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/button2", HTTP_GET, [](AsyncWebServerRequest *request){
    direction_state = UP;
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/button3", HTTP_GET, [](AsyncWebServerRequest *request){
    direction_state = UP_RIGHT;
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/button4", HTTP_GET, [](AsyncWebServerRequest *request){
    direction_state = LEFT;
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/button5", HTTP_GET, [](AsyncWebServerRequest *request){
    direction_state = STOP;
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/button6", HTTP_GET, [](AsyncWebServerRequest *request){
    direction_state = RIGHT;
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/button7", HTTP_GET, [](AsyncWebServerRequest *request){
    direction_state = DOWN_LEFT;
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/button8", HTTP_GET, [](AsyncWebServerRequest *request){
    direction_state = DOWN;
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/button9", HTTP_GET, [](AsyncWebServerRequest *request){
    direction_state = DOWN_RIGHT;
    request->send(SPIFFS, "/index.html", "text/html");
  });
 
  server.serveStatic("/", SPIFFS, "/");
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
  server.begin();
}

void rotateMotor(int motorNumber, int motorDirection)
{
  if (motorDirection == FORWARD)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);    
  }
  else if (motorDirection == BACKWARD)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, HIGH);     
  }
  else
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);       
  }
}


void processCarMovement(int inputValue)
{
  Serial.printf("Got value as %d\n", inputValue);  
  switch(inputValue)
  {
    case UP:
      rotateMotor(FRONT_RIGHT_MOTOR, FORWARD);
      rotateMotor(BACK_RIGHT_MOTOR, FORWARD);
      rotateMotor(FRONT_LEFT_MOTOR, FORWARD);
      rotateMotor(BACK_LEFT_MOTOR, FORWARD);                  
      break;
  
    case DOWN:
      rotateMotor(FRONT_RIGHT_MOTOR, BACKWARD);
      rotateMotor(BACK_RIGHT_MOTOR, BACKWARD);
      rotateMotor(FRONT_LEFT_MOTOR, BACKWARD);
      rotateMotor(BACK_LEFT_MOTOR, BACKWARD);   
      break;
  
    case LEFT:
      rotateMotor(FRONT_RIGHT_MOTOR, FORWARD);
      rotateMotor(BACK_RIGHT_MOTOR, BACKWARD);
      rotateMotor(FRONT_LEFT_MOTOR, BACKWARD);
      rotateMotor(BACK_LEFT_MOTOR, FORWARD);   
      break;
  
    case RIGHT:
      rotateMotor(FRONT_RIGHT_MOTOR, BACKWARD);
      rotateMotor(BACK_RIGHT_MOTOR, FORWARD);
      rotateMotor(FRONT_LEFT_MOTOR, FORWARD);
      rotateMotor(BACK_LEFT_MOTOR, BACKWARD);  
      break;
  
    case UP_LEFT:
      rotateMotor(FRONT_RIGHT_MOTOR, FORWARD);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, FORWARD);  
      break;
  
    case UP_RIGHT:
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, FORWARD);
      rotateMotor(FRONT_LEFT_MOTOR, FORWARD);
      rotateMotor(BACK_LEFT_MOTOR, STOP);  
      break;
  
    case DOWN_LEFT:
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, BACKWARD);
      rotateMotor(FRONT_LEFT_MOTOR, BACKWARD);
      rotateMotor(BACK_LEFT_MOTOR, STOP);   
      break;
  
    case DOWN_RIGHT:
      rotateMotor(FRONT_RIGHT_MOTOR, BACKWARD);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, BACKWARD);   
      break;
  
  
    case STOP:
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, STOP);    
      break;
  
    default:
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, STOP);    
      break;
  }
}



 
void loop(){
  processCarMovement(direction_state);
  ws.cleanupClients();
}



void onWebSocketEvent(AsyncWebSocket *server, 
                      AsyncWebSocketClient *client, 
                      AwsEventType type,
                      void *arg, 
                      uint8_t *data, 
                      size_t len) 
{                      
  switch (type) 
  {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      //client->text(getRelayPinsStatusJson(ALL_RELAY_PINS_INDEX));
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      processCarMovement(5);
      break;
    case WS_EVT_DATA:
      AwsFrameInfo *info;
      info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) 
      {
        std::string myData = "";
        myData.assign((char *)data, len);
        String temp = myData.c_str();
        processCarMovement(temp .toInt());       
      }
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
    default:
      break;  
  }
}
