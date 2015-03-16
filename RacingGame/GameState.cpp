#include "Head.h"
#include "GameState.h"

//-----

#include "GlobalManager.h"
#include "InputManager.h"

//#include "LoadingThread.h"
#include "CompositorLogic.h"

//#include "GameConsole.h"
#include "UtilltyFunc.h"
#include "LoThreadCom.h"

class GlowMaterialListener : public Ogre::MaterialManager::Listener
{
protected:
	Ogre::MaterialPtr mBlackMat;
public:
	GlowMaterialListener()
	{
		mBlackMat = Ogre::MaterialManager::getSingleton().create("mGlowBlack", "Internal");
		mBlackMat->getTechnique(0)->getPass(0)->setDiffuse(0.0,0.0,0.0,0.0);
		mBlackMat->getTechnique(0)->getPass(0)->setSpecular(0.0,0.0,0.0,0.0);
		mBlackMat->getTechnique(0)->getPass(0)->setAmbient(0.0,0.0,0.0);
		mBlackMat->getTechnique(0)->getPass(0)->setSelfIllumination(0.0,0.0,0.0);
	}

	Ogre::Technique *handleSchemeNotFound(unsigned short, const Ogre::String& schemeName, Ogre::Material*mat, unsigned short, const Ogre::Renderable*)
	{
		if (schemeName == "glow")
		{
			//LogManager::getSingleton().logMessage(">> adding glow to material: "+mat->getName());
			return mBlackMat->getTechnique(0);
		}
		return NULL;
	}
};

//-----

//---------------------------------------GameState--------------------------------------------------------
//--------------------------------------------------------------------------------------------------------

GameState* GameState::mGameState = 0;

//---------------------------------------------------------------------------------------------------------

GameState::GameState()
{

	mCamera			= NULL;
	mSceneMgr		= NULL;

	mPhysicsSDK		= NULL;
	mPhysicScene	= NULL;
	mUserAlloc		= NULL;
	car				= NULL;
	car2			= NULL;
	car3			= NULL;
	car4			= NULL;
	mChar			= NULL;

	mElapseTime		= 0.0f;
	mIsStart		= false;
	mWinTime		= 0.0f;

	mIsPause = true;
	mIsBackMenu = false;

}

//---------------------------------------------------------------------------------------------------------

