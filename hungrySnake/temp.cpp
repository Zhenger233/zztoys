//#include <bits/stdc++.h>
#include <iostream>
#include <ctime>
#include <deque>
#include <string>
#include <map>
#include <vector>
#include <conio.h>
#include <windows.h>
using namespace std;

const int length =8,width=16,foodKind=6;
struct point
{
	int x,y;
	point(int a,int b):x(a),y(b){};
	bool operator < (const point p)const{return this->x<p.x||(this->x==p.x&&this->y<p.y);}
};

/*
front:head
back:tail
*/
deque<point>snake;
/*
kind,coordinate
2:grow
3:gg
4:head-tail reverse
5:move faster
6:move slower
*/
map<int,vector<point> >food;
/*
-1:bbooddyy
0:nothing
1:body
2:food
*/
int mp[length][width];
map<point,int>mmp;
vector<point>spacemp;
/*
1:up
2:left
3:down
4:right
*/
int direction;
//flag=0,work
int selfCrackFlag=1,wallCrackFlag=1;
/*
0:1-2
1:x-2
2:1-y
3:x-y
*/
int foodFlag;
/*
0:quit
1:self
2:wall
3:poison
4:restart
*/
int errorFlag=4;
int sleepTime=500;
vector<string>errorInfo;
long long gameTime=0;
//length of snake
long long l=0;

void gameInit();
void gameQuit();
int keyscan();
void cook();
void work(int);
void show();
void grow();
void move();
void eat();
void hide();
point nextpoint(point);
bool outOfMap(point);

int main()
{
    int tempstate=0;
    gameInit();
    while(1){
		//if(errorFlag==0)break;
		Sleep(sleepTime);
		tempstate=keyscan();
		work(tempstate);
	}
    //system("pause");
	cout<<"Good luck!"<<endl;
    return 0;
}

void gameInit()
{
    char c,i,j;
	int tempx,tempy;
    srand(time(0));
	hide();
	system("mode con cols=16 lines=12");
	errorInfo.clear();
	errorInfo.push_back("quit.\n");
	errorInfo.push_back("Self Crack.\n");
	errorInfo.push_back("Wall Crack.\n");
	errorInfo.push_back("poison.\n");
	errorInfo.push_back("restart.\n");
	errorInfo.push_back("Too long to live!.\n");
	memset(mp,0,sizeof(mp));
	mmp.clear();
	snake.clear();
	food.clear();
	spacemp.clear();
	for(i=0;i<length;i++)for(j=0;j<width;j++){
		spacemp.push_back({i,j});
	}
	l=0;
	direction=0;
	gameTime=0;
	selfCrackFlag=1;
	wallCrackFlag=1;
	sleepTime=500;
	tempx=length/2,tempy=width/2;
	mp[tempx][tempy]=1;
    snake.push_back({tempx,tempy});
    cout<<"Push\nW/A/S/D\nto\nstart\n....\n";
    while(1){
        c=_getch();
        if(c=='w'){direction=1;break;}
        if(c=='a'){direction=2;break;}
        if(c=='s'){direction=3;break;}
        if(c=='d'){direction=4;break;}
    }
	cook();
    show();
}

void show()
{
	int i,j;
	system("cls");
    memset(mp,0,sizeof(mp));
	mmp.clear();
    for(auto &p:snake){
        if(mp[p.x][p.y]==0){mp[p.x][p.y]=1;mmp[p]=1;}
		else if(mp[p.x][p.y]==1){mp[p.x][p.y]=-1;mmp[p]=-1;}
    }
	auto &p=snake.front();mp[p.x][p.y]=-2;mmp[p]=-2;
	for(int i=2;i<=foodKind;i++)if(!food[i].empty())for(auto j:food[i]){
		mmp[j]=i;
		mp[j.x][j.y]=i;
	}
	for(i=0;i<length;i++){
		for(j=0;j<width;j++){
			switch(mp[i][j]){
				case -1:printf("\033[31m*\033[0m");break;
				case -2:printf("\033[36m*\033[0m");break;
				case 0:_putch(' ');break;
				case 1:printf("\033[32m*\033[0m");break;
				case 2:printf("\033[34m#\033[0m");break;
			}
		}
		_putch('\n');
	}
	printf("Timetick:%lld\nScore:%lld\nTimescale:%d",gameTime,l,sleepTime);
}

