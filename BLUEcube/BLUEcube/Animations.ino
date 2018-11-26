
//=== BASIC OPERATIONS ===

void TurnOnLayer(int8_t z)
{
  int8_t prev = z == 0 ? 7 : z - 1;

  // Prepare for data. Shift data to shift registers but do not reflect it on the outputs yet.
  digitalWrite(bluelatch, LOW);

  //-- Spit out the bits --
  DrawLayer(z);

  //-- Turn off previous layer --
  digitalWrite(2 + prev, LOW); // Turn off prev layer
}

void TurnOffLayer(int8_t z)
{
  // All data ready. Instantly reflect all 64 bits on all 8 shift registers to the led layer.
  digitalWrite(bluelatch, HIGH);

  //-- Turn on this layer --
  digitalWrite(2 + z, HIGH); // Turn on this layer
}

void DrawLayer(int8_t z)
{
  // Spit out all 64 bits for the layer.
  for (int8_t y = 0; y < 8; y++) {
    shiftOut(bluedata, blueclock, MSBFIRST, ~cube[y][z]); // Push Most significant BYTE first
  }
}

void LayerOn(int8_t z)
{
  for (int8_t y = 0; y < 8; y++) {
    for (int8_t x = 0; x < 8; x++) {
      SetDot(x, y, z);
    }
  }
}

void SetXPlane(int8_t x)
{
  x = Wrap(x);
  int8_t xPattern = 1 << x;
  for (int8_t z = 0; z < 8; z++) {
    for (int8_t y = 0; y < 8; y++) {
      cube[y][z] = xPattern;
    }
  }
}

void One_Pixel_Up_a_wall(int8_t y)
{
  for (int8_t z = 0; z < 8; z++) {
    for (int8_t x = 7; x >= 0; x--) {
      SetDot(x, y, z);
      delay(64);
      ClearDot(x, y, z);
    }
  }
}

void Line_Up_a_wall(int8_t y)
{
  for (int8_t z = 0; z < 8; z++) {
    for (int8_t x = 7; x >= 0; x--) {
      SetDot(x, y, z);
    }
    delay(64);
    CubeAllOff();
  }
}

void FillLayerLeftToRight(int8_t z)
{
  for (int8_t x = 0; x < 8; x++) {
    for (int8_t y = 0; y < 8; y++) {
      SetDot(x, y, z);
    }
    delay(64);
  }
}

void FillWallDownUp(int8_t x)
{
  for (int8_t z = 0; z < 8; z++) {
    for (int8_t y = 0; y < 8; y++) {
      SetDot(x, y, z);
    }
    delay(64);
  }
}


void FillLayerRightLeft(int8_t z)
{
  for (int8_t x = 7; x >= 0; x--) {
    for (int8_t y = 0; y < 8; y++) {
      SetDot(x, y, z);
    }
    delay(64);
  }
}

void DropOneCenterLine(int8_t x)
{
  for (int8_t z = 7; z >= 0; z--) {
    SetDot(x, 3, z);
    SetDot(x, 4, z);
    delay(64);
  }
}

void FillWallFromCenter(int8_t x)
{
  for (int8_t y = 1; y < 4; y++) {
    for (int8_t z = 0; z < 8; z++) {
      SetDot(x, 3 - y, z);
      SetDot(x, 4 + y, z);
    }
    delay(64);
  }
}

void PinWheel(int8_t y)
{
  for (int8_t n = 0; n < 8; n++)
  {
    DrawLine3D(0, y, n, 7, y, 7 - n);
    delay(64);
  }

  for (int8_t n = 0; n < 8; n++)
  {
    DrawLine3D(n, y, 7, 7 - n, y, 0);
    delay(64);
  }
}

void DrawCircle(int8_t z)
{
  for (int8_t n = 0; n < 20; n++)
  {
    SetDot(circle[n][0], circle[n][1], z);
    delay(128);
  }
}

void FillFrontAndBackRightLeft()
{
  for (int8_t x = 0; x < 8; x++) {
    for (int8_t z = 0; z < 8; z++) {
      SetDot(x, 0, z);
      SetDot(x, 7, z);
    }
    delay(64);
  }
}

void CubeShrink()
{
  int8_t cubeSize = 5;
  for (int8_t n = 1; n < 4; n++) {
    CubeAllOff();
    DrawRect(n, n, n, n, n + cubeSize, n + cubeSize); // YZ plane
    DrawRect(7 - n, n, n, 7 - n, n + cubeSize, n + cubeSize); // YZ plane
    DrawRect(n, n, n, n + cubeSize, n + cubeSize, n); // XY plane
    DrawRect(n, n, 7 - n, n + cubeSize, n + cubeSize, 7 - n); // XY plane
    cubeSize -= 2;
    delay(128);
  }
}

void CubeAllOn()
{
  //noInterrupts();
  for (int8_t z = 0; z < 8; z++) {
    for (int8_t y = 0; y < 8; y++) {
      for (int8_t x = 0; x < 8; x++) {
        SetDot(x, y, z);
      }
    }
  }
  //interrupts();
}

void CubeAllOff()
{
  for (int8_t z = 0; z < 8; z++) {
    SetLayer(z, 0x00);
  }
}

