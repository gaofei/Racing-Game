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

	//����OISInputSytstem
	void createInputSystem();

	//�������ͼ���
	void createInputObject();

	//���ûص�����
	void setEventCallback();

	//�������ü���Χ
	void setMouseClip();

public:

	//��������
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