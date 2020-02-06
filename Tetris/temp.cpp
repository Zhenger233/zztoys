#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
using namespace std;
const int length=20,width=10;
const char s[]="IOTSZLJ";
struct point
{
	int x,y;
	point(int a,int b):x(a),y(b){};
	bool operator < (const point p)const{return this->x<p.x||(this->x==p.x&&this->y<p.y);}
};

/*
0:space
1:real
2:float
*/
int mp[length][width];map<point,int>mmp;
queue<int>food;
set<int>bag7;
point p={0,4};

int sleepTime;
/*
0:down
1:fallDown
2:rotate
3:left
4:right
5:ddoowwnn
6:rotate2
*/
int state;

/*
0:I
1:O
2:T
3:S
4:Z
5:L
6:J
*/
int tile,tileNext;
int r;

void gameInit();
void gameQuit();
void show();
void fallDown();
void fallLine();
void rotate();
void cook();
int keyscan();
void work(int);
void p2p();
void mmp4(point&,int,int,int,int,int,int,int,int,int);

int main()
{
	gameInit();
	/*
	while(1)
	{
		Sleep(sleepTime);
		state=keyscan();
		work(state);
	}
	*/
	for(tile=0;tile<7;tile++){
		for(r=0;r<4;r++){
			show();
			system("pause");
		}
	}
	return 0;
}

void gameInit()
{
	srand(time(0));
	state=0;
	tile=rand()%7;
	tileNext=rand()%7;
	sleepTime=1000;
	mmp.clear();
	memset(mp,0,sizeof(mp));
	p={0,4};
}

int keyscan()
{
	int n=0;
	if(GetAsyncKeyState(VK_SPACE))n=1;
	if(GetAsyncKeyState('W')||GetAsyncKeyState(VK_UP))n=2;
	if(GetAsyncKeyState('A')||GetAsyncKeyState(VK_LEFT))n=3;
	if(GetAsyncKeyState('S')||GetAsyncKeyState(VK_DOWN))n=4;
	if(GetAsyncKeyState('D')||GetAsyncKeyState(VK_RIGHT))n=5;
	if(GetAsyncKeyState('Z'))n=6;
	if(GetAsyncKeyState('Q'))n=7;
	if(GetAsyncKeyState('E'))n=8;
	if(GetAsyncKeyState('1'))n=9;
	if(GetAsyncKeyState('2'))n=10;
	return n;
}

void work(int s)
{
	switch(s){
		case 1:fallDown();break;
		case 2:rotate();break;
		case 3:break;
	}
}

void cook()
{
	tile=tileNext;
	tileNext=rand()%7;
}

void show()
{
	memset(mp,0,sizeof(mp));
	mmp.clear();
	p2p();
	for(int i=0;i<length;i++){
		for(int j=0;j<width;j++){
			if(mp[i][j])_putch('#');
			else _putch(' ');
		}
		_putch('\n');
	}
}

void p2p()
{
	switch(tile){
		case 0:{
			if(r==0||r==2)mmp4(p,2,0,0,1,0,2,0,3,0);
			if(r==1||r==3)mmp4(p,2,0,0,0,1,0,2,0,3);
			break;
		}
		case 1:{
			mmp4(p,2,0,0,0,1,1,0,1,1);
			break;
		}
		case 2:{
			if(r==0)mmp4(p,2,0,1,1,0,1,1,1,2);
			if(r==1)mmp4(p,2,0,0,1,0,1,1,2,0);
			if(r==2)mmp4(p,2,0,0,0,1,0,2,1,1);
			if(r==3)mmp4(p,2,0,1,1,0,1,1,2,1);
			break;
		}
		case 3:{
			if(r==0||r==2)mmp4(p,2,0,0,1,0,1,1,2,1);
			if(r==1||r==3)mmp4(p,2,0,1,0,2,1,0,1,1);
			break;
		}
		case 4:{
			if(r==0||r==2)mmp4(p,2,0,1,1,0,1,1,2,0);
			if(r==1||r==3)mmp4(p,2,0,0,0,1,1,1,1,2);
			break;
		}
		case 5:{
			if(r==0)mmp4(p,2,0,0,1,0,2,0,2,1);
			if(r==1)mmp4(p,2,0,0,0,1,0,2,1,0);
			if(r==2)mmp4(p,2,0,0,0,1,1,1,2,1);
			if(r==3)mmp4(p,2,0,2,1,0,1,1,1,2);
			break;
		}
		case 6:{
			if(r==0)mmp4(p,2,0,1,1,1,2,0,2,1);
			if(r==1)mmp4(p,2,0,0,1,0,1,1,1,2);
			if(r==2)mmp4(p,2,0,0,0,1,1,0,2,0);
			if(r==3)mmp4(p,2,0,0,0,1,0,2,1,2);
			break;
		}
	}
}

void mmp4(point& p,int s,int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	int x=p.x,y=p.y;
	mmp[{x+x1,y+y1}]=mmp[{x+x1,y+y1}]=mmp[{x+x1,y+y1}]=mmp[{x+x1,y+y1}]=s;
	mp[x+x1][y+y1]=mp[x+x2][y+y2]=mp[x+x3][y+y3]=mp[x+x4][y+y4]=s;
}

void fallDown()
{

}

void rotate()
{
	r=(r+1)%4;
}
