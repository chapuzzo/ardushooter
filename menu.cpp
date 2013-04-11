#include "menu.h"
#include <LiquidCrystal.h>
#include "arrows.h"

#define DEBUG

menu root[10], *m;
extern LiquidCrystal lcd;

void creaMenus(){
  lcd.createChar(6, ard_l);
  lcd.createChar(7, ard_r);

  //root = (menu*)calloc(10,sizeof(menu));
  //              { texto           ,abbr, ini ,  s  ,              l  ,    u  ,    d  ,   r   }
  root[0] = (menu){"Ardush\6\7ter"  , 'A', NULL, NULL,           root+4, root+4, root+4, root+4};

  root[1] = (menu){"SETTINGS"       , 'S', NULL, NULL,           NULL  , root+4, root+4, root+2};
  root[2] = (menu){"Sensor"         , 's', NULL, sensor,         root+1, root+3, root+3, NULL  };
  root[3] = (menu){"Remote"         , 'r', NULL, remote,         root+1, root+2, root+2, NULL  };

  root[4] = (menu){"MODE"           , 'M', NULL, NULL ,          NULL  , root+1, root+1, root+5};
  root[5] = (menu){"Timer"          , 't', NULL, timer,          root+4, root+8, root+6, NULL  };
  root[6] = (menu){"Intervalometer" , 'i', NULL, intervalometer, root+4, root+5, root+7, NULL  };
  root[7] = (menu){"Bulb"           , 'b', NULL, bulb,           root+4, root+6, root+8, NULL  };
  root[8] = (menu){"HDR"            , 'h', NULL, hdr,            root+4, root+7, root+5, NULL  };

  setMenu(root);
  lcd.createChar(EA, empty_right_arrow);
  lcd.createChar(RA, right_arrow);
  lcd.createChar(LA, left_arrow);
  lcd.createChar(UA, up_arrow);
  lcd.createChar(DA, down_arrow);
  lcd.createChar(UD, down_up_arrow);
}

void setMenu(menu *m_){
    if (m_ != NULL){
        m = m_;
        printCaption(m);
        if (m->i){
          m->i();
        }
      #ifdef DEBUG
        Serial.println(m->a);
        lcd.setCursor(5,1);
        lcd.print(millis());
      #endif
    }
}

void printCaption(menu *m_){
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print(m_->t);

    if (m_->l != NULL){
        lcd.setCursor(0,0);
        lcd.write(LA);
    }
    if (m_->r != NULL){
        lcd.setCursor(strlen(m_->t)+1,0);
        lcd.write(RA);
    }
    if (m_->u != NULL){
        lcd.setCursor(15,0);
        lcd.write(UA);
    }
    if (m_->d != NULL){
        lcd.setCursor(15,1);
        lcd.write(DA);
    }
}

char buttonValue(int i){
    char *values="RUDLSSXX";
    return values[i>>7];
}

char readButtonPress(void){
  char value = buttonValue(analogRead(BUTTON_PIN));
  delay(10);
  if (value == buttonValue(analogRead(BUTTON_PIN)))
    return buttonValue(analogRead(BUTTON_PIN));
  else
    return 'X';
}

void waitButtonPress(){
  while (readButtonPress() == 'X');
}

char readButtonRelease(){
  char value;
  value = readButtonPress();
  while (readButtonPress() == value);
  return value;
}

void set(){
    if (m->s != NULL){
            m->s();
            setMenu(m);
    }
}

void moveLeft(){
    setMenu(m->l);
}

void moveRight(){
    setMenu(m->r);
}

void moveUp(){
    setMenu(m->u);
}

void moveDown(){
    setMenu(m->d);
}

void buttonDispatch(void){
  char *opts = "LRUDS";
  void (*i[5])() = {moveLeft,
                    moveRight,
                    moveUp,
                    moveDown,
                    set};
  (*i[strchr(opts,readButtonRelease())-opts])();

  /*switch (readButtonRelease()){
    case 'L': moveLeft(); break;
    case 'R': moveRight(); break;
    case 'U': moveUp(); break;
    case 'D': moveDown(); break;
    case 'S': set(); break;
  }*/
}
