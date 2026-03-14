#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>
#include <RF24Mesh.h>

RF24 radio(9, 10);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

#define NODE_TYPE 3  // SubChild Type
#define D5_PIN 5

void setup() {
    Serial.begin(9600);
    pinMode(D5_PIN, INPUT);
    mesh.begin();
    Serial.println("Sub-Child Node Starting...");
}

void loop() {
    mesh.update();
    mesh.DHCP();

    uint8_t nodeID = mesh.getNodeID();
    uint16_t childID = mesh.getNodeID(mesh.getAddress());
    uint16_t parentID = mesh.getNodeID(mesh.getAddress(childID));

    if (nodeID < 13 || nodeID > 20) {
        Serial.println("Error: Sub-Child ID not in range. Retrying...");
        delay(5000);
        mesh.DHCP();  // Request a new ID again
        nodeID = mesh.getNodeID();  // Check the new ID
    }

    int ledState = digitalRead(D5_PIN);
    char message[64];

    snprintf(message, sizeof(message), "LED STATE (%s) of NODE ID %d (SubChild) via Child NODE ID %d via Parent NODE ID %d",
             (ledState == HIGH) ? "HIGH" : "LOW", nodeID, childID, parentID);

    RF24NetworkHeader header(0);
    network.write(header, &message, sizeof(message));

    Serial.println(message);
    delay(5000);
}
