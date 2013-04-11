#ifndef _AS_MENU_H_
#define _AS_MENU_H_

#include "Arduino.h"

#define EA (uint8_t)0
#define RA 1
#define LA 2
#define UA 3
#define DA 4
#define UD 5
#define A "\6\7"

#define BUTTON_PIN A0
#define UMBRAL 10
#define masomenos(x,v) (x > (v - UMBRAL) && x < (v + UMBRAL))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct menu_t menu;
struct menu_t {
  char t[15];
  char a;
  void (*i)();
  void (*s)();
  menu *l;
  menu *u;
  menu *d;
  menu *r;
};



void creaMenus(void);
void setMenu(menu *m_);
void printCaption(menu *m_);
//~ void printIt(String s);
char buttonValue(int i);
char readButtonPress(void);
void waitButtonPress(void);
char readButtonRelease(void);

void buttonDispatch(void);

void timer(void);
void remote(void);
void sensor(void);

void bulb(void);
void hdr(void);
void intervalometer(void);


#ifdef __cplusplus
}
#endif

#endif //_AS_MENU_H_
