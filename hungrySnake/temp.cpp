#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

const int length =8,width=16;
/*
0:nothing
1:body
2:food
*/
int mp[length][width];
/*
1:up
2:down
3:left
4:right
*/
int state;
typedef struct{int x,y;}point;
/*
front:head
back:tail
*/
deque<point>snake;
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

int main()
{
    int tempState=0;
    gameInit();
    while(1){
		tempState=keyscan();
		work(tempState);
	}
    system("pause");
    return 0;
}

void gameInit()
{
    char c;
    srand(time(0));
	memset(mp,0,sizeof(mp));
	int tempx=length/2,tempy=width/2;
	mp[tempx][tempy]=1;
    snake.push_back({tempx,tempy});
    cout<<"push space to begin:\n";
    while(1){
        c=_getch();
        if(c==' ')break;
    }
	//cook();
    show();
}

void show()
{
	system("cls");
	printf("time:%lld  length:%lld\n",gameTime,l);
    memset(mp,0,sizeof(mp));
    for(auto p:snake){
        mp[p.x][p.y]=1;
    }
	int i,j;
	for(i=0;i<length;i++){
		for(j=0;j<width;j++){
			switch(mp[i][j])
			{
				case 0:_putch('`');break;
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
	int tempx,tempy;
	do
	{
		tempx=rand()%length;
		tempy=rand()%width;
	} while (mp[tempx][tempy]);
	mp[tempx][tempy]=2;
}

void work(int s){
	switch(s){
		case 1:gameTime++;move();show();break;
		case 2:state=1;show();break;
		case 3:state=2;show();break;
		case 4:state=3;show();break;
		case 5:state=4;show();break;
		case 6:grow();show();break;
		case 7:gameQuit();break;
	}
}

void gameQuit()
{
    system("cls");
    cout<<"score:"<<l<<endl;
    system("pause");
    system("exit");
    
}

void move()
{
    point tempp=snake.front();
	switch (state){
		case 1:snake.push_front({((tempp.x)-1+length)%length,tempp.y});snake.pop_back();break;
        case 2:snake.push_front({tempp.x,((tempp.y)-1+width)%width});snake.pop_back();break;
		case 3:snake.push_front({((tempp.x)+1)%length,tempp.y});snake.pop_back();break;
		case 4:snake.push_front({tempp.x,((tempp.y)+1)%width});snake.pop_back();break;
	}
}

void grow()
{
    point tempp=snake.front();
	switch (state){
		case 1:snake.push_front({((tempp.x)-1+length)%length,tempp.y});break;
		case 2:snake.push_front({tempp.x,((tempp.y)-1+width)%width});break;
		case 3:snake.push_front({((tempp.x)+1)%length,tempp.y});break;
		case 4:snake.push_front({tempp.x,((tempp.y)+1)%width});break;
	}
	l++;
}