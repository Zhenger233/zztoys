#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

const int length =8,width=16,foodKind=6;
typedef struct point
{
	int x,y;
	bool operator < (const point p)const{
		return this->x<p.x||(this->x==p.x&&this->y<p.y);
	}
}point;

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
0:nothing
1:body
2:food
*/
int mp[length][width];
map<point,int>mmp;
vector<point>spacemp;
/*
1:up
2:down
3:left
4:right
*/
int direction;
//flag=0,work
int selfCrackFlag=0,wallCrackFlag=1;
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
point nextpoint(point);
bool outOfMap(point);

int main()
{
    int tempstate=0;
    gameInit();
    while(1){
		if(errorFlag==0)break;
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
    srand(time(0));
	errorInfo.clear();
	errorInfo.push_back("quit.\n");
	errorInfo.push_back("self.\n");
	errorInfo.push_back("wall.\n");
	errorInfo.push_back("poison.\n");
	errorInfo.push_back("restart.\n");
	memset(mp,0,sizeof(mp));
	mmp.clear();
	snake.clear();
	food.clear();
	for(i=0;i<length;i++)for(j=0;j<width;j++){
		spacemp.push_back({i,j});
	}
	l=0;
	direction=0;
	gameTime=0;
	int tempx=length/2,tempy=width/2;
	mp[tempx][tempy]=1;
    snake.push_back({tempx,tempy});
    cout<<"push space to begin:\n";
    while(1){
        c=_getch();
        if(c==' ')break;
    }
	cook();
    show();
}

void show()
{
	int i,j;
	system("cls");
	printf("time:%lld  length:%lld\n",gameTime,l);
    memset(mp,0,sizeof(mp));
	mmp.clear();
    for(auto p:snake){
        mp[p.x][p.y]=1;
		mmp[p]=1;
    }
	for(int i=2;i<=foodKind;i++)if(!food[i].empty())for(auto j:food[i]){
		mmp[j]=i;
		mp[j.x][j.y]=i;
	}
	
	for(i=0;i<length;i++){
		for(j=0;j<width;j++){
			switch(mp[i][j])
			{
				case 0:_putch('`');spacemp.push_back({i,j});break;
				case 1:_putch('*');break;
				case 2:_putch('#');break;
			}
		}
		_putch('\n');
	}
}

int keyscan(){
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
	return 0;
}

void cook()
{
	int tempx,tempy,tempn,tempnn;
	tempn=spacemp.size();
	tempnn=rand()%tempn;
	tempx=spacemp[tempnn].x;
	tempy=spacemp[tempnn].y;
	mp[tempx][tempy]=2;
	food[2].push_back({tempx,tempy});
}

void work(int s){
	switch(s){
		case 1:gameTime++;move();show();break;
		case 2:direction=1;show();break;
		case 3:direction=2;show();break;
		case 4:direction=3;show();break;
		case 5:direction=4;show();break;
		case 6:grow();show();break;
		case 7:errorFlag=0;gameQuit();break;
	}
}

void gameQuit()
{
    system("cls");
    cout<<"score:"<<l<<endl;
	cout<<errorInfo[errorFlag];
	//gameInit();
	errorFlag=0;
    system("pause");
	
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
//return p.next.out of map
bool outOfMap(point p)
{
	return (direction==1&&p.x==0)||
		(direction==3&&p.x==length-1)||
		(direction==2&&p.y==0)||
		(direction==4&&p.y==width-1);
}
