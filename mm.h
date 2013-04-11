#ifndef _MM_H_
#define _MM_H_

#include "Arduino.h"

#define ARRAY_COUNT(a) (sizeof(a)/(sizeof(a[0])))

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    _int,
    _ms,
    _ok
} mm_t;

typedef struct minimenu_t minimenu;
struct minimenu_t{
  char active;
  char caption[6];
  int  x,y;
  mm_t type;
  uint16_t *val;
  char* (*callback)(uint16_t val);
};



void mm_print(minimenu *mm, int n);


#ifdef __cplusplus
}
#endif

#endif //_MM_H_
