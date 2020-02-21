#include<iostream>
#include<cstdio>
#include<map>
#include<queue>
#include<set>
#include<vector>
#include<cstring>
#include<ctime>
#include<windows.h>
#include<conio.h>
using namespace std;
const int LENGTH = 20, WIDTH = 10, SPACE = 0, SKY = -1, GND = 1;
const char s[] = "IOTSZLJ";
struct point
{
	int x, y;
	point(int a, int b) :x(a), y(b) {};
	bool operator < (const point p)const { return this->x < p.x || (this->x == p.x && this->y < p.y); }
	point operator + (const point p)const { return { this->x + p.x,this->y + p.y }; }
};
const point NULLPOINT = { 0,0 };

/*
0:SPACE
1:GND
-1:SKY
*/
int mp[LENGTH][WIDTH]; map<point, int>mmp;
queue<int>food;
set<int>bag7;
point p = { 0,4 };
vector<point>pp;
int sleepTime = 1000;
/*
0:down
1:fall
2:rotate
3:left
4:right
5:down2
6:rotate2
7:quit
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
int t, tileNext;
//rotate state
int r;
int score = 0;
bool fixFlag = 0;
int h = -1;

void gameInit();
void gameQuit();
void show();
void down();
void rotate();
void rotate2();
void left();
void right();
void fall();
void cook();
void hold();
void work(int);
void fix();
bool gg();
int keyscan();
vector<point> nextpp();
void mmp4(point, int, vector<point>);
vector<point> tileInfo(int t, int r);
void pandttov(vector<point>& vp, point po, int tt, int rr);

void hold()
{
	if (h != -1)swap(h, t);
	else {
		h = t;
		t = tileNext;
		tileNext = rand() % 7;
	}
}

void down()
{
	auto temp = tileInfo(t, r), tt = temp;
	pandttov(temp, p, t, r);
	bool f = 1;
	for (auto i : temp) {
		if (i.x == LENGTH - 1 || mp[i.x + 1][i.y] == GND)f = 0;
	}
	if (f) {
		mmp4(p, SPACE, tt);
		p = { p.x + 1,p.y };
		mmp4(p, SKY, tt);
	}
	else {
		fix();
	}
}

void left()
{
	auto temp = tileInfo(t, r), tt = temp;
	pandttov(temp, p, t, r);
	bool f = 1;
	for (auto i : temp) {
		if (i.y == 0)f = 0;
	}
	if (f) {
		mmp4(NULLPOINT, SPACE, temp);
		p = { p.x,p.y - 1 };
		mmp4(p, SKY, tt);
	}
}

void right()
{
	auto temp = tileInfo(t, r), tt = temp;
	pandttov(temp, p, t, r);
	bool f = 1;
	for (auto i : temp) {
		if (i.y == WIDTH - 1)f = 0;
	}
	if (f) {
		mmp4(NULLPOINT, SPACE, temp);
		p = { p.x,p.y + 1 };
		mmp4(p, SKY, tt);
	}
}

void rotate()
{
	vector<point>tt = { {0,0},{0,0},{0,0},{0,0} };
	pandttov(tt, p, t, r);
	auto temp = nextpp();
	bool f = 1;
	for (auto i : temp) {
		if (mmp[i] >= 1)f = 0;
	}
	if (f) {
		mmp4(NULLPOINT, SPACE, tt);
		r = (r + 1) % 4;
		mmp4(p, SKY, tileInfo(t, r));
	}
}

void rotate2()
{
	auto temp = nextpp();
	vector<point>tt = { {0,0},{0,0},{0,0},{0,0} };
	pandttov(tt, p, t, r);
	bool f = 1;
	for (auto i : temp) {
		if (mmp[i] >= 1)f = 0;
	}
	if (f) {
		mmp4(NULLPOINT, SPACE, tt);
		r = (r + 4 - 1) % 4;
		mmp4(p, SKY, tileInfo(t, r));
	}
}

void fall()
{
	auto temp = tileInfo(t, r);
	pandttov(temp, p, t, r);
	mmp4(NULLPOINT, SPACE, temp);
	while (1)
	{
		bool f = 1;
		pandttov(temp, p, t, r);
		for (auto i : temp) {
			if (i.x == LENGTH - 1 || mp[i.x + 1][i.y] == GND)f = 0;
		}
		if (f) {
			p = p + point{ 1,0 };
		}
		else {
			break;
		}
	}
	fix();
}

int main()
{
	gameInit();
	while (1)
	{
		Sleep(sleepTime);
		state = keyscan();
		work(state);
		show();
	}
	return 0;
}

void gameInit()
{
	ios::sync_with_stdio(0); cin.tie(0);
	srand(time(0));
	state = score = 0;
	t = rand() % 7;
	tileNext = rand() % 7;
	sleepTime = 200;
	mmp.clear();
	memset(mp, 0, sizeof(mp));
	p = { 0,4 };
	pp = { {0,0},{0,0},{0,0},{0,0} };
	//makepp(pp);
	pandttov(pp, p, t, r);
}

int keyscan()
{
	int n = 0;
	if (GetAsyncKeyState(VK_SPACE))n = 1;
	if (GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP))n = 2;
	if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT))n = 3;
	if (GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN))n = 4;
	if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT))n = 5;
	if (GetAsyncKeyState('Z') || GetAsyncKeyState(VK_LCONTROL))n = 6;
	if (GetAsyncKeyState('Q'))n = 7;
	if (GetAsyncKeyState('P'))n = 8;
	if (GetAsyncKeyState(VK_RSHIFT))n = 9;
	if (GetAsyncKeyState('2'))n = 10;
	return n;
}

void work(int s)
{
	switch (state) {
	case 0:down(); break;
	case 1:fall(); break;
	case 2:rotate(); break;
	case 3:left(); break;
	case 4:down(); down(); break;
	case 5:right(); break;
	case 6:rotate2(); break;
	case 7:gameQuit(); break;
	case 8:system("pause"); break;
	case 9:hold(); break;
	}
}

void show()
{
	system("cls");
	//memset(mp, 0, sizeof(mp));
	//mmp.clear();
	//mmp4(p, SKY, tileInfo(t, r));//pp
	if (fixFlag) {
		fixFlag = 0;
	}


	for (int i = 0; i < LENGTH; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (mp[i][j] != SPACE) {
				_putch('#');

			}
			else {
				_putch(' ');

			}
		}
		_putch('\n');

	}
	printf("next:%c\nhold:%c\nscore:%d", s[tileNext],(h?s[h]:'?'),score);
}
//point p and ti to v4pp
void pandttov(vector<point>& vp, point po, int tt, int rr)
{
	vector<point>temp = tileInfo(tt, rr);
	int l = temp.size();
	for (int i = 0; i < l; i++)vp[i] = temp[i] + po;
}

void fix()
{
	fixFlag = 1;
	mmp4(p, GND, tileInfo(t, r));
	vector<int>v;
	for (int i = 0; i < LENGTH; i++) {
		int n = 0;
		for (int j = 0; j < WIDTH; j++)n += mp[i][j];
		if (n == WIDTH) {
			v.push_back(i);
			memset(mp + i, 0, sizeof(int) * WIDTH);
			score++;
		}
	}
	for (auto i : v)for (int j = i; j > 0; j--)for (int k = 0; k <= WIDTH; k++)mp[j][k] = mp[j - 1][k];
	for (int i = 0; i < LENGTH; i++)for (int j = 0; j < WIDTH; j++)mmp[{i, j}] = mp[i][j];
	cook();
}

//p+v=s
void mmp4(point p, int s, vector<point>v)
{
	int l = v.size();
	for (int i = 0; i < l; i++) {
		mmp[p + v[i]] = s;
		mp[p.x + v[i].x][p.y + v[i].y] = s;
	}
}

//gg=1
bool gg()
{
	auto temp = nextpp();
	for (auto i : temp) {
		if (mmp[i] >= 1)return 1;
		switch (state)
		{
		case 0:
			if (i.x == LENGTH - 1)return 1;
			break;
		case 3:
			if (i.y == 0)return 1;
			break;
		default:
			break;
		}
	}
	return 0;
}

vector<point> nextpp()
{
	vector<point>temp = { {0,0},{0,0},{0,0},{0,0} };
	switch (state)
	{
	case 0: pandttov(temp, p + point{ 1,0 }, t, r); break;
	case 2: pandttov(temp, p, t, (r + 1) % 4); break;
	case 3: pandttov(temp, p + point{ 0,-1 }, t, r); break;
	case 4: pandttov(temp, p + point{ 0,1 }, t, r); break;
	case 6: pandttov(temp, p, t, (r + 4 - 1) % 4); break;
	}
	return temp;
}

void cook()
{
	t = tileNext;
	p = { 0,4 };
	pandttov(pp, p, t, r);
	for (auto i : pp) {
		if (mmp[i] == GND)gameQuit();
	}
	mmp4(p, SKY, tileInfo(t, r));
	tileNext = rand() % 7;
}

void gameQuit()
{
	system("cls");
	printf("score:%d", score);
	gameInit();
	system("pause");
}
vector<point> tileInfo(int t, int r)
{
	vector<point>v;
	switch (t) {
	case 0: {
		if (r == 0 || r == 2)v = { {0,0},{1,0},{2,0},{3,0} };
		else if (r == 1 || r == 3)v = { {0,0},{0,1},{0,2},{0,3} };
		break;
	}
	case 1: {
		v = { {0,0},{0,1},{1,0},{1,1} };
		break;
	}
	case 2: {
		if (r == 0)v = { {0,1},{1,0},{1,1},{1,2} };
		else if (r == 1)v = { {0,0},{1,0},{1,1},{2,0} };
		else if (r == 2)v = { {0,0},{0,1},{0,2},{1,1} };
		else if (r == 3)v = { {0,1},{1,0},{1,1},{2,1} };
		break;
	}
	case 3: {
		if (r == 0 || r == 2)v = { {0,0},{1,0},{1,1},{2,1} };
		else if (r == 1 || r == 3)v = { {0,1},{0,2},{1,0},{1,1} };
		break;
	}
	case 4: {
		if (r == 0 || r == 2)v = { {0,1},{1,0},{1,1},{2,0} };
		else if (r == 1 || r == 3)v = { {0,0},{0,1},{1,1},{1,2} };
		break;
	}
	case 5: {
		if (r == 0)v = { {0,0},{1,0},{2,0},{2,1} };
		else if (r == 1)v = { {0,0},{0,1},{0,2},{1,0} };
		else if (r == 2)v = { {0,0},{0,1},{1,1},{2,1} };
		else if (r == 3)v = { {0,2},{1,0},{1,1},{1,2} };
		break;
	}
	case 6: {
		if (r == 0)v = { {0,1},{1,1},{2,0},{2,1} };
		else if (r == 1)v = { {0,0},{1,0},{1,1},{1,2} };
		else if (r == 2)v = { {0,0},{0,1},{1,0},{2,0} };
		else if (r == 3)v = { {0,0},{0,1},{0,2},{1,2} };
		break;
	}
	}
	return v;
}
/*
vector<int> tileInfo(int t,int r)
{
	vector<int>v;
	switch(t){
		case 0:{
			if(r==0||r==2)v={0,0,0,0,0,0,0,0};
			if(r==1||r==3)v={0,0,0,1,0,2,0,3};
			break;
		}
		case 1:{
			v={0,0,0,1,1,0,1,1};
			break;
		}
		case 2:{
			if(r==0)v={0,1,1,0,1,1,1,2};
			if(r==1)v={0,0,1,0,1,1,2,0};
			if(r==2)v={0,0,0,1,0,2,1,1};
			if(r==3)v={0,1,1,0,1,1,2,1};
			break;
		}
		case 3:{
			if(r==0||r==2)v={0,0,1,0,1,1,2,1};
			if(r==1||r==3)v={0,1,0,2,1,0,1,1};
			break;
		}
		case 4:{
			if(r==0||r==2)v={0,1,1,0,1,1,2,0};
			if(r==1||r==3)v={0,0,0,1,1,1,1,2};
			break;
		}
		case 5:{
			if(r==0)v={0,0,1,0,2,0,2,1};
			if(r==1)v={0,0,0,1,0,2,1,0};
			if(r==2)v={0,0,0,1,1,1,2,1};
			if(r==3)v={0,2,1,0,1,1,1,2};
			break;
		}
		case 6:{
			if(r==0)v={0,1,1,1,2,0,2,1};
			if(r==1)v={0,0,1,0,1,1,1,2};
			if(r==2)v={0,0,0,1,1,0,2,0};
			if(r==3)v={0,0,0,1,0,2,1,2};
			break;
		}
	}
	return v;
}
*/
/*
for(tile=0;tile<7;tile++){
	for(r=0;r<4;r++){
		show();
		system("pause");
	}
}
*/
//void mmp4(point&,int,int,int,int,int,int,int,int,int);
/*
void mmp4(point& p,int s,int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	int x=p.x,y=p.y;
	mmp[{x+x1,y+y1}]=mmp[{x+x1,y+y1}]=mmp[{x+x1,y+y1}]=mmp[{x+x1,y+y1}]=s;
	mp[x+x1][y+y1]=mp[x+x2][y+y2]=mp[x+x3][y+y3]=mp[x+x4][y+y4]=s;
}
*/