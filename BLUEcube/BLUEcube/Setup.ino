void SetupPins(){
  
  pinMode(buttonPin, INPUT);
  pinMode(bluelatch, OUTPUT);
  pinMode(blueclock, OUTPUT);
  pinMode(bluedata, OUTPUT);

  for (int i=0; i<6; i++) {
    pinMode(2+i, OUTPUT);
  }
}
