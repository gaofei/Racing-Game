#include "Head.h"
#include "StatesManager.h"

StatesManager* StatesManager::mStateManger = 0;

//---------------------------------------------------------------------------------------------------------

StatesManager::StatesManager(Ogre::RenderWindow* win)
{
	mStateManger = this;
	attachWindow(win);
}

//---------------------------------------------------------------------------------------------------------

StatesManager::~StatesManager()
{
	while (mStateStack.size())
	{
		State* locState = mStateStack.back();
		locState->exit();
		delete locState;

		mStateStack.pop_back();
	}
}

//---------------------------------------------------------------------------------------------------------

StatesManager* StatesManager::getSingleton()
{
	if(mStateManger)
		return mStateManger;
	else
		return 0;
	//#~写错误代码！！
	//下面有size判断的的全部都是！
}

//---------------------------------------------------------------------------------------------------------

bool StatesManager::run()
{
	if(mStateStack.size() > 0)
	{
		mStateStack.back()->run(mWindow);
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------------------------

bool StatesManager::injectMouseMoved( const OIS::MouseEvent &e )
{
	if(mStateStack.size() > 0)
	{
		return mStateStack.back()->mouseMoved(e);
	}
	return true;
}

//---------------------------------------------------------------------------------------------------------

bool StatesManager::injectMousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
	if(mStateStack.size() > 0)
	{
		return mStateStack.back()->mousePressed(e, id);
	}
	return true;
}

//---------------------------------------------------------------------------------------------------------

bool StatesManager::injectMouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
	if(mStateStack.size() > 0)
	{
		return mStateStack.back()->mouseReleased(e, id);
	}
	return true;
}

//---------------------------------------------------------------------------------------------------------

bool StatesManager::injectKeyPressed( const OIS::KeyEvent &e )
{
	if(mStateStack.size() > 0)
	{
		return mStateStack.back()->keyPressed(e);
	}
	return true;
}

//---------------------------------------------------------------------------------------------------------

bool StatesManager::injectKeyReleased( const OIS::KeyEvent &e )
{
	if(mStateStack.size() > 0)
	{
		return mStateStack.back()->keyReleased(e);
	}
	return true;
}

//---------------------------------------------------------------------------------------------------------

void StatesManager::changeCurrentState(State* pState)
{
	if (!mStateStack.empty())
	{
		mStateStack.back()->exit();
		mStateStack.pop_back();
		mStateStack.push_back(pState);
	}
}

//---------------------------------------------------------------------------------------------------------

void StatesManager::resetCurrentState()
{
	if (!mStateStack.empty())
	{
		mStateStack.back()->exit();
		mStateStack.back()->setRunState(State::INIT);
	}
}

//--------------------------------------------------------------------------------------------------------

void StatesManager::buildNewState(State* pState)
{
	mStateStack.push_back(pState);
	mStateStack.back()->setRunState(State::INIT);
}

//---------------------------------------------------------------------------------------------------------

void StatesManager::resumeLastState()
{
	if (!mStateStack.empty())
	{
		State* state = mStateStack.back();
		state->exit();
		delete state;

		mStateStack.pop_back();
	}
}

//---------------------------------------------------------------------------------------------------------

void StatesManager::clearAllState()
{
	while (mStateStack.size() > 0)
	{
		mStateStack.back()->exit();
		mStateStack.pop_back();
	}
}

//---------------------------------------------------------------------------------------------------------
void StatesManager::attachWindow(Ogre::RenderWindow* win)
{
	mWindow = win;
}