//=== SHAPES ===

void Shape_L2(int z) {
  DrawLine3D(5, 4, z, 7, 4, z);
  DrawLine3D(7, 4, z, 7, 5, z);
}

void Shape_L1(int z) {
  DrawLine3D(5, 5, z, 5, 7, z);
  DrawLine3D(5, 7, z, 6, 7, z);
}

void Shape_I(int z) {
  DrawLine3D(4, 3, z, 7, 3, z);
}

void Shape_S2(int z) {
  DrawLine3D(4, 5, z, 4, 6, z);
  DrawLine3D(3, 6, z, 3, 7, z);
}

void Shape_S1(int z) {
  DrawLine3D(6, 5, z, 6, 6, z);
  DrawLine3D(7, 6, z, 7, 7, z);
}

void Shape_T(int z) {
  DrawLine3D(3, 3, z, 3, 5, z);
  DrawLine3D(3, 4, z, 4, 4, z);
}

void Shape_o(int z) {
  DrawLine3D(4, 7, z, 4, 7, z);
  //  SetDot(4, 7, z);
}

void SelectShape(int tmp, int z) {
  if (tmp == 0) {
    Shape_T(z);
  } else if (tmp == 1) {
    Shape_L2(z);
  } else if (tmp == 2) {
    Shape_L1(z);
  } else if (tmp == 3) {
    Shape_I(z);
  } else if (tmp == 4) {
    Shape_S2(z);
  } else if (tmp == 5) {
    Shape_S1(z);
  } else if (tmp == 6) {
    Shape_o(z);
  }

}

void E_Shape_L2(int z) {
  EraseLine3D(5, 4, z, 7, 4, z);
  EraseLine3D(7, 4, z, 7, 5, z);
}

void E_Shape_L1(int z) {
  EraseLine3D(5, 5, z, 5, 7, z);
  EraseLine3D(5, 7, z, 6, 7, z);
}

void E_Shape_I(int z) {
  EraseLine3D(4, 3, z, 7, 3, z);
}

void E_Shape_S2(int z) {
  EraseLine3D(4, 5, z, 4, 6, z);
  EraseLine3D(3, 6, z, 3, 7, z);
}

void E_Shape_S1(int z) {
  EraseLine3D(6, 5, z, 6, 6, z);
  EraseLine3D(7, 6, z, 7, 7, z);
}

void E_Shape_T(int z) {
  EraseLine3D(3, 3, z, 3, 5, z);
  EraseLine3D(3, 4, z, 4, 4, z);
}

void E_Shape_o(int z) {
  EraseLine3D(4, 7, z, 4, 7, z);
  //  ClearDot(4, 7, z);
}

void EraseShape(int tmp, int z) {
  if (tmp == 0) {
    E_Shape_T(z);
  } else if (tmp == 1) {
    E_Shape_L2(z);
  } else if (tmp == 2) {
    E_Shape_L1(z);
  }  else if (tmp == 3) {
    E_Shape_I(z);
  } else if (tmp == 4) {
    E_Shape_S2(z);
  } else if (tmp == 5) {
    E_Shape_S1(z);
  } else if (tmp == 6) {
    E_Shape_o(z);
  }
}

boolean LayerIsFull(int z) {
  // grid[x][y][z] 값이 1이면 켜져있음, 값이 0이면 꺼져있음
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 5; y++) {
      if (grid[x][y][z] != 1) return false; // z층에 한칸이라도 꺼져있으면 꽉차있지 않음
    }
  }
  return true; // z층 한 집씩 봤는데 전부 켜져있으면 꽉차있음
}

void ShiftLayers(int z) {
  // z층부터 한층씩 올라가면서 윗층 값을 복사
  for (; z < 6-1; z++) { // 꼭대기-1층까지 한층씩 올라감. (꼭대기 층은 윗집이 없으니까)
    for (int x = 0; x < 5; x++) {
      for (int y = 0; y < 5; y++) {
        if (grid[x][y][z+1] == 1) SetDotWrap(x,y,z); // 윗집 불이 켜져있으면 우리집 불 켬
        else if (grid[x][y][z+1] == 0) ClearDotWrap(x,y,z); // 윗집 불 꺼져있으면 우리집 불 끔
      }
    }
  }
  SetLayer(5, 0x00); // 꼭대기인 6층은 불을 다 끔
}


