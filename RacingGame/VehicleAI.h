#ifndef __VEHICLEAI__H__
#define __VEHICLEAI__H__

#include <vector>

#include "AIObject.h"

/*
����AI��
ʵ���������
��Ҫ����addNavigationPoint()���ӵ�����
����bindPredator()����׷����
������������
���Ե���getVehicleState�������AI״̬
ע��AIϵͳʹ����������ϵ
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
	//����׷����ģʽ
	//�о���׷��ͷ���׷��
	//Ĭ���Ƿ���׷��
	void setPredatorMode(PREDATOR_MODE mode);

	//��ʹ�þ���׷���ʱ����ã����þ��뾫��
	void setDistancePrecision(Ogre::Real d);

	//��ʹ�÷���׷���ʱ����ã����÷��򾫶�
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