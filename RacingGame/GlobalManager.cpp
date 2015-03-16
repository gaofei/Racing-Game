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
	//TODO ������Ϸ��ʼ����ʱ�����

	mGlobalWindow = (Ogre::RenderWindow*)Ogre::Root::getSingleton().getRenderTarget("Toy Formula");

	mSharedSceneMgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);

	//�����������
	mShareCamera = mSharedSceneMgr->createCamera("MainCamera");
	mShareCamera->setNearClipDistance(0.1f);

	//�����ӿ�
	mGlobalViewPort = mGlobalWindow->addViewport(mShareCamera);
	mGlobalViewPort->setBackgroundColour(Ogre::ColourValue(0,0,0));

	//��״̬
	StatesManager::getSingleton()->buildNewState(MenuState::getSingleton());
}

//---------------------------------------------------------------------------------------------------------

void GlobalManager::globalLoop()
{
	//TODO ������Ϸÿһ֡���е�ʱ�����

}

//---------------------------------------------------------------------------------------------------------

void GlobalManager::globalRelease()
{
	//TODO ������Ϸ�ͷ�ʱ����

}