#ifndef  __MENUSTATE__H__
#define  __MENUSTATE__H__

#include "State.h"

/*
Ö÷²Ëµ¥×´Ì¬Àà
*/

class MenuState : public State
{
public:

	MenuState();

	static MenuState* getSingleton()
	{
		if (!mMenuState)
		{
			mMenuState = new MenuState();
		}
		return mMenuState;
	}

	void init();

	void loop();

	void exit();

	bool mouseMoved( const OIS::MouseEvent &arg );

	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	void createAnimationTrack();

	void setGUI();

	void processTrackFrame();

protected:

	//Flash Control Event

	Hikari::FlashValue OnStartGame(Hikari::FlashControl* caller, const Hikari::Arguments& arg);

	Hikari::FlashValue OnExitGame(Hikari::FlashControl* caller, const Hikari::Arguments& arg);

public:

	Ogre::SceneManager*			mSceneMgr;

	Ogre::Camera*				mCamera;

	Ogre::Viewport*				mViewPort;

	Ogre::AnimationState*		mAnim;

	Ogre::SceneNode*			mCameraTrackNode;

	Hikari::FlashControl*		mMenuOverlay;


	static MenuState*			mMenuState;
};

#endif