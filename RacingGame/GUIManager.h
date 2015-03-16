#ifndef __GUIMANAGER__H__
#define __GUIMANAGER__H__

/*
GUI������
����GUI��ʼ��,�������
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

	//ע���������ͼ����¼�
	bool injectMouseMoved( const OIS::MouseEvent &e );
	bool injectMousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
	bool injectMouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

	bool injectKeyPressed( const OIS::KeyEvent &e );
	bool injectKeyReleased( const OIS::KeyEvent &e );

public:

	Hikari::HikariManager* getHikariManger();

protected:

	//��OISת����Hikari������ֵ
	Hikari::MouseButtonID convertButton(OIS::MouseButtonID buttonID);


protected:

	Hikari::HikariManager*	mHikariMgr;

	static GUIManager*		mGUIManager;
};


#endif //__GUIMANAGER__H__