void GameState::init()
{

	createRenderScene();

	setUpQuaternion();

	//LoadingThread::Begin();
	createPhysicScene();
	//LoadingThread::End();
	

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8f, 0.8f, 0.8f));

	vai2 = new VehicleAI;
	vai2->bindPredator(car2->getCarNode());
	vai2->setPredatorMode(VehicleAI::DISTANCE);
	for (int i=1;i<21;i++)
	{
		char* ich = new char[3];
		sprintf(ich, "%d", i);
		vai2->addNavigationPoint(mSceneMgr->getSceneNode("R1P" + Ogre::String(ich))->getPosition());
	}
	

	vai1 = new VehicleAI;
	vai3 = new VehicleAI;
	vai4 = new VehicleAI;

	*vai1 = *vai2;
	*vai3 = *vai2;
	*vai4 = *vai2;

	vai3->setDistancePrecision(1.8f);
	vai4->setDistancePrecision(1.8f);

	vai1->bindPredator(car->getCarNode());
	vai3->bindPredator(car3->getCarNode());
	vai4->bindPredator(car4->getCarNode());

	//vai3->setPredatorMode(VehicleAI::DEGREE);

	AIManager::getSingleton()->addAIObject(vai1);
	AIManager::getSingleton()->addAIObject(vai2);
	AIManager::getSingleton()->addAIObject(vai3);
	AIManager::getSingleton()->addAIObject(vai4);

	mTempSceneNode = mSceneMgr->createSceneNode();

	srand((unsigned int)time(NULL));
	for (int i=0;i<4;i++)
	{
		float ranf = (float)(rand()%10 + 10);
		times.push_back(0.0f);
		timeReach.push_back(ranf);
		cur.push_back(0);
		win.push_back(false);
	}

	mChar = new wchar_t[256];



	
	//particle
	Ogre::ParticleSystem* Lizi_qidian1 = mSceneMgr->createParticleSystem("Lizi_qidian1","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_qidian1")->createChildSceneNode()->attachObject(Lizi_qidian1);
	Ogre::ParticleSystem* Lizi_qidian2 = mSceneMgr->createParticleSystem("Lizi_qidian2","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_qidian2")->createChildSceneNode()->attachObject(Lizi_qidian2);
	Ogre::ParticleSystem* Lizi_qidian3 = mSceneMgr->createParticleSystem("Lizi_qidian3","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_qidian3")->createChildSceneNode()->attachObject(Lizi_qidian3);
	Ogre::ParticleSystem* Lizi_qidian4 = mSceneMgr->createParticleSystem("Lizi_qidian4","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_qidian4")->createChildSceneNode()->attachObject(Lizi_qidian4);
	Ogre::ParticleSystem* Lizi_qidian5 = mSceneMgr->createParticleSystem("Lizi_qidian5","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_qidian5")->createChildSceneNode()->attachObject(Lizi_qidian5);
	Ogre::ParticleSystem* Lizi_qidian6 = mSceneMgr->createParticleSystem("Lizi_qidian6","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_qidian6")->createChildSceneNode()->attachObject(Lizi_qidian6);
	Ogre::ParticleSystem* Lizi_qidian7 = mSceneMgr->createParticleSystem("Lizi_qidian7","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_qidian7")->createChildSceneNode()->attachObject(Lizi_qidian7);
	Ogre::ParticleSystem* Lizi_qidian8 = mSceneMgr->createParticleSystem("Lizi_qidian8","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_qidian8")->createChildSceneNode()->attachObject(Lizi_qidian8);

	Ogre::ParticleSystem* Lizi_zhongdian1 = mSceneMgr->createParticleSystem("Lizi_zhongdian1","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_zhongdian1")->createChildSceneNode()->attachObject(Lizi_zhongdian1);
	Ogre::ParticleSystem* Lizi_zhongdian2 = mSceneMgr->createParticleSystem("Lizi_zhongdian2","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_zhongdian2")->createChildSceneNode()->attachObject(Lizi_zhongdian2);
	Ogre::ParticleSystem* Lizi_zhongdian3 = mSceneMgr->createParticleSystem("Lizi_zhongdian3","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_zhongdian3")->createChildSceneNode()->attachObject(Lizi_zhongdian3);
	Ogre::ParticleSystem* Lizi_zhongdian4 = mSceneMgr->createParticleSystem("Lizi_zhongdian4","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_zhongdian4")->createChildSceneNode()->attachObject(Lizi_zhongdian4);
	Ogre::ParticleSystem* Lizi_zhongdian5 = mSceneMgr->createParticleSystem("Lizi_zhongdian5","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_zhongdian5")->createChildSceneNode()->attachObject(Lizi_zhongdian5);
	Ogre::ParticleSystem* Lizi_zhongdian6 = mSceneMgr->createParticleSystem("Lizi_zhongdian6","PurpleFountain");
	mSceneMgr->getSceneNode("Lizi_zhongdian6")->createChildSceneNode()->attachObject(Lizi_zhongdian6);

	Ogre::ParticleSystem* Lizi_luzhong1 = mSceneMgr->createParticleSystem("Lizi_luzhong1","Pentagram");
	mSceneMgr->getSceneNode("Lizi_luzhong1")->createChildSceneNode()->attachObject(Lizi_luzhong1);
	Ogre::ParticleSystem* Lizi_luzhong2 = mSceneMgr->createParticleSystem("Lizi_luzhong2","Pentagram");
	mSceneMgr->getSceneNode("Lizi_luzhong2")->createChildSceneNode()->attachObject(Lizi_luzhong2);

	Ogre::ParticleSystem* Lizi_luzhong5 = mSceneMgr->createParticleSystem("Lizi_luzhong5","Pentagram");
	mSceneMgr->getSceneNode("Lizi_luzhong5")->createChildSceneNode()->attachObject(Lizi_luzhong5);
	Ogre::ParticleSystem* Lizi_luzhong6 = mSceneMgr->createParticleSystem("Lizi_luzhong6","Pentagram");
	mSceneMgr->getSceneNode("Lizi_luzhong6")->createChildSceneNode()->attachObject(Lizi_luzhong6);

	Ogre::ParticleSystem* dust1 = mSceneMgr->createParticleSystem("dust1_0","GBoot/particle/dusttrail");
	Ogre::ParticleSystem* dust2 = mSceneMgr->createParticleSystem("dust2_0","GBoot/particle/dusttrail");
	car->mWheels[Vehicle::Bottom_LEFT].mSceneNode->attachObject(dust1);
	car->mWheels[Vehicle::Bottom_RIGHT].mSceneNode->attachObject(dust2);
	dust1->setEmitting(false);
	dust2->setEmitting(false);

	dust1 = mSceneMgr->createParticleSystem("dust1_1","GBoot/particle/dusttrail");
	dust2 = mSceneMgr->createParticleSystem("dust2_1","GBoot/particle/dusttrail");
	car2->mWheels[Vehicle::Bottom_LEFT].mSceneNode->attachObject(dust1);
	car2->mWheels[Vehicle::Bottom_RIGHT].mSceneNode->attachObject(dust2);
	dust1->setEmitting(false);
	dust2->setEmitting(false);

	dust1 = mSceneMgr->createParticleSystem("dust1_2","GBoot/particle/dusttrail");
	dust2 = mSceneMgr->createParticleSystem("dust2_2","GBoot/particle/dusttrail");
	car3->mWheels[Vehicle::Bottom_LEFT].mSceneNode->attachObject(dust1);
	car3->mWheels[Vehicle::Bottom_RIGHT].mSceneNode->attachObject(dust2);
	dust1->setEmitting(false);
	dust2->setEmitting(false);

	dust1 = mSceneMgr->createParticleSystem("dust1_3","GBoot/particle/dusttrail");
	dust2 = mSceneMgr->createParticleSystem("dust2_3","GBoot/particle/dusttrail");
	car4->mWheels[Vehicle::Bottom_LEFT].mSceneNode->attachObject(dust1);
	car4->mWheels[Vehicle::Bottom_RIGHT].mSceneNode->attachObject(dust2);
	dust1->setEmitting(false);
	dust2->setEmitting(false);

	Ogre::ParticleSystem* flare = mSceneMgr->createParticleSystem("no2","no2");
	flare->setEmitting(false);
	Ogre::SceneNode* sn = car4->getCarNode()->createChildSceneNode();
	sn->setScale(0.002f, 0.002, 0.002f);
	sn->attachObject(flare);
}

