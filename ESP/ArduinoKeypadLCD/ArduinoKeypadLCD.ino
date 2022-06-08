#include <MultitapKeypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
LiquidCrystal_I2C lcd(0X27,16,2); //SCL A5 SDA A4
const byte RX = 2;
const byte TX = 3;
SoftwareSerial mySerial = SoftwareSerial(RX,TX);

const byte ROW0 = 4;
const byte ROW1 = 5;
const byte ROW2 = 6;
const byte ROW3 = 7;
const byte COL0 = 8;
const byte COL1 = 9;
const byte COL2 = 10;
const byte COL3 = 11;





const byte CHR_BOUND = 3;
const byte BACKSPACE = 8;
const byte CLEARSCREEN = 12;
const byte CARRIAGE_RETURN = 13;
const byte CAPSLOCK_ON = 17;
const byte CAPSLOCK_OFF = 18;
const byte NUMLOCK_ON = 19;
const byte NUMLOCK_OFF = 20;

const char SYMBOL[] PROGMEM = {
  ',', '.', ';', ':', '!',
  '?', '\'', '"','-', '+',
  '*', '/', '=', '%', '^',
  '(', ')', '#', '@', '$',
  '[', ']', '{', '}', CHR_BOUND
};
const char ALPHABET[] PROGMEM = {
  'A', 'B', 'C', CHR_BOUND, CHR_BOUND,
  'D', 'E', 'F', CHR_BOUND, CHR_BOUND,
  'G', 'H', 'I', CHR_BOUND, CHR_BOUND,
  'J', 'K', 'L', CHR_BOUND, CHR_BOUND,
  'M', 'N', 'O', CHR_BOUND, CHR_BOUND,
  'P', 'Q', 'R', 'S',       CHR_BOUND,
  'T', 'U', 'V', CHR_BOUND, CHR_BOUND,
  'W', 'X', 'Y', 'Z',       CHR_BOUND
};

boolean alphaMode = true;
boolean upperCaseMode = true;
boolean autoOffBacklight = false;
boolean isEndOfDisplay = false;
byte startCursorPos;
byte endCursorPos;
byte cursorPos;
byte chrCtr;
unsigned long backlightTimeout;
char strBuffer[16];
String nameWifi="";

MultitapKeypad kpd(ROW0, ROW1, ROW2, ROW3, COL0, COL1, COL2, COL3);
Key key;

void setup() {
  kpd.attachFunction(otherProcess);

  lcd.init();
  lcd.clear();                    
  lcd.backlight(); 
  Serial.begin(9600);
  mySerial.begin(19200);
  
  Serial.println(F("Ket noi Wifi"));
  lcd.print(F("Ket noi Wifi"));
  delay(2000);
  lcd.clear();
  lcd.cursor();
  lcd.blink();

  lcd.setCursor(0, 1);
  lcd.print(F("Wifi:"));
  getString(21, 32);
  Serial.print(F("Wifi: "));
  Serial.println(strBuffer);
  
  mySerial.print(strBuffer);
  mySerial.flush();
  delay(100);

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(F("PW:"));
  getString(19, 32);
  Serial.print(F("Password: "));
  Serial.println(strBuffer);
  mySerial.print(strBuffer);
  mySerial.flush();
  delay(100);
  lcd.clear();
  lcd.noCursor();
  lcd.noBlink();
  lcd.print(F("Connecting"));
//  lcd.setCursor(0, 1);
//  lcd.print(F("Serial Monitor"));
  while(true){
    if(mySerial.available()){
    nameWifi=mySerial.readString();
    lcd.clear();
    lcd.print("Dang hoat dong");
    lcd.setCursor(0, 1);
    lcd.print(nameWifi);
    break;
    }
    
  }
  
}

void loop() {
  delay(1000);
}

char getAKey() {
  while(true) {
    key = kpd.getKey();
    if (key.state == KEY_DOWN || key.state == MULTI_KEY_DOWN)
      return key.character;
  }
}

void waitUntilUnpressed() {
  while(true) {
    key = kpd.getKey();
    if (key.state == KEY_UP)
      return;
  }
}

