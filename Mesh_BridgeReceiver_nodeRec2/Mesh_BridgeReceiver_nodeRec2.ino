#include "namedMesh.h"

//Mesh Network Name
#define MESH_PREFIX "pupadmin"
#define MESH_PASSWORD "pupadmin1234"
#define MESH_PORT 5555

//For Buttons in the NodeMCU
namedMesh mesh;
String nodeName = "nodeRec2";
String recName = "2nd_floor";

void receivedCallback(uint32_t from, String &msg){
  digitalWrite(LED_BUILTIN, LOW);
  Serial.printf("Received from %u msg = %s\n", from, msg.c_str());
  String mainMsg = recName + " " + msg.c_str();
  String to = "mainRec";
  mesh.sendSingle(to, mainMsg);
  Serial.println("Sent message: " + mainMsg);
}

void setup(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT);
  mesh.setName(nodeName);
  mesh.onReceive(&receivedCallback); 
  Serial.println(mesh.getNodeId());
  Serial.println(recName); 
}

void loop(){
  mesh.update();
  digitalWrite(LED_BUILTIN, HIGH);
}