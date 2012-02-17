/*
FARL - Fucking About RogueLike
Created: 17/02/12
Last updated: 17/02/12
Bugs:
Todo: Collision with other chars

Remember to clean before sending to Git
*/

#include "libtcod.hpp"

int HandleKeys(int &dx, int &dy);

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 50;

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
		x+=dx;
		y+=dy;
	}
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



int main()
{	
	int dx=0;
	int dy=0;
	bool exit;
	character PC;
	PC.Init(20,20,"@",TCODColor::white);
	character Goblin;
	Goblin.Init(12,12,"g",TCODColor::green);
	TCODConsole::root->initRoot(SCREEN_WIDTH,SCREEN_HEIGHT,"FARL",false); // inits libtcod
	TCODConsole *con = new TCODConsole(SCREEN_WIDTH,SCREEN_HEIGHT); // inits a new console

	
	while(1){

		PC.Draw(con);
		Goblin.Draw(con);
		TCODConsole::blit(con,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,TCODConsole::root,0,0,1.0); // blits con onto the root
		TCODConsole::root->flush(); // actually prints the stuff?
		exit = HandleKeys(dx,dy);
		if (exit)
			break;
		PC.Clear(con);
		Goblin.Clear(con);
		PC.Move(dx,dy);
		dx=0;
		dy=0;
		PC.Collision();
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
