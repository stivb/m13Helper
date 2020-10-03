  
#include <ClickEncoder.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>
#include <PS2KeyAdvanced.h>
#include <EEPROM.h>

int16_t oldEncPosA, encPosA;
int16_t oldEncPosB, encPosB;
uint8_t buttonStateA,buttonStateB;


#define pinA A2
#define pinB A1
#define pinSw A0 //switch
#define STEPS 4

ClickEncoder *encoderA;
ClickEncoder *encoderB;

byte vfxValues[144];

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void timerIsr() {
  encoderA->service();
  encoderB->service();
}

byte row1[8] = {
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte row2[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000
};

byte row3[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};




void setup() {


  Serial.begin(9600);

  EEPROM.get( 0, vfxValues );
  for (int i=0;i<144;i++) if (vfxValues[i]==255) vfxValues[i]=0;
  
  encoderA = new ClickEncoder(pinA, pinB, pinSw, STEPS);
  encoderA->setAccelerationEnabled(false);
  encoderB = new ClickEncoder(A5, A4, A3, STEPS);
  encoderB->setAccelerationEnabled(false);

  Serial.print("Acceleration is ");
  Serial.println((encoderA->getAccelerationEnabled()) ? "enabled" : "disabled");
  Serial.println((encoderB->getAccelerationEnabled()) ? "enabled" : "disabled");

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 

  oldEncPosA = 0;
  oldEncPosB = 0;
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  printA();
  printB();
}

void clearLcdLine(int num)
{
  lcd.setCursor(0, num);
  lcd.print("                ");
  lcd.setCursor(0, num);
}

void loop() {
  handleRotation();  
  handleRotationStatus();
  handleRotaryButtons();
  handleKeyboard();
  
}

void printToRow(char* str, int num)
{
  lcd.setCursor(0,num);
  lcd.print(str);
}

void printA()
{
  clearLcdLine(0);
  char str[7];
  numberToFSR(encPosA,str);
  lcd.setCursor(0,0);
  lcd.print(str);
  char a = 175;
  lcd.print(a);
}

void printB()
{
  int virtualFxNum;
  clearLcdLine(1);
  printToRow("VFX ",1);
  lcd.print(encPosB+1);
  lcd.print("=");
  virtualFxNum=vfxValues[encPosB];

  Serial.print(encPosB);
  Serial.print("****");
  Serial.println(virtualFxNum);
  if (virtualFxNum>=0)
    {
      char vstr[7];
      numberToFSR(virtualFxNum,vstr);
      lcd.print(vstr);
    }
}

void buttonAClicked(){}
void buttonBClicked(){}


void buttonALongClicked() {}
void buttonBLongClicked() 
{
  vfxValues[encPosB] = encPosA;
  Serial.print(encPosB);
  Serial.println(vfxValues[encPosB]);
  printB();
  EEPROM.put( 0, vfxValues );
}



void numberToFSR(int num, char* ostr)
{
  char str[7];
  int f = (int)num / 36;
  int s = (int) (  (num % 36)/3);
  int r = num %3 ;
  if (s+1<10) sprintf(str,"%d:0%d:%d\0",f+1,s+1,r+1);
  else sprintf(str,"%d:%d:%d\0",f+1,s+1,r+1);
  Serial.println(str);
  for(int i=0; i < sizeof(str)/sizeof(str[0]); ++i)ostr[i] = str[i];
}

char getRowChar(int rowVal)
{
  switch(rowVal)
  {
    case 0: return 175;
    case 1: return '-';
    case 2: return '_';
  }
  return '^';
  }
