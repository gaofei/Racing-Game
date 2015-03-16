#ifndef __LOTHREADRES__H__
#define __LOTHREADRES__H__

#include <vector>
#include <OgreTextAreaOverlayElement.h>
#include "OgreMax\OgreMaxScene.hpp"

/*
多线程资源加载类
资源加载未同步
需要用多线程方式重新编译Ogre
此类目前还不通用
new:删除多线程改用单线程
*/

class LoThreadRes;
struct WQRes
{
	LoThreadRes* woptr;
};

class LoThreadRes :  public Ogre::ResourceGroupListener, public OgreMax::OgreMaxSceneCallback
{

protected:

	LoThreadRes();
	~LoThreadRes();

public:

	static LoThreadRes* getSingleton();

	void loadAllResGroup(int groupNum, Ogre::RenderWindow* win, Ogre::SceneManager* mSceneMgr);

	bool checkTickets();

	static DWORD WINAPI Loading(LPVOID arg);

	//resource loading callback
	virtual void resourceGroupLoadStarted(const Ogre::String& groupName, size_t resourceCount);
	virtual void resourceGroupLoadEnded(const Ogre::String& groupName) {}
	virtual void resourceLoadStarted(const Ogre::ResourcePtr& resource);
	virtual void resourceLoadEnded();

	//resource script parsing call back
	virtual void resourceGroupScriptingStarted(const Ogre::String& groupName, size_t scriptCount);
	virtual void resourceGroupScriptingEnded(const Ogre::String& groupName) {}
	virtual void scriptParseStarted(const Ogre::String& scriptName, bool& skipThisScript);
	virtual void scriptParseEnded(const Ogre::String& scriptName, bool skipped);

	//other override
	virtual void worldGeometryStageStarted(const Ogre::String& description) {}
	virtual void worldGeometryStageEnded() {}

	//TODO: non-common framework content
	virtual void UpdatedLoadProgress(const OgreMax::OgreMaxScene* scene, Ogre::Real progress);


protected:

	static Ogre::RenderWindow*	mWindow;
	Ogre::ResourceGroupManager* mResgMgr;

	DWORD mTheadId;
	int mGroupNum;

	Ogre::Real mPercentInc;
	Ogre::Real mPercent;

	std::vector<Ogre::BackgroundProcessTicket> mBackgroundTickets;

	//TODO: non-common framework content
	static Ogre::SceneManager* mSceneMgr;

	static bool mIsEnd;
	static bool mIsReturn;
	static LoThreadRes* mLoThreadRes;
	static Hikari::HikariManager* mHikariMgr;

	static Ogre::TextAreaOverlayElement* mTextArea;
	static Ogre::TextAreaOverlayElement* mPercentTextArea;
};


#endif //__LOTHREADRES__H__