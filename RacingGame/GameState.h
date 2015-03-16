#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include "State.h"

#include "NxStream/NXU_helper.h"
#include "UserAllocator.h"

#include "Vehicle.h"

#include "AIManager.h"
#include "VehicleAI.h"

#include "VolumetricSunLightFrameListener.h"

/*
��Ϸ״̬��
*/
class GameState: public State
{

public:
	GameState();
	~GameState(){}

	static GameState* getSingleton()
	{
		if (!mGameState)
		{
			mGameState = new GameState();
		}
		return mGameState;
	}

	/*
	���غ���
	*/
	virtual void init();
	virtual void loop();
	virtual void exit();

	/*
	OIS
	*/
	virtual bool mouseMoved( const OIS::MouseEvent &e );
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	void createRenderScene();
	void createPhysicScene();

	bool loadSceneFile(Ogre::String filename,NXU::NXU_FileType type);
	void loadPhyScene();
	void setUpQuaternion();

	void cheat(Vehicle* ve, VehicleAI* veai, float id, bool isAi = true);

	void raceLogic(Vehicle* ve, VehicleAI* veai, int id);

protected:

	/*
	Ogre����
	*/

	Ogre::Camera*					mCamera;

	Ogre::SceneManager*				mSceneMgr;


protected:

	/*
	Physx����
	*/

	NxPhysicsSDK*		mPhysicsSDK;

	NxScene*			mPhysicScene;

	UserAllocator*		mUserAlloc;



protected:

	/*
	������
	*/

	VolumetricSunLightFrameListener * mEffectListener;

	Vehicle*						car;
	Vehicle*						car2;
	Vehicle*						car3;
	Vehicle*						car4;
	VehicleAI*						vai1;
	VehicleAI*						vai2;
	VehicleAI*						vai3;
	VehicleAI*						vai4;

	std::vector<int>				cur;
	std::vector<float>				times;
	std::vector<float>				timeReach;
	std::vector<bool>				win;

protected:

	/*
	����
	*/

	Ogre::Quaternion				mStdQuaternion;

	Ogre::Real						mElapseTime;

	Ogre::Real						mWinTime;

	wchar_t*						mChar;

protected:

	/*
	��־
	*/
	bool							mIsPause;
	bool							mIsStart;
	bool							mIsBackMenu;

public:

	static GameState*				mGameState;
	Ogre::SceneNode*				mTempSceneNode;

};

#endif
