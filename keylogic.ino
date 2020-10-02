void handleFootPedal(char kp, uint8_t lk, bool isDownPress)
{
  
  int keypedalled = int(kp);
  
  key_diagnostics( kp,  lk,  isDownPress);

  if (keypedalled==KEY_FIVE)
  {
    if (isDownPress)  nextFSR();
    else changeSound();
    return;
  }

  if (keypedalled==KEY_FOUR)
  {
    if (isDownPress)  prevFSR();
    else changeSound();
    return;
  }

  //has to be put here to stop any handling of down presses hereafter
  if (!isDownPress) return;
  if (keypedalled!=KEY_ONE && keypedalled!=KEY_TWO && keypedalled!=KEY_THREE) return;

  int nextActionIndex = getNextActionLogic(lk, keypedalled);
  actionNextAction(nextActionIndex, lk);

}

int nextFSR()
{

    if (totalPosition<143) totalPosition++;
    else totalPosition=0;
    showFSR();
    
}

int prevFSR()
{
  if (totalPosition>0) totalPosition--;
  else totalPosition=143;
  showFSR(); 
  
}

void showFSR()
{
    folderSelected = (int)totalPosition / 36;
    sceneSelected = (int) (  (totalPosition % 36)/3);
    rowSelected = totalPosition %3 ;
    showStatus();
}


int getNextActionLogic(int mode, int key)

{
  int i;

  if (debug_mode) { Serial.print("Mode is "); Serial.print(mode);Serial.print(" Key is "); Serial.println(key);}
  for (i=0;i<30;i++)
  {
    if (logics[i].currState==state && logics[i].currMode==mode && logics[i].currKey==key) return i;
  }
  return -1;
}



int actionNextAction(int logicSeqAt, byte state)
{
  
  int nextState = logics[logicSeqAt].nextState;
  int effectInc = logics[logicSeqAt].effectInc;
  if (debug_mode) {Serial.println("EFFECTINC = "); Serial.println(effectInc);}
  
  enactState(nextState);
  
  if (effectInc!=0)
    {
    incFSR(effectInc);
    changeSound();
    }
  
}

int incFSR(int by)
{
  int newTotPosition = totalPosition + by;
  totalPosition = newTotPosition %144;
  showFSR();
}

int enactState(int newState)
{

  if (debug_mode)
  {
  Serial.print("Current state: "); Serial.print(state);
  Serial.print(" Next state: ");Serial.println(newState);
  }

  
  if (state==newState) return;
  int prevState = state;
  
  

  switch (newState)
  {
    case STOPPED:
    if (prevState==RECORDING) loopEndedAt = millis();
    M13_Stop();
    break;

    case PLAYING:
    if (prevState==RECORDING) loopEndedAt = millis();
    M13_Play();
    break;

    case RECORDING:
    if (prevState==STOPPED) loopStartedAt = millis();
    M13_Record();
    break;

    case OVERDUBBING:
    if (prevState==RECORDING) loopEndedAt = millis();
    M13_Overdub();
    break;

    case PLAYING_ONCE:
    if (prevState==RECORDING) loopEndedAt = millis();
    M13_PlayOnce();
    break;

    case UNDO:
    M13_UndoRedo();
    break;
    
  }
  if (newState!=UNDO) state = newState;

}

void changeSound()
{
  clearAllFx();
  gotoFolderScene(folderSelected,sceneSelected);
  activateRow(rowSelected);
  totalPosition = folderSelected*36 + sceneSelected*3 + row; 
  showStatus();
}

void tot2FSRStr(char* outStr, int f, int s, int r)
{
  char str[7];
  if (sceneSelected+1<10) sprintf(str,"%d:0%d:%c",f+1,s+1,getRowChar(r));
  else sprintf(str,"%d:%d:%c",f+1,s+1,getRowChar(r));
  for(int i=0; i < 7; ++i){
    outStr[i] = str[i];
  }
}


void showStatus()
{
  char str[7];
  char vstr[7];
  lcd.clear();  
  lcd.setCursor(0, 0);  
  tot2FSRStr(str,folderSelected,sceneSelected,rowSelected);
  lcd.print(str);
}

void gotoFolderScene(int f, int s)
{
if (debug_mode) {Serial.print("gotoFolderScenef=");Serial.print(f);Serial.print(" s=");Serial.print(s);}
programChange(channel,scenes[f][s]);
delay(defDelay);
}

void activateRow(int startAt)
{
//startAt can be 0,1,2
int i;
int btnPos = startAt;
for (i=0;i<4;i++)
{
  controlChange(channel,fx[btnPos],0x70);
  btnPos+=3;
}
}
