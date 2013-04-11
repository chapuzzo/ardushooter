
/*
int fib(int n){
  if (n < 2) return n;
  else
  return fib(n-1) + fib(n-2); 
}*/

/*
void timescroll(char ud, unsigned int *value){
   unsigned int inc = 1;
   long t0 = millis();
   while(readButtonRelease() == ud){
    if (ud == 'R'){
     *time += 1;
     *time %= 1000;
    }
    else { 
     *time -= 1;
     *time = constrain(*value, 0, 1000);
    }
     
    lcd.setCursor(8,0);
    lcd.print(t_ms(*value));
    lcd.setCursor(9, 1);
    lcd.print("     ");
    lcd.setCursor(9, 1);
    lcd.print(inc);
    inc = fib(1+(millis() - t0)/1000);
    delay(1000/(2*inc));
   }       
}
*/

//~ void printString(String s){
  //~ lcd.setCursor(1,0);
  //~ lcd.print(s.substring(0,15));
  //~ if (s.length() > 16)
  //~ {
    //~ lcd.setCursor(0,1);
    //~ lcd.print(s.substring(16,32));
  //~ } 
//~ }
