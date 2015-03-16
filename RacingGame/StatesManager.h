#ifndef __STATEMANGER_H__
#define __STATEMANGER_H__

#include <vector>
#include "State.h"

/*
状态机管理类
*/

class StatesManager
{
public:
	StatesManager(){}
	StatesManager(Ogre::RenderWindow* win);
	~StatesManager();

	static StatesManager* getSingleton();

	//状态运转
	bool run();

	//注入程序的鼠标和键盘事件
	bool injectMouseMoved( const OIS::MouseEvent &e );
	bool injectMousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
	bool injectMouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

	bool injectKeyPressed( const OIS::KeyEvent &e );
	bool injectKeyReleased( const OIS::KeyEvent &e );


	//改变当前状态
	void changeCurrentState(State* pState);

	//重置（初始化）当前状态
	void resetCurrentState();

	//在当前状态上面新建新状态
	void buildNewState(State* pState);

	//返回上一个状态（保留现场）
	void resumeLastState();

	//清除所有状态，程序会关闭
	void clearAllState();

	//关联到窗口
	void attachWindow(Ogre::RenderWindow* win);

public:

	static StatesManager* mStateManger;

private:

	std::vector<State*>			mStateStack;

	Ogre::RenderWindow*			 mWindow;
};

#endif