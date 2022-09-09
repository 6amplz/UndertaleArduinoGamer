#include <Esplora.h>
#include <TFT.h>
#include <SPI.h>
char HP=10;//血量
unsigned char cx,cy;//屏幕中心点的坐标
unsigned char undamage=0;
unsigned long timing=0;//记录时长
unsigned char px,py;//记录心形上一帧的位置

void damage(){
  HP--;
  undamage=1;
  if(HP>0){//左上角刷新HP数值
    EsploraTFT.setTextSize(1);
    String str=(String)(int)(HP+1);
    EsploraTFT.stroke(0,0,0);
    EsploraTFT.text(str.c_str(),2,12);
    str=(String)(int)HP;
    EsploraTFT.stroke(255,255,255);
    EsploraTFT.text(str.c_str(),2,12);
  }
}

class BONE{//骨头类
public:
  BONE():state(1),x(30),h(30),t(0),v(0){};
  void Reset(bool dir,int tim,int hh,int vv){//重置参数
    state=1;
    t=tim;
    v=vv;
    h=hh;
    if(dir)x=-3;
    else {x=152;v*=-1;}
  }
  void drawBone(){//绘制骨头，超出边界部分不绘制
    if(1<x&&x<158)EsploraTFT.rect(x-1,128-h,3,h-2);
    else{
      for(int i=x-1;i<=x+1;i++)
        for(int j=128-h;j<=125;j++)
          if(0<i&&i<159)EsploraTFT.point(i,j);
    }
    
    if(0<x-2&&x-2<159){
      EsploraTFT.point(x-2,126);
      EsploraTFT.point(x-2,125);
      EsploraTFT.point(x-2,128-h);
      EsploraTFT.point(x-2,127-h);
    }
    if(0<x-1&&x-1<159){EsploraTFT.point(x-1,126);EsploraTFT.point(x-1,127-h);}
    if(0<x+1&&x+1<159){EsploraTFT.point(x+1,126);EsploraTFT.point(x+1,127-h);}
    if(0<x+2&&x+2<159){
      EsploraTFT.point(x+2,126);
      EsploraTFT.point(x+2,125);
      EsploraTFT.point(x+2,128-h);
      EsploraTFT.point(x+2,127-h);
    }
    
    int hh=45-h;
    if(1<x&&x<158)EsploraTFT.rect(x-1,66,3,hh-2);
    else{
      for(int i=x-1;i<=x+1;i++)
        for(int j=66;j<=63+hh;j++)
          if(0<i&&i<159)EsploraTFT.point(i,j);
    }
    
    if(0<x-2&&x-2<159){
      EsploraTFT.point(x-2,65);
      EsploraTFT.point(x-2,66);
      EsploraTFT.point(x-2,64+hh);
      EsploraTFT.point(x-2,63+hh);
    }
    if(0<x-1&&x-1<159){EsploraTFT.point(x-1,65);EsploraTFT.point(x-1,64+hh);}
    if(0<x+1&&x+1<159){EsploraTFT.point(x+1,65);EsploraTFT.point(x+1,64+hh);}
    if(0<x+2&&x+2<159){
      EsploraTFT.point(x+2,65);
      EsploraTFT.point(x+2,66);
      EsploraTFT.point(x+2,63+hh);
      EsploraTFT.point(x+2,64+hh);
    }
  }
  void Push(int gravity_py){//骨头位移函数
    t++;
    if(t>=0&&state){
      EsploraTFT.stroke(0,0,0);
      EsploraTFT.fill(0,0,0);
      drawBone();
      x+=v;//移动骨头并判断是否造成伤害
      if(abs(x-px)<5&&127-h-gravity_py<4&&undamage==0){
        if(abs(x-px)+127-h-gravity_py<6){
          state=0;
          damage();
        }
      }
      else if(abs(x-px)<5&&gravity_py-(64+45-h)<4&&undamage==0){
        state=0;
        damage();
      }
    }
    if(t>=0&&state){
      EsploraTFT.stroke(255,255,255);
      EsploraTFT.fill(255,255,255);
      drawBone();
    }
    //注意，此处将t设为public，对象重置将在Push()函数外进行
  }
  int t;//判断是否到时间出发
  bool state;//骨头状态，1为正在前进，0为已造成伤害，不再绘制
private:
  int x;//中心x坐标
  int h;//高度
  int v;//x方向速度
};
BONE bone[5];

