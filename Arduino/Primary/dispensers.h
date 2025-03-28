void BusSend(uint8_t cmd1, uint8_t cmd2) {
  Serial.println("BusSend " + String(cmd1) + "," + String(cmd2));
  uint8_t BusSend[] = { cmd1, cmd2 };
  bus.send_packet(PJON_BROADCAST, BusSend, sizeof(BusSend));
}
bool BusSendBlocking(uint8_t DispenserID, uint8_t cmd1, uint8_t cmd2) {
  Serial.println("BusSendBlocking " + String(cmd1) + "," + String(cmd2));
  uint8_t BusSend[] = { cmd1, cmd2 };
  uint16_t result = bus.send_packet_blocking(DispenserID, BusSend, sizeof(BusSend));
  if (result != PJON_ACK) {
    LcdPrint("BUS error:" + String(DispenserID), PJONresultToString(result));
    return false;
  }
  return true;
}
void BusAdopt(uint8_t i) {
  static uint8_t BusSend[] = { ADOPT, 0 };
  uint16_t result = bus.send_packet(i, BusSend, sizeof(BusSend));  //Tries to send data 1 time
  //it takes 6327 microseconds to recieve 2 uint8_t and process the response, we run at 1.97kB/s
  if (result == PJON_ACK) {
    uint16_t result2 = bus.receive(1000);  //Just handle the adoption right here (time in microseconds)
    Serial.println("Dispenser " + String(i) + " Connected " + String(result2));
  } else
    Serial.println("Dispenser " + String(i) + " error code " + String(result) + "=" + PJONresultToString(result));
}
void pingAll() {
  Serial.println("pingAll " + String(Dispensers_Amount));
  for (uint8_t i = 1; i < Dispensers_Amount; i++)
    BusAdopt(i);
}
void pingOnline() {  //We can call this code periodicaly this will check if all dispensers are still attached
  Serial.println("pingOnline " + String(Dispensers_Amount));
  static uint8_t BusSend[] = { DONTREPLY, 0 };
  for (uint8_t i = 1; i < Dispensers_Amount; i++) {
    if (Dispensers[i].IngredientID != 0) {  //Only checks slots that should be connected and respond
      Serial.println("Checking in on dispenser " + String(i));
      uint16_t result = bus.send_packet_blocking(i, BusSend, sizeof(BusSend));  //Check if they are all still online, to make sure also retry if we fail the first time
      if (result != PJON_ACK) {
        Dispensers[i].IngredientID = 0;  //Disconnect the dispenser by setting the fluid to 0 (UNK)
        LcdPrint("Dispenser " + String(i), "went offline");
        Serial.println("BUS error:" + String(i) + "=" + PJONresultToString(result) + " Dispenser" + String(i) + " went offline");
      }
    }
  }
}
void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  Serial.print("Received: ");
  for (uint16_t i = 0; i < length; i++)
    Serial.print(payload[i] + String(" "));
  Serial.println("From_ID=" + String(packet_info.tx.id));
  switch (payload[0]) {  // Check first uint8_t as a command ID
    case DISPENSERSTATUS:
      Dispensers[packet_info.tx.id].IngredientID = payload[1];
      Dispensers[packet_info.tx.id].TimeMSML = payload[2];
      Dispensers[packet_info.tx.id].DelayAir = payload[3];
      //Serial.println("Dispenser " + String(packet_info.tx.id) + " has fluid " + String(payload[1]) + "=" + IngredientIDtoString(payload[1]));
      break;
    case ADOPT:
      LcdPrint("Disp " + String(packet_info.tx.id) + " booted", "Scanning bus");
      pingAll();
      LcdPrint("", "Scanned bus");
      break;
    default:
      LcdPrint("Disp:" + String(packet_info.tx.id), "unk command" + String(payload[0]));
      break;
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