void DropShapes(boolean pushed, int selected) {

  if (pushed) {
    delay(200);

    for (int8_t z = 5; z >= cnt[selected]; z--) { // 해당 도형의 현재 로컬 레벨까지만 떨어짐
      if (z < 5) EraseShape(selected, z+1);   // 그린 후 지우면서 한 줄씩 내려감
      SelectShape(selected, z);   // 해당 층에 도형 그리고
      delay(5);
    }
    cnt[selected]++;
    delay(100);
  }

  // 방금 떨어진 도형이 멈춘 층이 꽉 찼는지 확인.
  // cnt[selected]는 위에서 ++해서 무조건 1 이상이기 때문에 -1 해도 0 이상
  if (LayerIsFull(cnt[selected] - 1)) {  
    SetLayer(cnt[selected] - 1, 0x00); // 해당 층을 전부 지우고 SetLayer(z, 0x00);
    ShiftLayers(cnt[selected] - 1); // 해당 층 이상을 전부 한칸씩 떨어뜨림
    for (int k = 0; k < 7; k++) {
      cnt[k]--;
    }
  }

  // 5층이 넘어가면 Re-start
  if (cnt[selected] >= 5) {
    CubeAllOn();
    delay(300);
    CubeAllOff();  // 전체를 우선 다 끈 후
    delay(200);
    CubeAllOn();
    delay(300);
    CubeAllOff();
    delay(200);
    CubeAllOn();
    delay(300);
    CubeAllOff();
    for (int k = 0; k < 7; k++) {
      cnt[k] = 0;   // 도형별 누적 층수 초기화
    }
  }

  delay(100);
}

//boolean LayerIsFull(int z) {
//  // grid[x][y][z] 값이 1이면 켜져있음, 값이 0이면 꺼져있음
//  for (int x = 0; x < 5; x++) {
//    for (int y = 0; y < 5; y++) {
//      // 빈칸을 1로 정의
//      if (grid[x][y][z] != 0){
//        return false; // z층에 한칸이라도 꺼져있으면 꽉차있지 않음
//      }else{
//        return true; // z층 한 집씩 봤는데 전부 켜져있으면 꽉차있음
//      }
//    }
//  }
//  
//}
//
//void ShiftLayers(int z) {
//  // z층부터 한층씩 올라가면서 윗층 값을 복사
//  for (; z < 6 - 1; z++) { // 꼭대기-1층까지 한층씩 올라감. (꼭대기 층은 윗집이 없으니까)
//    for (int x = 0; x < 5; x++) {
//      for (int y = 0; y < 5; y++) {
//        if (grid[x][y][z + 1] == 0) SetDotWrap(x, y, z); // 윗집 불이 켜져있으면 우리집 불 켬
//        else if (grid[x][y][z + 1] == 1) ClearDotWrap(x, y, z); // 윗집 불 꺼져있으면 우리집 불 끔
//      }
//    }
//  }
//  SetLayer(5, 0xFF); // 꼭대기인 6층은 불을 다 끔
//}
//
//void DropShapes(boolean pushed, int selected) {
//
//  if (pushed) {
//    delay(200);
//
//    for (int8_t z = 5; z >= cnt[selected]; z--) { // 해당 도형의 현재 로컬 레벨까지만 떨어짐
//      if (z < 5) EraseShape(selected, z + 1); // 그린 후 지우면서 한 줄씩 내려감
//      SelectShape(selected, z);   // 해당 층에 도형 그리고
//      delay(5);
//    }
//    cnt[selected]++;
//    delay(100);
//  }
//
//  // 방금 떨어진 도형이 멈춘 층이 꽉 찼는지 확인.
//  // cnt[selected]는 위에서 ++해서 무조건 1 이상이기 때문에 -1 해도 0 이상
//  
//  if (LayerIsFull(cnt[selected]-1)) {
//    Serial.println("Layer is full");
//    SetLayer(cnt[selected] - 1, 0xFF); // 해당 층을 전부 지우고 SetLayer(z, 0x00);
//    ShiftLayers(cnt[selected] - 1); // 해당 층 이상을 전부 한칸씩 떨어뜨림
//    for (int k = 0; k < 7; k++) {
//      cnt[k]--;
//      }
//  }
//
//  // 5층이 넘어가면 Re-start
//  if (cnt[selected] > 5) {
//    CubeAllOn();
//    delay(300);
//    CubeAllOff();  // 전체를 우선 다 끈 후
//    delay(200);
//    CubeAllOn();
//    delay(300);
//    CubeAllOff();
//    delay(200);
//    CubeAllOn();
//    delay(300);
//    CubeAllOff();
//    for (int k = 0; k < 7; k++) {
//      cnt[k] = 0;   // 도형별 누적 층수 초기화
//    }
//  }
//
//  delay(100);
//}

//void DropShapes(boolean pushed, int selected) {
//
//  if (pushed) {
//    delay(200);
//
//    for (int8_t z = 5; z >= cnt[selected]; z--) { // 해당 도형의 현재 로컬 레벨까지만 떨어짐
//      if (z < 5) EraseShape(selected, z+1);   // 그린 후 지우면서 한 줄씩 내려감
//      SelectShape(selected, z);   // 해당 층에 도형 그리고
//      delay(5);
//    }
//    cnt[selected]++;
//    delay(100);
//  }
//
//  // 5층이 넘어가면 Re-start
//  if (cnt[selected] > 5) {
//    CubeAllOn();
//    delay(300);
//    CubeAllOff();  // 전체를 우선 다 끈 후
//    delay(200);
//    CubeAllOn();
//    delay(300);
//    CubeAllOff();
//    delay(200);
//    CubeAllOn();
//    delay(300);
//    CubeAllOff();
//    for (int k = 0; k < 7; k++) {
//      cnt[k] = 0;   // 도형별 누적 층수 초기화
//    }
//  }
//  delay(100);
//}