//---------------------------------------------------------------------------------------------------------

void GameState::loop()
{
	Ogre::Real elapse = UtilityFunc::getElapseLastFrame();
	if (elapse > 0.1f) 
		elapse = 0.1f;

	AIManager::getSingleton()->run();

	cheat(car, vai1, 0);
	cheat(car2, vai2, 1);
	cheat(car3, vai3, 2);
	cheat(car4, vai4, 3, false);

	car->updata(elapse);
	car2->updata(elapse);
	car3->updata(elapse);
	car4->updata(elapse);

	raceLogic(car, vai1, 0);
	raceLogic(car2, vai2, 1);
	raceLogic(car3, vai3, 2);
	raceLogic(car4, vai4, 3);

	UtilityFunc::showFPS(mWindow);

	if (!mIsPause)
	{
		mPhysicScene->simulate(1.0f/6.0f);
		mPhysicScene->flushStream();
		mPhysicScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	}
}

//---------------------------------------------------------------------------------------------------------

void GameState::exit()
{

	if (mPhysicsSDK)
	{
		if (mPhysicScene)
		{
			mPhysicsSDK->releaseScene(*mPhysicScene);
			mPhysicScene = 0;
		}
		mPhysicsSDK->release();
		mPhysicsSDK = 0;
	}

	if (mCamera->isAttached())
		mCamera->detachFromParent();
}

//---------------------------------------------------------------------------------------------------------

bool GameState::mouseMoved( const OIS::MouseEvent &e )
{
	if(car4)
		car4->injectMouseMoved(e);
	return true;
}

//---------------------------------------------------------------------------------------------------------

bool GameState::keyPressed( const OIS::KeyEvent &arg )
{
	if(car4)
		car4->injectKeyPressed(arg);

	if (arg.key == OIS::KC_P)
		mIsPause = !mIsPause;

	if (arg.key == OIS::KC_F)
		mSceneMgr->getParticleSystem("no2")->setEmitting(true);

	return true;
}

