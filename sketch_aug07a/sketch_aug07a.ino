#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <time.h>
//Pins relativos ao display Nokia 5110
#define RST_PIN 12
#define CE_PIN 11
#define DC_PIN 10
#define DIN_PIN 9
#define CLK_PIN 8
#define BL_PIN 7
//Pins relativos aos botões
#define LEFT_BTN_PIN 6
#define RIGHT_BTN_PIN 3
#define GREEN_BTN_PIN 4
#define RED_BTN_PIN 5
#define LED_PIN 13
//outras constates
#define SNAKE_GRID_X 21//84
#define SNAKE_GRID_Y 12 //48
#define MAX_SNAKE_LENGHT SNAKE_GRID_X*SNAKE_GRID_Y

//inicialização do display
Adafruit_PCD8544 display = Adafruit_PCD8544(CLK_PIN, DIN_PIN, DC_PIN, CE_PIN, RST_PIN);
int contrast = 50;
int new_contrast = 50;
boolean backlight = false;
int current_game = 0; //0-nenhum, 1-snake, 2-tetris, 
int current_menu = 0;//0-menu principal, 1-jogos, 2-definições, 3-contraste
int selected_menu = 0;
boolean left_btn_state_prev = LOW;
boolean right_btn_state_prev = LOW;
boolean green_btn_state_prev = LOW;
boolean red_btn_state_prev = LOW;
const int width = display.width();
const int height = display.height();
int game_delay = 150;
int i = 0;
int j = 0;
typedef struct
{
  int v_x;
  int v_y;
  int dir;
  int head_x;
  int head_y;
  int size;
  int apple_x;
  int apple_y;
  char trail[MAX_SNAKE_LENGHT][2];
  boolean over;
  
}snake_info;


//inicialização da snake
snake_info snake = {0};

