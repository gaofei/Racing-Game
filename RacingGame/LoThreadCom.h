#ifndef __LOTHREADCOM__H__
#define __LOTHREADCOM__H__

#include "GUIManager.h"
#include "GlobalManager.h"

template<class T>
class Delegate;

/*
ͨ�ö��߳����ݶ�ȡ
ֻ�ܼ���Ogre���������
���������̲߳�ͬ��
ÿ��ʹ�ã��ػ���ʼ��������̬����
*/

template<class T>
class LoThreadCom
{

public:

	static void startLoading(Delegate<T> del)
	{
		mIsReturn = false;
		mLoadingDeleg = &del;

		Hikari::FlashControl* fc =
		GUIManager::getSingleton()->getHikariManger()
		->createFlashOverlay("LoadingCircle", GlobalManager::mGlobalWindow->getViewport(0), 
		1280, 800, Hikari::Center);
		fc->load("smoke.swf");
		fc->setDraggable(false);

		HANDLE hTh = CreateThread(NULL, 0, &Loading, (LPVOID)0, 0, &mThreadId);
		CloseHandle(hTh);

		MSG msg;
		while (!mIsReturn && PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			GUIManager::getSingleton()->Loop(0);
			Ogre::Root::getSingleton().renderOneFrame();
		}

		GUIManager::getSingleton()->getHikariManger()->destroyFlashControl(fc);
	}

protected:

	static DWORD WINAPI Loading(LPVOID arg)
	{
			mLoadingDeleg->invoke();
			mIsReturn = true;
			return 1;
	}

public:

	static Delegate<T>*	mLoadingDeleg;
	static DWORD		mThreadId;
	static bool		mIsReturn;
};

#ifndef INITSTATIC
#define INITSTATIC

#endif

/*
��ģ��ί��
*/

template<class T>
class Delegate
{
	typedef void (T::*func)(void);

public:

	Delegate(T* obj, func inFunc)
	{
		mObj	= obj;
		mFunc	= inFunc;
	}

	void invoke()
	{
		(mObj->*mFunc)();
	}

protected:

	T*		mObj;
	func	mFunc;
};

#endif //__LOTHREADCOM__H__