//---------------------------------------------------------------------------------------------------------

bool GameState::keyReleased( const OIS::KeyEvent &arg )
{
	if(car4)
		car4->injectKeyReleased(arg);

	if (arg.key == OIS::KC_F)
		mSceneMgr->getParticleSystem("no2")->setEmitting(false);

	return true;
}

//---------------------------------------------------------------------------------------------------------

bool GameState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(!mIsBackMenu && car4)
		car4->injectMousePressed(arg, id);
	return true;
}

//---------------------------------------------------------------------------------------------------------


bool GameState::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(!mIsBackMenu && car4)
		car4->injectMouseReleased(arg, id);
	return true;
}

//---------------------------------------------------------------------------------------------------------

void GameState::createRenderScene()
{
	//获取场景管理器
	mSceneMgr = GlobalManager::mSharedSceneMgr;

	//获取主摄像机
	mCamera = GlobalManager::mShareCamera;

	CompositorManager::getSingleton().addCompositor(mCamera->getViewport(), "Glow");
	CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(), "Glow", true);
	GlowMaterialListener *gml = new GlowMaterialListener();
	Ogre::MaterialManager::getSingleton().addListener(gml);

	//Ogre::Light * pSunLight = mSceneMgr->createLight("SunLight");
	//pSunLight->setType(Light::LT_POINT);
	//pSunLight->setPosition(Ogre::Vector3(660, 10, 660));
	//pSunLight->setDiffuseColour(ColourValue::Black);
	//pSunLight->setCastShadows(false);

	//mSceneMgr->setSkyBox(true, "Ogre/Compositor/VolumetricSunLight/FakeSky");

	//Ogre::Viewport * pViewport = mCamera->getViewport();

	//Ogre::CompositorInstance * pEffectInstance = Ogre::CompositorManager::getSingleton().addCompositor(pViewport, "VolumetricSunLights", -1);		
	//Ogre::CompositorManager::getSingleton().setCompositorEnabled(pViewport, "VolumetricSunLights", true);


	//mEffectListener = new VolumetricSunLightFrameListener(mSceneMgr, mCamera, mWindow->getViewport(0));
	//pEffectInstance->addListener(mEffectListener);

	/// Motion blur effect
	Ogre::CompositorPtr comp3 = Ogre::CompositorManager::getSingleton().create(
		"MotionBlur", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
		);
	{
		Ogre::CompositionTechnique *t = comp3->createTechnique();
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("scene");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("sum");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("temp");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		/// Render scene
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
			tp->setOutputName("scene");
		}
		/// Initialisation pass for sum texture
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
			tp->setOutputName("sum");
			tp->setOnlyInitial(true);
		}
		/// Do the motion blur
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			tp->setOutputName("temp");
			{ Ogre::CompositionPass *pass = tp->createPass();
			pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("Ogre/Compositor/Combine");
			pass->setInput(0, "scene");
			pass->setInput(1, "sum");
			}
		}
		/// Copy back sum texture
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			tp->setOutputName("sum");
			{ Ogre::CompositionPass *pass = tp->createPass();
			pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("Ogre/Compositor/Copyback");
			pass->setInput(0, "temp");
			}
		}
		/// Display result
		{
			Ogre::CompositionTargetPass *tp = t->getOutputTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			{ Ogre::CompositionPass *pass = tp->createPass();
			pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("Ogre/Compositor/MotionBlur");
			pass->setInput(0, "sum");
			}
		}
	}

	Ogre::CompositorManager::getSingleton().addCompositor(mWindow->getViewport(0), "MotionBlur");
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(mWindow->getViewport(0), "MotionBlur", true);
}

//---------------------------------------------------------------------------------------------------------

//GameState的模板初始化
Delegate<GameState>*	LoThreadCom<GameState>::mLoadingDeleg = NULL;
DWORD		LoThreadCom<GameState>::mThreadId = 0;
bool		LoThreadCom<GameState>::mIsReturn = false;