int keyscan()
{
	int n=1;
	if(GetAsyncKeyState('W')||GetAsyncKeyState(VK_UP))n=2;
	if(GetAsyncKeyState('A')||GetAsyncKeyState(VK_LEFT))n=3;
	if(GetAsyncKeyState('S')||GetAsyncKeyState(VK_DOWN))n=4;
	if(GetAsyncKeyState('D')||GetAsyncKeyState(VK_RIGHT))n=5;
	if(GetAsyncKeyState('E'))n=6;
	if(GetAsyncKeyState('Q'))n=7;
	if(GetAsyncKeyState('1'))n=8;
	if(GetAsyncKeyState('2'))n=9;
	return n;
}

void cook()
{
	int i,j,tempx,tempy,tempn,tempnn;
	spacemp.clear();
	for(i=0;i<length;i++){
		for(j=0;j<width;j++){
			if(mp[i][j]==0)spacemp.push_back({i,j});
		}
	}
	tempn=spacemp.size();
	tempnn=rand()%tempn;
	tempx=spacemp[tempnn].x;
	tempy=spacemp[tempnn].y;
	mp[tempx][tempy]=2;
	food[2].push_back({tempx,tempy});
}

void work(int s)
{
	switch(s){
		case 1:gameTime++;move();show();break;
		case 2:if(direction!=3)direction=1;show();break;
		case 3:if(direction!=4)direction=2;show();break;
		case 4:if(direction!=3)direction=3;show();break;
		case 5:if(direction!=2)direction=4;show();break;
		case 6:grow();show();break;
		case 7:errorFlag=0;gameQuit();break;
		case 8:wallCrackFlag=!wallCrackFlag;break;
		case 9:selfCrackFlag=!selfCrackFlag;break;
	}
}

void gameQuit()
{
    system("cls");
    cout<<"score:"<<l<<endl;
	cout<<errorInfo[errorFlag];
	errorFlag=0;
    system("pause");
	gameInit();
}

void move()
{
    point tempp=snake.front();
	if(!wallCrackFlag){
		if(outOfMap(tempp)){
			errorFlag=2;
			gameQuit();
		}
	}
	if(!selfCrackFlag){
		if(mmp[nextpoint(tempp)]==1){
			errorFlag=1;
			gameQuit();
		}
	}
	int tempfood=mmp[nextpoint(tempp)];
	if(tempfood>=2)
	{
		food[tempfood].pop_back();
		cook();
		grow();
	}
	snake.push_front(nextpoint(tempp));
	snake.pop_back();
}

void grow()
{
    point tempp=snake.front();
	snake.push_front(nextpoint(tempp));
	l++;
	if(l==255){
		errorFlag=5;
		gameQuit();
	}
	if(sleepTime>=200)sleepTime-=20;
	else sleepTime-=10;
	if(sleepTime<=100)sleepTime=500;
}

point nextpoint(point p)
{
	switch(direction){
		case 1:return {(p.x-1+length)%length,p.y};
		case 2:return {p.x,(p.y-1+width)%width};
		case 3:return {(p.x+1)%length,p.y};
		case 4:return {p.x,(p.y+1)%width};
	}
	return p;
}
//return p.next.outOfMap
bool outOfMap(point p)
{
	return (direction==1&&p.x==0)||
		(direction==3&&p.x==length-1)||
		(direction==2&&p.y==0)||
		(direction==4&&p.y==width-1);
}
//hide console cursor
void hide()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
}

	/*
	char c=_getch();
	switch(c)
	{
		case 't':return 1;//timeTick++
		case 'w':return 2;
		case 'a':return 3;
		case 's':return 4;
		case 'd':return 5;
		case 'e':return 6;//length++
		case 'q':return 7;
	}
	*/