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

#define SHUTTERPIN 4      // which pin is the shutter connected to?


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
  static char *res[] =  {"sel", "rmt", "sns"};
  return res[i];
}

unsigned long exptime[] = {1, 2, 3, 4, 6, 8, 11, 17, 22, 33, 50, 67, 100, 125, 167, 250, 333, 500, 666, 1000, 1500, 2600, 3300, 4600, 6600, 8600, 10500, 15500, 20500, 30500, 40500, 60500, 90500, 120500, 180500, 240500, 360500, 480500, 600500, 900500, 1200500, 1800500, 2400500, 3600500, 4800500, 7200500};


char *b_txt(uint16_t i){
    static char* expstr[] = {"1/1000s", "1/500s", "1/350s", "1/250s", "1/180s", "1/125s", "1/90s", "1/60s", "1/45s", "1/30s", "1/20s", "1/15s", "1/10s", "1/8s", "1/6s", "1/4s", "1/3s", "1/2s", "0.7s", "1sec", "1.5sec", "2sec", "3sec", "4sec", "6sec", "8sec", "10sec", "15sec", "20sec", "30sec", "40sec", "1min", "1.5min", "2min", "3min", "4min", "6min", "8min", "10min", "15min", "20min", "30min", "40min", "60min", "80min", "120min"};
    return expstr[i];
}

char *n_txt(uint16_t i){
  static char res[4];
  if (i == 0) return " \6\7";
  sprintf(res,"%3u",i);
  return res;
}

void timeset(unsigned int *i){
  char pos = 3, pos_[] = { 7, 8, 10, 11};
  boolean fin = false;
  int value[4];
  fill_tm(*i, value);
  lcd.setCursor(6,0);
  while (!fin){
    lcd.cursor();
    lcd.setCursor(pos_[pos],0);
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
    lcd.setCursor(7,0);
    *i = value[3] + value[2]*10 + value[1]*60 + value[0]*600;
    lcd.print(t_ms(*i));
  }
}

void numset(unsigned int *value, char ud){
   unsigned int inc = 1;
   long t0 = millis();
   while(readButtonPress() == ud){
    if (ud == '+'){
     *value += 1;
     *value %= 1000;
    }
    else if (ud == '-'){
     *value += (1000 - 1);
     *value %= 1000;
    }

    lcd.setCursor(7,1);
    lcd.print(n_txt(*value));
    /*lcd.setCursor(9, 1);
    lcd.print("     ");
    lcd.setCursor(9, 1);
    lcd.print(inc);*/
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
  }
  readButtonRelease();
  lcd.clear();
}

void timer(void){
  uint16_t setting = 0;
  uint16_t time = 5;
  uint16_t trigger = 0;

  minimenu mm[] = {
    (minimenu){false, "time", 0, 0, _ms,  &time,    t_ms},
    (minimenu){false, "trig", 0, 1, _int, &trigger, t_txt},
    (minimenu){false, "go",  12, 1, _ok,  0,        0},
    NULL
  };
  lcd.clear();

  while (true){
    mm[setting].active = true;
    mm_print(mm);
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
          timeset(&time);
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

  minimenu mm[] = {
    (minimenu){false, "time", 0, 0, _ms, &time,   t_ms},
    (minimenu){false, "num ", 0, 1, _int, &number,n_txt},
    (minimenu){false, "go",  12, 1, _ok, 0,       0},
    NULL
  };
  lcd.clear();

  while (true){
    mm[setting].active = true;
    mm_print(mm);
    //~ mm_print(mm+1);
    //~ mm_print(mm+2);
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
          timeset(&time);
        }
        else if (setting == 2){
          timeIt(time, number);
        }
        break;
      case 'R':
        if (setting == 1){
          numset(&number,'+');
        }
        break;
      case 'L':
        if (setting == 0){
            readButtonRelease();
          return;
        }
        else if (setting == 1){
          numset(&number,'-');
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

  minimenu mm[] = {
    (minimenu){false, "EV",   0, 0, _int, &ev, n_txt},
    (minimenu){false, "base", 0, 1, _int, &be, b_txt},
    (minimenu){false, "b",   10, 0, _int, &b,  n_txt},
    (minimenu){false, "go",  12, 1, _int, 0,   0},
    NULL
  };
  lcd.clear();

  while (true){
    mm[setting].active = true;
    mm_print(mm);
    //~ mm_print(mm+1);
    //~ mm_print(mm+2);
    //~ mm_print(mm+3);
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
        if (setting == 0){
          //numset(&
        }
        else if (setting == 2){
          //timeIt(time, number);
          //hdrIt(
        }
        break;
      case 'R':
        if (setting == 1){
          //numset(&number,'R');
        }
        break;
      case 'L':
        if (setting == 0){
            readButtonRelease();
          return;
        }
        else if (setting == 1){
          //numset(&number,'L');
        }
        break;
    }
  }
}