class SPIDER{//蜘蛛类
public:
  SPIDER():state(1),x(30),y(30),t(0),v(0){};
  void Reset(int dir,int tim,int vv){//重置参数
    state=1;
    t=tim;
    v=vv;
    switch(dir){
      case 1:x=-7;y=71;break;
      case 2:x=-7;y=92;break;
      case 3:x=-7;y=113;break;
      case 4:x=166;y=71;v*=-1;break;
      case 5:x=166;y=92;v*=-1;break;
      case 6:x=166;y=113;v*=-1;break;
    }
  }
  void drawSpider(){//绘制蜘蛛，超出边界部分不绘制
    int i,j;
    if(3<x&&x<156)EsploraTFT.rect(x-3,y-2,7,5);
    else{
      for(i=x-3;i<=x+3;i++)
        for(j=y-2;j<=y+2;j++)
          if(0<i&&i<159)EsploraTFT.point(i,j);
    }

    for(i=x-7;i<=x-4;i++)if(0<i&&i<159)EsploraTFT.point(i,y-2);
    for(i=x-7;i<=x-4;i++)if(0<i&&i<159)EsploraTFT.point(i,y+2);
    for(i=x+4;i<=x+7;i++)if(0<i&&i<159)EsploraTFT.point(i,y-2);
    for(i=x+4;i<=x+7;i++)if(0<i&&i<159)EsploraTFT.point(i,y+2);

    for(i=x-4;i<=x-2;i++)if(0<i&&i<159){EsploraTFT.point(i,y-5);EsploraTFT.point(i,y+5);}
    for(i=x+2;i<=x+4;i++)if(0<i&&i<159){EsploraTFT.point(i,y-5);EsploraTFT.point(i,y+5);}

    if(0<x-7&&x-7<159){EsploraTFT.point(x-7,y-3);EsploraTFT.point(x-7,y+3);}
    if(0<x+7&&x+7<159){EsploraTFT.point(x+7,y-3);EsploraTFT.point(x+7,y+3);}

    if(0<x-2&&x-2<159){
      EsploraTFT.point(x-2,y-3);
      EsploraTFT.point(x-2,y+3);
      EsploraTFT.point(x-2,y-4);
      EsploraTFT.point(x-2,y+4);
    }
    if(0<x+2&&x+2<159){
      EsploraTFT.point(x+2,y-3);
      EsploraTFT.point(x+2,y+3);
      EsploraTFT.point(x+2,y-4);
      EsploraTFT.point(x+2,y+4);
    }
  }
  void Push(int spider_py){//蜘蛛位移函数
    t++;
    if(t>=0&&state){
      EsploraTFT.stroke(0,0,0);
      EsploraTFT.fill(0,0,0);
      drawSpider();
      EsploraTFT.stroke(255,0,255);
      for(int i=x-3;i<=x+3;i++)if(0<i&&i<159)EsploraTFT.point(i,y);
      x+=v;//移动蜘蛛并判断是否造成伤害
      if(abs(x-px)<11&&y==spider_py&&undamage==0){state=0;damage();}
    }
    if(t>=0&&state){EsploraTFT.fill(255,255,255);EsploraTFT.stroke(255,255,255);drawSpider();}
    //注意，此处将t设为public，对象重置将在Push()函数外进行
  }
  int t;//判断是否到时间出发
  bool state;//子弹状态，1为正在前进，0为已造成伤害，不再绘制
private:
  int x;//中心x坐标
  int y;//中心y坐标
  int v;//x方向速度
};
SPIDER spider[10];

class BULLET{//子弹类
public:
  BULLET():state(1),x(30),y(30),t(0),vx(0),vy(0){};
  void Reset(int tim,int dir,int v0){//重置参数
    state=1;
    t=tim;
    switch(dir){
      case 1:x=14;y=-2;break;
      case 2:x=14;y=129;break;
      case 3:x=145;y=129;break;
      case 4:x=145;y=-2;break;
    }
    float Reset_distx=px-x,Reset_disty=py-y;
    vx=(float)v0*Reset_distx/sqrt(Reset_distx*Reset_distx+Reset_disty*Reset_disty);
    vy=(float)v0*Reset_disty/sqrt(Reset_distx*Reset_distx+Reset_disty*Reset_disty);
  }
  void drawBullet(){//绘制圆形子弹，超出边界部分不绘制
    int drawBullet_tempx=(int)x,drawBullet_tempy=(int)y;
    for(int i=drawBullet_tempx-1;i<=drawBullet_tempx+1;i++)
      for(int j=drawBullet_tempy-2;j<=drawBullet_tempy+2;j++)
        if(20<i&&i<139&&4<j&&j<123)EsploraTFT.point(i,j);
    for(int j=drawBullet_tempy-1;j<=drawBullet_tempy+1;j++){
      if(20<(drawBullet_tempx-2)&&(drawBullet_tempx-2)<139&&4<j&&j<123)EsploraTFT.point(drawBullet_tempx-2,j);
      if(20<(drawBullet_tempx+2)&&(drawBullet_tempx+2)<139&&4<j&&j<123)EsploraTFT.point(drawBullet_tempx+2,j);
    }
  }
  void Push(){//子弹位移函数
    t++;
    if(t>=0&&state){
      EsploraTFT.stroke(0,0,0);
      drawBullet();
      x+=vx;y+=vy;//移动子弹并判断是否造成伤害
      if(abs((int)x-px)<6&&abs((int)y-py)<6&&undamage==0){
        if(abs((int)x-px)+abs((int)y-py)!=10){
          state=0;
          damage();
        }
      }
    }
    if(t>=0&&state){EsploraTFT.stroke(255,255,255);drawBullet();}
    //注意，此处将t设为public，对象重置将在Push()函数外进行
  }
  int t;//判断是否到时间出发
  bool state;//子弹状态，1为正在前进，0为已造成伤害，不再绘制
private:
  float x;//中心x坐标
  float y;//中心y坐标
  float vx;//x方向速度
  float vy;//y方向速度
};
BULLET bullet[10];

