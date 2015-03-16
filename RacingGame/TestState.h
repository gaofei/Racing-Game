#ifndef __TESTSTATE__H__

#include "State.h"

#include "NxStream/NXU_helper.h"
#include "UserAllocator.h"

#include "Vehicle.h"
#include "VehicleAI.h"

class TestState : public State
{

public:
	static TestState* getSingleton()
	{
		if (!m_pTestState)
		{
			m_pTestState = new TestState();
		}
		return m_pTestState;
	}

	TestState();

	void init();

	void loop();

	void exit()
	{

	}

	bool mouseMoved( const OIS::MouseEvent &e )
	{
		//CEGUI::System::getSingleton().injectMouseMove(e.state.X.rel, e.state.Y.rel);
		if(car)
			car->injectMouseMoved(e);
		return true;
	}

	//---------------------------------------------------------------------------------------------------------

	bool keyPressed( const OIS::KeyEvent &arg )
	{
		if(car)
			car->injectKeyPressed(arg);
		if(arg.key == OIS::KC_P)
			isPause = !isPause;

		return true;
	}

	//---------------------------------------------------------------------------------------------------------

	bool keyReleased( const OIS::KeyEvent &arg )
	{
		if(car)
			car->injectKeyReleased(arg);
		return true;
	}

	//---------------------------------------------------------------------------------------------------------

	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if(car)
			car->injectMousePressed(arg, id);
		return true;
	}

	//---------------------------------------------------------------------------------------------------------


	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if(car)
			car->injectMouseReleased(arg, id);
		return true;
	}


	bool loadSceneFile(Ogre::String filename,NXU::NXU_FileType type)
	{
		bool success = false;

		if (mPhysicsSDK)
		{
			Ogre::String totalname;

			//获得完整文件路径
			Ogre::FileInfoListPtr svp = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("Popular", filename);
			totalname = svp->back().archive->getName() + "/" + filename;
			Ogre::StringVector sv = Ogre::StringUtil::split(totalname, "/");
			totalname = sv[0];
			for (unsigned int i = 1;i<sv.size();i++)
				totalname += "\\\\" + sv[i];

			//读取场景
			NXU::NxuPhysicsCollection *c = NXU::loadCollection(totalname.c_str(), type );
			if ( c )
			{
				if (mPhysicScene)
				{
					mPhysicsSDK->releaseScene(*mPhysicScene);
				}

				if (mPhysicsSDK)
				{
					success = NXU::instantiateCollection( c, *mPhysicsSDK, 0, 0, 0);
					mPhysicScene = mPhysicsSDK->getScene(0);
				}
				NXU::releaseCollection(c);
			}
			else
			{
			}
		}

		return success;
	}
public:
	static TestState* m_pTestState;

	Ogre::Real mLastElapse;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;

	Vehicle* car;

	NxPhysicsSDK*		mPhysicsSDK;

	NxScene*			mPhysicScene;

	UserAllocator*		mUserAlloc;

	VehicleAI* myai;

	bool isPause;

};

#define __TESTSTATE__H__
#endif //__TESTSTATE__H__