void GameState::createPhysicScene()
{
	UserAllocator *gAllocator = new UserAllocator;
	mPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, gAllocator);

	//LoThreadCom<GameState>::startLoading(Delegate<GameState>(this, &GameState::loadPhyScene));

	loadPhyScene();
	NxRemoteDebugger* rd = mPhysicsSDK->getFoundationSDK().getRemoteDebugger();
	rd->connect("localhost");

	//创建汽车 
	car = new Vehicle("car01", mSceneMgr, mWindow, mPhysicScene, mSceneMgr->getSceneNode("R1PS")->getPosition() + Ogre::Vector3(0, 1, 0),
		mStdQuaternion);

	car2 = new Vehicle("car02", mSceneMgr, mWindow, mPhysicScene, mSceneMgr->getSceneNode("R1PS")->getPosition() + Ogre::Vector3(0, 1, 0) + mStdQuaternion.xAxis()*2,
		mStdQuaternion);
	
	car3 = new Vehicle("car03", mSceneMgr, mWindow, mPhysicScene, mSceneMgr->getSceneNode("R1PS")->getPosition() + Ogre::Vector3(0, 1, 0) + mStdQuaternion.xAxis()*4,
		mStdQuaternion);

	car4 = new Vehicle("car04", mSceneMgr, mWindow, mPhysicScene, mSceneMgr->getSceneNode("R1PS")->getPosition() + Ogre::Vector3(0, 1, 0) - mStdQuaternion.xAxis()*2,
		mStdQuaternion);

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8f, 0.8f, 0.8f));

	mCamera->detachFromParent();
	car4->getCameraNode()->attachObject(mCamera);
	mCamera->setAutoTracking(true, car4->getCarNode());
}

//---------------------------------------------------------------------------------------------------------

bool GameState::loadSceneFile(Ogre::String filename,NXU::NXU_FileType type)
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
//---------------------------------------------------------------------------------------------------------

void GameState::loadPhyScene()
{
	loadSceneFile("scene_fangjian.xml", NXU::FT_XML);
}

//---------------------------------------------------------------------------------------------------------

void GameState::setUpQuaternion()
{
	//设置跑道标准开始方向
	Ogre::SceneNode* sn = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre::Vector3 dir = mSceneMgr->getSceneNode("R1PS")->_getDerivedPosition() - mSceneMgr->getSceneNode("R1P1")->_getDerivedPosition();
	sn->setDirection(dir);
	mStdQuaternion = sn->getOrientation();
}

//---------------------------------------------------------------------------------------------------------

