#include "Head.h"
#include "Main.h"

#include "GUIManager.h"
#include "LoThreadRes.h"
#include "GlobalManager.h"
#include "UtilltyFunc.h"

//-------------------------------------------------------------------------------------

Main::Main():mRoot(0),mWindow(0),mInputManager(0),mMouse(0),
			mKeyboard(0),mStateManager(0),
			mShutDown(false),mLastElapse(0),mHWND(0)
{

}

//-------------------------------------------------------------------------------------

Main::~Main()
{
	GlobalManager::globalRelease();

	if (mStateManager)
	{
		delete mStateManager;
		mStateManager = 0;
	}

	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);

	if( mInputManager )
	{
		mInputManager->destroyInputObject( mMouse );
		mInputManager->destroyInputObject( mKeyboard );

		OIS::InputManager::destroyInputSystem(mInputManager);
		mInputManager = 0;
	}
}

//-------------------------------------------------------------------------------------
bool Main::setup()
{
	mRoot = new Ogre::Root(mPluginsCfg);

	bool carryOn = configure();
	if (!carryOn) return false;

	//创建状态管理器
	mStateManager = new StatesManager(mWindow);
	//游戏全局初始化
	GlobalManager::globalInit();
	//初始化GUIManger
	GUIManager::getSingleton()->Initialize();

	//读取资源
	setupResources();
	//初始化资源
	if (true)
		LoThreadRes::getSingleton()->loadAllResGroup(1, mWindow, GlobalManager::mSharedSceneMgr);
	else
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		OgreMax::OgreMaxScene oms;
		oms.Load("scene_fangjian.scene", mWindow, OgreMax::OgreMaxScene::NO_OPTIONS, 
			GlobalManager::mSharedSceneMgr, GlobalManager::mSharedSceneMgr->getRootSceneNode());
	}

	//创建窗口监听
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
	//初始化InputManager
	InputManager::Initialize(mWindow, mStateManager);

	return true;
}

//-------------------------------------------------------------------------------------

void Main::go()
{
//#ifdef _DEBUG
//	mResourcesCfg = "res_d.cfg";
//	mPluginsCfg = "plugins_d.cfg";
//#else
	mResourcesCfg = "res.cfg";
	mPluginsCfg = "plugins.cfg";
//#endif

	//程序初始化
	if (!setup())
		return;

	//游戏循环
	loop();

	//游戏结束清理
	//clear();
}

//-------------------------------------------------------------------------------------

void Main::loop()
{//游戏主循环

	//先初始化timer
	mRoot->getTimer()->reset();

	while (!mShutDown)
	{
		UtilityFunc::elapseTick();

		//输入捕捉
		InputManager::getSingleton()->capture();

		//注入GUI循环
		GUIManager::getSingleton()->Loop(UtilityFunc::getElapseLastFrame());

		//状态机运作
		mShutDown = !mStateManager->run();

		//全局处理
		GlobalManager::globalLoop();

		//渲染
		mRoot->renderOneFrame(UtilityFunc::getElapseLastFrame());

		//Window消息回调
		messagePump();
	}

}

//-------------------------------------------------------------------------------------

void Main::setupResources()
{
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}

//-------------------------------------------------------------------------------------

bool Main::configure()
{
	if(mRoot->restoreConfig())
	{
		mWindow = mRoot->initialise(true, "Toy Formula");
		mWindow->getCustomAttribute("WINDOW", (void*)&mHWND);
//
//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//		HWND hwnd;
//		mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
//		LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON) );
//		SetClassLong( hwnd, GCL_HICON, iconID );
//#endif
		return true;
	}
	else
	{
		return false;
	}
}

//-------------------------------------------------------------------------------------

void Main::messagePump()
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		if(msg.message == WM_KEYDOWN)
		{
			//此处写处理代码
		}

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

//-------------------------------------------------------------------------------------

void Main::shutDown()
{
	mShutDown = true;
}

//-------------------------------------------------------------------------------------

void Main::windowClosed(Ogre::RenderWindow *  rw)
{
	shutDown();
}

//-------------------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{	
		Main gamemain;
		//try {
			gamemain.go();
		//} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			//MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		//}

		return 0;
	}

#ifdef __cplusplus
}
#endif
