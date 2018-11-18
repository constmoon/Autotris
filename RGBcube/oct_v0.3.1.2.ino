#include <FirmataDefines.h>
#include <FirmataConstants.h>
#include <Boards.h>
#include <Firmata.h>
#include <FirmataMarshaller.h>
#include <FirmataParser.h>

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
int index[40];

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
int layerPin[6] = { 2, 3, 4, 5, 6, 7 };

int pattern = ' ';    // 출력 패턴
int cnt = 0;
char recv = ' ';       // incoming byte
char dir;        // 방향


// 프로그램 시작 - 초기화
void setup() {
  Serial.begin(9600); //시리얼 통신 초기화

  // PUSH BUTTON
  pinMode(buttonPin, INPUT);

  for (int i = 0; i < 6; i++) {
    pinMode(layerPin[i], OUTPUT);
  }

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
void loop() {

  int new_button = digitalRead(buttonPin);

  if (new_button > 0 && old_button != new_button)
  {
    pushed += 1;
    if (pushed > 1)
      pushed = 0;
  }
  old_button = new_button;

  if (pushed == 1) {
    Serial.print(" Pushed,  ");
    select += 0;
    int tmp = select;
    index[data] = tmp;
    Serial.print(data);

    level -= 1;
    if (level == 1) {
      data++;
    }
    if (level < 1) {
      level = 1;

    }
    delay(200);
    //    delayMicroseconds(5);

    //    for(int i = 0; i<6; i++) {
    //    updateShiftRegisterAnode(level);
    //    }
    updateShiftRegisterAnode(level);
    //updateShiftRegisterCathode(select);
    for (int i = 0; i <= data; i++) {
      updateShiftRegisterCathode(index[i]);
    }
    updateShiftRegisterCathode(0);  // --------------------------------[led 잡티 제거]
    Serial.print(", current level is... ");
    Serial.print(level);
    Serial.print(", random selecting... : ");
    Serial.println(select);

  } else if (pushed == 0) {
    Serial.print("unPushed, ");
    Serial.print(data);
    Serial.print(", current level is... ");
    Serial.print(level);
    Serial.print(", random selecting... : ");
    Serial.println(select);
  }

  if (level == 1) {
    pushed = 0;
    level = 7;
  } else if (level == 7) {
    Serial.print("unPushed, ");
    Serial.print(data);

    select += 1;
    if (select > 5) {
      select = 1;
    }
    Serial.print(", current level is... ");
    Serial.print(level);
    Serial.print(", random selecting... : ");
    Serial.println(select);

  }
}

// 5*5 이미지
// byte 1:off  0:On
byte imageG[] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
  };

byte image1[5] = {
  B11111,
  B11111,
  B11011,
  B10001,
  B11111
};

byte image2[5] = {
  B11111,
  B10011,
  B11011,
  B11011,
  B11111
};

byte image3[5] = {
  B11111,
  B11011,
  B11011,
  B11011,
  B11011
};

byte image4[5] = {
  B11111,
  B11111,
  B11001,
  B10011,
  B11111
};

byte image5[5] = {
  B11111,
  B11111,
  B10011,
  B10011,
  B11111
};

byte image6[5] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte *patterns[] {
  image1,
  image2,
  image3,
  image4,
  image5,
  image6
};

byte pat[] = {
  B11111,
  B11111,
  B11001,
  B10011,
  B11111,

  B11111,
  B11111,
  B10011,
  B10011,
  B11111,

  B11111,
  B11011,
  B11011,
  B11011,
  B11011,

  B11111,
  B11111,
  B11011,
  B10001,
  B11111,

  B11111,
  B11111,
  B10001,
  B11101,
  B11111,
};

void gridPattern() {
  if (level == 1) {
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < data; j++) {
        // index[]: 선택한 패턴들의 배열
        int tep = index[j];
        // 밑바닥(1층)인 경우 빨간색 표시
        digitalWrite(redLatch,  LOW);
        shiftOut(redData, redClock, LSBFIRST, (imageG[i] & pat[index[j]+i]));
        digitalWrite(redLatch,  HIGH);
      }
    }
    for (int i = 0; i < 5; i++) {
      digitalWrite(greenLatch, LOW);
      shiftOut(greenData, greenClock, LSBFIRST, image6[i]);
      digitalWrite(greenLatch,  HIGH);
    }

    for (int i = 0; i < 5; i++) {
      digitalWrite(blueLatch, LOW);
      shiftOut(blueData, blueClock, LSBFIRST, image6[i]);
      digitalWrite(blueLatch,  HIGH);
    }
  } 
  
  else {
    for (int i = 0; i < 5; i++) {
      digitalWrite(redLatch,  LOW);
      shiftOut(redData, redClock, LSBFIRST, image6[i]);
      digitalWrite(redLatch,  HIGH);
    }
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < data; j++) {
        digitalWrite(greenLatch, LOW);
        shiftOut(greenData, greenClock, LSBFIRST, (imageG[i] & pat[index[j]+i] ));
        digitalWrite(greenLatch,  HIGH);
      }
    }
    for (int i = 0; i < 5; i++) {
      digitalWrite(blueLatch, LOW);
      shiftOut(blueData, blueClock, LSBFIRST, image6[i]);
      digitalWrite(blueLatch,  HIGH);
    }
  }
}


