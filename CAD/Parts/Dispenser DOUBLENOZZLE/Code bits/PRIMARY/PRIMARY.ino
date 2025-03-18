/*
  Program written by JelleWho https://github.com/jellewie
  Board: Attiny85
  PRIMARY

  There multible PCB's with chips
  1. PRIMARY, This one sends commands to the DISPENSER.
  2. DISPENSER, Does all the action of dispensing the liquid and such. connected to both PRIMARY to recieve commands from and SLOT to recieve its ID from
  3. SLOT, this is just a secondary giving the DISPENSER an ID. This ID is used to talk to the PRIMARY
*/
#include <PJONSoftwareBitBang.h>
#define SWBB_MODE 1  //Should be default, 1=1.97kB/s //https://github.com/gioblu/PJON/blob/master/src/strategies/SoftwareBitBang/README.md#performance
//#define SWBB_MAX_ATTEMPTS 20;	//Maximum transmission attempts	Numeric value (20 by default)

const uint8_t PrimaryID = 254;
PJONSoftwareBitBang bus(PrimaryID);            //Master Device ID
const uint8_t MAX_Dispensers = 253;            //Maximum possible IDs
uint8_t Dispensers[MAX_Dispensers] = { 250 };  //Array of available DISPENSER IDs
bool SlotConnected[MAX_Dispensers];
uint8_t availableCount = 0;     //Number of detected devices
const uint8_t PDI_Button = 33;  //Button pin with internal pull-up
const uint8_t PDIO_buspin = 25;

enum COMMANDS { UNK,
                DISPENSE,
                GETFLUID,
                CHANGEFLUID,
                CHANGEDELAY,
                CHANGECOLOR,
                ADOPT };
enum COMMAND { DISPENSERUNK,
               DISPENSERSTATUS,
};

void setup() {
  Serial.begin(115200);
  pinMode(PDI_Button, INPUT_PULLUP);

  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
  bus.strategy.set_pin(PDIO_buspin);
  bus.begin();
  pingAll();
}

void loop() {
  int result = bus.receive(50000);  //Stop and receive commands for x microseconds (1000 micro = 1ms)
  if (result != PJON_ACK and result != PJON_FAIL) {
    /* https://github.com/gioblu/PJON/blob/master/documentation/data-reception.md
      receive returns the following values:
      PJON_ACK (6) if a correct reception occurred
      PJON_NAK (21) if a mistake is found in CRC
      PJON_BUSY (666) if a transmission for other devices is occurring
      PJON_FAIL (65535) if no data is received
    */
    Serial.println("ERROR bus.receive " + String(result));
  }

  bus.update();  //Handle bus updates

  bool button_state = digitalRead(PDI_Button);
  static bool OLD_button_state = button_state;
  if (button_state != OLD_button_state) {  //Only update if state changes
    OLD_button_state = button_state;
    if (button_state == LOW) {  //If button is pressed (LOW due to internal pull-up)


      //To send reset color command:
      uint8_t BusSend1[] = { CHANGECOLOR, 0b00000000 };
      bus.send_packet(PJON_BROADCAST, BusSend1, sizeof(BusSend1));

      uint8_t DispenserID = Dispensers[0];
      uint8_t BusSend[] = { DISPENSE, 100 };

      uint16_t result = bus.send_packet_blocking(DispenserID, BusSend, sizeof(BusSend));
      if (result != PJON_ACK) {
        /* https://github.com/gioblu/PJON/blob/master/documentation/data-transmission.md#send
          send_packet returns the following values:
          PJON_ACK (6) if transmission occurred and acknowledgement is received if requested
          PJON_BUSY (666) if bus is busy
          PJON_FAIL (65535) if transmission failed
        */
        Serial.println("ERROR send_packet_blocking " + String(result));
      }
    }
  }

  //To send IDLE Rainbow command:
  //uint8_t BusSend[] = { CHANGECOLOR, 0b00000010 };
  //bus.send_packet(PJON_BROADCAST, BusSend, sizeof(BusSend));
  //
  //To send reset color command:
  //uint8_t BusSend[] = { CHANGECOLOR, 0b00000000 };
  //bus.send_packet(PJON_BROADCAST, BusSend, sizeof(BusSend));
}
void pingAll() {
  //bus.send_packet(PJON_BROADCAST, "Message for all connected devices.", 34);
  //if we assume the command is 9 byte (header and such also included) and we run at 1.97kB/s it should take 4.57ms. So for 253 dispensers it could take up to 1.5S to scan all. Thats decent
  Serial.println("pingAll " + String(MAX_Dispensers));
  static uint8_t BusSend[] = { ADOPT, 0 };
  for (uint8_t i = 1; i < MAX_Dispensers; i++) {
    int result = bus.send_packet(i, BusSend, sizeof(BusSend));  //Tries to send data 1 time. its a blocking function so it will wait here
    if (result == PJON_ACK) {
      SlotConnected[i] = true;
      Serial.println("Dispenser " + String(i) + " Connected");
    } else {
      SlotConnected[i] = false;
      Serial.println("Dispenser " + String(i) + " not connected, code=" + String(result));
    }
  }
}
void pingOnline() {  //We can call this code periodicaly this will check if all dispensers are still attached
  Serial.println("pingOnline " + String(MAX_Dispensers));
  static uint8_t BusSend[] = { ADOPT, 0 };
  for (uint8_t i = 0; i < MAX_Dispensers; i++) {
    if (SlotConnected[i] == true) {                                        //Only checks slots that should be connected and respond
      int result = bus.send_packet_blocking(i, BusSend, sizeof(BusSend));  //Check if they are all still online, to make sure also retry if we fail the first time
      if (result != PJON_ACK) {
        SlotConnected[i] = false;
        Serial.println("Dispenser " + String(i) + " disconnected, code=" + String(result));
      }
    }
  }
}
void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  Serial.print("Received: ");
  for (uint16_t i = 1; i < length; i++)
    Serial.print(payload[i] + String(" "));
  Serial.println("From_ID=" + String(packet_info.tx.id));

  if (length == 2) {
    //FROM, WHAT, DATA
    switch (payload[0]) {  // Check first byte as a command ID
      case DISPENSERSTATUS:
        Serial.println("Dispenser " + String(packet_info.tx.id) + " gave command DISPENSERSTATUS=" + String(payload[1]));
        break;
      default:
        Serial.print("Which is an unknown Command");
        break;
    }
  }
}
void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  if (code == PJON_CONNECTION_LOST) {
    Serial.print("Connection with device ID ");
    Serial.print(bus.packets[data].content[0], DEC);
    Serial.println(" is lost.");
  }
  if (code == PJON_PACKETS_BUFFER_FULL) {
    Serial.print("Packet buffer is full, has now a length of ");
    Serial.println(data, DEC);
    Serial.println("Possible wrong bus configuration!");
    Serial.println("higher PJON_MAX_PACKETS if necessary.");
  }
  if (code == PJON_CONTENT_TOO_LONG) {
    Serial.print("Content is too long, length: ");
    Serial.println(data);
  }
};
