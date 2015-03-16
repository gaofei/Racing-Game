#ifndef __INPUTMANAGER__H
#define __INPUTMANAGER__H

#include "StatesManager.h"

class InputManager: public OIS::KeyListener, public OIS::MouseListener
{

public:
	~InputManager();

private:
	InputManager(Ogre::RenderWindow* win, StatesManager* sm);

public:

	static void Initialize(Ogre::RenderWindow* win, StatesManager* sm);

	static InputManager* getSingleton();

private:

	//创建OISInputSytstem
	void createInputSystem();

	//创建鼠标和键盘
	void createInputObject();

	//设置回调函数
	void setEventCallback();

	//设置鼠标裁剪范围
	void setMouseClip();

public:

	//捕获输入
	void capture();

	// OIS::MouseListener
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//OIS::KeyListener
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );

private:

	Ogre::RenderWindow*			mWindow;

public:

	OIS::InputManager*			mOISInputManager;

	OIS::Mouse*					mMouse;

	OIS::Keyboard*				mKeyboard;

	StatesManager*				mStateManager;

	static InputManager*		mInputManager;

};

#endif