class ARROW{//箭头类
public:
  ARROW():state(1),dire(1),x(30),y(30),t(0),v(0){};
  void Reset(int dir,int tim,int vi){
    state=1;
    dire=dir;
    t=tim;
    v=vi;
    switch(dire){
      case 1:x=cx;y=cy-60;break;
      case 2:x=cx-60;y=cy;break;
      case 3:x=cx;y=cy+60;break;
      case 4:x=cx+60;y=cy;break;
    }
  }
  void drawArrow(){//绘制箭头
    EsploraTFT.point(x,y);
    switch(dire){
        case 1:EsploraTFT.point(x-1,y-1);EsploraTFT.point(x,y-1);EsploraTFT.point(x+1,y-1);EsploraTFT.point(x-1,y-2);EsploraTFT.point(x+1,y-2);break;
        case 2:EsploraTFT.point(x-1,y-1);EsploraTFT.point(x-1,y);EsploraTFT.point(x-1,y+1);EsploraTFT.point(x-2,y-1);EsploraTFT.point(x-2,y+1);break;
        case 3:EsploraTFT.point(x-1,y+1);EsploraTFT.point(x,y+1);EsploraTFT.point(x+1,y+1);EsploraTFT.point(x-1,y+2);EsploraTFT.point(x+1,y+2);break;
        case 4:EsploraTFT.point(x+1,y-1);EsploraTFT.point(x+1,y);EsploraTFT.point(x+1,y+1);EsploraTFT.point(x+2,y-1);EsploraTFT.point(x+2,y+1);break;
      }
  }
  void Push(int shield){//箭头位移函数
    t++;
    if(t>=0&&state){
      EsploraTFT.stroke(0,0,0);
      drawArrow();
      switch(dire){//移动箭头，并判断是否被格挡或造成伤害
        case 1:y+=v;if((cy-7)<=y&&y<=(cy-5)&&shield==1)state=0;
        if(y>=(cy-4)){state=0;if(undamage==0)damage();}break;
        case 2:x+=v;if((cx-7)<=x&&x<=(cx-5)&&shield==2)state=0;
        if(x>=(cx-4)){state=0;if(undamage==0)damage();}break;
        case 3:y-=v;if((cy+5)<=y&&y<=(cy+7)&&shield==3)state=0;
        if(y<=(cy+4)){state=0;if(undamage==0)damage();}break;
        case 4:x-=v;if((cx+5)<=x&&x<=(cx+7)&&shield==4)state=0;
        if(x<=(cx+4)){state=0;if(undamage==0)damage();}break;
      }
    }
    if(t>=0&&state){EsploraTFT.stroke(255,255,255);drawArrow();}
    //注意，此处将t设为public，对象重置将在Push()函数外进行
  }
  int t;//判断是否到时间出发
private:
  bool state;//箭头状态，1为正在前进，0为已被格挡或已造成伤害，不再绘制
  int dire;//方向，1为从上方来，2为从左方来，3为从下方来，4为从右方来
  int x;//尖端x坐标
  int y;//尖端y坐标
  int v;//前进速度
};
ARROW arrow[10];





void setup() {
  // put your setup code here, to run once:
  EsploraTFT.begin();
  EsploraTFT.background(0,0,0);
  cx=EsploraTFT.width()/2;
  cy=EsploraTFT.height()/2;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int y0=48;
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.setTextSize(3);
  EsploraTFT.text("UNDERTALE",1,8);
  
  EsploraTFT.fill(255,255,255);
  EsploraTFT.rect(76,11,9,6);
  EsploraTFT.stroke(0,0,255);
  drawHeart(80,14);
  EsploraTFT.noFill();
  
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.setTextSize(1);
  EsploraTFT.text("Bonetrousle",48,y0);
  EsploraTFT.text("Spider Dance",45,y0+12);
  EsploraTFT.text("Enemy Approaching",30,y0+24);
  EsploraTFT.text("Spear of Justice",33,y0+36);
  EsploraTFT.text("* Press UP/DOWN to select",4,105);
  EsploraTFT.text("* Press RIGHT to continue",4,117);

  int right=Esplora.readButton(SWITCH_RIGHT),up,down;
  int select_flag=0,select=1,select_pre=1;
  EsploraTFT.stroke(0,0,255);
  drawSelect(select,y0);
  while(right==HIGH){
    right=Esplora.readButton(SWITCH_RIGHT);
    up=Esplora.readButton(SWITCH_UP);
    down=Esplora.readButton(SWITCH_DOWN);
    if(up==LOW&&select_flag==0){
      if(select==1)select=4;
      else select--;
      select_flag=1;
    }
    if(down==LOW&&select_flag==0){
      if(select==4)select=1;
      else select++;
      select_flag=1;
    }
    if(up==HIGH&&down==HIGH)select_flag=0;

    if(select!=select_pre){
      EsploraTFT.stroke(0,0,0);
      drawSelect(select_pre,y0);
    }
    EsploraTFT.stroke(0,0,255);
    drawSelect(select,y0);
    select_pre=select;
  }

  while(Esplora.readButton(SWITCH_RIGHT)==LOW);
  EsploraTFT.background(0,0,0);
  switch(select){
    case 1:Bonetrousle();break;
    case 2:Spider_Dance();break;
    case 3:Enemy_Approaching();break;
    case 4:Spear_of_Justice();break;
  }
}





