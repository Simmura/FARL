/*
FARL - Fucking About RogueLike
Created: 16/02/12
Last updated: 06/03/12
Bugs: 
Todo: Get multiple rooms working properly

Remember to clean before sending to Git
*/

#include "libtcod.hpp"
#include <list>
using namespace std;

bool HandleKeys(int &dx, int &dy);


const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 50;
const int MAP_WIDTH = 80;
const int MAP_HEIGHT = 45;
const int ROOM_MAX_SIZE = 10;
const int ROOM_MIN_SIZE = 6;
const int MAX_ROOMS = 30;

TCODColor colour_dark_wall(0,0,100);
TCODColor colour_dark_ground(50,50,150);
TCODRandom * default = TCODRandom::getInstance();


class tile{
public:
	bool blocked;
	bool block_sight;

	tile(bool a, bool b)
	{
		blocked = a;
		block_sight = b;
	}
	tile()
	{
	}
};

class rect{
public:
	int x1;
	int x2;
	int y1;
	int y2;
	int centre_x;
	int centre_y;
	rect(int x,int y,int w,int h)
	{
		x1 = x;
		y1 = y;
		x2 = x+w;
		y2 = y+h;
		centre_x = ((x1 + x2)/2);
		centre_y = ((y1 + y2)/2);
	}
	bool Intersect(rect other)
	{
		if(x1 <= other.x2 && x2 >= other.x1 && y1 <= other.y2 && y2 >= other.y1)
			return 1;
		else
			return 0;
	}
};

void CreateRoom(rect room);
void CreateHTunnel(int x1, int x2, int y);
void CreateVTunnel(int y1,int y2,int x);


tile map[MAP_WIDTH][MAP_HEIGHT];

class character{
public:
	int x;
	int y;
	const char* symbol;
	TCODColor colour;

	void Init(int a, int b, const char* c, TCODColor d)
	{
		x = a;
		y = b;
		symbol = c;
		colour = d;
	}
	void Move(int dx, int dy)
	{
		if(map[x+dx][y+dy].blocked==false){
			x+=dx;
			y+=dy;
		}
		Collision();
	}
private:
	void Collision()
	{
		if(x<0)
			x=0;
		if(x>=SCREEN_WIDTH)
			x=SCREEN_WIDTH-1;
		if(y<0)
			y=0;
		if(y>=SCREEN_HEIGHT)
			y=SCREEN_HEIGHT-1;
	}
public:
	void Draw(TCODConsole *console)
	{
		console->setForegroundColor(colour);
		console->printLeft(x, y, TCOD_BKGND_NONE, symbol);
	}
	void Clear(TCODConsole *console)
	{
		console->printLeft(x, y, TCOD_BKGND_NONE, " ");
	}
};


void RenderAll(TCODConsole *console, TCODList<character*> objects);
void MakeMap(TCODList<rect*> rooms, character &player);



int main()
{	
	
	int dx=0;
	int dy=0;
	bool exit = 0;
	TCODList<character*> objects;
	TCODList<rect*> rooms;
	character PC;
	PC.Init(25,23,"@",TCODColor::white);
	TCODConsole::root->initRoot(SCREEN_WIDTH,SCREEN_HEIGHT,"FARL",false); // inits libtcod
	TCODConsole *con = new TCODConsole(SCREEN_WIDTH,SCREEN_HEIGHT); // inits a new console

	objects.push(&PC);
	MakeMap(rooms, PC);

	
	while(1){
		RenderAll(con,objects);
		TCODConsole::root->flush(); // actually prints the stuff
		exit = HandleKeys(dx,dy);
		if (exit)
			break;
		for(character **it = objects.begin(); it!= objects.end(); it++)
			(*it)->Clear(con);
		PC.Move(dx,dy);
		dx=0;
		dy=0;
	}
	return 0;
}

