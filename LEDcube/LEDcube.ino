#include <FirmataDefines.h>
#include <FirmataConstants.h>
#include <Boards.h>
#include <Firmata.h>
#include <FirmataMarshaller.h>
#include <FirmataParser.h>

//anode
int pinData = A3;   // DATA
int pinLatch = A4;  // STCP
int pinClock = A5;  // SHCP

// cathode
int layerPin[5] = {2,3,4,5,6};
int pattern = 0;       // 출력 패턴
int cnt = 0;           // 회전 토글
char recv = ' ';       // incoming byte
int level = 6;         // 테스트용 층수
int moved = 1;
int rotate = constrain(rotate, 0, 3);  // 숫자 범위 제한

// Pattern state
#define NUM_PIECE_TYPES  (2)
// pointer들의 배열 = 형태들의 배열 
const char *pieces[NUM_PIECE_TYPES] = 
{
  imageL1,
  imageT1
};

char getData(){
  char incomingByte;
  if(Serial.available()){
    incomingByte = Serial.read();
    cnt = 1;
  }
  delay(1);
  return incomingByte;
}

// 5*5 이미지 
// byte 1:ON  0:OFF
void pattern1(){
  for(int i=0; i<5; i++){
     digitalWrite(pinLatch, LOW);
     shiftOut(pinData, pinClock, LSBFIRST, imageL1[i + rotate*5]);
     digitalWrite(pinLatch,  HIGH);
  }
}

void pattern2(){
  for(int i=0; i<5; i++){
     digitalWrite(pinLatch, LOW);
     shiftOut(pinData, pinClock, LSBFIRST, imageT1[i + rotate*5]);
     digitalWrite(pinLatch,  HIGH);
  }
}
void pattern5(){
  byte image5[5]={
       B00000,
       B00000,
       B00000,
       B00000,
       B00000
  };
  for(int i=0; i<5; i++){
     digitalWrite(pinLatch, LOW);
     shiftOut(pinData, pinClock, LSBFIRST, image5[i]);
     digitalWrite(pinLatch,  HIGH);
  }
}

// 입력값에 따른 패턴 출력
void updateShiftRegisterAnode(int pattern){
  switch(pattern){
    case 1:
      pattern1();
      break;
    case 2:
      pattern2();  
      break;
    case 5:
      pattern5();  
      break;
  }
}
// 각 층(layer) 제어
void updateShiftRegisterCathode(int layer){
    switch(layer){
      // 1층
      case 1:
        for(int i=0; i<5; i++){
          if(i==0){ digitalWrite(layerPin[i],  LOW); }
          else{ digitalWrite(layerPin[i],  HIGH); }
        }
        delay(1);
        digitalWrite(layerPin[0],  HIGH);
        break;

      // 2층
      case 2:
        for(int i=0; i<5; i++){
          if(i==1){ digitalWrite(layerPin[i],  LOW); }
          else{ digitalWrite(layerPin[i],  HIGH); }
        }
        delay(1);
        digitalWrite(layerPin[1],  HIGH);
        break;

      // 3층
      case 3:
        for(int i=0; i<5; i++){
          if(i==2){ digitalWrite(layerPin[i],  LOW); }
          else{ digitalWrite(layerPin[i],  HIGH); }
        }
      delay(1);
      digitalWrite(layerPin[2],  HIGH);
      break; 

      // 4층
      case 4:
        for(int i=0; i<5; i++){
        if(i==3){ digitalWrite(layerPin[i],  LOW); }
        else{ digitalWrite(layerPin[i],  HIGH); }
      }
      delay(1);
      digitalWrite(layerPin[3],  HIGH);
      break; 

      // 5층
      case 5:
        for(int i=0; i<5; i++){
          if(i==4){ digitalWrite(layerPin[i],  LOW); }
          else{ digitalWrite(layerPin[i],  HIGH); }
        }
        delay(1);
        digitalWrite(layerPin[4],  HIGH);
        break;   
    }
}

void move_block_down(){
  while(level>1){
    delay(200);
    level--;
    updateShiftRegisterCathode(level);
    updateShiftRegisterAnode(pattern); 
  }
  updateShiftRegisterCathode(5);
  updateShiftRegisterAnode(pattern); 
}


// 도형 선택
// 5층에서 선택된 도형을 확인할 수 있다
// 선택한 숫자를 계속 누르면 회전
void choose_new_block(char recv){
  // 블럭 고르기
   pattern = recv - '0';

   // cnt: 회전 활성화 토글
   if(cnt){
     rotate += 1; 
     if(rotate > 3){    // 0~3까지 4가지 회전만 가능
       rotate = 0;
     }
   }

   for(int i=1; i<=5; i++){
      if(i==5){
        // 5층에서 블럭을 고름
        updateShiftRegisterCathode(i);
        updateShiftRegisterAnode(pattern); 
      }
      else{
        // 나머지 층들은 점멸
        updateShiftRegisterCathode(i);
        updateShiftRegisterAnode(5); 
      }
   }
    cnt = 0;
}



void setup() { 
 Serial.begin(9600);   // 시리얼 통신 초기화
 
 for(int i=0; i<5; i++){
   pinMode(layerPin[i], OUTPUT);
 }
   pinMode(pinData, OUTPUT);
   pinMode(pinLatch, OUTPUT);
   pinMode(pinClock, OUTPUT);
}

 
void loop(){
    recv = getData();
    moved = 1;

    // 블럭 내리기
    if(recv == 's'){
      if(moved){
        move_block_down();
      }
      moved = 0;
    }

    // 블럭 선택
    else{
      choose_new_block(recv);
    }
    
}
