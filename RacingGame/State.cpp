#include "Head.h"
#include "State.h"

void State::run(Ogre::RenderWindow* win)
{
	switch(mRunState)
	{
	case INIT:
		mWindow = win;
		init();
		isInit = true;
		mRunState = LOOP;
		break;
	case  LOOP:
		loop();
		break;
	}
}