#include "time.h"
#include "menu.h"
#include "mm.h"
#include "LiquidCrystal.h"

extern LiquidCrystal lcd;

int fib(int n){
  if (n < 2) return n;
  else
  return fib(n-1) + fib(n-2);
}

void fill_tm(int i, int v[]){
  v[0] = i/600;
  i %= 600;
  v[1] = i/60;
  i %= 60;
  v[2] = i/10;
  i %= 10;
  v[3] = i;
}

#define SHUTTERPIN A3      // which pin is the shutter connected to?


void fire(unsigned long t){
    unsigned long t0;
    pinMode(SHUTTERPIN, OUTPUT);        //outpin gives output
    t0 = millis();
    digitalWrite(SHUTTERPIN, HIGH);     //fire
    while (millis() < t0+t){}           //wait t millisec
    digitalWrite(SHUTTERPIN, LOW);      //end fire
}

char *t_ms(uint16_t t){
  static char res[6];
  sprintf(res,"%02u:%02u",t/60, t%60);
  return res;
}

char *t_txt(uint16_t i){
  static char *res[] =  {" ok", "rmt", "sns"};
  return res[i];
}

unsigned long exptime[] = {1, 2, 3, 4, 6, 8, 11, 17, 22, 33, 50, 67, 100, 125, 167, 250, 333, 500, 666, 1000, 1500, 2600, 3300, 4600, 6600, 8600, 10500, 15500, 20500, 30500, 40500, 60500, 90500, 120500, 180500, 240500, 360500, 480500, 600500, 900500, 1200500, 1800500, 2400500, 3600500, 4800500, 7200500};


char *b_txt(uint16_t i){
    static char* expstr[] = {"1/1000", "1/500", "1/350", "1/250", "1/180", "1/125",
                              " 1/90", " 1/60", " 1/45", " 1/30", " 1/20", " 1/15",
                              " 1/10", "  1/8", "  1/6", "  1/4", "  1/3", "  1/2",
                              " 0.7s", "   1s", " 1.5s", "   2s", "   3s", "   4s",
                              "   6s", "   8s", "  10s", "  15s", "  20s", "  30s",
                              "  40s", "   1m", " 1.5m", "   2m", "   3m", "   4m",
                              "   6m", "   8m", "  10m", "  15m", "  20m", "  30m",
                              "  40m", "  60m", "  80m", " 120m"};
    return expstr[i];
}

char *n_txt(uint16_t i){
  static char res[4];
  if (i == 0) return " \6\7";
  sprintf(res, "%3u", i);
  return res;
}

void timeset(unsigned int *i, int x, int y){
  char pos = 3, pos_[] = { 0, 1, 3, 4};
  boolean fin = false;
  int value[4];
  fill_tm(*i, value);
  lcd.setCursor(x,y);
  while (!fin){
    lcd.cursor();
    lcd.setCursor(pos_[pos]+x,y);
    switch (readButtonRelease()){
      case 'R':
        pos++;
        pos %= 4;
        break;
      case 'L':
        pos--;
        pos += 4;
        pos %= 4;
        break;
      case 'U':
        value[pos] ++;
        value[pos] %= (pos%2)? 10: 6;
        break;
      case 'D':
        value[pos] --;
        value[pos] += (pos%2)? 10: 6;
        value[pos] %= (pos%2)? 10: 6;
        break;
      case 'S':
        fin = true;
        lcd.noCursor();
        break;
    }
    lcd.noBlink();
    lcd.setCursor(x,y);
    *i = value[3] + value[2]*10 + value[1]*60 + value[0]*600;
    lcd.print(t_ms(*i));
  }
}

void numset(unsigned int *value, char ud, int x, int y, char* (*callback)(uint16_t val)){
   unsigned int inc = 1;
   long t0 = millis();
   while(readButtonPress() == ud){
    if (ud == 'R'){
     *value += 1;
     *value %= 1000;
    }
    else if (ud == 'L'){
     *value += (1000 - 1);
     *value %= 1000;
    }

    lcd.setCursor(x,y);
    lcd.print((*callback)(*value));
    inc = fib(1+(millis() - t0)/1000);
    delay(1000/(2*inc));
   }
}

void timeIt(uint16_t time, uint16_t number){
  long t_0;
  lcd.clear();
  while (number--){
    uint16_t t = time;
    t_0 = millis();

    while (t--){
        lcd.setCursor(6,0);
        lcd.print(t_ms(t));
        delay(999);
    }
    lcd.setCursor(6,1);
    lcd.print("FOTO  ");
    lcd.print(number);
    lcd.setCursor(0,1);
    lcd.print(millis() - t_0);
    fire(500);
  }
  
  readButtonRelease();
  lcd.clear();
}

void timer(void){
  uint16_t setting = 0;
  uint16_t time = 5;
  uint16_t trigger = 0;

  minimenu mm[3] = {
    (minimenu){false, "time", 0, 0, _ms,  &time,    t_ms},
    (minimenu){false, "trig", 0, 1, _int, &trigger, t_txt},
    (minimenu){false, "ok",  12, 1, _ok,  0,        0}
  };
  lcd.clear();

  while (true){
    mm[setting].active = true;
    mm_print(mm, 3);
    switch (readButtonRelease()){
      case 'D':
        mm[setting].active = false;
        setting += 1;
        setting %= 3;
        break;
      case 'U':
        mm[setting].active = false;
        setting += 3;
        setting -= 1;
        setting %= 3;
        break;
      case 'S':
        if (setting == 2){
          timeIt(time, 1);
        }
        break;
      case 'R':
        if (setting == 1){
          trigger += 3;
          trigger -= 1;
          trigger %= 3;
        }

        if (setting == 0){
          timeset(&time, mm[setting].x+strlen(mm[setting].caption)+2, mm[setting].y);
        }

        break;
      case 'L':
        if (setting == 0){
          return;
        }
        else if (setting == 1){
          trigger += 1;
          trigger %= 3;
        }
        break;
    }
  }
}

