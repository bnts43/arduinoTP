boolean ledIsRed = true;

// switch leds from green to red or vice-versa

void switchLeds() {
    if (ledIsRed) {
      digitalWrite(Led_Green,LOW); // Green LED will be switched off
      delay(400);
      digitalWrite(Led_Red,HIGH); // Red LED will be switched on
      delay(400);
    } else {
      digitalWrite(Led_Red,LOW); // Red LED will be switched off
      delay(400);
      digitalWrite(Led_Green,HIGH); // Green LED will be switched on    
      delay(400);
    }
    ledIsRed = !ledIsRed;
}
