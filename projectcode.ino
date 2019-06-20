//Remote Gas Pipeline Tunnel Temperature Monitoring System
//gas sensor D6 and temp sensor A0
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "YOUJNTU1S12";
const char* password = "YOUJNTU1S12"; 
#define ORG "dno83a"
#define DEVICE_TYPE "ruchika"
#define DEVICE_ID "1234"
#define TOKEN "12345678"
 char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic1[] = "iot-2/evt/gas/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);
int gas = D6;
const int LM_35 = A0;  
int input_val = 0;     
float temp = 0; 

void setup()
{
  Serial.begin(115200);
 Serial.println();
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
  pinMode(gas, INPUT);
  pinMode(LM_35,INPUT);
  
}

void loop()
{
  int gassensor = digitalRead(gas);
  input_val = analogRead(LM_35);             
   temp = (input_val * 4.88);
   temp = temp/10;
   Serial.print("Temperature is : " );                       
   Serial.println(temp);     
  Serial.print("Pin gas sensor: ");
  Serial.println(gassensor);
delay(1000);
if (isnan(temp) || isnan(gassensor))
{
Serial.println("Failed to read from  sensor!");
delay(1000);
return;
}
PublishData(temp,gassensor);
delay(1000);
}
void PublishData(float temp, int gassensor){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += temp;
  payload+="," "\"gassensor\":";
 payload += gassensor;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic1, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