void setup() 
{
  srand(time(NULL));
  pinMode(LEFT_BTN_PIN, INPUT);
  pinMode(RIGHT_BTN_PIN, INPUT);
  pinMode(GREEN_BTN_PIN, INPUT);
  pinMode(RED_BTN_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  display.begin();      
  display.setContrast(contrast); //Set contrast to 50
  display.clearDisplay(); 
  display.display();   
  snake.v_x = snake.v_y = snake.dir = 0;
  snake.head_x = SNAKE_GRID_X/2;
  snake.head_y = SNAKE_GRID_Y/2;
  snake.size = 2;
  snake.trail[0][0]= SNAKE_GRID_X/2;
  snake.trail[0][1]= SNAKE_GRID_Y/2 - 1;
  snake.over = false;
  Serial.begin(9600);
  Serial.println(sizeof(int));
  Serial.println(sizeof(unsigned int));
  Serial.println(sizeof(char));
  Serial.println(sizeof(long));
  Serial.println(sizeof(bool));
  Serial.println(sizeof(boolean));
}

void loop() 
{
  if(current_game == 0)
  {
    menu();
  }
  else if(current_game == 1)
  {
    snake_game();
  }
  else if(current_game == 2)
  {
    //tetris();
  }
  else if(current_game == 3)
  {
    //spaceInvaders();
  }

}

void snake_game()
{
  boolean left = false;
  boolean right = false;
  
  
  for(i = 0; i < game_delay*3/4; i++)
  {
    if(checkLeftBtnPress())
    {
      left = false;
      right = true;
    }
    if(checkRightBtnPress())
    {
      left = true;
      right = false;
    }
    if(checkRedBtnPress())
    {
      snake.v_x = snake.v_y = 0;
      current_game = 0;
      if(snake.over)
      {
        snake.dir = 0;
        snake.head_x = SNAKE_GRID_X/2;
        snake.head_y = SNAKE_GRID_Y/2;
        snake.size = 2;
        snake.trail[0][0]= SNAKE_GRID_X/2;
        snake.trail[0][1]= SNAKE_GRID_Y/2 - 1;
        snake.over = false;
      }
    }
    delay(1);
  }
  delay(game_delay/4);

  if (left || right) 
  {
    if(left)
        (snake.dir)++;
    else if(right)
    {
      (snake.dir)--;
      if((snake.dir) < 0)
        (snake.dir) = 3;
    }
    (snake.dir) = (snake.dir)%4;
    if((snake.dir) == 0)
      { snake.v_x = 1; snake.v_y = 0;}
    else if(snake.dir == 1)
       {snake.v_x = 0; snake.v_y = 1;}
    else if(snake.dir == 2)
       {snake.v_x = -1; snake.v_y = 0;}
    else if(snake.dir == 3)
      { snake.v_x = 0; snake.v_y = -1;}
  }
  if(snake.v_x != 0 || snake.v_y != 0)
  {
    for(i = snake.size - 2; i > 0; i--)
    {
      snake.trail[i][0] = snake.trail[i-1][0];
      snake.trail[i][1] = snake.trail[i-1][1];
    }
    snake.trail[0][0] = snake.head_x;
    snake.trail[0][1] = snake.head_y;
    snake.head_x += snake.v_x;
    snake.head_y += snake.v_y;
  }
  
  if(snake.head_x >= SNAKE_GRID_X) snake.head_x = 0;
  if(snake.head_x < 0) snake.head_x = SNAKE_GRID_X - 1;
  if(snake.head_y >= SNAKE_GRID_Y) snake.head_y = 0;  
  if(snake.head_y < 0) snake.head_y = SNAKE_GRID_Y - 1;
  for(i = snake.size - 2; i > 0; i--)
  {
    if(snake.head_x == snake.trail[i][0] && snake.head_y == snake.trail[i][1])
      snake.over = true;
  }
  if(snake.head_x == snake.apple_x && snake.head_y == snake.apple_y)
  {
    snake.size++;
    snake.trail[snake.size - 2][0] = snake.apple_x;
    snake.trail[snake.size - 2][1] = snake.apple_y;
    generateApple();
  }
  
  drawSnake();
  if(snake.over)
  {
    snake.v_x = snake.v_y = 0;
    gameOverText();
  }
  display.display();
}

void gameOverText()
{
  display.clearDisplay(); 
  display.setTextSize(2);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(20, 10);
  display.print("GAME   OVER");
}

void drawSnake()
{
  int block_size = width/SNAKE_GRID_X;
  display.clearDisplay(); 
  //apple
  display.fillRect(snake.apple_x * block_size,snake.apple_y * block_size,block_size,block_size,BLACK);
  //Head
  display.fillCircle(snake.head_x * block_size + block_size/2,snake.head_y * block_size + block_size/2,block_size/2,BLACK);
  //body
  for(i = snake.size - 2; i >= 0; i--)
  {
    display.drawCircle(snake.trail[i][0] * block_size + block_size/2 ,snake.trail[i][1] * block_size + block_size/2 ,block_size/2,BLACK);
  }
}


void generateApple()
{
  snake.apple_x = -1;
  snake.apple_y = -1;
  do{
    snake.apple_x = rand()%SNAKE_GRID_X;
    snake.apple_y = rand()%SNAKE_GRID_Y;
  }while(isOccupied(snake.apple_x, snake.apple_y));
}

boolean isOccupied(int x, int y)
{
  if(snake.head_x == x && snake.head_y == y)
    return true;
   for(i = 0; i < snake.size - 1; i++)
   {
    if(snake.trail[i][0] == x && snake.trail[i][1] == y)
      return true;
   }
   return false;
}
void menu()
{
  if(digitalRead(LEFT_BTN_PIN) == HIGH || digitalRead(RIGHT_BTN_PIN) == HIGH)
    digitalWrite(LED_PIN, HIGH);
   else
   digitalWrite(LED_PIN, LOW);
   
  int num_of_menus = 0;
  drawMenu();
  
  if(checkRightBtnPress())
  {
    if(current_menu != 3)
      selected_menu = selected_menu + 1;
    else
      new_contrast = new_contrast + 1;
  }
  if(checkLeftBtnPress())
  {
    if(current_menu != 3)
      selected_menu = selected_menu - 1;
    else
      new_contrast = new_contrast - 1;
  }
  if(checkGreenBtnPress())
  {
    if(current_menu == 0)
    {
      if(selected_menu == 0)
        current_menu = 1;
      else if(selected_menu == 1)
        current_menu = 2;

      selected_menu = 0;
    }
    else if(current_menu == 1)
    {
      if(selected_menu == 0)
        current_game = 1;
      else if(selected_menu == 1)
        current_game = 2;
      else if(selected_menu == 2)
        current_game = 3;
    }
    else if(current_menu == 2)
    {
      if(selected_menu == 0)
        current_menu = 3;
      else if(selected_menu == 1)
      {
        backlight = !backlight;
        digitalWrite(BL_PIN, backlight);
      }
        
    }
    else if(current_menu == 3)
    {
      contrast = new_contrast;
      display.setContrast(contrast);
      current_menu = 2;
    }
  }
  if(checkRedBtnPress())
  {
    if(current_menu == 1 || current_menu == 2)
    {
      current_menu = 0;
      selected_menu = 0;
    }
    else if(current_menu == 3)
    {
      new_contrast = contrast;
      display.setContrast(contrast);
      current_menu = 2;
      selected_menu = 0;
    }
  }
  
  if(current_menu == 0 || current_menu == 2)
  {
    num_of_menus = 2;
  }
  else if(current_menu == 1)
  {
    num_of_menus = 3;
  }
  
  if(selected_menu < 0)
  {
    selected_menu = num_of_menus - 1;
  }
  if(selected_menu >= num_of_menus)
  {
    selected_menu = 0;
  }

  if(new_contrast < 0)
  {
    new_contrast = 99;
  }
  if(new_contrast >= 99)
  {
    new_contrast = 0;
  }
}

void drawMenu()
{
  if(current_menu == 0) 
  {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(0, 1);
    display.print("Menu Principal");
    display.drawFastHLine(0,10,width,BLACK);
    display.setCursor(0, 15);

    if(selected_menu == 0)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Jogos");
    display.setCursor(0, 25);

    if(selected_menu == 1)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Definicoes");
  }
  else if(current_menu == 1)
  {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(25, 1);
    display.print("Jogos");
    display.drawFastHLine(0,10,width,BLACK);
    display.setCursor(0, 15);

    if(selected_menu == 0)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Snake");
    display.setCursor(0, 25);

    if(selected_menu == 1)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Tetris");
    display.setCursor(0, 35);

    if(selected_menu == 2)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Space Invader");
  }
  else if(current_menu == 2)
  {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 1);
    display.print("Definicoes");
    display.drawFastHLine(0,10,width,BLACK);
    display.setCursor(0, 15);

    if(selected_menu == 0)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    display.print(">Contraste: ");
    display.setCursor(70, 15);
    display.print(contrast);
    display.setCursor(0, 25);

    if(selected_menu == 1)
    {
      display.setTextColor(WHITE, BLACK);
    }
    else 
    {
      display.setTextColor(BLACK, WHITE);
    }
    if(backlight)
      display.print(">Luz:     <ON>");
    else
      display.print(">Luz:    <OFF>");
  }
  else if(current_menu == 3)
  {
    display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 1);
    display.print("Contraste");
    display.drawFastHLine(0,10,width,BLACK);
    display.setTextSize(3);
    display.setCursor(5, 20);
    display.print("<");
    display.setCursor(60, 20);
    display.print(">");
    display.setCursor(25, 20);
    display.print(new_contrast);
    display.setContrast(new_contrast);
  }
  display.display();
}