void pattern1() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(redLatch,  LOW);
    shiftOut(redData, redClock, LSBFIRST, pat[i]);
    digitalWrite(redLatch,  HIGH);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(greenLatch, LOW);
    shiftOut(greenData, greenClock, LSBFIRST, pat[i]);
    digitalWrite(greenLatch,  HIGH);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(blueLatch, LOW);
    shiftOut(blueData, blueClock, LSBFIRST, pat[i]);
    digitalWrite(blueLatch,  HIGH);
  }
}

void pattern2() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(redLatch,  LOW);
    shiftOut(redData, redClock, LSBFIRST, pat[i * 5]);
    digitalWrite(redLatch,  HIGH);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(greenLatch,  LOW);
    shiftOut(greenData, greenClock, LSBFIRST, pat[i * 5]);
    digitalWrite(greenLatch,  HIGH);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(blueLatch,  LOW);
    shiftOut(blueData, blueClock, LSBFIRST, pat[i * 5]);
    digitalWrite(blueLatch,  HIGH);
  }
}

void pattern3() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(redLatch,  LOW);
    shiftOut(redData, redClock, LSBFIRST, pat[i * 10]);
    digitalWrite(redLatch,  HIGH);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(greenLatch,  LOW);
    shiftOut(greenData, greenClock, LSBFIRST, pat[i * 10]);
    digitalWrite(greenLatch,  HIGH);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(blueLatch,  LOW);
    shiftOut(blueData, blueClock, LSBFIRST, pat[i * 10]);
    digitalWrite(blueLatch,  HIGH);
  }
}

void pattern4() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(redLatch,  LOW);
    shiftOut(redData, redClock, LSBFIRST, pat[i * 15]);
    digitalWrite(redLatch,  HIGH);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(greenLatch,  LOW);
    shiftOut(greenData, greenClock, LSBFIRST, pat[i * 15]);
    digitalWrite(greenLatch,  HIGH);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(blueLatch,  LOW);
    shiftOut(blueData, blueClock, LSBFIRST, pat[i * 15]);
    digitalWrite(blueLatch,  HIGH);
  }
}
void pattern5() {

  for (int i = 0; i < 5; i++) {
    digitalWrite(redLatch,  LOW);
    shiftOut(redData, redClock, LSBFIRST, pat[i * 20]);
    digitalWrite(redLatch,  HIGH);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(greenLatch,  LOW);
    shiftOut(greenData, greenClock, LSBFIRST, pat[i * 20]);
    digitalWrite(greenLatch,  HIGH);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(blueLatch,  LOW);
    shiftOut(blueData, blueClock, LSBFIRST, pat[i * 20]);
    digitalWrite(blueLatch,  HIGH);
  }
}

void pattern6() {
  // ----------------------------------------------------[ LED OFF ]
  for (int i = 0; i < 5; i++) {
    digitalWrite(redLatch,  LOW);
    shiftOut(redData, redClock, LSBFIRST, image6[i]);
    digitalWrite(redLatch,  HIGH);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(redLatch,  LOW);
    shiftOut(greenData, greenClock, LSBFIRST, image6[i]);
    digitalWrite(greenLatch,  HIGH);
  }
  for (int i = 0; i < 5; i++) {
    digitalWrite(redLatch,  LOW);
    shiftOut(blueData, blueClock, LSBFIRST, image6[i]);
    digitalWrite(blueLatch,  HIGH);
  }
}

void updateShiftRegisterCathode(int pattern) {
  digitalWrite(redLatch,  LOW);
  digitalWrite(greenLatch,  LOW);
  digitalWrite(blueLatch,  LOW);
  // 패턴 cathode LOW일 때 켜짐(-니까..)

  gridPattern();

  digitalWrite(redLatch,  HIGH);
  digitalWrite(greenLatch,  HIGH);
  digitalWrite(blueLatch,  HIGH);
}

// 각 층(layer) 제어
void updateShiftRegisterAnode(int layer) {
  for (int i = 0; i < 6; i++) {
    digitalWrite(layerPin[i],  LOW);// 레이어가 anode HIGH일 때 켜짐(-니까..)
    
    // -------------------누적시키려면 다 켜져야 함
    delay(1);
    digitalWrite(layerPin[5],  HIGH);
  }

  switch (layer) {
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

    case 6:
      delay(1);
      digitalWrite(layerPin[5],  HIGH);
      break;

    // ----------------------------------------------------[ LED OFF ]
    case 7:
      for (int i = 0; i < 6; i++)
        digitalWrite(layerPin[i],  LOW);
      break;
  }
}
