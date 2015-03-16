#ifndef __GLOBALMANAGER__H__
#define __GLOBALMANAGER__H__

/*
ȫ�ֹ�����
��������ȫ��Ҫִ�еķ���
�Լ�����ȫ�ֵı���
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