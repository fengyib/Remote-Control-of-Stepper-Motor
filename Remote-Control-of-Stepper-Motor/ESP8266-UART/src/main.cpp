#include <Arduino.h>
#include <ESP8266WiFi.h>//ESP32WIFI
#include <PubSubClient.h>//MQTT
#include <SoftwareSerial.h>
//wifi配置
const char *ssid="Redmi K40";
const char *password="12345678";

const char *mqtt_broker = "154.8.192.235";
const char *mqtt_username = "admin";
const char *mqtt_password = "12345678";
const int mqtt_port = 1883;


WiFiClient espClient;
PubSubClient client(espClient);
SoftwareSerial espSerial(1, 3);

void setup()
{
  Serial.begin(115200);
  espSerial.begin(9600);
  WiFi.begin(ssid,password);
  while(WiFi.status() !=WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi");
  }
  client.setServer(mqtt_broker,mqtt_port);
  while(!client.connected())
  {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());    
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
    } else {
    delay(2000);
    }
  }
  
  // 初始化ESP8266
  //espSerial.println("AT"); // 发送AT指令
  //delay(1000); // 等待ESP8266响应

  // 设置ESP8266的工作模式为Station模式
  //espSerial.println("AT+CWMODE=1");
}
//回调函数只能有一个
void callback(char* topic, byte* message, unsigned int length) {
  

  char message_str=(char)message[0];
  /*for (int i = 0; i < length; i++) {
    message_str += (char)message[i];
  }*/
  Serial.println(message_str);
  if(message_str=='1')
  {espSerial.println("1");}
  else if(message_str=='3')
  {
    espSerial.println("3");
  }
  
  
  
}
void loop() {
  client.loop();
  client.subscribe("ESP8266");
  client.setCallback(callback);//client.setCallback(callback)在vscode中只能放在loop函数中而不能放在setup中,而且在arduino中无论是放在哪里速度都差不多
  delay(50);
}
