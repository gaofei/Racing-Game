#include "Head.h"
#include "LoThreadRes.h"

#include "GUIManager.h"
#include <OgreFontManager.h>

Ogre::RenderWindow*		LoThreadRes::mWindow = NULL;
Ogre::SceneManager*		LoThreadRes::mSceneMgr = NULL;

bool					LoThreadRes::mIsEnd	 = true;
bool					LoThreadRes::mIsReturn = false;
LoThreadRes*			LoThreadRes::mLoThreadRes	= NULL;
Hikari::HikariManager*	LoThreadRes::mHikariMgr		= NULL;

Ogre::TextAreaOverlayElement* LoThreadRes::mTextArea		 = NULL;
Ogre::TextAreaOverlayElement* LoThreadRes::mPercentTextArea	 = NULL;

LoThreadRes::LoThreadRes()
{
	mIsEnd		= true;
	mIsReturn	= false;
	mWindow		= NULL;
	mResgMgr	= NULL;
	mHikariMgr	= NULL;
	mTextArea	= NULL;
	mPercentTextArea = NULL;

	mGroupNum	= 0;
	mPercentInc = 0;
	mPercent	= 0;

	mBackgroundTickets.clear();
;
}

LoThreadRes::~LoThreadRes()
{
}

LoThreadRes* LoThreadRes::getSingleton()
{
	if (!mLoThreadRes)
		mLoThreadRes = new LoThreadRes;

	return mLoThreadRes;
}

void LoThreadRes::loadAllResGroup(int groupNum, Ogre::RenderWindow* win, 
								  Ogre::SceneManager* sm)
{

	
	//todo
	mSceneMgr = sm;

	
	//how many group should you listen
	mGroupNum = groupNum;

	mWindow = win;

	//add this to listening global resource group manager
	mResgMgr = Ogre::ResourceGroupManager::getSingletonPtr();
	mResgMgr->addResourceGroupListener(this);

	//loading flash overlay
	mHikariMgr = GUIManager::getSingleton()->getHikariManger();
	Hikari::FlashControl* ctrl = mHikariMgr->createFlashOverlay("LoThreadRes", mWindow->getViewport(0), 
		1280, 800, Hikari::Center);
	ctrl->load("progressBar1.swf");
	ctrl->setTransparent(false, false);
	ctrl->setScaleMode(Hikari::SM_NOBORDER);
	ctrl->setIgnoreTransparentPixels(false);
	ctrl->setDraggable(false);

	//loading font for textArea overlay
	mResgMgr->addResourceLocation("resource\\Gui", "FileSystem");
	Ogre::FontManager& fontMgr = Ogre::FontManager::getSingleton();
	Ogre::ResourcePtr font = fontMgr.create("LoThreadResFont", "General");
	font->setParameter("type", "truetype");
	font->setParameter("source", "solo5.ttf");
	font->setParameter("size", "16");
	font->setParameter("resolution", "96");
	font->load();


	//panel
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
	Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(
		overlayManager.createOverlayElement("Panel", "TextPanel"));
	panel->setPosition(0.505f, 0.48f);
	panel->setDimensions(100, 200);

	//textArea
	mTextArea = static_cast<Ogre::TextAreaOverlayElement*>(
		overlayManager.createOverlayElement("TextArea", "LoThreadResTextArea"));
	mTextArea->setMetricsMode(Ogre::GMM_PIXELS);
	mTextArea->setPosition(0, 0);
	mTextArea->setDimensions(100, 100);
	mTextArea->setAlignment(Ogre::TextAreaOverlayElement::Center);
	mTextArea->setCaption("null");
	mTextArea->setCharHeight(16);
	mTextArea->setFontName("LoThreadResFont");

	Ogre::Overlay* overlay = overlayManager.create("LoThreadResLay");
	overlay->add2D(panel);

	// Add the text area to the panel
	panel->addChild(mTextArea);
	// Show the overlay
	overlay->show();
	mHikariMgr->update();

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	//CreateThread(0, 0, LoThreadRes::Loading, (LPVOID)0, 0, &mTheadId);

	// rendering loop
	//MSG msg;
	//while(true)
	//{
	//	PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);
	//	TranslateMessage( &msg );
	//	DispatchMessage( &msg );
	//	mWindow->update();
	//	mHikariMgr->update();
	//	if (mIsReturn) break;
	//}

	//TODO: common framework shouldn't has this part, remove it if is not necessary
	OgreMax::OgreMaxScene oms;
	oms.Load("scene_fangjian.scene", mWindow, OgreMax::OgreMaxScene::NO_OPTIONS, 
		mSceneMgr, mSceneMgr->getRootSceneNode(), mLoThreadRes);

	overlay->hide();
	mHikariMgr->destroyFlashControl(ctrl);
}
bool LoThreadRes::checkTickets()
{
	bool result;
	unsigned int size = mBackgroundTickets.size();
	Ogre::ResourceBackgroundQueue* rbq = Ogre::ResourceBackgroundQueue::getSingletonPtr();
	for (unsigned int i=0;i<size;i++)
		result &= rbq->isProcessComplete(mBackgroundTickets[i]);

	return result;
}

