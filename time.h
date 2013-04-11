#ifndef _AS_TIME_H_
#define _AS_TIME_H_

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

void fill_tm(int i, int v[]);
char *t_ms(uint16_t t);
char *t_txt(uint16_t i);
char *n_txt(uint16_t i);
char *exp_txt(uint16_t i);

void timeset(unsigned int *i);
void timeIt(uint16_t);



#ifdef __cplusplus
}
#endif

#endif //_AS_TIME_H_
