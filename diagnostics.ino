void fsr_diagnostics(int f, int s, int r)
{
  if (!debug_mode) return;
  char data[20];
  sprintf(data, "F %d S %d R %d", f, s, r);
  Serial.println(data);
}

void key_diagnostics(char kp, uint8_t lk, bool isDownPress)
{
  if (!debug_mode) return;
  int keypedalled = int(kp);
  Serial.print("kp: "); Serial.print(kp);
  Serial.print(" lk: "); Serial.print(lk);
  Serial.print(" keypedalled: "); Serial.print(keypedalled);
  Serial.print(" isDownPress: "); Serial.print(isDownPress);
}
