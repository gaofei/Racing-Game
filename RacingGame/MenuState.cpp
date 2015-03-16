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
	//��������������
	mSceneMgr	= GlobalManager::mSharedSceneMgr;
	mCamera		= GlobalManager::mShareCamera;
	mViewPort	= GlobalManager::mGlobalViewPort;

	//�����������߱�
	mCamera->setAspectRatio(
		Ogre::Real(mViewPort->getActualWidth()) / Ogre::Real(mViewPort->getActualHeight()));

	//����mipmaps�ȼ�
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	//����������켣����
	createAnimationTrack();

	//����GUI
	setGUI();


	//
	//���ó����Ӿ�
	//

	//mSceneMgr->setSkyBox(true, "CloudyNoonSkyBox", 50.0f);
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8f, 0.8f, 0.8f));

	//����ӿ�HDR
	//CompositorManager::getSingleton().registerCompositorLogic("HDR", new HDRLogic);
	//CompositorManager::getSingleton().addCompositor(mViewPort, "HDR");
	//CompositorManager::getSingleton().setCompositorEnabled(mViewPort, "HDR", true);

	//
	//�����Ӿ��������
	//
}

//-------------------------------------------------------------------------------------

void MenuState::loop()
{
	float elapseTime = UtilityFunc::getElapseLastFrame();

	//�켣����
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
	//�����켣������ڵ�
	mCameraTrackNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mCameraTrackNode->attachObject(mCamera);

	//��������ʵ��
	Ogre::Animation* animation	= mSceneMgr->createAnimation("NodeTracking_Animation", 25);
	animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);


	//
	//��ʵ���д����켣����
	//

	Ogre::NodeAnimationTrack* animTrack	= animation->createNodeTrack(0, mCameraTrackNode);


	//������йؼ������λ��
	Ogre::Vector3 track1_cam0(mSceneMgr->getSceneNode("track1_cam0")->getPosition());
	Ogre::Vector3 track1_cam1(mSceneMgr->getSceneNode("track1_cam1")->getPosition());
	Ogre::Vector3 track2_cam0(mSceneMgr->getSceneNode("track2_cam0")->getPosition());
	Ogre::Vector3 track2_cam1(mSceneMgr->getSceneNode("track2_cam1")->getPosition());
	Ogre::Vector3 track2_cam2(mSceneMgr->getSceneNode("track2_cam2")->getPosition());
	Ogre::Vector3 track2_cam3(mSceneMgr->getSceneNode("track2_cam3")->getPosition());

	//�������λ�ã���Ϊ֡�����translate
	Ogre::Vector3 track_0_10(track1_cam1 - track1_cam0);
	Ogre::Vector3 track_10_15(track2_cam1 - track2_cam0);
	Ogre::Vector3 track_15_20(track2_cam2 - track2_cam1);
	Ogre::Vector3 track_20_25(track2_cam3 - track2_cam2);

	//������һ���켣
	animTrack->createNodeKeyFrame(0)->setTranslate(mSceneMgr->getSceneNode("track1_cam0")->getPosition());
	animTrack->createNodeKeyFrame(10)->setTranslate(mSceneMgr->getSceneNode("track1_cam1")->getPosition());

	//�����ڶ����켣
	animTrack->createNodeKeyFrame(10)->setTranslate(mSceneMgr->getSceneNode("track2_cam0")->getPosition());
	animTrack->createNodeKeyFrame(15)->setTranslate(mSceneMgr->getSceneNode("track2_cam1")->getPosition());
	animTrack->createNodeKeyFrame(20)->setTranslate(mSceneMgr->getSceneNode("track2_cam2")->getPosition());
	animTrack->createNodeKeyFrame(25)->setTranslate(mSceneMgr->getSceneNode("track2_cam3")->getPosition());

	//��������State�Խ��в���
	mAnim = mSceneMgr->createAnimationState("NodeTracking_Animation");
	mAnim->setEnabled(true);

	//
	//���켣�����������
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

	//�����¼���Ӧ
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