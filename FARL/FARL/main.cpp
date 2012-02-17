#include <iostream>
#include <time.h>
#include "libtcod.hpp"
#include <string.h>

using namespace std;

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
	}

};

int main()
{	
	character PC;
	PC.Init(20,20,"@");
	TCODConsole::root->initRoot(80,50,"FARL",false);
	TCODConsole::root->printLeft(PC.x, PC.y, TCOD_BKGND_SET, PC.symbol);
	
	while(1){
		PC.GetMove();
		TCODConsole::root->clear();
		TCODConsole::root->printLeft(PC.x, PC.y, TCOD_BKGND_SET, PC.symbol);
		TCODConsole::root->flush();
	}
	return 0;
}
