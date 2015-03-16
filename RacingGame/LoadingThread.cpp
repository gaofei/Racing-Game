#include "Head.h"
#include "LoadingThread.h"

#include "UtilltyFunc.h"

bool LoadingThread::isEnd = true;

bool LoadingThread::isReturn = false;

DWORD LoadingThread::mThreadID = 0;

CEGUI::AnimationInstance* LoadingThread::ai = 0;

CEGUI::Window* LoadingThread::mSheet = 0;

CEGUI::Window* LoadingThread::mNullSheet = 0;

Ogre::SceneManager* LoadingThread::mNullScene = 0;

Ogre::RenderTarget* LoadingThread::mWindow = 0;

Ogre::Camera* LoadingThread::mCamera = 0;

Ogre::Camera* LoadingThread::mCurCamera = 0;


void LoadingThread::Begin()
{
	isEnd = false;
	UtilityFunc::getElapseLastFrame();

	static bool isFirst = true;
	if (isFirst)
	{
		isFirst = false;
		mNullScene  = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "LoadingThread_NullScene");
		mWindow		= Ogre::Root::getSingleton().getRenderTarget("Racing Game");

		Ogre::Viewport* vp;
		vp			= mWindow->getViewport(0);
		mCamera		= mNullScene->createCamera("LoadingThread_NullCamera");
		mCurCamera  = vp->getCamera();
		
		mCamera->setAspectRatio(vp->getActualWidth() / vp->getActualHeight());
		vp->setCamera(mCamera);

		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
		try
		{
			mSheet = wmgr.loadWindowLayout("loading.layout");
			mNullSheet = wmgr.loadWindowLayout("nullWindow.layout");
		}
		catch (CEGUI::Exception &e)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, Ogre::String(e.getMessage().c_str()), "Error Parsing Menu");
		}
		/*wmgr.createWindow("DefaultWindow", "root");*/
		CEGUI::System::getSingleton().setGUISheet(mSheet);

		CEGUI::AnimationManager::getSingleton().loadAnimationsFromXML("rotationZ.xml");
		for (int i=0;i<8;i++)
		{
			char winChar[64];
			sprintf(winChar, "Loading/Point%d", i);

			CEGUI::AnimationInstance* ai = 
				CEGUI::AnimationManager::getSingleton().instantiateAnimation("Rotation");
			CEGUI::Window* w = CEGUI::WindowManager::getSingleton().getWindow(winChar);
			ai->setTarget(w);
			ai->start();
		}

		ai = CEGUI::AnimationManager::getSingleton().instantiateAnimation("RotationNag");
		CEGUI::Window* w = CEGUI::WindowManager::getSingleton().getWindow("Loading/Center_0");
		ai->setTarget(w);
		ai->start();

		//set up null sheet
	}
	else
	{
		mCurCamera = mWindow->getViewport(0)->getCamera();
		mWindow->getViewport(0)->setCamera(mCamera);
		CEGUI::System::getSingleton().setGUISheet(mSheet);
	}

	isReturn = false;
	HANDLE hTh = CreateThread(0, 0, &LoadingThread::Render, (LPVOID)0, 0, &mThreadID);
	CloseHandle(hTh);
}

void LoadingThread::End()
{
	isEnd = true;
	//while(!isReturn);
	CEGUI::System::getSingleton().setGUISheet(mNullSheet);
	mWindow->getViewport(0)->setCamera(mCurCamera);
}

DWORD WINAPI LoadingThread::Render(LPVOID arg)
{
	while(!isEnd)
	{
		CEGUI::System::getSingleton().injectTimePulse(UtilityFunc::getElapseLastFrame());

		static int index = 1;
		if(index < 7 && ai->getPosition() > 0.25)
		{
			ai = CEGUI::AnimationManager::getSingleton().instantiateAnimation("RotationNag");

			char winChar[64];
			sprintf(winChar, "Loading/Center_%d", index);
			CEGUI::Window* w = CEGUI::WindowManager::getSingleton().getWindow(winChar);
			ai->setTarget(w);
			ai->start();
			index++;
		}

		Ogre::Root::getSingleton().renderOneFrame();
	}
	isReturn = true;
	return 1;
}