/*
  Program written by JelleWho https://github.com/jellewie
  Board: Attiny85
  PRIMARY

  There multible PCB's with chips
  1. PRIMARY, This one sends commands to the DISPENSER.
  2. DISPENSER, Does all the action of dispensing the liquid and such. connected to both PRIMARY to recieve commands from and SLOT to recieve its ID from
  3. SLOT, this is just a secondary giving the DISPENSER an ID. This ID is used to talk to the PRIMARY

  PCB General rules:
    The board is designed with SMD parts on only 1 side
    The board has fiducials so it is PnP compatable
    The board shoud have labels at each THT pin to make debugging easier
  PCB DISPENSER:
    Magnet pins from the SLOT to the DISPENSER are defined as:
      [24V] [GND] [PRIMARY_RTX] [] [SLOT_TXRX] (This order)
    The DISPENSER PCB has an ATTiny85 chip that has these Pins
      [5V] [GND] [Button] [ValveFluid] [PDO_ValveAir] and [SLOT_TXRX] [LED] [PRIMARYTX] [PRIMARYRX]
    The board converts 24V to 5V for the chip
    The output to the valves must be done with an transistor or sorts, and will need to be gound switching
    The button is an SMD internal part, but has jumpoutpins to be external. The button will be debounce by hardware
    SLOT_TXRX 1M down
  PCB SLOT:
    The SLOT PCB has an ATTiny85 chip that has these Pins
      [5V] [GND] [SLOT_TXRX]
    The board imports AND exports these pins from other SLOTs/the PRIMARY
      [24V] [GND] [5V] [PRIMARY_RTX] []
    SLOT_TXRX  10k up, 660 in line
  PCB PRIMARY:
    Exports these pins (Its cheaper to have one 5V power supply, so import this)
      [24V] [GND] [5V] [PRIMARY_RTX] []
    PRIMARY_RTX 150 in line, then 10k down


  
  <Dispensor ID> To remember what dispenser is what. can also be Fluid ID at the same time, but its nice to have them seperated
    <Slot ID> to know wher to move glass to
    <Fluid ID> What drink is settup

*/
#include <PJONSoftwareBitBang.h>

PJONSoftwareBitBang bus(0);  //Master Device ID

const byte MAX_Dispensers = 255;          //Maximum possible IDs
byte Dispensers[MAX_Dispensers] = { 1 };  //Array of available DISPENSER IDs
byte availableCount = 0;                  //Number of detected devices
const byte PDI_Button = 33;               //Button pin with internal pull-up
const byte PDIO_buspin = 25;
enum COMMANDS { UNK,
                DISPENSE,
                GETFLUID,
                CHANGEFLUID,
                CHANGEDELAY,
                CHANGECOLOR };
enum COMMAND { DISPENSERUNK,
               DISPENSERSTATUS,
               DISPENSERFLUID,
               DISPENSERSLOT };

void setup() {
  Serial.begin(115200);
  pinMode(PDI_Button, INPUT_PULLUP);

  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
  bus.strategy.set_pin(PDIO_buspin);
  bus.begin();
}

void loop() {
  bus.receive(50000);  //Receive messages
  bus.update();        //Handle bus updates

  bool button_state = digitalRead(PDI_Button);
  static bool OLD_button_state = button_state;
  if (button_state != OLD_button_state) {  //Only update if state changes
    OLD_button_state = button_state;
    if (button_state == LOW) {  //If button is pressed (LOW due to internal pull-up)
      byte DispenserID = Dispensers[0];

      byte BusSend[] = { DISPENSE, 100 };
      bus.send(DispenserID, BusSend, sizeof(BusSend));  // Send the array to device 45
      delay(100);                                       //Make sure the bus has time to be clear
    }
  }
}

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  Serial.print("Received: ");
  for (uint16_t i = 0; i < length; i++)
    Serial.print(payload[i] + String(" "));
  Serial.println(); 

  if (length == 3) {
    //FROM, WHAT, DATA
    switch (payload[0]) {  // Check first byte as a command ID
      case DISPENSERSTATUS:
        Serial.println("Dispenser " + String(payload[0]) + " gave command DISPENSERSTATUS=" + String(payload[1]));
        break;
      default:
        Serial.print("Unknown Command: 0x");
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
