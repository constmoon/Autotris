void Bresenham3D(int8_t x1, int8_t y1, int8_t z1, const int8_t x2, const int8_t y2, const int8_t z2, const byte mode)
{
  // This routine is from:
  //https://gist.github.com/yamamushi/5823518
  // I only adapted it for my sketch.
  
  int8_t i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc, err_1, err_2, dx2, dy2, dz2;
  int8_t point[3];
  
  point[0] = x1;
  point[1] = y1;
  point[2] = z1;
  dx = x2 - x1;
  dy = y2 - y1;
  dz = z2 - z1;
  x_inc = (dx < 0) ? -1 : 1;
  l = abs(dx);
  y_inc = (dy < 0) ? -1 : 1;
  m = abs(dy);
  z_inc = (dz < 0) ? -1 : 1;
  n = abs(dz);
  dx2 = l << 1;
  dy2 = m << 1;
  dz2 = n << 1;
  
  if ((l >= m) && (l >= n)) {
      err_1 = dy2 - l;
      err_2 = dz2 - l;
      for (i = 0; i < l; i++) {
          //output->getTileAt(point[0], point[1], point[2])->setSymbol(symbol);
          if (mode) SetDot(point[0], point[1], point[2]); else ClearDot(point[0], point[1], point[2]);
          if (err_1 > 0) {
              point[1] += y_inc;
              err_1 -= dx2;
          }
          if (err_2 > 0) {
              point[2] += z_inc;
              err_2 -= dx2;
          }
          err_1 += dy2;
          err_2 += dz2;
          point[0] += x_inc;
      }
  } else if ((m >= l) && (m >= n)) {
      err_1 = dx2 - m;
      err_2 = dz2 - m;
      for (i = 0; i < m; i++) {
          //output->getTileAt(point[0], point[1], point[2])->setSymbol(symbol);
          if (mode) SetDot(point[0], point[1], point[2]); else ClearDot(point[0], point[1], point[2]);
          if (err_1 > 0) {
              point[0] += x_inc;
              err_1 -= dy2;
          }
          if (err_2 > 0) {
              point[2] += z_inc;
              err_2 -= dy2;
          }
          err_1 += dx2;
          err_2 += dz2;
          point[1] += y_inc;
      }
  } else {
      err_1 = dy2 - n;
      err_2 = dx2 - n;
      for (i = 0; i < n; i++) {
          //output->getTileAt(point[0], point[1], point[2])->setSymbol(symbol);
          if (mode) SetDot(point[0], point[1], point[2]); else ClearDot(point[0], point[1], point[2]);
          if (err_1 > 0) {
              point[1] += y_inc;
              err_1 -= dz2;
          }
          if (err_2 > 0) {
              point[0] += x_inc;
              err_2 -= dz2;
          }
          err_1 += dy2;
          err_2 += dx2;
          point[2] += z_inc;
      }
  }
  //output->getTileAt(point[0], point[1], point[2])->setSymbol(symbol);
  if (mode) SetDot(point[0], point[1], point[2]); else ClearDot(point[0], point[1], point[2]);
}

void CalcLine3D(int8_t x1, int8_t y1, int8_t z1, const int8_t x2, const int8_t y2, const int8_t z2, byte mode)
{
  Bresenham3D(x1,y1,z1, x2,y2,z2, mode);
}

void DrawLine3D(int8_t x0, int8_t y0, int8_t z0, const int8_t x1, const int8_t y1, const int8_t z1)
{
  CalcLine3D(x0,y0,z0, x1,y1,z1, 1);
}

void EraseLine3D(int8_t x0, int8_t y0, int8_t z0, const int8_t x1, const int8_t y1, const int8_t z1)
{
  CalcLine3D(x0,y0,z0, x1,y1,z1, 0);
}

void SetLayer(int8_t z, int8_t xByte)
{
  //z = Wrap(z);
  for (int8_t y=0; y<8; y++) {
    cube[y][z] = xByte;
    //for (int8_t x=0; x<8; x++) {
    //  if (xByte==0) ClearDot(x,y,z); else SetDot(x,y,z);
    //}
  }
}

void SetDot(int8_t x,int8_t y, int8_t z)
{
  //noInterrupts();
  bitSet(cube[y][z], x);

//  Serial.print("X,Y,Z = ");
//  Serial.print(x);  Serial.print(",");
//  Serial.print(y);  Serial.print(",");
//  Serial.println(z);

}

void ClearDot(int8_t x,int8_t y,int8_t z)
{
  bitClear(cube[y][z], x);
}

void SetDotWrap(int8_t x, int8_t y, int8_t z) {
  SetDot(x + 3, y + 3, z);
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
