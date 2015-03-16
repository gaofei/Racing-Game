#include "Head.h"
#include "GlobalManager.h"

#include "StatesManager.h"
#include "GameState.h"
#include "MenuState.h"
#include "TestState.h"

Ogre::SceneManager*		GlobalManager::mSharedSceneMgr	= NULL;

Ogre::RenderWindow*		GlobalManager::mGlobalWindow	= NULL;

Ogre::Camera*			GlobalManager::mShareCamera		= NULL;

Ogre::Viewport*			GlobalManager::mGlobalViewPort	= NULL;

//---------------------------------------------------------------------------------------------------------

void GlobalManager::globalInit()
{
	//TODO 整个游戏初始化的时候调用

	mGlobalWindow = (Ogre::RenderWindow*)Ogre::Root::getSingleton().getRenderTarget("Toy Formula");

	mSharedSceneMgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);

	//创建主摄像机
	mShareCamera = mSharedSceneMgr->createCamera("MainCamera");
	mShareCamera->setNearClipDistance(0.1f);

	//创建视口
	mGlobalViewPort = mGlobalWindow->addViewport(mShareCamera);
	mGlobalViewPort->setBackgroundColour(Ogre::ColourValue(0,0,0));

	//新状态
	StatesManager::getSingleton()->buildNewState(MenuState::getSingleton());
}

//---------------------------------------------------------------------------------------------------------

void GlobalManager::globalLoop()
{
	//TODO 整个游戏每一帧运行的时候调用

}

//---------------------------------------------------------------------------------------------------------

void GlobalManager::globalRelease()
{
	//TODO 整个游戏释放时调用

}