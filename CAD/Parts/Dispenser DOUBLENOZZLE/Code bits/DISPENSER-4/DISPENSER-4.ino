/*
  Program written by JelleWho https://github.com/jellewie
  Board: Attiny85

  There multible PCB's with chips
  1. PRIMARY, This one sends commands to the DISPENSER.
  2. DISPENSER [This code], Does all the action of dispensing the liquid and such. connected to both PRIMARY to recieve commands from and SLOT to recieve its ID from
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
*/
#include <FastLED.h>  //Include the libary FastLED (If you get a error here, make sure it's installed!)
#include <EEPROM.h>
#include <PJONSoftwareBitBang.h>

const byte PDI_Button = 14;      //Pull down to trigger
const byte PDO_ValveFluid = 33;  //LOW=OFF
const byte PDO_ValveAir = 32;    //LOW=OFF
const byte PDI_SLOT_TXRX = 13;   //The wire from which to get local ID from    //must be 2 or 3 on attiny85 I think
const byte PAO_LED = 26;         //To which pin the <LED> is connected to
const byte PDIO_buspin = 25;

const int TotalLEDs = 1;  //The total amounts of LEDs in the strip
CRGB LEDs[TotalLEDs];
CRGB ColorBoot = CRGB(255, 128, 0);
CRGB ColorGetID = CRGB(255, 0, 0);
CRGB ColorIdle = CRGB(0, 0, 255);
CRGB ColorDispencing = CRGB(0, 255, 0);
byte deviceID = 0;                //Store the received ID
byte FluidID = 0;                 //Store the fluid of this dispenser
unsigned long lastPulseTime = 0;  //Time of last revieved pulse from SLOT
int DelayAir = 100;               //ms to let the air valve open before the fluid valve, to get rid of pressure buildup in the bottle
enum COMMANDS { DISPENSE,
                GETFLUID,
                CHANGEFLUID,
                CHANGEDELAY,
                CHANGECOLOR };
PJONSoftwareBitBang bus(45);  // Device ID

void setup() {
  Serial.begin(115200);
  pinMode(PDI_Button, INPUT_PULLUP);
  pinMode(PDO_ValveFluid, OUTPUT);
  pinMode(PDO_ValveAir, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(PDO_ValveFluid, LOW);  //Ensure valve is OFF at start
  digitalWrite(PDO_ValveAir, LOW);    //Ensure valveAir is OFF at start
  FluidID = EEPROM.read(0);           //Read what our fluid is
  FastLED.addLeds<WS2812B, PAO_LED, GRB>(LEDs, TotalLEDs);
  fill_solid(&(LEDs[0]), TotalLEDs, ColorBoot);  //RED to show we do not have an ID
  FastLED.show();
  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
  bus.strategy.set_pin(PDIO_buspin);
  bus.begin();
  Serial.println("Start done");
}

void loop() {
  CheckAndGetDeviceID();  //Try and get the SLOT ID

  //Manual control
  bool button_state = digitalRead(PDI_Button);
  static bool OLD_button_state = button_state;
  if (button_state != OLD_button_state) {  //Only update if state changes
    OLD_button_state = button_state;
    (button_state == LOW) ? DispenseStart() : DispenseStop();  //Button pressed
  }

  bus.receive(50000);  // Receive messages
  bus.update();        // Handle bus updates
  //Serial.println(".");

  //  if (PrimarySerial.available() >= 3) {// Check if data is available to read
  //    byte SerialdeviceID = PrimarySerial.read();
  //    byte Serialcmd1 = PrimarySerial.read();
  //    byte Serialcmd2 = PrimarySerial.read();
  //    while (PrimarySerial.available()) PrimarySerial.read();   //Trash all the other commands if there any
  //    if (SerialdeviceID == deviceID){ //If it is for us
  //      byte answer = triggerAction(Serialcmd1, Serialcmd2)
  //      PrimarySerial.print(answer);
  //    }
  //  }
}

void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  // Handle errors (optional)
  Serial.println("Bus error_handler");
}

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  Serial.println("Data from primary " + String(payload[0]));
  digitalWrite(LED_BUILTIN, HIGH);  // Turn on LED
  int TimeDelay = payload[0] * 10;
  DispenseStart();
  delay(TimeDelay);
  DispenseStop();
  digitalWrite(LED_BUILTIN, LOW);  // Turn off LED
}

