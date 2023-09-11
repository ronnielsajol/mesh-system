// #include <EasyButton.h>
#include <OneButton.h>
#include <painlessMesh.h>
#include "namedMesh.h"
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <TinyGPSplus.h>


#define MESH_PREFIX "pupadmin"
#define MESH_PASSWORD "pupadmin1234"
#define MESH_PORT 5555
#define BUTTON_PIN D6

OneButton button(BUTTON_PIN, true);
namedMesh mesh;
SoftwareSerial gpsSerial(D2, D4);
TinyGPSPlus gps;

String nodeName = "nodeRec";



void sendToReceiver1(){
    String msg = "14.599633 121.003925";
    String to = "nodeRec";
    Serial.println("Button pressed 1 time");
    Serial.println("Sending to Receiver 1");
    mesh.sendSingle(to, msg);
}

void sendToReceiver2(){
    String msg = "14.599633 121.003925";
    String to = "nodeRec2";
    Serial.println("Button pressed 2 times");
    Serial.println("Sending to Receiver 2");
    mesh.sendSingle(to, msg);
}

void sendToReceiver3(){
    String msg = "14.599633 121.003925";
    String to = "nodeRec3";
    Serial.println("Long pressed the button");
    Serial.println("Sending to Receiver 3");
    mesh.sendSingle(to, msg);
}

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);
  mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT);

  button.attachClick(sendToReceiver1);
  button.attachDoubleClick(sendToReceiver2);
  button.attachLongPressStop(sendToReceiver3);
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      Serial.print("Location: ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(", ");
      Serial.print(gps.location.lng(), 6);
      Serial.println("");
    }
  }
  mesh.update();
  button.tick();
  delay(10);
}
