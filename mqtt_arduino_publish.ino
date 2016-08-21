/*
 Basic MQTT example with Authentication

 How to send MQTT data to IBM Bluemix from Arduino.
 
 by Shashi Kiran
*/

#include <SPI.h>
#include <PubSubClient.h>
#include <Ethernet.h>

String buildClientName();

// IBM Bluemix Info
char orgName[] = "8ubmht";      // Your org name, found under the "Credentials" area in your Bluemix dashboard.
byte mac[] = { 0x00, 0xFF, 0xBB, 0xCC, 0xDE, 0x02 };

char server[] = "8ubmht.messaging.internetofthings.ibmcloud.com"; // MQTT Host (taken from Bluemix dashboard)
char type[] = "Arduino";    // Type of device you have registered in the IBM Bluemix Devices area.
char token[] = "iyI8aVbyYLJEy_4xRZ"; // Token issued when you first register your IoT device (only appears once)
int port = 1883;

String clientName = buildClientName();
String topicName = String("iot-2/evt/status/fmt/json"); // Topic


EthernetClient ethernetClient;
int status = 0;
// PubSub Client.
PubSubClient client(server, port, 0 , ethernetClient);

void setup()
{
  Serial.begin(9600);
  status = Ethernet.begin(mac);
  /* sashi
  if(status == WL_CONNECTED){
    Serial.println("WiFi Connected!");
  } else {
    Serial.println("WiFi Failed!");
  } */
}

void loop()
{
  char clientStr[34];
  clientName.toCharArray(clientStr,34);
  char topicStr[26];
  topicName.toCharArray(topicStr,26);
  
  if (!client.connected()) {
    Serial.print("Trying to connect to: ");
    Serial.println(clientStr);
    client.connect(clientStr, "use-token-auth", token);
  }
  if(client.connected()) {
       Serial.println("Success getting online...Begin transmit...");

      // Build the JSON data to publish.
      String json = buildJson();
      char jsonStr[200];
      json.toCharArray(jsonStr,200);

      // Publish the data.
      boolean pubresult = client.publish(topicStr,jsonStr);
      Serial.print("attempt to send ");
      Serial.println(jsonStr);
      Serial.print("to ");
      Serial.println(topicStr);

      if (pubresult)
        Serial.println("successfully sent");
      else
        Serial.println("unsuccessfully sent");
  }
  delay(5000);
}

// Builds the clientName
String buildClientName (){
  String data = "";
  data+="d:";
  data+=orgName;
  data+=":";
  data+=type;
  data+=":";
  data+="00FFBBCCDE02";
  return data;
}

// Sends the string: {d:{"Voltage" : "5"}} to Bluemix
String buildJson() {
  String data = "{";
  data+="\n";
  data+= "\"d\": {";
  data+="\n";
  data+="\"Volts\": 5"; // Note: wrap like \"5\" for a string, this is a number we are sending.
  data+="\n";
  data+="}";
  data+="\n";
  data+="}";
  return data;
}
