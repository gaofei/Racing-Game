#ifndef _VEHICLE_H
#define _VEHICLE_H

#include <NxPhysics.h>

#include "VehicleCamera.h"
#include "DecalShadow.h"
#include "VehicleParam.h"
#include "VehicleRenderable.h"

/*
Vehicle��
����ʵ����
�Դ���������ļ�����
���Ĳٿ�
*/

class Vehicle
{

public:

	//���ӽṹ��
	struct VehicleWheel
	{
		std::string				mName;

		NxWheelShape*			mWheel;
		NxWheelShapeDesc		mWheelDesc;

		Ogre::SceneNode*		mSceneNode;
	};

	typedef std::vector<VehicleWheel> VehicleWheels;

	enum WHEEL_SERIAL{TOP_LEFT, TOP_RIGHT, Bottom_LEFT, Bottom_RIGHT};

public:

	Vehicle(const std::string &fileName, Ogre::SceneManager* sm, Ogre::RenderWindow* win, NxScene* ns,
		const Ogre::Vector3 pos, const Ogre::Quaternion ori = Ogre::Quaternion::IDENTITY);

	~Vehicle();

	/*
	������Ա����
	*/
	void turnLeft(bool isTurn);
	void turnRight(bool isTurn);
	void brake(NxReal torque);

	//����������Ĺ���
	//��ѭ���б������
	void updata(Ogre::Real timeSinceLastFrame);

	//��λ
	void resetCarPos();

	//ˢ�³�����
	void refreshParameter();

	//��ȡ�������
	Ogre::SceneNode* getCarNode(){ return mBaseCarNode; }
	Ogre::SceneNode* getCameraNode(){ return mCameraNode; }

	void setVehicleParam(VehicleParam vp);
	VehicleParam getVehicleParam();

	//���ó���ʼλ��
	void setOriginalPos(Ogre::Vector3 pos) { mOriginalPos = pos; }
	void setOriginalQuat(Ogre::Quaternion quat) { mOriginalQuat = quat; }

	void setMotor(NxReal inp) { mSpeed = inp; }

	//��ȡactor
	NxActor* getActor(){return mActor;}

	//��ȡ��ǰ�ٶ�
	NxReal getSpeed();

	//���Դ����Ŀ��ƺ�����������ע��������봦������ʵ�ֿ���
	virtual bool injectMouseMoved( const OIS::MouseEvent &e );
	virtual bool injectMousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool injectMouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool injectKeyPressed(const OIS::KeyEvent &arg);
	virtual bool injectKeyReleased(const OIS::KeyEvent &arg);

protected:

	/*
	�ڲ���Ա����
	*/

	void updataBodyAndWheelNode(NxReal timeSinceLastFrame);
	void updataCar(NxReal timeSinceLastFrame);

	void loadScene(const std::string &fileName);
	void createWheelShapeDesc(VehicleWheel* vw, bool isFront);
	void createPeriphery();

	void jet(NxReal stiff);

private:

	void checkPassExist(String matName);

public:

	/*
	������ָ��,���������࣬�������ط��ͷ�
	*/

	Ogre::SceneManager*					mSceneMgr;

	Ogre::RenderWindow*					mWindow;

	NxScene*							mNxScene;

	NxBoxShape*							mBodyShape;

	Ogre::SceneNode*					mCameraNode;

	Ogre::SceneNode*					mBodyNode;

	VehicleWheels						mWheels;//rel

	/*
	ʵ����ָ�룬�����Լ���������Ҫ�Լ��ͷţ�
	*/

	Ogre::SceneNode*					mCameraDerivedNode;

	Ogre::SceneNode*					mCarNode;

	Ogre::SceneNode*					mBaseCarNode;

	NxActor*							mActor;

	VehicleCamera*						mVehicleCamer;

	VehicleRenderable*					mVehicleRenderable;

	DecalShadow*						mDecalShadow;

protected:

	/*
	���ݱ���
	*/

	Ogre::Real 					mSpeed;

	Ogre::Real 					mAngleDelta;

	Ogre::Real 					mTurnRight;

	Ogre::Real 					mTurnLeft;

	Ogre::Real 					mAngle;

	Ogre::Vector3				mOriginalPos;

	Ogre::Quaternion			mOriginalQuat;

	Ogre::AxisAlignedBox		mBoundingBox;

	VehicleParam				mVehicleParam;

	NxReal						mRollAngle;

	NxVec3						tforce;
	NxVec3						ttortue;

	/*
	��־����
	*/

	bool isSteer;
	bool isJet;
	
};

#endif