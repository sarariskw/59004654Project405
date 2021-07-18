#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <PubSubClient.h>

//Wi-Fi settings
const char* ssid = "cutesuaywifi28";
const char* password = "suaymak28";

const char* topic = "server";
#define mqtt_server "driver.cloudmqtt.com"
#define mqtt_port 18763
#define mqtt_user "ktzuvtgv"
#define mqtt_password  "nRcALzAXMNjj"

WiFiClient espClient;
PubSubClient client(espClient);

int Relay1 = 1;       //led
char *led_status = "OFF";
int pumpPin2 = 2;    //ปั้ม
int analogPin = A0; //ประกาศตัวแปร ให้ analogPin แทนขา analog ขาที่5
int val = 0;

void setup() {
          /*------------NETWORK-------------*/
pinMode(pumpPin2, OUTPUT); // sets the pin as output
Serial.begin(9600);
pinMode(Relay1, OUTPUT); 
Serial.begin(115200);

Serial.println();
Serial.print("Connecting to");
Serial.println(ssid);
WiFi.begin(ssid,password);
while (WiFi.status() != WL_CONNECTED)
{
  delay(500);
  Serial.print(".");
}
    Serial.println("");
    Serial.println("Wifi connected ");
    Serial.println("IP Address:");
    Serial.println(WiFi.localIP());

    /*------------MQTT-------------*/
client.setServer(mqtt_server,mqtt_port);
client.setCallback(callback);

}

void loop() {
if(!client.connected())
{
  Serial.print("MQTT Connecting...");
  if(client.connect("ESP32Client",mqtt_user,mqtt_password))
  {
    client.subscribe(topic);
    Serial.println("Connected");
  }
  else
  {
    Serial.print("failed,rc =");
    Serial.print(client.state());
    Serial.println("Try agian in 5 seconds");
    delay(5000);
    return;
  }
}
          /*---------------------------------*/
val = analogRead(analogPin); //อ่านค่าสัญญาณ analog ขา5 ที่ต่อกับ Soil Moisture Sensor Module v1
Serial.print("val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
Serial.println(val); // พิมพ์ค่าของตัวแปร val
if (val > 500) {
digitalWrite(pumpPin2, HIGH); // สั่งให้ ปั้มน้ำเปิด
}
else {
digitalWrite(pumpPin2, LOW); // สั่งให้ ปั้มน้ำเปิด

 
}
delay(100);
client.loop();
}

void callback(char* topic,byte* payload,unsigned int length)
{
  Serial.print("Message from");
  Serial.println(topic);
  String msg = "";
  int i = 0;
  while(1 < length)
  {
    msg += (char)payload[i++];
  }
  Serial.print("receive");
  Serial.println(msg);

  if (msg == "on")
  {
    digitalWrite(Relay1,1);
    led_status = "NodeMCU : LED ON";
    client.publish("/NodeMCU",led_status);
  }else if (msg == "off")
    {
      digitalWrite(Relay1,0);
      led_status = "NodeMCU : OFF";
      client.publish("/NodeMCU",led_status);
    }else if (msg == "status")
    {
      client.publish("/NodeMCU",led_status);
    }

    if(led_status != "" )
    {
      Serial.print("Answer");
      Serial.println(led_status);
    }
}