void drawSelect(int select,int y0){
  switch(select){
    case 1:drawHeart(40,y0+3);break;
    case 2:drawHeart(37,y0+15);break;
    case 3:drawHeart(22,y0+27);break;
    case 4:drawHeart(25,y0+39);break;
  }
}

int selectDif(){//难度选择界面
  int difficulty=1;
  EsploraTFT.setTextSize(1);
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.text("Select the difficulty:",4,4);
  EsploraTFT.text("Press UP +1, DOWN -1",4,16);
  EsploraTFT.text("Press RIGHT to start",4,28);
  HP=10;undamage=0;difficulty=1;//各变量重新赋初值

  EsploraTFT.text("* You can use the joystick",2,80);
  EsploraTFT.text("or the buttons to play.",14,92);
  EsploraTFT.text("* To pause the game, move",2,104);
  EsploraTFT.text("the slider to the right.",14,116);

  int range=6;
  bool select_flag=0;
  int select_pre=difficulty;
  int rx;
  int right=Esplora.readButton(SWITCH_RIGHT),up,down;
  while(right==HIGH){
    right=Esplora.readButton(SWITCH_RIGHT);
    up=Esplora.readButton(SWITCH_UP);
    down=Esplora.readButton(SWITCH_DOWN);
    if(down==LOW&&select_flag==0){
      if(difficulty==1)difficulty=range;
      else difficulty--;
      select_flag=1;
    }
    if(up==LOW&&select_flag==0){
      if(difficulty==range)difficulty=1;
      else difficulty++;
      select_flag=1;
    }
    if(up==HIGH&&down==HIGH)select_flag=0;

    if(difficulty!=select_pre){
      EsploraTFT.stroke(0,0,0);
      String str=(String)select_pre;
      EsploraTFT.text(str.c_str(),136,4);
    }
    EsploraTFT.stroke(255,255,255);
    String str=(String)difficulty;
    EsploraTFT.text(str.c_str(),136,4);
    select_pre=difficulty;
  }
  while(Esplora.readButton(SWITCH_RIGHT)==LOW);
  return difficulty;
}

void gravityMove(int x1,int x2,int y1,int y2,float& gravity_py,float& gravity_vy){//重力模式移动
  int rx=Esplora.readJoystickX(),ry=Esplora.readJoystickY();
  int up=Esplora.readButton(SWITCH_UP);
  int left=Esplora.readButton(SWITCH_LEFT);
  int right=Esplora.readButton(SWITCH_RIGHT);
  int vx;
  if(-rx>300||right==LOW)vx=3;
  else if(-rx<-300||left==LOW)vx=-3;
  else vx=0;

  if(gravity_py==(float)y2){
    if(-ry>300||up==LOW)gravity_vy=-5;else gravity_vy=0;
  }
  else {
    if(-ry<256&&up==HIGH&&gravity_vy<0)gravity_vy=-0.5;
    gravity_vy+=0.5;//加重力
  }
  
  int x=constrain(px+vx,x1,x2);
  float y=constrain(gravity_py+gravity_vy,y1,y2);
  if(px!=x||(int)gravity_py!=(int)y){
    EsploraTFT.stroke(0,0,0);
    drawHeart(px,(int)gravity_py);
  }
  if(undamage){//受伤无敌时间,3秒
    undamage++;
    if(undamage%6>=3)EsploraTFT.stroke(0,0,0);
    else EsploraTFT.stroke(255,0,0);
    if(undamage>60)undamage=0;
  }
  else EsploraTFT.stroke(255,0,0);
  drawHeart(x,(int)y);
  px=x;
  gravity_py=y;
}

void spiderMove(int x1,int x2,int& move_count,int& spider_py){//spiderDance模式移动
  int rx=Esplora.readJoystickX();int ry=Esplora.readJoystickY();
  int up=Esplora.readButton(SWITCH_UP);
  int down=Esplora.readButton(SWITCH_DOWN);
  int left=Esplora.readButton(SWITCH_LEFT);
  int right=Esplora.readButton(SWITCH_RIGHT);
  int vx;
  int x,y;
  y=spider_py;
  if(-rx>300||right==LOW)vx=4;
  else if(-rx<-300||left==LOW)vx=-4;
  else vx=0;
  if(-ry<-300||down==LOW)move_count++;
  else if(-ry>300||up==LOW)move_count--;
  else move_count=0;
  if(spider_py==71&&move_count%5==1)y=92;
  else if(spider_py==92){
    if(move_count%5==1)y=113;
    else if(move_count%5==-1)y=71;
  }
  else if(spider_py==113&&move_count%5==-1)y=92;
  
  x=constrain(px+vx,x1,x2);
  if(px!=x||spider_py!=y){
    EsploraTFT.stroke(0,0,0);
    spiderDance_drawHeart(px,spider_py);
  }
  if(undamage){//受伤无敌时间,3秒
    undamage++;
    if(undamage%6>=3)EsploraTFT.stroke(0,0,0);
    else EsploraTFT.stroke(255,0,255);
    if(undamage>60)undamage=0;
  }
  else EsploraTFT.stroke(255,0,255);
  spiderDance_drawHeart(x,y);
  px=x;
  spider_py=y;
}

