/*
FARL - Fucking About RogueLike
Created: 16/02/12
Last updated: 01/03/12
Bugs: 
Todo: Part 3 of the tutorial

Remember to clean before sending to Git
*/

#include "libtcod.hpp"
#include <list>
using namespace std;

int HandleKeys(int &dx, int &dy);
void MakeMap();

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 50;
const int MAP_WIDTH = 80;
const int MAP_HEIGHT = 45;

TCODColor colour_dark_wall(0,0,100);
TCODColor colour_dark_ground(50,50,150);


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



int main()
{	
	int dx=0;
	int dy=0;
	bool exit = 0;
	TCODList<character*> objects;
	character PC;
	PC.Init(20,20,"@",TCODColor::white);
	//character Goblin;
	//Goblin.Init(12,12,"g",TCODColor::green);
	TCODConsole::root->initRoot(SCREEN_WIDTH,SCREEN_HEIGHT,"FARL",false); // inits libtcod
	TCODConsole *con = new TCODConsole(SCREEN_WIDTH,SCREEN_HEIGHT); // inits a new console

	objects.push(&PC);
	//objects.push(&Goblin);
	MakeMap();

	map[30][22].blocked = true;
	map[30][22].block_sight = true;
	map[50][22].blocked = true;
	map[50][22].block_sight = true;
	
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

int HandleKeys(int &dx, int &dy)
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

void MakeMap()
{
	for (int i = 0;i<MAP_WIDTH;i++){
		for (int j = 0;j<MAP_HEIGHT;j++)
			map[i][j]=tile(false,false);
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