void intervalometer(void){
  uint16_t setting = 0;
  uint16_t time = 5;
  uint16_t number = 10;

  minimenu mm[3] = {
    (minimenu){false, "time", 0, 0, _ms,  &time,   t_ms},
    (minimenu){false, "num", 0, 1,  _int, &number, n_txt},
    (minimenu){false, "ok",  12, 1, _ok,  0,       0}
  };
  lcd.clear();

  while (true){
    mm[setting].active = true;
    mm_print(mm, 3);
    switch (readButtonPress()){
      case 'D':
        readButtonRelease();
        mm[setting].active = false;
        setting += 1;
        setting %= 3;
        break;
      case 'U':
      readButtonRelease();
        mm[setting].active = false;
        setting += 3;
        setting -= 1;
        setting %= 3;
        break;
      case 'S':
      readButtonRelease();
        if (setting == 0){
          timeset(&time, mm[setting].x+strlen(mm[setting].caption)+2, mm[setting].y);
        }
        else if (setting == 2){
          timeIt(time, number);
        }
        break;
      case 'R':
        if (setting == 1){
          numset(&number, 'R', mm[setting].x+strlen(mm[setting].caption)+2, mm[setting].y,n_txt);
        }
        break;
      case 'L':
        if (setting == 0){
            readButtonRelease();
          return;
        }
        else if (setting == 1){
          numset(&number, 'L', mm[setting].x+strlen(mm[setting].caption)+2, mm[setting].y,n_txt);
        }
        break;
    }
  }
}

#define BRACKETWAIT 500

#define EVMIN 19          // min 1 second
#define EVMAX 45          // max 120 min

void hdrIt(int b, int w, int n){

/*
byte b = 27;             // base exposure 15 sec, from exptime[]
byte w = 3;              // +- 3EV
byte n = 5;              // 5 brackets
*/
    int ev;
    delay(2000);
    fire(exptime[b]);
    delay(BRACKETWAIT);

    // Do the EV Bracket dance
    for (byte i=(n-1)/2; i>=1; i--){
      ev = b-((4*w*i)/(n-1));          // bracket dance on the short side
      if (ev>=EVMIN){                  // only exposures 1 sec and longer
        fire(exptime[ev]);
        delay(BRACKETWAIT);
      }
      ev = b+((4*w*i)/(n-1));          // bracket dance on the long side
      if (ev<=EVMAX){                  // only exposures 120min and shorter
        fire(exptime[ev]);
        delay(BRACKETWAIT);
      }
    }
}


void hdr(void){
  uint16_t setting = 0;
  uint16_t be = 5;
  uint16_t b = 10;
  uint16_t ev = 3;

  minimenu mm[4] = {
    (minimenu){false, "EV",   0, 0, _int, &ev, n_txt},
    (minimenu){false, "base", 0, 1, _int, &be, b_txt},
    (minimenu){false, "n",    9, 0, _int, &b,  n_txt},
    (minimenu){false, "ok",  12, 1, _int, 0,   0}
  };
  lcd.clear();

  while (true){
    mm[setting].active = true;
    mm_print(mm, 4);
    switch (readButtonPress()){
      case 'D':
        readButtonRelease();
        mm[setting].active = false;
        setting += 1;
        setting %= 4;
        break;
      case 'U':
      readButtonRelease();
        mm[setting].active = false;
        setting += 4;
        setting -= 1;
        setting %= 4;
        break;
      case 'S':
      readButtonRelease();
        if (setting == 3)
            hdrIt(be, ev, b);
        break;
      case 'R':
        numset(mm[setting].val, 'R', mm[setting].x+strlen(mm[setting].caption)+2, mm[setting].y,mm[setting].callback);
        //~ if (setting == 0)
        break;
      case 'L':
        numset(mm[setting].val, 'L', mm[setting].x+strlen(mm[setting].caption)+2, mm[setting].y,mm[setting].callback);
        break;
    }
  }
}


void bulb(void){
  uint16_t setting = 0;
  uint16_t time = 5;
  uint16_t trigger = 0;

  minimenu mm[3] = {
    (minimenu){false, "time", 0, 0, _ms,  &time,    t_ms},
    (minimenu){false, "trig", 0, 1, _int, &trigger, t_txt},
    (minimenu){false, "ok",  12, 1, _ok,  0,        0}
  };
  lcd.clear();

  while (true){
    mm[setting].active = true;
    mm_print(mm, 3);
    switch (readButtonRelease()){
      case 'D':
        mm[setting].active = false;
        setting += 1;
        setting %= 3;
        break;
      case 'U':
        mm[setting].active = false;
        setting += 3;
        setting -= 1;
        setting %= 3;
        break;
      case 'S':
        if (setting == 2){
          fire(time*1000);
        }
        break;
      case 'R':
        if (setting == 1){
          trigger += 3;
          trigger -= 1;
          trigger %= 3;
        }

        if (setting == 0){
          timeset(&time, mm[setting].x+strlen(mm[setting].caption)+2, mm[setting].y);
        }

        break;
      case 'L':
        if (setting == 0){
          return;
        }
        else if (setting == 1){
          trigger += 1;
          trigger %= 3;
        }
        break;
    }
  }
}
