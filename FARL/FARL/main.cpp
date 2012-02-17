/*
FARL - Fucking About RogueLike
Created: 17/02/12
Last updated: 17/02/12
Bugs:
Todo: Get the draw and clear functions working

Remember to clean before sending to Git
*/

#include "libtcod.hpp"

int HandleKeys(int &x, int &y);

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
	void Move(int deltax, int deltay)
	{
		x+=deltax;
		y+=deltay;
	}
	void Collision()
	{
		if(x<0)
			x=0;
		if(x>=79)
			x=79;
		if(y<0)
			y=0;
		if(y>=49)
			y=49;
	}
	//void Draw()
	//{
	//	con->setForegroundColor(colour);
	//	con->printLeft(x, y, TCOD_BKGND_NONE, symbol);
	//}
	//void Clear()
	//{
		//con->printLeft(x, y, TCOD_BKGND_NONE, " ");
	//}

};

int main()
{	
	int dx=0;
	int dy=0;
	bool exit;
	character PC;
	PC.Init(20,20,"@",TCODColor::white);
	character Goblin;
	//Goblin.Init(12,12,"g",TCODColor::green);
	TCODConsole::root->initRoot(80,50,"FARL",false); // inits libtcod
	TCODConsole *con = new TCODConsole(80,50); // inits a new console

	
	while(1){
		con->clear();
		//PC.Draw();
		//Goblin.Draw();
		//con->setForegroundColor(PC.colour);
		con->printLeft(PC.x, PC.y, TCOD_BKGND_SET, PC.symbol);
		TCODConsole::blit(con,0,0,80,50,TCODConsole::root,0,0,1.0); // blits con onto the root
		TCODConsole::root->flush(); // actually prints the stuff?
		exit = HandleKeys(dx,dy);
		if (exit)
			break;
		PC.Move(dx,dy);
		//PC.Collision();
	}
	return 0;
}

int HandleKeys(int &x, int &y)
{
	TCOD_key_t key = TCODConsole::waitForKeypress(true);
	if(key.vk==TCODK_UP)
		y = -1;
	if(key.vk==TCODK_DOWN)
		y = 1;
	if(key.vk==TCODK_LEFT)
		x = -1;
	if(key.vk==TCODK_RIGHT)
		x = 1;
	if(key.vk==TCODK_ESCAPE)
		return 1;

	return 0;
}
