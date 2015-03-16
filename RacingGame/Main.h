#ifndef __MAIN_H__
#define __MAIN_H__

#include "StatesManager.h"
#include "InputManager.h"


class Main: public Ogre::WindowEventListener
{
public:
	Main();
	~Main();
	bool setup();
	void go();
	void loop();

protected:
	void setupResources();
	void setupWindowEventListener();
	bool configure();
	void messagePump();
	void shutDown();

	virtual void windowClosed(Ogre::RenderWindow *  rw);

protected:

	Ogre::Root*						mRoot;

	Ogre::RenderWindow*				mWindow;

	Ogre::String					mResourcesCfg;

	Ogre::String					mPluginsCfg;

	Ogre::Real						mLastElapse;


	//OIS Input devices
	OIS::InputManager*				mInputManager;

	OIS::Mouse*						mMouse;

	OIS::Keyboard*					mKeyboard;

	StatesManager*					mStateManager;

	HWND							mHWND;

	//πÿ±’±Í÷æ
	bool mShutDown;

};

#endif