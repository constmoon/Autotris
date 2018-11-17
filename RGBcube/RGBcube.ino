#include <Boards.h>

#define GRID_W           (5)
#define GRID_H           (5)
#define PIECE_W          (4)
#define PIECE_H          (4)

int buttonPin = 13;
int old_button ;
int pushed = 0;
int level = 6;
int select = 1;
int e;

int data = 0;
int index[] = {};

//-----------------[anode] 
// RED
int redData = 9;
int redLatch = 10;
int redClock = 11;

//GREEN
int greenData = A3; // DATA
int greenLatch = A4; // STCP
int greenClock = A5; // SHCP

//BLUE
int blueData = A0;
int blueLatch = A1;
int blueClock = A2;

//-----------------[cathode}
int layerPin[5] = { 2, 3, 4, 5, 6 };

int pattern = ' ';    // 출력 패턴
int cnt = 0;
char recv = ' ';       // incoming byte
char dir;        // 방향


// 프로그램 시작 - 초기화
void setup() { 
 Serial.begin(9600); //시리얼 통신 초기화

// PUSH BUTTON
 pinMode(buttonPin, INPUT);
 
 for(int i=0; i<5; i++){
  pinMode(layerPin[i], OUTPUT);
 }
// pinMode(2, OUTPUT);   // 1층 2번핀
// pinMode(3, OUTPUT);   // 2층 3번핀
// pinMode(4, OUTPUT);   // 3층 4번핀
// pinMode(5, OUTPUT);   // 4층 5번핀
// pinMode(6, OUTPUT);   // 5층 6번핀
 
 pinMode(greenData, OUTPUT);
 pinMode(greenLatch, OUTPUT);
 pinMode(greenClock, OUTPUT);

 pinMode(blueData, OUTPUT);
 pinMode(blueLatch, OUTPUT);
 pinMode(blueClock, OUTPUT);

 pinMode(redData, OUTPUT);
 pinMode(redLatch, OUTPUT);
 pinMode(redClock, OUTPUT);
}


// 계속 실행할 무한 루프
void loop(){
  
  int new_button = digitalRead(buttonPin); 
  
//  index[data] = e;
//    data = data + 1;

  if(new_button > 0 && old_button != new_button) 
  {
    pushed += 1;
    if(pushed > 1)
      pushed = 0;
  }
  old_button = new_button;
  Serial.print(pushed);
  Serial.print(", ");
    
  if(pushed == 1){
    Serial.print(" Pushed ");
    select += 0;
    e = select;

    level -= 1;
    if(level < 1){
      level = 1;
    }
    delay(500);
//    delayMicroseconds(5);
    updateShiftRegisterAnode(level); 
    updateShiftRegisterCathode(select);
    updateShiftRegisterCathode(index[data]);
    Serial.print(", ");
    Serial.print(level);
    Serial.print(", ");
    Serial.println(select);
  
  }else if(pushed == 0){
    Serial.print("unPushed");
    Serial.print(", ");
    Serial.print(level);
    Serial.print(", ");
    Serial.println(select);
  }
  
  if(level == 1){
    pushed = 0;
    level = 6;
  }else if(level == 6){
    Serial.print("unPushed");
    
    select = random(1, 5);
    if(select > 5){
      select = 1;
    }
    Serial.print(", ");
    Serial.print(level);
    Serial.print(", ");
    Serial.println(select);

//      level == 1 일때, 배경 GRID에 저장하기 ---- level이 6으로 리셋될때마다 같이 리셋됨;
//      if(level == 1){
//        groundGrid(select);
//      }

  }
}

// 5*5 이미지 
// byte 1:off  0:On

byte image1[5]={  
   B11111,
   B11111,
   B11011,
   B10001,
   B11111
};

byte image2[5]={
   B11111,
   B10011,
   B11011,
   B11011,
   B11111
};

byte image3[5]={
   B11111,
   B11011,
   B11011,
   B11011,
   B11011
};

byte image4[5]={
   B11111,
   B11111,
   B11001,
   B10011,
   B11111
};

byte image5[5]={
   B11111,
   B11111,
   B10011,
   B10011,
   B11111
};

byte image6[5]={
   B11111,
   B11111,
   B11111,
   B11111,
   B11111
};

byte imageG[5]={
   B11111,
   B11111,
   B11111,
   B11111,
   B11111
};

byte image[] = {
  
};

