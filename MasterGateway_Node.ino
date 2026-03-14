#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>
#include <RF24Mesh.h>

RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

// Define ID ranges
#define PARENT_START_ID 1
#define PARENT_END_ID 5

#define CHILD_START_ID 6
#define CHILD_END_ID 12

#define SUBCHILD_START_ID 13
#define SUBCHILD_END_ID 20

uint8_t nextParentID = PARENT_START_ID;
uint8_t nextChildID = CHILD_START_ID;
uint8_t nextSubChildID = SUBCHILD_START_ID;

void setup() {
    Serial.begin(9600);
    mesh.begin();
    Serial.println("Gateway Node Initialized");
}

void loop() {
    mesh.update();
    mesh.DHCP();

    while (network.available()) {
        RF24NetworkHeader header;
        uint8_t incomingType;

        if (!network.read(header, &incomingType, sizeof(incomingType))) {
            Serial.println("Error: Failed to read data.");
            return;
        }

        uint8_t newNodeID = 0;
        if (incomingType == 1 && nextParentID <= PARENT_END_ID) {
            newNodeID = nextParentID++;
        }
        else if (incomingType == 2 && nextChildID <= CHILD_END_ID) {
            newNodeID = nextChildID++;
        }
        else if (incomingType == 3 && nextSubChildID <= SUBCHILD_END_ID) {
            newNodeID = nextSubChildID++;
        }
        else {
            Serial.println("Error: No available IDs in the requested category.");
            return;
        }

        Serial.print("Assigned ID ");
        Serial.print(newNodeID);
        Serial.print(" to Node Type ");
        Serial.println(incomingType);
    }

    delay(100);
}
