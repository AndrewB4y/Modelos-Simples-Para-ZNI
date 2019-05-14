#include <CRC.h>

//               dir  func   reg_ini     no_regs      CRC16
// 0304000000027029
byte frame[] = {0x1F, 0x03, 0x10, 0x02, 0x00, 0x02, 0x00, 0x00};
//byte frame[] = {0x03, 0x03, 0x00, 0x00, 0x00, 0x02, 0x48, 0x45};

int cont = 0;
void setup() {
  // put your setup code here, to run once:
  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);
  Serial1.begin(4800);
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(32, OUTPUT);
  cont = 1;
  while (Serial1.available())Serial1.read();

  uint16_t crc =  CRC::crc16(frame, sizeof(frame)-2);
  frame[sizeof(frame)-2] = crc & 0xff;
  frame[sizeof(frame)-1] = (crc >> 8);
  for (int i = 0; i < sizeof(frame); i++)
  {
    Serial.println(frame[i],HEX);
  }
  Serial.println("SEND");
}

void loop() {
  while (cont) {
    // put your main code here, to run repeatedly:
    delay(3000);
    digitalWrite(8, HIGH);
    Serial1.write(frame, 8);
    //delayMicroseconds(33000);   //para 2400baud
    delayMicroseconds(17000);     //para 4800baud
    digitalWrite(8, LOW);
    delay(200);

    
    while (!Serial1.available());
    if (Serial1.available()) {
      cont = 1;
      digitalWrite(32, HIGH);
      while (Serial1.available()) {
        char inByte = Serial1.read();
        Serial.println(inByte, HEX);
      }
      digitalWrite(32, LOW);
      Serial.println("REND");
    }
    
  }
}


//void serialEvent1() {
//  //statements
//  cont = 0;
//  digitalWrite(32, HIGH);
//  while (Serial1.available()) {
//    int inByte = Serial1.read();
//    Serial.print(inByte, HEX);
//  }
//  digitalWrite(32, LOW);
//  Serial.print(F("END"));
//}
