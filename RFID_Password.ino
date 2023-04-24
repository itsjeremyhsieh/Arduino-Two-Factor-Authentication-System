
#include "Adafruit_Keypad.h"
#include "string.h"
#include <LiquidCrystal_PCF8574.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522;
char *reference;

struct users {  //a structure for login info
  byte uid[4];
  String name;
  String password;
};

struct users user_list[] = {  //users info
  { { 67, 23, 161, 165 }, "Jeremy", "0107" },
  { { 3, 186, 183, 170 }, "Sophia", "0813" }
};

byte totalTags = sizeof(user_list) / sizeof(users);

LiquidCrystal_PCF8574 lcd(0x27);
const byte ROWS = 4;
const byte COLS = 4;
//define keys
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 8, 7, 6, 5 };
byte colPins[COLS] = { 4, 3, 2, };

//initialize keypad
Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
int i = 0;
bool set = false;
bool scan_card = false;
String psw = "";
int user_idx = 0;

void denied() {
  digitalWrite(A2, HIGH);
  digitalWrite(A1, HIGH);
  delay(100);
  digitalWrite(A1, LOW);
  delay(100);
  digitalWrite(A1, HIGH);
  delay(100);
  digitalWrite(A1, LOW);
  delay(500);
  digitalWrite(A2, LOW);
}

void access() {
  digitalWrite(A3, HIGH);
  digitalWrite(A1, HIGH);
  delay(100);
  digitalWrite(A1, LOW);
  delay(100);
  digitalWrite(A1, HIGH);
  delay(100);
  digitalWrite(A1, LOW);
  delay(100);
  digitalWrite(A1, HIGH);
  delay(100);
  digitalWrite(A1, LOW);
  delay(500);
  digitalWrite(A3, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A1, OUTPUT);

  SPI.begin();
  mfrc522.PCD_Init(SS_PIN, RST_PIN);  // 初始化MFRC522卡

  customKeypad.begin();
  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please scan card");
}

void loop() {
  if (!scan_card) {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      byte *id = mfrc522.uid.uidByte;
      byte idSize = mfrc522.uid.size;
      /* for(int j = 0 ; j < 4 ; j ++)
      {
        Serial.println(id[j]);
      }*/
      bool match = false;
      for (byte i = 0; i < totalTags; i++) {
        if (memcmp(user_list[i].uid, id, idSize) == 0) {
          match = true;
          user_idx = (int)i;
          break;
        }
      }
      lcd.clear();
      if (match) {
        scan_card = true;
        access();
        lcd.clear();
      } else {
        lcd.print("Access Denied");
        denied();
        delay(1000);
        lcd.clear();
        lcd.print("Please scan card");
      }
      mfrc522.PICC_HaltA();
    }
  }

  if (scan_card == true && set == false) {
    customKeypad.tick();
    lcd.setCursor(0, 1);
    lcd.setCursor(0, 0);
    lcd.print("Input Password");

    while (customKeypad.available()) {
      keypadEvent e = customKeypad.read();
      if (e.bit.EVENT == KEY_JUST_PRESSED) {
        if ((char)e.bit.KEY == '*' && (char)e.bit.KEY != '#' && i > 0) {  //delete
          i -= 1;
          lcd.setCursor(i, 1);
          lcd.print(" ");
          psw = psw.substring(0, psw.length() - 1);
          digitalWrite(A1, HIGH);
          delay(100);
          digitalWrite(A1, LOW);
        } else if ((char)e.bit.KEY != '*' && (char)e.bit.KEY != '#') {  //enter psw
          lcd.setCursor(i, 1);
          lcd.print((char)e.bit.KEY);
          i += 1;
          psw = psw + (char)e.bit.KEY;
          digitalWrite(A1, HIGH);
          delay(100);
          digitalWrite(A1, LOW);
        } else if ((char)e.bit.KEY == '#') {  //enter
          lcd.clear();
          lcd.setCursor(0, 0);
          set = true;

        } else if ((char)e.bit.KEY == '*' && i == 0) {  //back to previous
          scan_card = false;
          set = false;
          lcd.clear();
          lcd.print("Please scan card");
        }
      }
    }
  }

  if (set) {
    if (user_list[user_idx].password == psw) {
      lcd.setCursor(0, 0);
      lcd.print("Welcome,");
      lcd.setCursor(0, 1);
      lcd.print(user_list[user_idx].name);
      access();
      delay(2000);
      lcd.setCursor(0, 0);
      lcd.clear();
      psw = "";
      set = false;
      scan_card = false;
      i = 0;
      lcd.print("Please scan card");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Password");
      lcd.setCursor(0, 1);
      lcd.print("Incorrect");
      denied();
      lcd.setCursor(0, 0);
      lcd.clear();
      psw = "";
      set = false;
      i = 0;
    }
  }
  delay(50);
}
