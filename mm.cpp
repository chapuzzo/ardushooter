#include "mm.h"
#include "menu.h"
#include "time.h"
#include "LiquidCrystal.h"

extern LiquidCrystal lcd;

void mm_print(minimenu *mm){
    while (mm){
      lcd.setCursor(mm->x,mm->y);
      if (mm->active) lcd.write(RA);
      else lcd.write(EA);
      lcd.print(mm->caption);
      lcd.print(' ');
        if (mm->type != 2)
            if (mm->active) lcd.write(LA);
            else lcd.write(' ');
        lcd.print((*mm->callback)(*mm->val));
        if (mm->type != 2)
            if (mm->active) lcd.write(RA);
            else lcd.write(' ');
    }


  //~ switch (mm->type){
    //~ case 0:
        //~ if (mm->active) lcd.write(LA);
        //~ else lcd.write(' ');
        //~ lcd.print(t_ms(*mm->val));
        //~ if (mm->active) lcd.write(RA);
        //~ else lcd.write(' ');
    //~ break;
    //~ case 1:
        //~ if (mm->active) lcd.write(LA);
        //~ else lcd.write(' ');
        //~ lcd.print(t_txt(*mm->val));
        //~ if (mm->active) lcd.write(RA);
        //~ else lcd.write(' ');
    //~ break;
    //~ case 3:
        //~ if (mm->active) lcd.write(LA);
        //~ else lcd.write(' ');
        //~ lcd.print(n_txt(*mm->val));
        //~ if (mm->active) lcd.write(RA);
        //~ else lcd.write(' ');
    //~ break;
    //~ case 4:
        //~ if (mm->active) lcd.write(LA);
        //~ else lcd.write(' ');
        //~ lcd.print(exp_txt(*mm->val));
        //~ if (mm->active) lcd.write(RA);
        //~ else lcd.write(' ');
    //~ break;
  //~ }
}
