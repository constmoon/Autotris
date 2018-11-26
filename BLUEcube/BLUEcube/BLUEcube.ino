/* 
 *  Reference:
 *  https://github.com/hwiguna/g33k/tree/master/ArduinoProjects/8x8x8LEDCube
 *  
 */
//-- Shift Register pins --
int redlatch = 10; 
int redclock = 11; 
int reddata  = 9; 

int greenlatch = A1; 
int greenclock = A2; 
int greendata  = A0;

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
 
//-- Preferences --
int baudRate = 9600;
byte slomo = 0;
int8_t grid[5][5][6];
int stack[2][7] = {0, };   // stack{중복 여부, 현재 층수} * 7(도형 개수만큼)
int cnt[7]={0, };          // cnt{도}

//-- Globals --
volatile int8_t cube[8][8]; // byte bits = X, 1st index=Y, 2nd index = Z
volatile int8_t gZ = 0;
int oldPot0;
int animSpeed; // Animation speed controlled by pot0
float pi = 3.14;
float pi2 = 6.28;
int8_t prevD = 0; // last depth value
int8_t drawD = 0; // Last drawn depth
int8_t dCount = 0;// How many times has the same depth been reported by distance sensor?

const int8_t sineMaxIndex = 32;
int8_t sineArray[sineMaxIndex];

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

//-- USB Serial commands --
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

//-- Paths --
int8_t circle[][2] = {
  {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 6},
  {7, 5}, {7, 4}, {7, 3}, {7, 2}, {6, 1},
  {5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 1},
  {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 6}
};


//-- SETUP --
void setup(void) {
  SetupPins();
  Serial.begin(baudRate);
  inputString.reserve(600);
  PreComputes();
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

void DrawPatternAtLayer(int pattern_index, int8_t z) {
  for (int8_t x = 0; x < 5; x++) {
    for (int8_t y = 0; y < 5; y++) {
      if (((pat[x + pattern_index * 5] >> y) & 1) == 0) {
        SetDotWrap(x, y, z);
      }
    }
  }
}

void EraseGrid(int z) {
  EraseRect(3, 3, z, 7, 7, z);
}

void SetDotWrap(int8_t x, int8_t y, int8_t z) {
  SetDot(x + 3, y + 3, z);
}

void ClearDotWrap(int8_t x, int8_t y, int8_t z) {
  ClearDot(x + 3, y + 3, z);
}

void ClearAll() {
  for (int8_t x = 0; x < 5; x++) {
    for (int8_t y = 0; y < 5; y++) {
      for (int8_t z = 0; z < 6; z++) {
        ClearDotWrap(x, y, z);
      }
    }
  }
}

void DrawGrid(int8_t grid){
  for (int8_t x=0; x<5; x++) {
    for (int8_t y=0; y<5; y++){
      for (int8_t z=0; z<6; z++){
        if (grid == 0)
          SetDotWrap(x,y,z);
      }
    }
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
