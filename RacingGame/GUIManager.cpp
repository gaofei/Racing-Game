#include "Head.h"
#include "GUIManager.h"

GUIManager* GUIManager::mGUIManager = NULL;

//-------------------------------------------------------------------------------------

GUIManager::GUIManager()
{
	mHikariMgr = NULL;
}

//-------------------------------------------------------------------------------------

GUIManager::~GUIManager()
{
}

//-------------------------------------------------------------------------------------

GUIManager* GUIManager::getSingleton()
{
	if (!mGUIManager)
		mGUIManager = new GUIManager;

	return mGUIManager;
}

//-------------------------------------------------------------------------------------

void GUIManager::Initialize()
{
	mHikariMgr = new Hikari::HikariManager("resource\\Gui");
}

//-------------------------------------------------------------------------------------

void GUIManager::Loop(float timeElapsed)
{
	mHikariMgr->update();
}

//-------------------------------------------------------------------------------------


void GUIManager::Destory()
{
	if (mHikariMgr)
		delete mHikariMgr;
	mHikariMgr = NULL;

	if (mGUIManager)
		delete mGUIManager;
	mGUIManager = NULL;
}

//-------------------------------------------------------------------------------------

bool GUIManager::injectKeyPressed( const OIS::KeyEvent &e )
{
	unsigned int key = e.key;
	mHikariMgr->injectKeyEvent(WM_KEYDOWN, key, 0);

	return true;
}

//-------------------------------------------------------------------------------------

bool GUIManager::injectKeyReleased( const OIS::KeyEvent &e )
{
	unsigned int key = e.key;
	mHikariMgr->injectKeyEvent(WM_KEYUP, key, 0);

	return true;
}

//-------------------------------------------------------------------------------------

bool GUIManager::injectMouseMoved( const OIS::MouseEvent &e )
{
	mHikariMgr->injectMouseMove((short)e.state.X.abs, (short)e.state.Y.abs);

	return true;
}

//-------------------------------------------------------------------------------------

bool GUIManager::injectMousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
	mHikariMgr->injectMouseDown(convertButton(id));
	return true;
}

//-------------------------------------------------------------------------------------

bool GUIManager::injectMouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
	mHikariMgr->injectMouseUp(convertButton(id));

	return true;
}

//-------------------------------------------------------------------------------------

Hikari::HikariManager* GUIManager::getHikariManger()
{
	return mHikariMgr;
}

//-------------------------------------------------------------------------------------

Hikari::MouseButtonID GUIManager::convertButton(OIS::MouseButtonID buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return Hikari::LeftMouseButton;

	case OIS::MB_Right:
		return Hikari::RightMouseButton;

	case OIS::MB_Middle:
		return Hikari::MiddleMouseButton;

	default:
		return Hikari::LeftMouseButton;
	}
}