void GameState::cheat(Vehicle* ve, VehicleAI* veai, float id, bool isAi)
{
	//时间实在不足，多次优化车性能未果，只能让电脑不大好看的作弊方式前进


	//NxVec3 dir = NxVec3(veai->m_cDirect.x, 0, veai->m_cDirect.y);
	//ve->getActor()->addForce(dir*50, NX_IMPULSE);

	//Ogre::Vector3 dirOgre = ve->getCarNode()->getOrientation().zAxis();
	//dirOgre.x = dir.x;
	//dirOgre.z = dir.z;
	//mTempSceneNode->setOrientation(Ogre::Quaternion::IDENTITY);
	//mTempSceneNode->setDirection(dirOgre);
	//Ogre::Quaternion quat = mTempSceneNode->getOrientation();
	//NxQuat nxquat = UtilityFunc::OgreQuat_To_NxQuat(quat);
	//ve->getActor()->setGlobalOrientationQuat(nxquat);

	int lasti = veai->getNavigationCurrent();
	lasti = lasti-1 > 0 ? lasti-1 : 0;

	Ogre::Vector3 oriVec3(veai->getNavigationPoint(lasti));
	oriVec3.y += 2;

	ve->setOriginalPos(oriVec3);

	mTempSceneNode->setOrientation(Ogre::Quaternion::IDENTITY);

	Ogre::Vector3 dir(veai->m_cDirect.x, 0, veai->m_cDirect.y);
	mTempSceneNode->setDirection(-dir);

	ve->setOriginalQuat(mTempSceneNode->getOrientation());

	float speed = ve->getSpeed();

	if (speed >= 5.0f)
	{
		mSceneMgr->getParticleSystem("dust1_" + Ogre::StringConverter::toString(id))->setEmitting(true);
		mSceneMgr->getParticleSystem("dust2_"+ Ogre::StringConverter::toString(id))->setEmitting(true);
	}
	else
	{
		mSceneMgr->getParticleSystem("dust1_"+ Ogre::StringConverter::toString(id))->setEmitting(false);
		mSceneMgr->getParticleSystem("dust2_"+ Ogre::StringConverter::toString(id))->setEmitting(false);
	}

	NxWheelContactData d;
	for (int i=0;i<4;i++)
	{
		ve->mWheels[i].mWheel->getContact(d);
		if (d.otherShapeMaterialIndex == 18)
			ve->resetCarPos();
	}

	if (!isAi) return;
	if (mIsPause) return;
	if (win[id]) return;

	if (cur[id] < veai->getNavigationCurrent())
	{
		times[id] = 0;
		cur[id] = veai->getNavigationCurrent();
	}

	//if (speed < 1.0 && times[id] > timeReach[id]/2.0f)
	//{
	//	times[id] = 0;
	//	//ve->resetCarPos();
	//}

	NxVec3 force = UtilityFunc::OgreVec3_To_NxVec3(
		ve->getCarNode()->getOrientation().zAxis());

	//if (times[id] > timeReach[id])
	//{
	//	times[id] = 0;
	//	timeReach[id] = (float)(rand()%10 + 5) + rand()%10/10.0f;
	//	//ve->resetCarPos();
	//}
	//else
	//{
	//	times[id] += UtilityFunc::getElapseLastFrame();
	//}

	if (speed < 1.0f)
	{
		times[id] += UtilityFunc::getElapseLastFrame();
	}

	if (speed < 1.0f && times[id] > 3.0f)
	{
		ve->resetCarPos();
		times[id] = 0.0f;
	}

	NxVec3 torque(0, 200, 0);
	if (veai->getVehicleDirection() == VehicleAI::LEFT)
	{
		//ve->turnLeft(true);
		//ve->turnRight(false);
		if (speed < 1.0)
			ve->getActor()->addTorque(torque*4, NX_IMPULSE);
		else
			ve->getActor()->addTorque(torque, NX_IMPULSE);

		ve->getActor()->addForce(force*200, NX_IMPULSE);
	}
	else if (veai->getVehicleDirection() == VehicleAI::RIGHT)
	{
		//ve->turnLeft(false);
		//ve->turnRight(true);
		if (speed < 1.0)
			ve->getActor()->addTorque(-torque*3, NX_IMPULSE);
		else
			ve->getActor()->addTorque(-torque, NX_IMPULSE);
		ve->getActor()->addForce(force*200, NX_IMPULSE);
	}
	else
	{
		//NxVec3 force = NxVec3(veai->m_cDirect.x, 0, veai->m_cDirect.y);
		//car->getActor()->addForce(force*50, NX_IMPULSE);
		ve->getActor()->addForce(force*400, NX_IMPULSE);
		//ve->turnLeft(false);
		//ve->turnRight(false);
	}
	float sp = ve->getSpeed();
}

//---------------------------------------------------------------------------------------------------------

void GameState::raceLogic(Vehicle* ve, VehicleAI* veai, int id)
{
	switch(veai->m_iLap)
	{
	case 0:
		{
			if (veai->isOver())
			{
				veai->reset();
				for (int i=1;i<50;i++)
				{
					char* ich = new char[3];
					sprintf(ich, "%d", i);
					veai->addNavigationPoint(mSceneMgr->getSceneNode("R2P" + Ogre::String(ich))->getPosition());
				}
				ve->setOriginalPos(mSceneMgr->getSceneNode("R2PS")->getPosition() + Ogre::Vector3(0, 1, 0));
				mTempSceneNode->setOrientation(Ogre::Quaternion::IDENTITY);
				mTempSceneNode->setDirection(-mSceneMgr->getSceneNode("R2P1")->getPosition() + mSceneMgr->getSceneNode("R2PS")->getPosition());
				ve->setOriginalQuat(mTempSceneNode->getOrientation());
				ve->resetCarPos();
				veai->m_iLap++;
			}
		}break;
	case 1:
		{
			if (veai->isOver())
			{
				win[id] = true;
				veai->m_iLap++;

				if (id == 3) mWinTime = mElapseTime;
				else ve->resetCarPos();
			}
		}break;
	case 2:
		{
			//TODO
		}break;
	}

}