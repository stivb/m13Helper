  
#include <ClickEncoder.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>

int16_t oldEncPosA, encPosA;
int16_t oldEncPosB, encPosB;
uint8_t buttonStateA,buttonStateB;

#define pinA A2
#define pinB A1
#define pinSw A0 //switch
#define STEPS 4

ClickEncoder *encoderA;
ClickEncoder *encoderB;

byte values[144];

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void timerIsr() {
  encoderA->service();
  encoderB->service();
}


void setup() {


  Serial.begin(9600);
  encoderA = new ClickEncoder(pinA, pinB, pinSw, STEPS);
  //encoderA->setAccelerationEnabled(true);
  encoderB = new ClickEncoder(A5, A4, A3, STEPS);
  //encoderB->setAccelerationEnabled(true);

  Serial.print("Acceleration is ");
  Serial.println((encoderA->getAccelerationEnabled()) ? "enabled" : "disabled");
  Serial.println((encoderB->getAccelerationEnabled()) ? "enabled" : "disabled");

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 

  oldEncPosA = -1;
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  
}

void loop() {

  encPosA += encoderA->getValue();
  if (encPosA<0) {lcd.clear(); encPosA=143;lcd.print(encPosA);}
  if (encPosA>143) {lcd.clear(); encPosA=0; lcd.print(encPosA);}

  if (encPosA != oldEncPosA) {
    oldEncPosA = encPosA;
    Serial.print("Encoder Value A: ");
    Serial.println(encPosA);
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print(encPosA+1);
  }


  
  encPosB += encoderB->getValue();

  if (encPosB != oldEncPosB) {
    oldEncPosB = encPosB;
    Serial.print("Encoder Value B: ");
    Serial.println(encPosB);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print(encPosB+1);
  }
  
  buttonStateA = encoderA->getButton();
  buttonStateB = encoderB->getButton();

  if (buttonStateA==5) buttonAClicked();
  if (buttonStateA==4) buttonALongClicked();

  if (buttonStateB==5) buttonBClicked();
  if (buttonStateB==4) buttonBLongClicked();
 
}

void buttonARotated() 
{
  char s[9];
  numberToFSR(encPosA,s);
  Serial.print(s);
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("FX: ");
  lcd.print(s);
}
void buttonAClicked(){}


void buttonBRotated()
{
  char s[9];
  numberToFSR(encPosB,s);
  Serial.print(s);
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("VFX: ");
  lcd.print(s);
}
void buttonBClicked(){}


void buttonALongClicked() {}
void buttonBLongClicked() {}

void numberToFSR(byte num, char* ostr)
{
  char str[10];
  byte f = (byte)num / 36;
  byte s = (byte) (  (num % 36)/3);
  byte r = num %3 ;
  if (s+1<10) sprintf(str,"F%d:S0%d:R%d\0",f+1,s+1,r+1);
  else sprintf(str,"F%d:S%d:R%d\0",f+1,s+1,r+1);
  Serial.println(str);
  for(int i=0; i < 9; ++i)ostr[i] = str[i];
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); 
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