void switchMove(int x1,int x2,int y1,int y2){//普通移动
  int rx=Esplora.readJoystickX();int ry=Esplora.readJoystickY();
  int up=Esplora.readButton(SWITCH_UP);
  int down=Esplora.readButton(SWITCH_DOWN);
  int left=Esplora.readButton(SWITCH_LEFT);
  int right=Esplora.readButton(SWITCH_RIGHT);
  int vx,vy;
  if(-rx>300||right==LOW)vx=1;
  else if(-rx<-300||left==LOW)vx=-1;
  else vx=0;
  if(-ry<-300||down==LOW)vy=1;
  else if(-ry>300||up==LOW)vy=-1;
  else vy=0;
  if(vx==0||vy==0){vx*=3;vy*=3;}else{vx*=2;vy*=2;}
  
  int x=constrain(px+vx,x1,x2);
  int y=constrain(py+vy,y1,y2);
  if(px!=x||py!=y){
    EsploraTFT.stroke(0,0,0);
    drawHeart(px,py);
  }
  if(undamage){//受伤无敌时间,3秒
    undamage++;
    if(undamage%6>=3)EsploraTFT.stroke(0,0,0);
    else EsploraTFT.stroke(0,0,255);
    if(undamage>60)undamage=0;
  }
  else EsploraTFT.stroke(0,0,255);
  drawHeart(x,y);
  px=x;
  py=y;
}

void drawShield(int d){//绘制护盾
  switch(d){
    case 1:for(int i=-4;i<=4;i++)EsploraTFT.point(cx+i,cy-7);
    EsploraTFT.point(cx-5,cy-6);EsploraTFT.point(cx-5,cy-5);EsploraTFT.point(cx-4,cy-6);EsploraTFT.point(cx-3,cy-6);
    EsploraTFT.point(cx+5,cy-6);EsploraTFT.point(cx+5,cy-5);EsploraTFT.point(cx+4,cy-6);EsploraTFT.point(cx+3,cy-6);
    break;
    case 2:for(int i=-4;i<=4;i++)EsploraTFT.point(cx-7,cy+i);
    EsploraTFT.point(cx-6,cy-5);EsploraTFT.point(cx-5,cy-5);EsploraTFT.point(cx-6,cy-4);EsploraTFT.point(cx-6,cy-3);
    EsploraTFT.point(cx-6,cy+5);EsploraTFT.point(cx-5,cy+5);EsploraTFT.point(cx-6,cy+4);EsploraTFT.point(cx-6,cy+3);
    break;
    case 3:for(int i=-4;i<=4;i++)EsploraTFT.point(cx+i,cy+7);
    EsploraTFT.point(cx-5,cy+6);EsploraTFT.point(cx-5,cy+5);EsploraTFT.point(cx-4,cy+6);EsploraTFT.point(cx-3,cy+6);
    EsploraTFT.point(cx+5,cy+6);EsploraTFT.point(cx+5,cy+5);EsploraTFT.point(cx+4,cy+6);EsploraTFT.point(cx+3,cy+6);
    break;
    case 4:for(int i=-4;i<=4;i++)EsploraTFT.point(cx+7,cy+i);
    EsploraTFT.point(cx+6,cy-5);EsploraTFT.point(cx+5,cy-5);EsploraTFT.point(cx+6,cy-4);EsploraTFT.point(cx+6,cy-3);
    EsploraTFT.point(cx+6,cy+5);EsploraTFT.point(cx+5,cy+5);EsploraTFT.point(cx+6,cy+4);EsploraTFT.point(cx+6,cy+3);
    break;
  }
}

void drawHeart(int x,int y){//绘制心形
  for(int i=x-3;i<=x+3;i++)for(int j=y-2;j<=y;j++)EsploraTFT.point(i,j);
  for(int i=x-2;i<=x+2;i++)EsploraTFT.point(i,y+1);
  EsploraTFT.point(x-2,y-3);EsploraTFT.point(x-1,y-3);EsploraTFT.point(x+1,y-3);EsploraTFT.point(x+2,y-3);
  EsploraTFT.point(x-1,y+2);EsploraTFT.point(x,y+2);EsploraTFT.point(x+1,y+2);EsploraTFT.point(x,y+3);
}

void spiderDance_drawHeart(int x,int y){//绘制心形，spiderDance模式限定
  for(int i=x-3;i<=x+3;i++)for(int j=y-2;j<y;j++)EsploraTFT.point(i,j);
  for(int i=x-2;i<=x+2;i++)EsploraTFT.point(i,y+1);
  EsploraTFT.point(x-2,y-3);EsploraTFT.point(x-1,y-3);EsploraTFT.point(x+1,y-3);EsploraTFT.point(x+2,y-3);
  EsploraTFT.point(x-1,y+2);EsploraTFT.point(x,y+2);EsploraTFT.point(x+1,y+2);EsploraTFT.point(x,y+3);
}

