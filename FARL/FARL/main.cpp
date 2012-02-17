/*
FARL - Fucking About RogueLike
Created: 17/02/12
Last updated: 17/02/12
Bugs:
Todo: Get collision detection with objects working

Remember to clean before sending to Git
*/

#include "libtcod.hpp"



class character{
public:
	int x;
	int y;
	const char* symbol;

	void Init(int a, int b, const char* c)
	{
		x = a;
		y = b;
		symbol = c;
	}

	void GetMove()
	{
		TCOD_key_t key = TCODConsole::checkForKeypress();
		if (key.vk == TCODK_UP)
			y--;
		if (key.vk == TCODK_DOWN)
			y++;
		if (key.vk == TCODK_LEFT)
			x--;
		if (key.vk == TCODK_RIGHT)
			x++;

		if(x<0)
			x=0;
		if(x>=79)
			x=79;
		if(y<0)
			y=0;
		if(y>=49)
			y=49;
	}

};

int main()
{	
	character PC;
	PC.Init(20,20,"@");
	TCODConsole::root->initRoot(80,50,"FARL",false); // inits libtcod
	
	while(1){
		PC.GetMove();
		TCODConsole::root->clear();
		TCODConsole::root->printLeft(PC.x, PC.y, TCOD_BKGND_SET, PC.symbol); // TCOD_BKGND_SET just sets it to the console's colours - white on black
		TCODConsole::root->printLeft(12, 12, TCOD_BKGND_SET, "#");
		TCODConsole::root->flush(); // actually prints the stuff?
	}
	return 0;
}