byte triggerAction(byte cmd1, byte cmd2) {
  /*
    DISPENSE, Time_in_ms*10.   
    GETFLUID, IGNORED.         
    CHANGEFLUID, New_ID        
    CHANGEDELAY, delay_in_ms between air and fluid   
    CHANGECOLOR, RRGGBBMM (2 bytes for RGB, where 00=off. MM=01=rainbow)  
  */
  byte r, g, b, m;
  Serial.print("Action" + String(cmd1) + "triggered with variable " + String(cmd2));
  switch (cmd1) {
    case DISPENSE:
      DispenseStart();
      delay(cmd2 * 2);
      DispenseStop();
      break;
    case GETFLUID:
      return FluidID;
    case CHANGEFLUID:
      FluidID = cmd2;
      b = map(b, 0, 3, 0, 255);  // Map the 2-bit value to the range 0-255
      fill_solid(&(LEDs[0]), TotalLEDs, CRGB(r, g, b));
      m = cmd2 & 0x03;  // Mask with 0x03 (00000011)
      if (m == 0b01) {
      }
    //TODO, we can define color modes
    //m 00 = just default RGB,
    //m 01 = rainbow mode
    //m 1x = reserved for other modesDelayAir
    default:
      return 4;
      break;
  }
  return 200;
}
void DispenseStart() {
  Serial.println("DispenseStart");
  fill_solid(&(LEDs[0]), TotalLEDs, ColorDispencing);
  digitalWrite(PDO_ValveAir, HIGH);
  delay(DelayAir);
  digitalWrite(PDO_ValveFluid, HIGH);
  FastLED.show();
}
void DispenseStop() {
  Serial.println("DispenseStop");
  digitalWrite(PDO_ValveAir, LOW);
  digitalWrite(PDO_ValveFluid, LOW);
  fill_solid(&(LEDs[0]), TotalLEDs, ColorIdle);
  FastLED.show();
}
void CheckAndGetDeviceID() {
  if (deviceID == 0) {
    if (LEDs[0] != ColorGetID and LEDs[0] != ColorDispencing) {  //If not yet desired color, but do not overwrite ColorDispencing
      fill_solid(&(LEDs[0]), TotalLEDs, ColorGetID);
      FastLED.show();
    }
    byte ID1 = GetDeviceID();
    delay(10);
    byte ID2 = GetDeviceID();
    delay(10);
    byte ID3 = GetDeviceID();
    if (ID1 > 0 && ID1 == ID2 && ID2 == ID3) {
      deviceID = ID1;
      if (LEDs[0] != ColorDispencing) {  //Do not overwrite ColorDispencing
        fill_solid(&(LEDs[0]), TotalLEDs, ColorIdle);
        FastLED.show();
      }
      Serial.println("deviceID recieved, I am " + String(deviceID));
    } else {
      Serial.println("Error in deviceID recieved, ID1=" + String(ID1) + " ID2=" + String(ID2) + " ID3=" + String(ID3));
    }
  }
}

byte GetDeviceID() {
  Serial.println(String(millis()) + "GetDeviceID");
  pinMode(PDI_SLOT_TXRX, OUTPUT);
  digitalWrite(PDI_SLOT_TXRX, LOW);
  delay(1);
  pinMode(PDI_SLOT_TXRX, INPUT);
  int pulseCount = 0;  //Amount of pulses counted from SLOT
  unsigned long lastPulseTime = millis();
  bool OLD_PIN_state = 0;
  while (millis() - lastPulseTime < 20) {  //Do NOT put Serial in this while loop. It will be to slow to count the pulses
    bool PIN_state = digitalRead(PDI_SLOT_TXRX);
    if (PIN_state != OLD_PIN_state) {  //Only update if state changes
      OLD_PIN_state = PIN_state;
      if (PIN_state == LOW)
        pulseCount++;
      lastPulseTime = millis();  //Reset timeout on pulse detection
    }
  }
  return pulseCount;
}