bool HandleKeys(int &dx, int &dy)
{
	TCOD_key_t key = TCODConsole::waitForKeypress(true);
	if(key.vk==TCODK_UP)
		dy = -1;
	if(key.vk==TCODK_DOWN)
		dy = 1;
	if(key.vk==TCODK_LEFT)
		dx = -1;
	if(key.vk==TCODK_RIGHT)
		dx = 1;
	if(key.vk==TCODK_ESCAPE)
		return 1;

	return 0;
}

void MakeMap(TCODList<rect*> rooms, character &player)
{
	for (int i = 0;i<MAP_WIDTH;i++){
		for (int j = 0;j<MAP_HEIGHT;j++)
			map[i][j]=tile(true,true);
	}
	int num_rooms = 0;
	int w,h,x,y,new_x,new_y,prev_x,prev_y,hold;
	bool failed = false;
	rect *new_room;
	for(int i = 0;i<MAX_ROOMS;i++){
		failed = false;
		w = default->getInt(ROOM_MIN_SIZE,ROOM_MAX_SIZE);
		h = default->getInt(ROOM_MIN_SIZE,ROOM_MAX_SIZE);
		x = default->getInt(0,MAP_WIDTH-w-1);
		y = default->getInt(0,MAP_HEIGHT-h-1);
		new_room = new rect(x,y,w,h);
		if (num_rooms!=0){
			for(rect **it = rooms.begin(); it != rooms.end(); it++){
				if(new_room->Intersect(**it)){
					failed = true;
					break;
				}
			}
		}
		if(failed == false){
			CreateRoom(*new_room);
			new_x=new_room->centre_x;
			new_y=new_room->centre_y;
			if(num_rooms==0){
				player.x=new_x;
				player.y=new_y;
			}
			else{
				rect hold = *rooms.peek();
				prev_x = hold.centre_x;
				prev_y = hold.centre_y;
				if(default->getInt(0,1)==1){
					CreateHTunnel(prev_x,new_x,prev_y);
					CreateVTunnel(prev_y,new_y,new_x);
				}
				else{
					CreateVTunnel(prev_y,new_y,new_x);
					CreateHTunnel(prev_x,new_x,prev_y);
				}
			}
			rooms.push(new_room);
			num_rooms++;
		}
	}
}

void RenderAll(TCODConsole *console, TCODList<character*> objects)
{
	bool wall;
	for(int i = 0;i<MAP_WIDTH;i++){
		for(int j=0;j<MAP_HEIGHT;j++){
			wall = map[i][j].block_sight;
			if(wall==true)
				console->setBack(i, j, colour_dark_wall, TCOD_BKGND_SET);
			else
				console ->setBack(i, j, colour_dark_ground, TCOD_BKGND_SET);
		}
	}

	for(character **it = objects.begin(); it!= objects.end(); it++)
			(*it)->Draw(console);
	TCODConsole::blit(console,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,TCODConsole::root,0,0,1.0);
}

void CreateRoom(rect room)
{
	for(int i = room.x1;i<room.x2;i++){
		for(int j = room.y1;j<room.y2;j++){
			map[i][j].blocked=false;
			map[i][j].block_sight=false;
		}
	}
}

void CreateHTunnel(int x1,int x2,int y)
{
	if(x2>x1){
		for(int i = x1;i<x2+1;i++){
			map[i][y].blocked=false;
			map[i][y].block_sight=false;
		}
	}
	else{
		for(int i = x2;i<x1+1;i++){
			map[i][y].blocked=false;
			map[i][y].block_sight=false;
		}
	}
}

void CreateVTunnel(int y1,int y2,int x)
{
	if(y2>y1){
		for(int i = y1;i<y2+1;i++){
			map[x][i].blocked=false;
			map[x][i].block_sight=false;
		}
	}
	else{
		for(int i = y2;i<y1+1;i++){
			map[x][i].blocked=false;
			map[x][i].block_sight=false;
		}
	}

}
