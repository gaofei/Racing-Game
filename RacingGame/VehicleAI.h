#ifndef __VEHICLEAI__H__
#define __VEHICLEAI__H__

#include <vector>

#include "AIObject.h"

/*
赛车AI类
实例化该类后
需要调用addNavigationPoint()增加导航点
调用bindPredator()设置追踪者
管理器运作后
可以调用getVehicleState（）获得AI状态
注：AI系统使用右手坐标系
*/

class VehicleAI : public AIObject
{
public:
	enum VEHICLE_STATE	{STAY, ADVANCE, BRAKE};
	enum VEHICLE_DIRECT {LEFT, MIDDLE, RIGHT};
	enum PREDATOR_MODE	{DISTANCE, DEGREE};

public:

	VehicleAI();
	~VehicleAI();

public:

	virtual void init();
	virtual void advance();
	virtual void exit();

	void addNavigationPoint(Ogre::Vector3 pVector3);
	void bindPredator(Ogre::SceneNode* pPredator);

	VEHICLE_STATE	getVehicleState();
	VEHICLE_DIRECT	getVehicleDirection();

	Ogre::Vector3 getCurrentNavigationPoint();
	Ogre::Vector3 getNavigationPoint(int index);
	int getNavigationCurrent();
	unsigned int getNavigationSize();

	bool isOver();

	void reset();
	//设置追逐者模式
	//有距离追逐和方向追逐
	//默认是方向追逐
	void setPredatorMode(PREDATOR_MODE mode);

	//当使用距离追逐的时候可用，设置距离精度
	void setDistancePrecision(Ogre::Real d);

	//当使用方向追逐的时候可用，设置方向精度
	void setDegreePrecision(Ogre::Degree degree);

	Ogre::Vector2 V3_To_V2_XZ(Ogre::Vector3 v3);

protected:

	VEHICLE_STATE					m_eVehicleState;

	VEHICLE_DIRECT					m_eVehicleDirction;

	PREDATOR_MODE					m_ePredatorMode;

	std::vector<Ogre::Vector3>		m_cNavigationPointsVector;

	Ogre::SceneNode*				m_pPredator;

	Ogre::Vector2					m_cLastTarget;

	Ogre::Vector2					m_cOrbit;

	Ogre::Real						m_fPreciDistance;

	Ogre::Degree					m_cPreciDegree;
	
	int								m_iNavigationRest;

	int								m_iNavigationCurrent;

	bool							m_bIsFirst;

	bool							m_bIsOver;


public:

	Ogre::Vector2					m_cDirect;

	int								m_iLap;

};


#endif //__VEHICLEAI__H__