void pause(){
  if(Esplora.readSlider()<50){//暂停功能，通过电位器滑块实现
    unsigned long pausy=millis();
    Esplora.writeRGB(64,0,0);
    while(Esplora.readSlider()<50);
    for(int i=0;i<3;i++){
      Esplora.writeRGB(0,0,0);
      delay(500);
      Esplora.writeRGB(0,64,0);
      delay(500);
    }
    Esplora.writeRGB(0,0,0);
    pausy=millis()-pausy;
    timing+=pausy;
  }
}

void Bonetrousle(){
  int difficulty=selectDif();
  int v0=1;//弹幕速度设定
  int T=10;//弹幕出发时间间隔设定，单位：帧
  float gravity_py;//记录心形上一帧的坐标，浮点型（重力模式限定）
  float gravity_vy=0;//心形y方向速度（重力模式限定）

  v0=5;
  T=22-difficulty;
  px=cx;gravity_py=123;
  for(int i=0;i<5;i++)bone[i].Reset(random(2),-i*T,random(16)+8,v0);//初始化BONE对象

  EsploraTFT.background(0,0,0);
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.rect(0,64,160,64);
  EsploraTFT.text("HP",2,2);
  String str=(String)(int)HP;
  EsploraTFT.text(str.c_str(),2,12);
  timing=millis();

  while(HP>0){
    unsigned long delay_count=millis();
    
    gravityMove(4,155,68,123,gravity_py,gravity_vy);
    for(int i=0;i<5;i++){
      bone[i].Push((int)gravity_py);
      if(bone[i].t>=5*T)bone[i].Reset(random(2),0,random(16)+8,v0);
      if(HP<=0)break;
    }
    if(undamage<3){
      EsploraTFT.stroke(255,0,0);
      drawHeart(px,(int)gravity_py);
    }
    
    delay_count=millis()-delay_count;
    if(delay_count<50)delay(50-delay_count);
    pause();
  }

  gameOver(px,(int)gravity_py);
}

void Spear_of_Justice(){
  int difficulty=selectDif();
  int v0=1;//弹幕速度设定
  int T=10;//弹幕出发时间间隔设定，单位：帧
  int shield=1,pshield=1;//护盾方向判定
  
  v0=3;
  T=16-2*difficulty;
  for(int i=0;i<10;i++)arrow[i].Reset(random(4)+1,-i*T,v0);//初始化箭头对象

  EsploraTFT.background(0,0,0);
  EsploraTFT.stroke(0,255,0);
  drawHeart(cx,cy);
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.text("HP",2,2);
  String str=(String)(int)HP;
  EsploraTFT.text(str.c_str(),2,12);
  timing=millis();
  
  while(HP>0){
    unsigned long delay_count=millis();
    
    int rx=Esplora.readJoystickX();int ry=Esplora.readJoystickY();
    if(-256<-rx&&-rx<250&&-ry>300||Esplora.readButton(SWITCH_UP)==LOW)shield=1;//控制护盾方向
    else if(-256<-ry&&-ry<256&&-rx<-300||Esplora.readButton(SWITCH_LEFT)==LOW)shield=2;
    else if(-256<-rx&&-rx<256&&-ry<-300||Esplora.readButton(SWITCH_DOWN)==LOW)shield=3;
    else if(-256<-ry&&-ry<256&&-rx>300||Esplora.readButton(SWITCH_RIGHT)==LOW)shield=4;
    if(shield!=pshield){//清除旧的护盾
      EsploraTFT.stroke(0,0,0);
      drawShield(pshield);
    }
    for(int i=0;i<10;i++){
      arrow[i].Push(shield);
      if(arrow[i].t>=10*T)arrow[i].Reset(random(4)+1,0,v0);
      if(HP<=0)break;
    }//各箭头前进一帧
    EsploraTFT.stroke(255,0,0);
    drawShield(shield);//绘制新的护盾
    pshield=shield;
    if(undamage){//受伤无敌时间,3秒
      undamage++;
      if(undamage%6>=3){EsploraTFT.stroke(0,0,0);drawHeart(cx,cy);}
      else {EsploraTFT.stroke(0,255,0);drawHeart(cx,cy);}
      if(undamage>60)undamage=0;
    }
      
    delay_count=millis()-delay_count;
    if(delay_count<50)delay(50-delay_count);
    pause();
  }

  gameOver(cx,cy);
}

void Enemy_Approaching(){
  int difficulty=selectDif();
  int v0=1;//弹幕速度设定
  int T=10;//弹幕出发时间间隔设定，单位：帧
  
  v0=6;
  T=11-difficulty;
  px=cx;
  py=cy;
  for(int i=0;i<10;i++)bullet[i].Reset(-i*T,random(4)+1,v0);

  EsploraTFT.background(0,0,0);
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.rect(20,4,120,120);
  EsploraTFT.text("HP",2,2);
  String str=(String)(int)HP;
  EsploraTFT.text(str.c_str(),2,12);
  timing=millis();

  while(HP>0){
    unsigned long delay_count=millis();
    
    switchMove(24,135,8,119);
    for(int i=0;i<10;i++){
      bullet[i].Push();
      if(bullet[i].t>=10*T)bullet[i].Reset(0,random(4)+1,v0);
      if(HP<=0)break;
    }
    if(undamage<3){
      EsploraTFT.stroke(0,0,255);
      drawHeart(px,py);
    }
    
    delay_count=millis()-delay_count;
    if(delay_count<50)delay(50-delay_count);
    pause();
  }

  gameOver(px,py);
}

