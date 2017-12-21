#include<U8glib.h>
int randNumber;
//定义字体
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
#define setFont_SS u8g.setFont(u8g_font_fub25n)
//定义显示屏型号
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
//定义手柄接口
#define pinUP 4
#define pinDOWN 10
#define pinRIGHT 12
#define pinLEFT 18
//下面定义的这个数组，表示构成蛇身体的位图单元，是一个4×4的方阵
const uint8_t snake_body[] U8G_PROGMEM = {
  0xf0, //B1111 0000
  0xb0, //B1011 0000
  0xd0, //B1101 0000
  0xf0, //B1111 0000
};
//定义蛇运动的方向
#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4

//定义蛇的结构体
struct SNAKE{
  int x[200];//蛇身的x坐标数组
  int y[200];//蛇身的y坐标数组
  int node;//蛇身节数
  int dir;//运动方向
};
//初始化一条蛇
SNAKE snake = {{9,5},{30,30},2,RIGHT};

//定义食物的结构体
struct FOOD{
  int x;
  int y;
  int yes;
};

FOOD food = {25,30,1};

//初始化分数和等级
int score=0;
int level=0;

void setup() {

Serial.begin(9600);
Serial.println("Initialisation complete.");
randomSeed(analogRead(0));


}


void loop() {
if(judge()==true)
  {   
rebegin:
u8g.firstPage();

do{
  UI();
  key();
  for(int i=0;i<snake.node;i++){
    element(snake.x[i],snake.y[i]);
  }
  mov();
  foodfresh();
  eat();
  element(food.x,food.y);
  printSL(109,50,level);
  printSL(109,22,score);
}while(u8g.nextPage());
}

if(judge()==false)
{ 
  do{
  UI();
  for(int i=0;i<=1000;i++)
  do{
  u8g.setPrintPos(25,30);
  u8g.print("Game Over!");}while(u8g.nextPage());
  printSL(109,50,level);
  printSL(109,22,score);
  snake = {{9,5},{30,30},2,RIGHT};
  level=0;
  score=0;
}while(u8g.nextPage());

}

goto rebegin;
}

//定义食物随机出现
void foodfresh(){
  if(food.yes==0){
  randNumber=random(98);
  food.x=randNumber;
  randNumber=random(62);
  food.y=randNumber;
  food.yes++;
  }
}


//下面来写构成蛇躯体的函数
void element(int x, int y){
  u8g.drawBitmapP(x, y, 1, 4, snake_body);
}

//游戏的主界面
void UI(){
  u8g.drawFrame(0,1,98,62);   //界面框架构图
  u8g.drawFrame(0,0,98,64);   //界面框架构图
  setFont_M;  //设置字体
  u8g.drawStr(100,12,"Score");  //分数
  u8g.drawStr(100,40,"Level");  //等级
  
}
//定义输出分数和等级的函数
void  printSL(int x,int y,int M){
  u8g.setPrintPos(x,y);
  u8g.print(M);
}
  
bool judge(){
  if(snake.node>=4)
  {
  for(int i=4;i<=snake.node;i++)
  {
    if((snake.x[0]==snake.x[i])&&(snake.y[0]==snake.y[i]))
    return false;
  }
  }
  return true;
}

//定义手柄控制的按键判断
void key() {
if(digitalRead(pinUP)!=1){
snake.dir = UP;
}
if(digitalRead(pinRIGHT)!=1){
snake.dir = RIGHT;
}
if(digitalRead(pinLEFT)!=1){
snake.dir = LEFT;
}
if(digitalRead(pinDOWN)!=1){
snake.dir = DOWN;
}
}

//定义蛇的移动
void mov(){
    switch(snake.dir) {
    case RIGHT: 
          snake.x[0] += 4;
          if(snake.x[0]>=101) {
            snake.x[0] = 1;
          } break;
    case UP: 
          snake.y[0] -= 4;
          if(snake.y[0]<=1) {
            snake.y[0] = 58;
          } break;
    case LEFT: 
          snake.x[0] -= 4;
          if(snake.x[0]<=0) {
            snake.x[0] = 97;
          } break;
    case DOWN:
          snake.y[0] += 4;
          if(snake.y[0]>=62) {
            snake.y[0] = 2;
          } break;
  } 

  for(int i=snake.node-1;i>0;i--) {
    snake.x[i] = snake.x[i-1];
    snake.y[i] = snake.y[i-1];
  }
}

//定义吃到食物
void eat(){
    if((snake.x[0] == food.x) && (snake.y[0] == food.y)) {
    snake.x[0] = food.x;
    snake.y[0] = food.y;

    snake.node++;
    food.yes --;
    score += 2;
    level = score/10+1;
  }
}

