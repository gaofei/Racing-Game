#ifndef __GLOBALMANAGER__H__
#define __GLOBALMANAGER__H__

/*
全局管理器
包含用于全局要执行的方法
以及用于全局的变量
*/

class GlobalManager
{
public:
	static void globalInit();
	static void globalLoop();
	static void globalRelease();

public:
	static Ogre::RenderWindow*		mGlobalWindow;
	static Ogre::SceneManager*		mSharedSceneMgr;
	static Ogre::Camera*			mShareCamera;
	static Ogre::Viewport*			mGlobalViewPort;
};

#endif	//__GLOBALMANAGER__H__