/*
  debouncer for the button
*/

         byte btnState         = LOW;
         byte lastButtonState  = LOW;
         bool btnFlank         = false;
         byte debounceDelay    = 50;
unsigned long lastDebounceTime = 0;

void debouceTick(bool rst = false) {
  byte reading = digitalRead(btn);
  btnFlank = false;

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

   if ((millis() - lastDebounceTime) > debounceDelay || rst) {
    if (reading != btnState) {
      btnFlank = true;
      btnState = reading;
    }
  }
  lastButtonState = reading;
}
