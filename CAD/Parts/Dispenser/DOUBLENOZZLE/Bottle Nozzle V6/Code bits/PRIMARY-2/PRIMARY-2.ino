#include <PJONSoftwareBitBang.h>

PJONSoftwareBitBang bus(44);  // Device ID

const byte PDI_Button = 33;  // Button pin with internal pull-up
const byte PDIO_buspin = 25;

void setup() {
  Serial.begin(115200);
  pinMode(PDI_Button, INPUT_PULLUP);

  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
  bus.strategy.set_pin(PDIO_buspin);
  bus.begin();
}

void loop() {
  bus.receive(50000);  // Receive messages
  bus.update();        // Handle bus updates


  bool button_state = digitalRead(PDI_Button);
  static bool OLD_button_state = button_state;
  if (button_state != OLD_button_state) {  //Only update if state changes
    OLD_button_state = button_state;
    if (button_state == LOW) {  // If button is pressed (LOW due to internal pull-up)
      static byte BusSend[] = { 127 };
      Serial.print("Sending: ");
      for (int i = 0; i < sizeof(BusSend); i++) {
        Serial.print(BusSend[i]);
        Serial.print(" ");
      }
      Serial.println();  // Print the entire array

      bus.send(45, BusSend, sizeof(BusSend));  // Send the array to device 45

      BusSend[0] = BusSend[0] + 32;
      delay(200);  //Make sure the bus has time to be clear
    }
  }
}

void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  // Handle errors (optional)
}

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  // Handle received messages (optional)
}
