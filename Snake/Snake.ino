#include <LedControl.h>
#include <time.h>
#define GRID   8
#define DELAY 250

int DIN = 12;
int CS =  11;
int CLK = 10;
const int button1Pin = 2;     // the number of the pushbutton pin
const int button2Pin = 1;     // the number of the pushbutton pin


// variables will change:
int button1State = 0, button2State = 0; 
int matriz[GRID][GRID];
int dir = 0;
int vx = 0, vy = 0, headx = 4, heady = 4, applex = 0, appley = 0, snake_size = 2;
int trail[GRID*GRID][2];
byte g_over[GRID] = {B01110000,
  B10000000,
  B10110000,
  B10010000,
  B01100110,
  B00001001,
  B00001001,
  B00000110};

LedControl lc=LedControl(DIN,CLK,CS,0);

void setup(){
  srand(time(NULL));
  lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0,0);      // Set the brightness to maximum value
  lc.clearDisplay(0);         // and clear the display
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  vx = vy = dir = button1State = button2State = 0;
  headx = 4;
  heady = 4;
  snake_size = 2;
  trail[0][0]= 4;
  trail[0][1]= 3;
  generateApple();
  refreshMatrix();
  printMatrix();
  while(digitalRead(button1Pin) != HIGH && digitalRead(button2Pin) != HIGH)
    delay(1);
}

void loop(){ 
  int game_over = 0, i = 0;
  button1State = LOW;
  button2State = LOW;
  for(i = 0; i < DELAY*3/4; i++)
  {
    if(digitalRead(button1Pin) == HIGH)
    {
      button1State = HIGH;
      button2State = LOW;
    }
    if(digitalRead(button2Pin) == HIGH)
    {
      button2State = HIGH;
      button1State = LOW;
    }
    delay(1);
  }
  delay(DELAY/4);

  if (button1State == HIGH || button2State == HIGH) 
  {
    if(button1State == HIGH)
        dir++;
    else if(button2State == HIGH)
    {
      dir--;
      if(dir < 0)
        dir = 3;
    }
    dir = dir%4;
    if(dir == 0)
      { vx = 1; vy = 0;}
    else if(dir == 1)
       {vx = 0; vy = 1;}
    else if(dir == 2)
       {vx = -1; vy = 0;}
    else if(dir == 3)
      { vx = 0; vy = -1;}
  }
  for(i = snake_size - 2; i > 0; i--)
  {
    trail[i][0] = trail[i-1][0];
    trail[i][1] = trail[i-1][1];
  }
  trail[0][0] = headx;
  trail[0][1] = heady;
  headx += vx;
  heady += vy;
  if(headx >= GRID) headx = 0;
  if(headx < 0) headx = GRID - 1;
  if(heady >= GRID) heady = 0;  
  if(heady < 0) heady = GRID - 1;
  for(i = snake_size - 2; i > 0; i--)
  {
    if(headx == trail[i][0] && heady == trail[i][1])
      game_over = 1;
  }
  if(headx == applex && heady == appley)
  {
    snake_size++;
    trail[snake_size - 2][0] = applex;
    trail[snake_size - 2][1] = appley;
    generateApple();
  }
  
  refreshMatrix();
  printMatrix();
  if(game_over == 1)
  {
    while(digitalRead(button1Pin) != HIGH && digitalRead(button2Pin) != HIGH)
    {
      for(i = 0; i < GRID; i++)
      {
        lc.setRow(0,i,g_over[i]);
      }
      delay(1);
    }
    setup();
  }
  
    
}

void generateApple()
{
    applex = -1;
    appley = -1;
    do{
      applex = rand()%GRID;
      appley = rand()%GRID;
    }while(matriz[applex][appley] != 0);
    matriz[applex][appley] = 1;
}

void printMatrix()
{
  int i = 0;
  byte character = 0x00;
  byte aux = 0x00;
  for(i=0;i<GRID;i++)
  {
    character = 0x00;
    for(int j = 0; j < GRID; j++)
    {
      if(matriz[i][j] == 1)
      {
        aux = 1 << j;
        character = character | aux ;
      }
    }
    lc.setRow(0,i,character);
  }
}

void refreshMatrix()
{
  for(int i = 0; i < GRID; i++)
  {
    for(int j = 0; j < GRID; j++)
    {
      matriz[i][j] = 0;
    }
  }
  for(int i = snake_size - 2; i >= 0; i--)
  {
    matriz[trail[i][0]][trail[i][1]] = 1;
  }
  matriz[headx][heady] = 1;
  matriz[applex][appley] = 1;
}


