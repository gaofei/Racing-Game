#ifndef _VEHICLE_H
#define _VEHICLE_H

#include <NxPhysics.h>

#include "VehicleCamera.h"
#include "DecalShadow.h"
#include "VehicleParam.h"
#include "VehicleRenderable.h"

/*
Vehicle类
车的实现类
自带车体参数文件输入
车的操控
*/

class Vehicle
{

public:

	//轮子结构体
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
	公共成员函数
	*/
	void turnLeft(bool isTurn);
	void turnRight(bool isTurn);
	void brake(NxReal torque);

	//更新整个类的过程
	//在循环中必须调用
	void updata(Ogre::Real timeSinceLastFrame);

	//复位
	void resetCarPos();

	//刷新车参数
	void refreshParameter();

	//获取相关数据
	Ogre::SceneNode* getCarNode(){ return mBaseCarNode; }
	Ogre::SceneNode* getCameraNode(){ return mCameraNode; }

	void setVehicleParam(VehicleParam vp);
	VehicleParam getVehicleParam();

	//设置车初始位置
	void setOriginalPos(Ogre::Vector3 pos) { mOriginalPos = pos; }
	void setOriginalQuat(Ogre::Quaternion quat) { mOriginalQuat = quat; }

	void setMotor(NxReal inp) { mSpeed = inp; }

	//获取actor
	NxActor* getActor(){return mActor;}

	//获取当前速度
	NxReal getSpeed();

	//类自带车的控制函数，将函数注入相关输入处理函数以实现控制
	virtual bool injectMouseMoved( const OIS::MouseEvent &e );
	virtual bool injectMousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool injectMouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool injectKeyPressed(const OIS::KeyEvent &arg);
	virtual bool injectKeyReleased(const OIS::KeyEvent &arg);

protected:

	/*
	内部成员函数
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
	引用性指针,来自其他类，在其他地方释放
	*/

	Ogre::SceneManager*					mSceneMgr;

	Ogre::RenderWindow*					mWindow;

	NxScene*							mNxScene;

	NxBoxShape*							mBodyShape;

	Ogre::SceneNode*					mCameraNode;

	Ogre::SceneNode*					mBodyNode;

	VehicleWheels						mWheels;//rel

	/*
	实用性指针，本类自己创建（需要自己释放）
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
	数据变量
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
	标志变量
	*/

	bool isSteer;
	bool isJet;
	
};

#endif