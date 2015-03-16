#ifndef __GUIMANAGER__H__
#define __GUIMANAGER__H__

/*
GUI管理器
负责GUI初始化,输入管理
*/


class GUIManager
{
protected:
	GUIManager();
	~GUIManager();

public:
	static GUIManager* getSingleton();

public:

	void Initialize();
	void Loop(float timeElapsed);
	void Destory();

	//注入程序的鼠标和键盘事件
	bool injectMouseMoved( const OIS::MouseEvent &e );
	bool injectMousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
	bool injectMouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

	bool injectKeyPressed( const OIS::KeyEvent &e );
	bool injectKeyReleased( const OIS::KeyEvent &e );

public:

	Hikari::HikariManager* getHikariManger();

protected:

	//从OIS转换到Hikari的鼠标键值
	Hikari::MouseButtonID convertButton(OIS::MouseButtonID buttonID);


protected:

	Hikari::HikariManager*	mHikariMgr;

	static GUIManager*		mGUIManager;
};


#endif //__GUIMANAGER__H__