boolean checkLeftBtnPress()
{
  boolean left_btn_state = digitalRead(LEFT_BTN_PIN);
  if(left_btn_state == HIGH && left_btn_state_prev == LOW)
  {
    left_btn_state_prev = left_btn_state;
    return true;
  }
  else
  {
    left_btn_state_prev = left_btn_state;
    return false;
  }
}

boolean checkRightBtnPress()
{
  boolean right_btn_state = digitalRead(RIGHT_BTN_PIN);
  if(right_btn_state == HIGH && right_btn_state_prev == LOW)
  {
    right_btn_state_prev = right_btn_state;
    return true;
  }
  else
  {
    right_btn_state_prev = right_btn_state;
    return false;
  } 
}

boolean checkRedBtnPress()
{
  boolean red_btn_state = digitalRead(RED_BTN_PIN);
  if(red_btn_state == HIGH && red_btn_state_prev == LOW)
  {
    red_btn_state_prev = red_btn_state;
    return true;
  }
  else
  {
    red_btn_state_prev = red_btn_state;
    return false;
  }
}

boolean checkGreenBtnPress()
{
  boolean green_btn_state = digitalRead(GREEN_BTN_PIN);
  if(green_btn_state == HIGH && green_btn_state_prev == LOW)
  {
    green_btn_state_prev = green_btn_state;
    return true;
  }
  else
  {
    green_btn_state_prev = green_btn_state;
    return false;
  }
}

