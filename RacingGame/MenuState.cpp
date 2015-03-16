#include "Head.h"
#include "MenuState.h"

#include "GameState.h"
#include "GUIManager.h"
#include "StatesManager.h"
#include "GlobalManager.h"

#include "UtilltyFunc.h"

//---------------------------------------MenuState-------------------------------------
//-------------------------------------------------------------------------------------


MenuState* MenuState::mMenuState = 0;

//-------------------------------------------------------------------------------------

MenuState::MenuState()
{
	mSceneMgr	= NULL;
	mCamera		= NULL;
	mViewPort	= NULL;
	mAnim		= NULL;
	mCameraTrackNode	= NULL;
	mMenuOverlay		= NULL;
}

//-------------------------------------------------------------------------------------

void MenuState::init()
{
	//创建场景管理器
	mSceneMgr	= GlobalManager::mSharedSceneMgr;
	mCamera		= GlobalManager::mShareCamera;
	mViewPort	= GlobalManager::mGlobalViewPort;

	//设置摄像机宽高比
	mCamera->setAspectRatio(
		Ogre::Real(mViewPort->getActualWidth()) / Ogre::Real(mViewPort->getActualHeight()));

	//设置mipmaps等级
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	//创建摄像机轨迹动画
	createAnimationTrack();

	//设置GUI
	setGUI();


	//
	//设置场景视觉
	//

	//mSceneMgr->setSkyBox(true, "CloudyNoonSkyBox", 50.0f);
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8f, 0.8f, 0.8f));

	//添加视口HDR
	//CompositorManager::getSingleton().registerCompositorLogic("HDR", new HDRLogic);
	//CompositorManager::getSingleton().addCompositor(mViewPort, "HDR");
	//CompositorManager::getSingleton().setCompositorEnabled(mViewPort, "HDR", true);

	//
	//场景视觉设置完毕
	//
}

//-------------------------------------------------------------------------------------

void MenuState::loop()
{
	float elapseTime = UtilityFunc::getElapseLastFrame();

	//轨迹动画
	mAnim->addTime(elapseTime);
	processTrackFrame();
}

//-------------------------------------------------------------------------------------

void MenuState::exit()
{
	if (mSceneMgr)
		Ogre::Root::getSingleton().destroySceneManager(mSceneMgr);
}

//-------------------------------------------------------------------------------------

bool MenuState::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}

//-------------------------------------------------------------------------------------

bool MenuState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

//-------------------------------------------------------------------------------------

bool MenuState::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

//-------------------------------------------------------------------------------------

void MenuState::createAnimationTrack()
{
	//创建轨迹摄像机节点
	mCameraTrackNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mCameraTrackNode->attachObject(mCamera);

	//创建动画实例
	Ogre::Animation* animation	= mSceneMgr->createAnimation("NodeTracking_Animation", 25);
	animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);


	//
	//在实例中创建轨迹动画
	//

	Ogre::NodeAnimationTrack* animTrack	= animation->createNodeTrack(0, mCameraTrackNode);


	//获得所有关键摄像机位置
	Ogre::Vector3 track1_cam0(mSceneMgr->getSceneNode("track1_cam0")->getPosition());
	Ogre::Vector3 track1_cam1(mSceneMgr->getSceneNode("track1_cam1")->getPosition());
	Ogre::Vector3 track2_cam0(mSceneMgr->getSceneNode("track2_cam0")->getPosition());
	Ogre::Vector3 track2_cam1(mSceneMgr->getSceneNode("track2_cam1")->getPosition());
	Ogre::Vector3 track2_cam2(mSceneMgr->getSceneNode("track2_cam2")->getPosition());
	Ogre::Vector3 track2_cam3(mSceneMgr->getSceneNode("track2_cam3")->getPosition());

	//计算相对位置，作为帧补间的translate
	Ogre::Vector3 track_0_10(track1_cam1 - track1_cam0);
	Ogre::Vector3 track_10_15(track2_cam1 - track2_cam0);
	Ogre::Vector3 track_15_20(track2_cam2 - track2_cam1);
	Ogre::Vector3 track_20_25(track2_cam3 - track2_cam2);

	//创建第一条轨迹
	animTrack->createNodeKeyFrame(0)->setTranslate(mSceneMgr->getSceneNode("track1_cam0")->getPosition());
	animTrack->createNodeKeyFrame(10)->setTranslate(mSceneMgr->getSceneNode("track1_cam1")->getPosition());

	//创建第二条轨迹
	animTrack->createNodeKeyFrame(10)->setTranslate(mSceneMgr->getSceneNode("track2_cam0")->getPosition());
	animTrack->createNodeKeyFrame(15)->setTranslate(mSceneMgr->getSceneNode("track2_cam1")->getPosition());
	animTrack->createNodeKeyFrame(20)->setTranslate(mSceneMgr->getSceneNode("track2_cam2")->getPosition());
	animTrack->createNodeKeyFrame(25)->setTranslate(mSceneMgr->getSceneNode("track2_cam3")->getPosition());

	//创建动画State以进行播放
	mAnim = mSceneMgr->createAnimationState("NodeTracking_Animation");
	mAnim->setEnabled(true);

	//
	//建轨迹动画创建完毕
	//

}

//-------------------------------------------------------------------------------------

void MenuState::setGUI()
{
	Hikari::HikariManager* hikariMgr = GUIManager::getSingleton()->getHikariManger();

	mMenuOverlay = hikariMgr->createFlashOverlay("MainMenu", mViewPort, 
		(int)mViewPort->getActualWidth(), (int)mViewPort->getActualHeight(), 
		Hikari::Position(Hikari::Center));

	mMenuOverlay->load("ss.swf");
	mMenuOverlay->setDraggable(false);
	mMenuOverlay->setIgnoreTransparentPixels(false);
	mMenuOverlay->setTransparent(true, false);

	//创建事件响应
	mMenuOverlay->bind("startGame", Hikari::FlashDelegate(this, &MenuState::OnStartGame));
	mMenuOverlay->bind("exitGame", Hikari::FlashDelegate(this, &MenuState::OnExitGame));
}

//-------------------------------------------------------------------------------------

void MenuState::processTrackFrame()
{
	if (mAnim->getTimePosition() > 10)
		mCamera->lookAt(mSceneMgr->getSceneNode("track2_cam1")->getPosition());
	else
		mCamera->lookAt(mSceneMgr->getRootSceneNode()->getPosition());
}

Hikari::FlashValue MenuState::OnStartGame(Hikari::FlashControl* caller, const Hikari::Arguments& arg)
{
	mMenuOverlay->hide();
	mMenuOverlay->unbind("startGame");
	StatesManager::getSingleton()->buildNewState(new GameState);

	return FLASH_VOID;
}

Hikari::FlashValue MenuState::OnExitGame(Hikari::FlashControl* caller, const Hikari::Arguments& arg)
{
	StatesManager::getSingleton()->clearAllState();
	return FLASH_VOID;
}