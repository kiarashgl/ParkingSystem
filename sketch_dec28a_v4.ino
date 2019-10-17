#include <LiquidCrystal.h>
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define INCOME 0
#define CARS_INSIDE 1
#define TOTAL_CAPACITY 2
#define TOTAL_IN 3
#define TOTAL_OUT 4
#define WELCOME_SCREEN -1
#define FULL_CAPACITY -2
#define EMPTY_PARKING -3
int ir_out = 3,ir_in = 2;
bool flag = false;
int menu = -1, prevMenu;
int income = 0, total_capacity = 10, cars_inside = 0, total_cars_in = 0, total_cars_out = 0;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ir_in, INPUT);
  pinMode(ir_out, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to ");
  lcd.setCursor(0,1);
  lcd.print("parking");
// Print a message to the LCD.

  lcd.noAutoscroll();
}

void car_in()
{
  if(cars_inside == total_capacity)
  {
    prevMenu = menu;
    menu = FULL_CAPACITY;
  }
  else
  {
    total_cars_in ++;
    cars_inside ++;
  }
}
void car_out()
{
  if(cars_inside == 0)
  {
    prevMenu = menu;
    menu = EMPTY_PARKING;
  }
  else
  {
    total_cars_out ++;
    cars_inside --;
    income += 5;
  }
}



void print_menu()
{
  lcd.clear();
  if (menu == INCOME)
  {
    lcd.setCursor(0,0);
    lcd.print("INCOME: ");
    lcd.setCursor(0,1);
    lcd.print(income);
    lcd.print("$");
  }
  if (menu == TOTAL_IN)
  {
    lcd.setCursor(0,0);
    lcd.print("Total Cars in: ");
    lcd.setCursor(0,1);
    lcd.print(total_cars_in);
  }
  if (menu == TOTAL_OUT)
  {
    lcd.setCursor(0,0);
    lcd.print("Total Cars out: ");
    lcd.setCursor(0,1);
    lcd.print(total_cars_out);
  }
  if (menu == TOTAL_CAPACITY)
  {
    lcd.setCursor(0,0);
    lcd.print("Capacity: ");
    lcd.setCursor(0,1);
    lcd.print(total_capacity);
  }
  if (menu == CARS_INSIDE)
  {
    lcd.setCursor(0,0);
    lcd.print("Cars Inside: ");
    lcd.setCursor(0,1);
    lcd.print(cars_inside);
  }
  if (menu == FULL_CAPACITY)
  {
    lcd.setCursor(0,0);
    lcd.print("Full Capacity!");
    delay(2000);
    menu=prevMenu;
    print_menu();
  }
  if (menu == EMPTY_PARKING)
  {
    lcd.setCursor(0,0);
    lcd.print("Empty Parking!");
    delay(2000);
    menu=prevMenu;
    print_menu();
  }
}




int read_LCD_buttons()
{
 int key = analogRead(0);
 if (key > 1000) return btnNONE;
 if (key < 50)   return btnRIGHT;  
 if (key < 250)  return btnUP; 
 if (key < 450)  return btnDOWN; 
 if (key < 650)  return btnLEFT; 
 if (key < 850)  return btnSELECT;  
 return btnNONE;
}
void handle_button()
{
 int lcd_key = read_LCD_buttons();
 switch (lcd_key)
 {
   case btnRIGHT:
     {
     menu = INCOME;
     print_menu();
     break;
     }
   case btnLEFT:
     {
     menu = TOTAL_CAPACITY;
     print_menu();
     break;
     }
   case btnUP:
     {
       menu = TOTAL_IN;
       print_menu();
     break;
     }
   case btnDOWN:
     {
     menu = TOTAL_OUT;
     print_menu();
     break;
     }
   case btnSELECT:
     {
     menu = CARS_INSIDE;
     print_menu();
     break;
     }
     case btnNONE:
     {
     break;
     }
 }
 
}
void loop() {
  // put your main code here, to run repeatedly:
  int read_ir_in = digitalRead(ir_in);
  int read_ir_out = digitalRead(ir_out);
  
  if( read_ir_in ==0 && read_ir_out == 0)
  {
    Serial.println("Car");
    flag = true;
  }
  else if(flag)
  {
    flag = false;
    if (read_ir_in == 1 && read_ir_out == 0)
     car_in();
    else if(read_ir_out == 1 && read_ir_in == 0)
      car_out();
    print_menu();
  }
  if(menu == WELCOME_SCREEN)
  {
    lcd.scrollDisplayRight();
    delay(400);
  }
  handle_button();
  digitalWrite(LED_BUILTIN,read_ir_in);
  

}
