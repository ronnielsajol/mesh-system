#include <ESP8266WiFi.h>
#include "namedMesh.h"

#define MESH_PREFIX "pupadmin"
#define MESH_PASSWORD "pupadmin1234"
#define MESH_PORT 5555

#define LED_PIN_1 4
#define LED_PIN_2 14
#define LED_PIN_3 5

namedMesh mesh;

bool ledState1 = false;
bool ledState2 = false;
bool ledState3 = false;

bool isSent = false;

const char* ssid = "S_PLDTWIFI";
const char* password = "Sajol162831!";
String receivedMsg;
String nodeName = "mainRec";

void receivedCallback(uint32_t from, String& msg) {
  Serial.printf("Received from %u msg = %s\n", from, msg.c_str());
  receivedMsg = msg;

  if (from == 2740128077) {  // Check if message is from Node 1
    if (msg.indexOf("floor") != -1) {
      digitalWrite(LED_PIN_1, HIGH);
      delay(2000);
      digitalWrite(LED_PIN_1, LOW);
      ledState1 = true;
      Serial.println("Node 1 turned LED ON");
    } else {
      digitalWrite(LED_PIN_1, LOW);
      ledState1 = false;
      Serial.println("Node 1 turned LED OFF");
    }
  } else if (from == 2740130629) {  // Check if message is from Node 2
    if (msg.indexOf("floor") != -1) {
      digitalWrite(LED_PIN_2, HIGH);
      delay(2000);
      digitalWrite(LED_PIN_2, LOW);
      ledState2 = true;
      Serial.println("Node 2 turned LED ON");
    } else {
      digitalWrite(LED_PIN_2, LOW);
      ledState2 = false;
      Serial.println("Node 2 turned LED OFF");
    }
  } else if (from == 2740253064) {  // Check if message is from Node 3
    if (msg.indexOf("floor") != -1) {
      digitalWrite(LED_PIN_3, HIGH);
      delay(2000);
      digitalWrite(LED_PIN_3, LOW);
      ledState3 = true;
      Serial.println("Node 3 turned LED ON");
    } else {
      digitalWrite(LED_PIN_3, LOW);
      ledState3 = false;
      Serial.println("Node 3 turned LED OFF");
    }
  }
}


void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);

  // Initialize PainlessMesh
  mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION);  // Enable debug messages
  mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP);
  mesh.onReceive(&receivedCallback);
  mesh.setName(nodeName);
}

void loop() {
  mesh.update();
  //If a message has been received, send it to the website using a GET request
  if (receivedMsg.length() > 0) {
    // Connect to Wi-Fi
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    delay(1000);

    String convertMessage = String(receivedMsg);
    convertMessage.replace(" ", "+");
    Serial.println(convertMessage);

    WiFiClient client;
    const char* host = "192.168.1.22";                                    // replace with the hostname of your website
    String url = "/php_arduino/insert_data.php?data=" + convertMessage;  // replace with the URL of the script that inserts the value into your database
    Serial.println(url);
    if (client.connect(host, 80)) {
      Serial.println("Connected to website");
      Serial.print("GET ");
      Serial.print(url);
      Serial.println(" HTTP/1.1");
      client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      Serial.println("Sent GET request");

      String header = client.readStringUntil('\n');
      Serial.println(header);

    } else {
      Serial.println("Failed to connect to website");
    }
    client.stop();
    receivedMsg = "";  // clear the received message after sending it to the website
  }
}
