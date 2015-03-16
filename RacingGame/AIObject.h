#ifndef __AIOBJECT__H__
#define __AIOBJECT__H__

/*
AI实体类
纯虚类
继承后
每个类的对象都是独立的AI执行对象
可以独立运作，但是建议使用AI管理器运作
*/

class AIObject
{
public:
	AIObject();
	~AIObject();

public:
	virtual void init() = 0;		//在实体加入到AI管理器的时候做的初始化工作
	virtual void advance() = 0;		//让AI管理器执行的AI运作
	virtual void exit() = 0;		//从AI管理器清除后的释放操作

};

#endif //__AIOBJECT__H__