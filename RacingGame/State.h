#ifndef __STATE_H__
#define __STATE_H__

//#include "GlobalManager.h"

/*
表示程序状态的基类
继承以编写该状态的运行代码
无法实例化该类
*/

class State
{
	
public:
	enum RUN_STATE{INIT, LOOP};

	State():mRunState(INIT), isInit(false){}
	//状态运作
	virtual void run(Ogre::RenderWindow* win);

	//状态的鼠标和键盘事件
	virtual bool mouseMoved( const OIS::MouseEvent &e ) { return true; }
	virtual bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )  { return true; }
	virtual bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )  { return true; }

	virtual bool keyPressed( const OIS::KeyEvent &e )  { return true; }
	virtual bool keyReleased( const OIS::KeyEvent &e )  { return true; }


	//设置当前运作状态
	virtual void setRunState(RUN_STATE rs) { mRunState = rs; }

	//每个状态包含三种固定的子状态
	//分别是初始化、循环和结束清理
	//建议使用setRunState()而不是调用下面三个函数

	//初始化：
	virtual void init() = 0; 

	//循环
	virtual void loop() = 0;

	//结束清理
	virtual void exit() = 0;

public:
	bool isInit;

protected:
	RUN_STATE mRunState;
	Ogre::RenderWindow* mWindow;
};

#endif