/* 
 *  Reference:
 *  https://github.com/hwiguna/g33k/tree/master/ArduinoProjects/8x8x8LEDCube
 *  
 */
//-- Shift Register pins --
int bluelatch = A1; // Arduino D13 to IC pin 12 (ST_CP) -- White
int blueclock = A2; // Arduino D12 to IC pin 11 (SH_CP) -- Yelow
int bluedata  = A0; // Arduino D11 to IC pin 14 (DS) -- Blue

int buttonPin = 13;
int old_button = 0;
boolean pushed = false;
int select = 0;
int8_t gridFloor[6][5]; // z,y

int index[100];
int data = 0;
int8_t patt;
int8_t level = 0;
int8_t gZ = 0;
 
//-- Cube --
int8_t grid[5][5][6];
int stack[2][7] = {0, };   // stack{중복 여부, 현재 층수} * 7(도형 개수만큼)
int cnt[7]={0, };          // cnt{도}
int8_t cube[8][8]; // byte bits = X, 1st index=Y, 2nd index = Z


byte pat[35] = {

  B00011,
  B10111,
  B11111,
  B11111,
  B11111,

  B11111,
  B11111,
  B10111,
  B10111,
  B10011,

  B11111,
  B11111,
  B11000,
  B11110,
  B11111,

  B11111,
  B01111,
  B01111,
  B01111,
  B01111,

  B11100,
  B11001,
  B11111,
  B11111,
  B11111,

  B11111,
  B11111,
  B11111,
  B11001,
  B11100,

  B11111,
  B11110,
  B11111,
  B11111,
  B11111 
  };


//-- SETUP --
void setup(void) {
  SetupPins();
  Serial.begin(9600);
  CubeAllOff();
  SetupTimer();
}

void loop() {
  tetrisLoop();
}

void tetrisLoop(){
  select = random(0, 7);
//  Serial.println(select);
  
  int new_button = digitalRead(buttonPin);

  if (new_button > 0 && old_button != 1)
  {
    pushed = true;  // ---------------------------------------- 버튼 활성화
  } else {
    pushed = false;
  }
  old_button = new_button;

  if (pushed) {
//    Serial.println("  Pushed,  ");
//    Tetris(); 
    DropShapes(pushed, select);
  } else {
//    Serial.println(" Unpushed, ");
  }
}

void Tetris()
{
  int pattern_index = 1;
  int8_t grid[5][5][6]; // 이미 놓여진 도형들 저장
  for (int8_t x=0; x<5; x++) {
    for (int8_t y=0; y<5; y++) {
      for (int8_t z=0; z<6; z++) {
        grid[x][y][z] = 1; // 처음엔 모두 빈칸으로 정의
      }
    }
  }

  // 예시 - ㄹ자 도형이 이미 놓여진 상태
  grid[1][3][0] = 0;
  grid[1][4][0] = 0;
  grid[2][2][0] = 0;
  grid[2][3][0] = 0;

  for (int8_t z=5; z>=0; z--) {
    ClearAll();
    DrawGrid(grid);
    ClearAll();
    DrawPatternAtLayer(select, z);// pattern_index, z);
    delay(100);
  }
}
