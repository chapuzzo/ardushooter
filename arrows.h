#ifndef _AS_ARROWS_H_
#define _AS_ARROWS_H_
#include "binary.h"
/*
#ifdef __cplusplus
extern "C" {
#endif
*/
byte right_arrow[8] = {
  0b00000,
  0b10000,
  0b11000,
  0b11100,
  0b11000,
  0b10000,
  0b00000,
  0b00000
};

byte left_arrow[8] = {
  0b00000,
  0b00001,
  0b00011,
  0b00111,
  0b00011,
  0b00001,
  0b00000,
  0b00000
};

byte up_arrow[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b00000
};

byte down_arrow[8] = {
  0b00000,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte down_up_arrow[8] = {
  0b00000,
  0b11111,
  0b01110,
  0b00100,
  0b00100,
  0b01110,
  0b11111,
  0b00000
};

byte ard_l[8] = {
  0b00000,
  0b00000,
  0b01100,
  0b10010,
  0b10001,
  0b10010,
  0b01100,
  0b00000
};

byte ard_r[8] = {
  0b00000,
  0b00000,
  0b00110,
  0b01001,
  0b10001,
  0b01001,
  0b00110,
  0b00000
};

byte empty_right_arrow[8] = {
  0b00000,
  0b10000,
  0b01000,
  0b00100,
  0b01000,
  0b10000,
  0b00000,
  0b00000
};
/*
#ifdef __cplusplus
}
#endif
*/
#endif //_AS_ARROWS_