DWORD WINAPI LoThreadRes::Loading(LPVOID arg)
{
	//loads the resourcegroup in background
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	mIsReturn = true;

	return 1;
}

void LoThreadRes::resourceGroupLoadStarted(const Ogre::String& groupName, size_t resourceCount)
{
	mPercentInc = 0.3f / (Ogre::Real)mGroupNum / (Ogre::Real)resourceCount;
	mWindow->update();
}

void LoThreadRes::resourceLoadStarted(const Ogre::ResourcePtr& resource)
{
	mTextArea->setCaption(Ogre::StringConverter::toString(int(mPercent*100)) + 
		"\n\n\n\n\n\n\n\n\n\n" + "Loading:\n\n" + resource->getName());
	mWindow->update();
}

void LoThreadRes::resourceLoadEnded()
{
	mPercent += mPercentInc;

	if (mPercent > 0.9999f) mPercent = 1.0f; //fix the float error 
	mTextArea->setCaption(Ogre::StringConverter::toString(int(mPercent*100)) + 
		"\n\n\n\n\n\n\n\n\n\n" + "LoadingOK");
	mWindow->update();
}

void LoThreadRes::resourceGroupScriptingStarted(const Ogre::String& groupName, size_t scriptCount)
{

	mPercentInc = 0.5f / (Ogre::Real)mGroupNum / (Ogre::Real)scriptCount;
}

void LoThreadRes::scriptParseStarted(const Ogre::String& scriptName, bool& skipThisScript)
{
	mTextArea->setCaption(Ogre::StringConverter::toString(int(mPercent*100)) + 
		"\n\n\n\n\n\n\n\n\n\n" + "Parsing:\n\n" + scriptName);
	mWindow->update();
}

void LoThreadRes::scriptParseEnded(const Ogre::String& scriptName, bool skipped)
{
	mPercent += mPercentInc;

	if (mPercent > 0.9999f) mPercent = 1.0f; //fix the float error 
	mTextArea->setCaption(Ogre::StringConverter::toString(int(mPercent*100)) + 
		"\n\n\n\n\n\n\n\n\n\n" + "Parsing:\n\n" + scriptName);
	mWindow->update();
}

void LoThreadRes::UpdatedLoadProgress(const OgreMax::OgreMaxScene* scene, Ogre::Real progress)
{
	mPercent = 0.5f + 0.5f * progress;

	if (mPercent > 0.9999f) 
		mPercent = 1.0f; //fix the float error 
	mTextArea->setCaption(Ogre::StringConverter::toString(int(mPercent*100)) + 
		"\n\n\n\n\n\n\n\n\n\n" + "Loading Scene...");

	mWindow->update();
}