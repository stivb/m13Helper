void handleRotation()
{
  encPosA += encoderA->getValue();
  encPosB += encoderB->getValue();
  if (encPosA<0) encPosA=143;
  if (encPosA>143) encPosA=0; 
  if (encPosB<0) encPosB=143;
  if (encPosB>143) encPosB=0; 
}

void handleRotationStatus()
{
    if (encPosA != oldEncPosA) {
    oldEncPosA = encPosA;
    totalPosition = encPosA;
    printA();
  }

  if (encPosB != oldEncPosB) {    
    oldEncPosB = encPosB;
    totalPosition = encPosB;
    printB();
    
  }
}

void handleRotaryButtons()
{
  buttonStateA = encoderA->getButton();
  buttonStateB = encoderB->getButton();

  if (buttonStateA==5) buttonAClicked();
  if (buttonStateA==4) buttonALongClicked();

  if (buttonStateB==5) buttonBClicked();
  if (buttonStateB==4) buttonBLongClicked();
}

void handleKeyboard()
{
  if( keyboard.available() )
    {
    // read the next key
    uint16_t c = keyboard.read();
    if (c<=0) return;
    char cc = (char) c;
    int keypressUpDownValue=(int) (c >> 8);
    bool isDownPress = keypressUpDownValue<20;  
    handleFootPedal(cc,keyboard.getLock(), isDownPress);
    }
}