void groundGrid(int pattern){

  if(pattern == 1){
    for(int i=0; i<5; i++){
      image[i] = image1[i];
    }
  }else if(pattern == 2){
    for(int i=0; i<5; i++){
      image[i] = image2[i];
    }
  }else if(pattern == 3){
    for(int i=0; i<5; i++){
      image[i] = image3[i];
    }
  }else if(pattern == 4){
    for(int i=0; i<5; i++){
      image[i] = image4[i];
    }
  }else if(pattern == 5){
    for(int i=0; i<5; i++){
      image[i] = image5[i];
    }
  }
  for(int i=0; i<5; i++){
     digitalWrite(redLatch,  LOW);
     shiftOut(redData, redClock, LSBFIRST, imageG[i] - (image6[i] - image[i]));
     digitalWrite(redLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
     digitalWrite(greenLatch, LOW);
     shiftOut(greenData, greenClock, LSBFIRST, imageG[i] - (image6[i] - image[i]));
     digitalWrite(greenLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
     digitalWrite(blueLatch, LOW);
     shiftOut(blueData, blueClock, LSBFIRST, imageG[i] - (image6[i] - image[i]));
    digitalWrite(blueLatch,  HIGH);
  }
}
  
void pattern1(){
  
  for(int i=0; i<5; i++){
     digitalWrite(redLatch,  LOW);
     shiftOut(redData, redClock, LSBFIRST, image1[i]);
     digitalWrite(redLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
     digitalWrite(greenLatch, LOW);
     shiftOut(greenData, greenClock, LSBFIRST, image1[i]);
     digitalWrite(greenLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
     digitalWrite(blueLatch, LOW);
     shiftOut(blueData, blueClock, LSBFIRST, image1[i]);
    digitalWrite(blueLatch,  HIGH);
  }
}
void pattern2(){

  for(int i=0; i<5; i++){
     digitalWrite(redLatch,  LOW);
     shiftOut(redData, redClock, LSBFIRST, image2[i]);
     digitalWrite(redLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
     digitalWrite(greenLatch,  LOW);
     shiftOut(greenData, greenClock, LSBFIRST, image2[i]);
     digitalWrite(greenLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
     digitalWrite(blueLatch,  LOW);
     shiftOut(blueData, blueClock, LSBFIRST, image2[i]);
     digitalWrite(blueLatch,  HIGH);
  }
}
void pattern3(){
  
  for(int i=0; i<5; i++){
    digitalWrite(redLatch,  LOW);
     shiftOut(redData, redClock, LSBFIRST, image3[i]);
     digitalWrite(redLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
    digitalWrite(redLatch,  LOW);
     shiftOut(greenData, greenClock, LSBFIRST, image3[i]);
     digitalWrite(greenLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
    digitalWrite(redLatch,  LOW);
     shiftOut(blueData, blueClock, LSBFIRST, image3[i]);
     digitalWrite(blueLatch,  HIGH);
  }
}
void pattern4(){

  for(int i=0; i<5; i++){
     digitalWrite(redLatch,  LOW);
     shiftOut(redData, redClock, LSBFIRST, image4[i]);
     digitalWrite(redLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
     digitalWrite(greenLatch,  LOW);
     shiftOut(greenData, greenClock, LSBFIRST, image4[i]);
     digitalWrite(greenLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
     digitalWrite(blueLatch,  LOW);
     shiftOut(blueData, blueClock, LSBFIRST, image4[i]);
     digitalWrite(blueLatch,  HIGH);
  }
}
void pattern5(){
// ----------------------------------------------------[ !!! LED ON !!! ]
  for(int i=0; i<5; i++){
      digitalWrite(redLatch,  LOW);
     shiftOut(redData, redClock, LSBFIRST, image5[i]);
     digitalWrite(redLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
     digitalWrite(greenLatch,  LOW);
     shiftOut(greenData, greenClock, LSBFIRST, image5[i]);
     digitalWrite(greenLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
     digitalWrite(blueLatch,  LOW);
     shiftOut(blueData, blueClock, LSBFIRST, image5[i]);
     digitalWrite(blueLatch,  HIGH);
  }
}

void pattern6(){
  // ----------------------------------------------------[ LED OFF ]
  for(int i=0; i<5; i++){
     digitalWrite(redLatch,  LOW);
     shiftOut(redData, redClock, LSBFIRST, image6[i]);
     digitalWrite(redLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
     digitalWrite(redLatch,  LOW);
     shiftOut(greenData, greenClock, LSBFIRST, image6[i]);
     digitalWrite(greenLatch,  HIGH);
  }
  for(int i=0; i<5; i++){
     digitalWrite(redLatch,  LOW);
     shiftOut(blueData, blueClock, LSBFIRST, image6[i]);
     digitalWrite(blueLatch,  HIGH);
  }
}

void updateShiftRegisterCathode(int pattern){
  digitalWrite(redLatch,  LOW);
  digitalWrite(greenLatch,  LOW);
  digitalWrite(blueLatch,  LOW);
  // 패턴 cathode LOW일 때 켜짐(-니까..)

  switch(pattern){
    case 1:
      pattern1();
      break;
    case 2:
      pattern2();  
      break;
    case 3:
      pattern3();  
      break;
    case 4:
      pattern4();  
      break;
    case 5:
      pattern5();  
      break;

// ----------------------------------------------------[ LED OFF ]
    case 6:
      pattern6();
      break;
  }

  digitalWrite(redLatch,  HIGH);
  digitalWrite(greenLatch,  HIGH);
  digitalWrite(blueLatch,  HIGH);
}
// 각 층(layer) 제어
void updateShiftRegisterAnode(int layer){
  for(int i = 0; i < 6; i++)
        digitalWrite(layerPin[i],  LOW);// 레이어가 anode HIGH일 때 켜짐(-니까..)
      
  switch(layer){
    case 1:
      delay(1);
      digitalWrite(layerPin[0],  HIGH);
      break;

    case 2:
      delay(1);
      digitalWrite(layerPin[1],  HIGH);
      break;

    case 3:
      delay(1);
      digitalWrite(layerPin[2],  HIGH);
      break;

    case 4:
      delay(1);
      digitalWrite(layerPin[3],  HIGH);
      break;

    case 5:
      delay(1);
      digitalWrite(layerPin[4],  HIGH);
      break;   
      
// ----------------------------------------------------[ LED OFF ]
    case 6:
    for(int i = 0; i < 6; i++)
      digitalWrite(layerPin[i],  LOW);
      break;  
    }
}