void Spider_Dance(){
  int difficulty=selectDif();
  int v0=1;//弹幕速度设定
  int T=10;//弹幕出发时间间隔设定，单位：帧
  int spider_py=92;//记录心形上一帧的y坐标，spiderDance模式限定
  int move_count=0;
  
  v0=difficulty+5;
  T=10;
  px=cx;
  for(int i=0;i<10;i++)spider[i].Reset(random(6)+1,-i*T,v0);//初始化SPIDER对象

  EsploraTFT.background(0,0,0);
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.rect(0,57,160,71);
  EsploraTFT.stroke(255,0,255);
  for(int i=1;i<159;i++)EsploraTFT.point(i,71);
  for(int i=1;i<159;i++)EsploraTFT.point(i,92);
  for(int i=1;i<159;i++)EsploraTFT.point(i,113);
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.text("HP",2,2);
  String str=(String)(int)HP;
  EsploraTFT.text(str.c_str(),2,12);
  timing=millis();

  while(HP>0){
    unsigned long delay_count=millis();
    
    spiderMove(4,155,move_count,spider_py);
    for(int i=0;i<10;i++){
      spider[i].Push(spider_py);
      if(spider[i].t>=10*T)spider[i].Reset(random(6)+1,0,v0);
      if(HP<=0)break;
    }
    if(undamage<3){
      EsploraTFT.stroke(255,0,255);
      spiderDance_drawHeart(px,spider_py);
    }
    
    delay_count=millis()-delay_count;
    if(delay_count<50)delay(50-delay_count);
    pause();
  }

  gameOver(px,spider_py);
}

void gameOver(int x,int y){//游戏结束界面
  double cost_time=((double)millis()-(double)timing)/(double)1000;
  EsploraTFT.background(0,0,0);
  while(Esplora.readButton(SWITCH_RIGHT)==LOW);
  
  //第一帧
  frame1(0,0,255,x,y);
  delay(1000);
  frame1(0,0,0,x,y);
  
  //第二帧
  frame2(0,0,255,x,y);
  delay(100);
  frame2(0,0,0,x,y);

  //第三帧
  frame3(0,0,255,x,y);
  delay(100);
  frame3(0,0,0,x,y);

  //第四帧
  frame4(0,0,255,x,y);
  delay(100);
  frame4(0,0,0,x,y);

  //第五帧
  frame5(0,0,255,x,y);
  delay(100);
  frame5(0,0,0,x,y);

  delay(1000);
  
  EsploraTFT.stroke(255,255,255);
  EsploraTFT.setTextSize(3);
  EsploraTFT.text("GAME",45,6);EsploraTFT.text("OVER",45,32);
  EsploraTFT.setTextSize(1);
  String str="You lasted "+(String)cost_time+" s";
  EsploraTFT.text(str.c_str(),4,60);
  EsploraTFT.text("Press RIGHT to continue...",4,72);
  EsploraTFT.text("* Stay determined...",20,99);
  /*
  EsploraTFT.setTextSize(3);
  for(int i=1;i<=8;i++){
    EsploraTFT.stroke(32*i-1,32*i-1,32*i-1);
    EsploraTFT.text("GAME",45,6);EsploraTFT.text("OVER",45,32);delay(100);
  }
  delay(900);
  
  EsploraTFT.setTextSize(1);
  String str="You lasted "+(String)timing+" s";
  EsploraTFT.stroke(64,64,64);EsploraTFT.text(str.c_str(),4,60);EsploraTFT.text("Press RIGHT to continue...",4,70);
  EsploraTFT.stroke(255,255,255);EsploraTFT.text("* ",22,99);delay(50);
  EsploraTFT.stroke(128,128,128);EsploraTFT.text(str.c_str(),4,60);EsploraTFT.text("Press RIGHT to continue...",4,70);
  EsploraTFT.stroke(255,255,255);EsploraTFT.text("S",32,99);delay(50);
  EsploraTFT.stroke(192,192,192);EsploraTFT.text(str.c_str(),4,60);EsploraTFT.text("Press RIGHT to continue...",4,70);
  EsploraTFT.stroke(255,255,255);EsploraTFT.text("t",38,99);delay(50);
  EsploraTFT.stroke(255,255,255);EsploraTFT.text(str.c_str(),4,60);EsploraTFT.text("Press RIGHT to continue...",4,70);
  EsploraTFT.text("a",44,99);delay(50);EsploraTFT.text("y",50,99);delay(50);EsploraTFT.text(" ",56,99);delay(50);
  EsploraTFT.text("d",62,99);delay(50);EsploraTFT.text("e",68,99);delay(50);EsploraTFT.text("t",74,99);delay(50);
  EsploraTFT.text("e",80,99);delay(50);EsploraTFT.text("r",86,99);delay(50);EsploraTFT.text("m",92,99);delay(50);
  EsploraTFT.text("i",98,99);delay(50);EsploraTFT.text("n",104,99);delay(50);EsploraTFT.text("e",110,99);delay(50);
  EsploraTFT.text("d",116,99);delay(50);EsploraTFT.text(".",122,99);delay(50);EsploraTFT.text(".",128,99);delay(50);
  EsploraTFT.text(".",134,99);*/
  
  while(Esplora.readButton(SWITCH_RIGHT)==HIGH);
  while(Esplora.readButton(SWITCH_RIGHT)==LOW);
  EsploraTFT.background(0,0,0);
  EsploraTFT.text("Continue...",2,2);
  delay(1500);
  EsploraTFT.background(0,0,0);
}

