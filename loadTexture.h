#include <iostream>
#include <string>
#include "BMP.h"

void loadTextureIntoArray(unsigned char data[256][256][3],
                          std::string bmpName) {
  BMP bmp5(bmpName.c_str());
  bmp5.write("copia.bmp");

  int channels = bmp5.bmp_info_header.bit_count / 8;

  std::cout << channels << "\n";

  for (int y = 0; y < 255; y++) {
    for (int x = 0; x < 255; x++) {
      unsigned char* p = data[y][x];

      int yAux = y + 1;
      int xAux = x + 1;

      char blue =
          bmp5.data[channels * (yAux * bmp5.bmp_info_header.width + xAux)];
      char green =
          bmp5.data[channels * (yAux * bmp5.bmp_info_header.width + xAux) + 1];
      char red =
          bmp5.data[channels * (yAux * bmp5.bmp_info_header.width + xAux) + 2];

      p[0] = red;
      p[1] = green;
      p[2] = blue;
    }
  }
}