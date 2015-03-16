#ifndef __LOADING_THREAD__H__
#define __LOADING_THREAD__H__

/*
资源读取类
用于读取大量数据
并且能同时显示进度圈
*/

class LoadingThread
{

public:

	static void Begin();

	static void End();

protected:

	static DWORD WINAPI Render(LPVOID arg);

protected:

	static bool isEnd;

	static bool isReturn;

	static DWORD mThreadID;

	static CEGUI::AnimationInstance* ai;

	static CEGUI::Window* mSheet;

	static CEGUI::Window* mNullSheet;

	static Ogre::SceneManager* mNullScene;

	static Ogre::RenderTarget* mWindow;

	static Ogre::Camera* mCamera;

	static Ogre::Camera* mCurCamera;

};

#endif