
//=== BASIC OPERATIONS ===

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

void DisplayShapes(int selected, boolean displayOn, int z) {
  if (displayOn) {
    if (selected == 0) {
      // T
      DrawLine3D(4, 5, z, 6, 5, z);
      DrawLine3D(5, 5, z, 5, 4, z);
    } else if (selected == 1) {
      // L2
      DrawLine3D(5, 6, z, 5, 4, z);
      DrawLine3D(5, 4, z, 6, 4, z);
    }  else if (selected == 2) {
      // L1
      DrawLine3D(5, 6, z, 5, 4, z);
      DrawLine3D(4, 4, z, 5, 4, z);
    }  else if (selected == 3) {
      // I
      DrawLine3D(5, 6, z, 5, 3, z);
    }  else if (selected == 4) {
      // S2
      DrawLine3D(5, 6, z, 5, 5, z);
      DrawLine3D(6, 5, z, 6, 4, z);
    }  else if (selected == 5) {
      // S1
      DrawLine3D(5, 6, z, 5, 5, z);
      DrawLine3D(4, 5, z, 4, 4, z);

    }  else if (selected == 6) {
      // o
      DrawLine3D(5, 6, z, 5, 3, z);
      //      SetDot(5, 5, z);
    }
  } else if (!displayOn) {
    if (selected == 0) {
      // T
      EraseLine3D(4, 5, z, 6, 5, z);
      EraseLine3D(5, 5, z, 5, 4, z);
    } else if (selected == 1) {
      // L2
      EraseLine3D(5, 6, z, 5, 4, z);
      EraseLine3D(5, 4, z, 6, 4, z);
    }  else if (selected == 2) {
      // L1
      EraseLine3D(5, 6, z, 5, 4, z);
      EraseLine3D(4, 4, z, 4, 5, z);
    }  else if (selected == 3) {
      // I
      EraseLine3D(5, 6, z, 5, 3, z);
    }  else if (selected == 4) {
      // S2
      EraseLine3D(5, 6, z, 5, 5, z);
      EraseLine3D(6, 5, z, 6, 4, z);
    }  else if (selected == 5) {
      // S1
      EraseLine3D(5, 6, z, 5, 5, z);
      EraseLine3D(4, 5, z, 4, 4, z);
    }  else if (selected == 6) {
      // o
      EraseLine3D(5, 6, z, 5, 3, z);
      //      ClearDot(5, 5, z + 3);
    }
  }
}

void SelectShapes(int selected, boolean displayOn, int z) {
  if (displayOn) {
    if (selected == 0) {
      // T
      DrawLine3D(3, 3, z, 3, 5, z);
      DrawLine3D(3, 4, z, 4, 4, z);
    } else if (selected == 1) {
      // L2
      DrawLine3D(5, 4, z, 7, 4, z);
      DrawLine3D(7, 4, z, 7, 5, z);
    }  else if (selected == 2) {
      // L1
      DrawLine3D(5, 5, z, 5, 7, z);
      DrawLine3D(5, 7, z, 6, 7, z);
    }  else if (selected == 3) {
      // I
      DrawLine3D(4, 3, z, 7, 3, z);
    }  else if (selected == 4) {
      // S2
      DrawLine3D(4, 5, z, 4, 6, z);
      DrawLine3D(3, 6, z, 3, 7, z);
    }  else if (selected == 5) {
      // S1
      DrawLine3D(6, 5, z, 6, 6, z);
      DrawLine3D(7, 6, z, 7, 7, z);
    }  else if (selected == 6) {
      // o
      SetDot(4, 7, z);
    }
  } else if (!displayOn) {
    if (selected == 0) {
      // T
      EraseLine3D(3, 3, z, 3, 5, z);
      EraseLine3D(3, 4, z, 4, 4, z);
    } else if (selected == 1) {
      // L2
      EraseLine3D(5, 4, z, 7, 4, z);
      EraseLine3D(7, 4, z, 7, 5, z);
    }  else if (selected == 2) {
      // L1
      EraseLine3D(5, 5, z, 5, 7, z);
      EraseLine3D(5, 7, z, 6, 7, z);
    }  else if (selected == 3) {
      // I
      EraseLine3D(4, 3, z, 7, 3, z);
    }  else if (selected == 4) {
      // S2
      EraseLine3D(4, 5, z, 4, 6, z);
      EraseLine3D(3, 6, z, 3, 7, z);
    }  else if (selected == 5) {
      // S1
      EraseLine3D(6, 5, z, 6, 6, z);
      EraseLine3D(7, 6, z, 7, 7, z);
    }  else if (selected == 6) {
      // o
      ClearDot(4, 7, z + 3);
    }
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
  for (; z < 6 - 1; z++) { // 꼭대기-1층까지 한층씩 올라감. (꼭대기 층은 윗집이 없으니까)
    for (int x = 0; x < 5; x++) {
      for (int y = 0; y < 5; y++) {
        if (grid[x][y][z + 1] == 1) SetDotWrap(x, y, z); // 윗집 불이 켜져있으면 우리집 불 켬
        else if (grid[x][y][z + 1] == 0) ClearDotWrap(x, y, z); // 윗집 불 꺼져있으면 우리집 불 끔
      }
    }
  }
  SetLayer(5, 0x00); // 꼭대기인 6층은 불을 다 끔
}

void DropShapes(boolean pushed, int selected) {

  if (pushed) {
    delay(300);
    DisplayShapes(selected, true, 5);
    delay(600);
    DisplayShapes(selected, false, 5);

    for (int8_t z = 5; z >= cnt[selected]; z--) { // 해당 도형의 현재 로컬 레벨까지만 떨어짐
      if (z < 5) SelectShapes(selected, false, z + 1); // 그린 후 지우면서 한 줄씩 내려감
      SelectShapes(selected, true, z); // 해당 층에 도형 그리고
      delay(8);
    }

    if (selected == 6) {
      if (cnt[6] == 0) {
        cnt[6] += 3;
      } else {
        cnt[6] += 4;
      }
    } else {
      cnt[selected]++;
    }
    delay(100);
  }

  // 방금 떨어진 도형이 멈춘 층이 꽉 찼는지 확인.
  // cnt[selected]는 위에서 ++해서 무조건 1 이상이기 때문에 -1 해도 0 이상
  if (LayerIsFull(cnt[selected] - 1)) {
    delay(500);
    SetLayer(cnt[selected] - 1, 0x00); // 해당 층을 전부 지우고 SetLayer(z, 0x00);
    ShiftLayers(cnt[selected] - 1); // 해당 층 이상을 전부 한칸씩 떨어뜨림
    for (int k = 0; k < 7; k++) {
      cnt[k]--;
    }
  }

  // 5층이 넘어가면 Re-start
  if (cnt[selected] > 5) {
    CubeAllOn();
    delay(400);
    CubeAllOff();  // 전체를 우선 다 끈 후
    delay(250);
    CubeAllOn();
    delay(400);
    CubeAllOff();
    delay(250);
    CubeAllOn();
    delay(400);
    CubeAllOff();
    for (int k = 0; k < 7; k++) {
      cnt[k] = 0;   // 도형별 누적 층수 초기화
    }
  }

  delay(100);
}