void frame1(int b,int g,int r,int x,int y){//游戏结束界面第一帧
  EsploraTFT.stroke(b,g,r);
  EsploraTFT.point(x-1,y-1);EsploraTFT.point(x-1,y+1);EsploraTFT.point(x-1,y+3);
  EsploraTFT.point(x+1,y-2);EsploraTFT.point(x+1,y);EsploraTFT.point(x+1,y+2);
  for(int j=y-2;j<=y;j++)EsploraTFT.point(x-4,j);
  for(int j=y-3;j<=y+1;j++)EsploraTFT.point(x-3,j);
  for(int j=y-3;j<=y+2;j++)EsploraTFT.point(x-2,j);
  for(int j=y-2;j<=y;j++)EsploraTFT.point(x+4,j);
  for(int j=y-3;j<=y+1;j++)EsploraTFT.point(x+3,j);
  for(int j=y-3;j<=y+2;j++)EsploraTFT.point(x+2,j);
}

void frame2(int b,int g,int r,int x,int y){//游戏结束界面第二帧
  EsploraTFT.stroke(b,g,r);
  EsploraTFT.point(x-7,y-3);EsploraTFT.point(x-7,y-2);EsploraTFT.point(x-6,y-4);EsploraTFT.point(x-6,y-3);
  EsploraTFT.point(x-5,y-4);EsploraTFT.point(x-5,y-3);EsploraTFT.point(x-2,y-5);EsploraTFT.point(x-1,y-5);
  EsploraTFT.point(x-1,y-6);EsploraTFT.point(x,y-6);EsploraTFT.point(x+1,y-6);EsploraTFT.point(x+6,y+4);
  EsploraTFT.point(x+6,y+5);EsploraTFT.point(x+7,y+4);EsploraTFT.point(x-6,y+2);EsploraTFT.point(x-5,y+2);
  EsploraTFT.point(x-4,y+2);EsploraTFT.point(x-5,y+3);EsploraTFT.point(x-4,y+3);EsploraTFT.point(x-4,y+4);
  EsploraTFT.point(x+1,y+6);EsploraTFT.point(x+2,y+5);EsploraTFT.point(x+2,y+4);EsploraTFT.point(x+5,y+2);
  EsploraTFT.point(x+6,y);EsploraTFT.point(x+6,y+1);EsploraTFT.point(x+6,y+2);EsploraTFT.point(x+7,y);
  EsploraTFT.point(x+7,y+1);EsploraTFT.point(x,y-5);EsploraTFT.point(x+5,y-5);
}

void frame3(int b,int g,int r,int x,int y){//游戏结束界面第三帧
  EsploraTFT.stroke(b,g,r);
  EsploraTFT.point(x-11,y-3);EsploraTFT.point(x-10,y-3);EsploraTFT.point(x-10,y-4);EsploraTFT.point(x-9,y-4);
  EsploraTFT.point(x-1,y-9);EsploraTFT.point(x,y-9);EsploraTFT.point(x+1,y-10);EsploraTFT.point(x+8,y-6);
  EsploraTFT.point(x+8,y-7);EsploraTFT.point(x+9,y-6);EsploraTFT.point(x-9,y+6);EsploraTFT.point(x-9,y+7);
  EsploraTFT.point(x-8,y+5);EsploraTFT.point(x-8,y+6);EsploraTFT.point(x-8,y+7);EsploraTFT.point(x-7,y+7);
  EsploraTFT.point(x+3,y+6);EsploraTFT.point(x+3,y+7);EsploraTFT.point(x+7,y+4);EsploraTFT.point(x+7,y+5);
  EsploraTFT.point(x+8,y+3);EsploraTFT.point(x+8,y+4);EsploraTFT.point(x+8,y+5);EsploraTFT.point(x,y-10);
}

void frame4(int b,int g,int r,int x,int y){//游戏结束界面第四帧
  EsploraTFT.stroke(b,g,r);
  EsploraTFT.point(x-13,y-2);EsploraTFT.point(x-13,y-3);EsploraTFT.point(x+2,y-11);EsploraTFT.point(x+2,y-12);
  EsploraTFT.point(x+11,y-7);EsploraTFT.point(x-11,y+9);EsploraTFT.point(x-11,y+10);EsploraTFT.point(x-10,y+11);
  EsploraTFT.point(x+4,y+10);EsploraTFT.point(x+9,y+8);EsploraTFT.point(x+9,y+9);EsploraTFT.point(x+10,y+8);
  EsploraTFT.point(x+5,y-5);EsploraTFT.point(x-10,y+10);
}

void frame5(int b,int g,int r,int x,int y){//游戏结束界面第五帧
  EsploraTFT.stroke(b,g,r);
  EsploraTFT.point(x-12,y+14);EsploraTFT.point(x-12,y+15);EsploraTFT.point(x+10,y+13);
}
