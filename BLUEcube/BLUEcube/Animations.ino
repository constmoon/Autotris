void CubeAllOn(){
  for (int8_t z = 0; z < 8; z++) {
    for (int8_t y = 0; y < 8; y++) {
      for (int8_t x = 0; x < 8; x++) {
        SetDot(x, y, z);
      }
    }
  }
}

void CubeAllOff(){
  for (int8_t z = 0; z < 8; z++) {
    SetLayer(z, 0x00);
  }
}


/* Drawing Functions */
void Shape_T(int z) {
  DrawLine3D(3, 3, z, 3, 5, z);
  DrawLine3D(3, 4, z, 4, 4, z);
}
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
void Shape_o(int z) {
  SetDot(4, 7, z);
}
void SelectShape(int shape, int z) {
  if (shape == 0) {
    Shape_T(z);
  } else if (shape == 1) {
    Shape_L2(z);
  } else if (shape == 2) {
    Shape_L1(z);
  } else if (shape == 3) {
    Shape_I(z);
  } else if (shape == 4) {
    Shape_S2(z);
  } else if (shape == 5) {
    Shape_S1(z);
  } else if (shape == 6) {
    Shape_o(z);
  }

}

/* Erasing Functions */
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
  ClearDot(4, 7, z);
}

void EraseShape(int shape, int z) {
  if (shape == 0) {
    E_Shape_T(z);
  } else if (shape == 1) {
    E_Shape_L2(z);
  } else if (shape == 2) {
    E_Shape_L1(z);
  }  else if (shape == 3) {
    E_Shape_I(z);
  } else if (shape == 4) {
    E_Shape_S2(z);
  } else if (shape == 5) {
    E_Shape_S1(z);
  } else if (shape == 6) {
    E_Shape_o(z);
  }
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

  // 5층이 넘어가면 Re-start
  if (cnt[selected] > 5) {
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
