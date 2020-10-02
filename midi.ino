void clearAllFx()
{
  if (debug_mode) return;
  for (int i=0;i<12;i++)
    {
      controlChange(channel,fx[i],0x01);
    }
  delay(defDelay);
}



void Midi_Send(byte cmd, byte data1, byte data2) {
  Serial.write(cmd);
  Serial.write(data1);
  Serial.write(data2);
}


void M13_Record()
{
  if (debug_mode) {Serial.println("STARTING RECORDING");printstate();}
  controlChange(channel,0x32,0x50);
}

void M13_Overdub()
{
  if (debug_mode) {Serial.println("STARTING OVERDUBBING");printstate();}
  controlChange(channel,0x32,0x00);
}

void M13_Pre()
{
  if (debug_mode) Serial.println("SHIFT KEY");
  controlChange(channel,0x54,0x00);
  isPre=true;
}

void M13_Post()
{
  controlChange(channel,0x54,0x7F);
  isPre=false;
}

void M13_PrePost()
{
  if (isPre) M13_Post();
  else M13_Pre();
}

void M13_Play()
{
  if (debug_mode) {Serial.println("STARTING PLAYING");printstate();}
  controlChange(channel,0x1C,0x50);
}

void M13_Stop()
{
  if (debug_mode) {Serial.println("STARTING STOP");printstate();}
  controlChange(channel,0x1C,0x00);
}

void M13_ReverseToggle()
{
  controlChange(channel,0x55,0x00);
}

void M13_UndoRedo()
{
  if (debug_mode) {Serial.println("STARTING UNDOREDO");printstate();}
  controlChange(channel,0x52,0x7F);
}

void M13_PlayOnce()
{
  if (debug_mode) {Serial.println("STARTING PLAYONCE");printstate();}
  controlChange(channel,0x50,0x00);
}


void controlChange(byte channel, byte control, byte value) {
  Serial.write(0xB0 | channel);
  Serial.write(control);
  Serial.write(value);
}

void programChange(byte channel, byte programNumber) {

  Serial.write(0xC0 | channel);
  Serial.write(programNumber);
}

void noteOn(byte channel, byte pitch, byte velocity) {
  Serial.write(0x90 | channel);
  Serial.write(pitch);
  Serial.write(velocity);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  Serial.write(0x80 | channel);
  Serial.write(pitch);
  Serial.write(velocity);
}

void printstate()
{
  if (!debug_mode) return;
  Serial.print("State: "); Serial.print(state);
}

void printstateafter()
{
  if (!debug_mode) return;
  Serial.print("StateAfter: "); Serial.print(state);
} 
