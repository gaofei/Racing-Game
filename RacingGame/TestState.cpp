#include "Head.h"
#include "TestState.h"

//#include "DotSceneLoader.h"

#include "Vehicle.h"

#include "AIManager.h"
#include "VehicleAI.h"

#include "UtilltyFunc.h"

TestState* TestState::m_pTestState = NULL;

TestState::TestState()
{
	car = NULL;
	isPause = true;
}

void TestState::init()
{
	mSceneMgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_EXTERIOR_REAL_FAR);
	//创建主摄像机
	mCamera = mSceneMgr->createCamera("MainCamera");
	mCamera->setNearClipDistance(0.1f);
	//创建视口
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	//设置mipmaps等级
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(30);
	Ogre::TextureManager::getSingleton().setPreferredFloatBitDepth(32);


	//mSceneMgr->setFog(FOG_LINEAR, Ogre::ColourValue(0.9f, 0.9f, 0.9f), 0.0f, 70.f, 300.f);

	//DotSceneLoader* dsl = new DotSceneLoader;
	//dsl->parseDotScene("scene_fangjian.scene", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mSceneMgr);
	//delete dsl;


	UserAllocator *gAllocator = new UserAllocator;
	mPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, gAllocator);

	NxRemoteDebugger* rd = mPhysicsSDK->getFoundationSDK().getRemoteDebugger();
	rd->connect("localhost");

	loadSceneFile("scene_fangjian.xml", NXU::FT_XML);
	car = new Vehicle("car01", mSceneMgr, mWindow, mPhysicScene, mSceneMgr->getSceneNode("StartPoint")->getPosition() + Ogre::Vector3(0, 1, 0));

	car->getCameraNode()->attachObject(mCamera);
	mCamera->setAutoTracking(true, car->getCarNode());

	mSceneMgr->setAmbientLight( ColourValue( 0.8, 0.8, 0.8f ) );

// 	Ogre::Light* lg = mSceneMgr->createLight();
// 	lg->setPosition(0, 8, 0);
// 	lg->setType(Ogre::Light::LT_POINT);
	//mSceneMgr->setShadowUseInfiniteFarPlane(false);
	mSceneMgr->setShadowTechnique( Ogre::SHADOWTYPE_TEXTURE_MODULATIVE );
	mSceneMgr->setShadowCameraSetup(ShadowCameraSetupPtr(new FocusedShadowCameraSetup()));
	//mSceneMgr->setShadowUseInfiniteFarPlane(false);
	mSceneMgr->setShadowFarDistance(1000);
	//mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
	//myai = new VehicleAI;
	//for (int i=1;i<20;i++)
	//{
	//	char ichar[3];
	//	sprintf(ichar, "%d", i);
	//	if (i < 10)
	//		myai->addNavigationPoint(mSceneMgr->getSceneNode("Box0" + Ogre::String(ichar))->getPosition());
	//	else
	//		myai->addNavigationPoint(mSceneMgr->getSceneNode("Box" + Ogre::String(ichar))->getPosition());
	//}

	//myai->bindPredator(car->getCarNode());
	//AIManager::getSingleton()->addAIObject(myai);
}

void TestState::loop()
{
	Ogre::Real elapse = UtilityFunc::getElapseLastFrame();
	car->updata(elapse);
	//AIManager::getSingleton()->run();

	if (!isPause)
	{
		mPhysicScene->simulate(elapse);
		mPhysicScene->flushStream();
		mPhysicScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	}

	//if (myai->getVehicleDirection() == VehicleAI::LEFT)
	//{
	//	car->turnLeft(true);
	//	car->turnRight(false);
	//}
	//else if (myai->getVehicleDirection() == VehicleAI::RIGHT)
	//{
	//	car->turnLeft(false);
	//	car->turnRight(true);
	//}
	//else
	//{
	//	car->turnLeft(false);
	//	car->turnRight(false);
	//}
}