#include <iostream>
#include <time.h>
#include "libtcod.hpp"
#include <string.h>

using namespace std;

class character{
public:
	int x;
	int y;
	char symbol;

	void Init(int a, int b, char c)
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
	PC.Init(20,20,'@');
	TCODConsole::root->initRoot(80,50,"FARL",false);
	//TCODConsole::credits();
	TCODConsole::setColorControl(TCOD_COLCTRL_1,TCODColor::red,TCODColor::black);
	TCODConsole::root->printLeft(PC.x, PC.y, TCOD_BKGND_SET, "RAR",TCOD_COLCTRL_1,TCOD_COLCTRL_STOP);
	
	while(1){
		PC.GetMove();
		TCODConsole::root->clear();
		TCODConsole::root->printLeft(PC.x, PC.y, TCOD_BKGND_SET, "RAR",TCOD_COLCTRL_1,TCOD_COLCTRL_STOP);
		TCODConsole::root->flush();
	}
	return 0;
}