void getString(byte startPos, byte endPos) {
  char chr;
  byte strSize = endPos - startPos + 1;
  startCursorPos = startPos;
  endCursorPos = endPos;
  cursorPos = startPos;
  setCursorPos();
  chrCtr = 0;
  displayInputMode();
  boolean isCompleted = false;
  while (true) {
    key = kpd.getKey();
    switch (key.state) {
      case KEY_DOWN:
      case MULTI_TAP:
        digitalWrite(LED_BUILTIN, HIGH);
        switch (key.code) {
          case KEY_1:
            chr = alphaMode ? getSymbol(key.tapCounter) : '1';
            break;
          case KEY_ASTERISK:
            chr = alphaMode ? 0 : key.character;
            break;
          case KEY_0:
            chr = alphaMode ? ' ' : '0';
            break;
          case KEY_NUMBER_SIGN:
            if (alphaMode) {
              upperCaseMode = !upperCaseMode;
              chr = upperCaseMode ? CAPSLOCK_ON : CAPSLOCK_OFF;
            }
            else
              chr = key.character;
            break;
          case KEY_A:
            chr = BACKSPACE;
            break;
          case KEY_B:
          case KEY_C:
            chr = 0;
            break;
          case KEY_D:
            strBuffer[chrCtr] = 0;
            isCompleted = true;
            break;
          default:
            chr = alphaMode ? getAlphabet(key.character, key.tapCounter) : key.character;
        }
        if (isCompleted)
          break;
        if (!upperCaseMode && chr >= 'A')
          chr += 32;
        if (key.state == MULTI_TAP && alphaMode && key.character >= '1' && key.character <= '9') {
          printToLcd(BACKSPACE);
          chrCtr--;
        }
        if (chr > 0) {
          if (chr == BACKSPACE)
            chrCtr--;
          if (chr >= ' ') {
            strBuffer[chrCtr] = chr;
            if (chrCtr < strSize)
              chrCtr++;
          }
          printToLcd(chr);
        }
        autoOffBacklight = false;
        break;
      case LONG_TAP:
        switch (key.code) {
          case KEY_A:
            chr = CLEARSCREEN;
            break;
          case KEY_NUMBER_SIGN:
            if (alphaMode)
              upperCaseMode = !upperCaseMode;
            alphaMode = !alphaMode;
            chr = alphaMode ? NUMLOCK_OFF : NUMLOCK_ON;
            break;
          default:
            chr = key.character;
        }
        if (chr < ' ' || alphaMode && chr >= '0' && chr <= '9') {
          if (chr >= '0' || chr == NUMLOCK_OFF) {
            printToLcd(BACKSPACE);
            chrCtr--;
          }
          if (chr == CLEARSCREEN)
            chrCtr = 0;
          if (chr >= ' ') {
            strBuffer[chrCtr] = chr;
            if (chrCtr < strSize)
              chrCtr++;
          }
          printToLcd(chr);
        }
        break;
      case MULTI_KEY_DOWN:
//        tone(BEEP, 4000, 100);
        digitalWrite(LED_BUILTIN, LOW);
        break;
      case KEY_UP:
        digitalWrite(LED_BUILTIN, LOW);
//        backlightTimeout = millis() + BACKLIGHT_PERIODS;
        autoOffBacklight = true;
        if (isCompleted)
          return;
    }
  }
}

void otherProcess(void) {
  if (autoOffBacklight && millis() >= backlightTimeout) {
    autoOffBacklight = false;
  }
}

void printToLcd(char chr) {
  switch (chr) {
    case BACKSPACE:
      if (cursorPos > startCursorPos) {
        if (!isEndOfDisplay)
          decCursorPos();
        setCursorPos();
        lcd.print(F(" "));
        setCursorPos();
        isEndOfDisplay = false;
      }
      break;
    case CLEARSCREEN:
      while (cursorPos > startCursorPos) {
        decCursorPos();
        setCursorPos();
        lcd.print(F(" "));
        setCursorPos();
      }
      break;
    case CAPSLOCK_ON:
    case CAPSLOCK_OFF:
    case NUMLOCK_ON:
    case NUMLOCK_OFF:
      displayInputMode();
      break;
    default:
      if (cursorPos == endCursorPos)
        isEndOfDisplay = true;
      lcd.print(chr);
      incCursorPos();
      setCursorPos();
  }
}

void displayInputMode(void) {
  lcd.setCursor(13, 0);
  if (alphaMode) {
    if (upperCaseMode)
      lcd.print(F("ABC"));
    else
      lcd.print(F("abc"));
  }
  else
    lcd.print(F("123"));
  setCursorPos();
}

void incCursorPos(void) {
  if (cursorPos < endCursorPos) cursorPos++;
}

void decCursorPos( void ) {
  if (cursorPos > startCursorPos) cursorPos--;
}

void setCursorPos( void ) {
  lcd.setCursor(cursorPos % 16, cursorPos / 16);
}

byte getSymbol(byte ctr) {
  byte sym = pgm_read_byte_near(SYMBOL + ctr);
  if (sym == CHR_BOUND) {
    sym = pgm_read_byte_near(SYMBOL);
    kpd.resetTapCounter();
  }
  return sym;
}

byte getAlphabet(byte chr, byte ctr) {
  chr = (chr - '2') * 5;
  byte alpha = pgm_read_byte_near(ALPHABET + chr + ctr);
  if (alpha == CHR_BOUND) {
    alpha = pgm_read_byte_near(ALPHABET + chr);
    kpd.resetTapCounter();
  }
  return alpha;
}
