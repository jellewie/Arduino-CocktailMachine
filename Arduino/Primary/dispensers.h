void BusSend(uint8_t cmd1, uint8_t cmd2) {
  uint8_t BusSend[] = { CHANGECOLOR, 0b00000000 };  //Send reset Rainbow color
  bus.send_packet(PJON_BROADCAST, BusSend, sizeof(BusSend));
}
bool BusSendBlocking(uint8_t DispenserID, uint8_t cmd1, uint8_t cmd2) {
  static uint8_t BusSend[] = { cmd1, cmd2 };
  uint16_t result = bus.send_packet_blocking(DispenserID, BusSend, sizeof(BusSend));
  if (result != PJON_ACK) {
    LcdPrint("BUS error:" + String(DispenserID), PJONresultToString(result));
    return false;
  }
  return true;
}

void pingAll() {
  //bus.send_packet(PJON_BROADCAST, "Message for all connected devices.", 34);
  //if we assume the command is 9 uint8_t (header and such also included) and we run at 1.97kB/s it should take 4.57ms. So for 253 dispensers it could take up to 1.5S to scan all. Thats decent
  Serial.println("pingAll " + String(Dispensers_Amount));
  static uint8_t BusSend[] = { ADOPT, 0 };
  for (uint8_t i = 1; i < Dispensers_Amount; i++) {
    uint16_t result = bus.send_packet(i, BusSend, sizeof(BusSend));  //Tries to send data 1 time. its a blocking function so it will wait here
    if (result == PJON_ACK) {
      Serial.println("Dispenser " + String(i) + " Connected");
      uint16_t result = bus.receive(1000);  //Just handle the adoption right here (time in microseconds)
    } else
      Serial.println("Dispenser " + String(i) + " not connected, code=" + PJONresultToString(result));
  }
}
void pingOnline() {  //We can call this code periodicaly this will check if all dispensers are still attached
  Serial.println("pingOnline " + String(Dispensers_Amount));
  static uint8_t BusSend[] = { UNKNOWN, 0 };
  for (uint8_t i = 1; i < Dispensers_Amount; i++) {
    if (Dispensers[i].IngredientID != 0) {  //Only checks slots that should be connected and respond
      Serial.println("Checking in on dispenser " + String(i));
      uint16_t result = bus.send_packet_blocking(i, BusSend, sizeof(BusSend));  //Check if they are all still online, to make sure also retry if we fail the first time
      if (result != PJON_ACK and result != 154) {                               //No clue what error 154 is, but we keep running into it on normal conditions
        Serial.println("dispenser" + String(i) + " went offline");
        Dispensers[i].IngredientID = 0;
        LcdPrint("BUS error:" + String(i), PJONresultToString(result));
      }
    }
  }
}
void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  Serial.print("Received: ");
  for (uint16_t i = 0; i < length; i++)
    Serial.print(payload[i] + String(" "));
  Serial.println("From_ID=" + String(packet_info.tx.id));
  if (length == 2) {
    switch (payload[0]) {  // Check first uint8_t as a command ID
      case DISPENSERSTATUS:
        Dispensers[packet_info.tx.id].IngredientID = payload[1];
        Serial.println("Dispenser " + String(packet_info.tx.id) + " has fluid=" + String(payload[1]));
        break;
      case ADOPT:
        LcdPrint("Disp booted:" + String(packet_info.tx.id), "scanning bus");
        pingAll();
        break;
      default:
        LcdPrint("Disp:" + String(packet_info.tx.id), "unk command" + String(payload[0]));
        break;
    }
  }
}
void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  if (code == PJON_CONNECTION_LOST)
    LcdPrint("BUS conn lost", "ID=" + String(bus.packets[data].content[0]));
  if (code == PJON_PACKETS_BUFFER_FULL)
    LcdPrint("BUS buffer full", "length=" + String(data));
  if (code == PJON_CONTENT_TOO_LONG)
    LcdPrint("BUS too long", "length=" + String(data));
};