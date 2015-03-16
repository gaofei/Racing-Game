#include "Head.h"
#include "Vehicle.h"

#include "UtilltyFunc.h"

//---------------------------------------------------------------------------------------------------------
Vehicle::Vehicle(const std::string &fileName,Ogre::SceneManager* sm,
				 Ogre::RenderWindow* win, NxScene* ns, 
				 const Ogre::Vector3 pos, const Ogre::Quaternion ori):
                 mSpeed(0), mAngle(0),mAngleDelta(0),mTurnLeft(0),mTurnRight(0),mRollAngle(0)
{

	//初始化各种变量
	mSceneMgr		= sm;
	mWindow			= win;
	mOriginalPos	= pos;
	mOriginalQuat	= ori;
	mNxScene		= ns;
	//mVehicleInfo.loadFromFile("carinfo.cfg");	
	mDecalShadow = NULL;

	VehicleWheel vw;
	mWheels.push_back(vw);
	mWheels.push_back(vw);
	mWheels.push_back(vw);
	mWheels.push_back(vw);

	loadScene(fileName);

	//load parameter data from file
	Ogre::FileInfoListPtr fp = 
		Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("Popular", fileName + ".vpf");
	Ogre::String total = fp->back().archive->getName() + "\\" + fileName + ".vpf";
	mVehicleParam.loadFromFile(total);
	refreshParameter();

	mCarNode = mBaseCarNode;

	//创建摄像机节点
	mCameraDerivedNode = mCarNode->createChildSceneNode(Ogre::Vector3(0.0f, mBoundingBox.getSize().y, -mBoundingBox.getSize().z*2));
	mVehicleCamer = new VehicleCamera(fileName + "VehicleCamera", mWindow, mSceneMgr);
	mVehicleCamer->setTarget(mCameraDerivedNode, mCarNode);
	mVehicleCamer->setTightness(2.5f);

	//获取VehicleCamera计算后得到的CameraNode
	mCameraNode = mVehicleCamer->getCameraNode();

	//创建车的附加物体外设
	createPeriphery();

	CompositorManager::getSingleton().addCompositor(mWindow->getViewport(0), "Radial Blur");
	CompositorManager::getSingleton().setCompositorEnabled(mWindow->getViewport(0), "Radial Blur", false);

	tforce = ttortue = NxVec3(0, 0, 0);
	isJet = false;
}

//---------------------------------------------------------------------------------------------------------

Vehicle::~Vehicle()
{

	if (mVehicleCamer)
	{
		delete mVehicleCamer;
		mVehicleCamer = NULL;
	}

	if (mDecalShadow)
	{
		delete mDecalShadow;
		mDecalShadow = NULL;
	}

	if (mVehicleRenderable)
	{
		delete mVehicleRenderable;
		mVehicleRenderable = NULL;
	}

	if (mActor)
	{
		mNxScene->releaseActor(*mActor);
		mActor = NULL;
	}

	if (mCameraDerivedNode)
	{
		mSceneMgr->destroySceneNode(mCameraDerivedNode);
		mCameraDerivedNode = NULL;
	}

	if(mBaseCarNode)
	{
		mBaseCarNode->removeAndDestroyAllChildren();
		mSceneMgr->destroySceneNode(mBaseCarNode);
		mBaseCarNode	= NULL;
		mCarNode		= NULL;
	}
}

//---------------------------------------------------------------------------------------------------------

void Vehicle::turnLeft(bool isTurn)
{
	if (isTurn)
		mTurnLeft = mVehicleParam.m_sVehicleParam.SteerSpeed;
	else
		mTurnLeft = 0.f;
}

//---------------------------------------------------------------------------------------------------------

void Vehicle::turnRight(bool isTurn)
{
	if (isTurn)
		mTurnRight = -mVehicleParam.m_sVehicleParam.SteerSpeed;
	else
		mTurnRight = 0.f;
}

//---------------------------------------------------------------------------------------------------------

void Vehicle::brake(NxReal torque)
{
	mWheels[0].mWheel->setBrakeTorque(torque);
	mWheels[1].mWheel->setBrakeTorque(torque);
	mWheels[2].mWheel->setBrakeTorque(torque);
	mWheels[3].mWheel->setBrakeTorque(torque);
}

//---------------------------------------------------------------------------------------------------------

void Vehicle::updata(Ogre::Real timeSinceLastFrame)
{
	updataCar(timeSinceLastFrame);
	updataBodyAndWheelNode(timeSinceLastFrame);
	mVehicleCamer->update(timeSinceLastFrame);
	//mDecalShadow->updata();
}

//---------------------------------------------------------------------------------------------------------

void Vehicle::updataBodyAndWheelNode(NxReal timeSinceLastFrame)
{//更新车身和轮子节点

	NxQuat	nxquat;

	//更新父节点
	mBaseCarNode->setPosition(mActor->getGlobalPosition().x, mActor->getGlobalPosition().y, 
		mActor->getGlobalPosition().z);
	mActor->getGlobalOrientation().toQuat(nxquat);
	mBaseCarNode->setOrientation(nxquat.w, nxquat.x, nxquat.y, nxquat.z);

	//更新车身节点
	//mBodyNode->setPosition(mBodyShape->getLocalPosition().x, mBodyShape->getLocalPosition().y, 
	//						mBodyShape->getLocalPosition().z);
	//mBodyShape->getLocalPose().M.toQuat(nxquat);
	//mBodyNode->setOrientation(nxquat.w, nxquat.x, nxquat.y, nxquat.z);


	//更新轮子节点
	for (uint i = 0;i<mWheels.size();i++)
	{
		NxMat33 wheelMat;
		NxMat33 steerMat, rollMat;

		NxVec3 wheelPos;

		NxWheelShape* wheel = mWheels[i].mWheel;
		wheelPos = wheel->getLocalPosition();

		//Ogre::Vector3 vec3(UtilityFunc::NxVec3_To_OgreVec3(mWheels[i].mWheel->getLocalPosition()));

		//nxquat.fromAngleAxis(NxMath::radToDeg(mWheels[i].mWheel->getSteerAngle()), NxVec3(0, 1, 0));

		//轮子角度
		steerMat.rotY(wheel->getSteerAngle());

		mRollAngle += wheel->getAxleSpeed() * timeSinceLastFrame;

		//防溢出
		while (mRollAngle > NxTwoPi)
			mRollAngle -= NxTwoPi;
		while (mRollAngle < -NxTwoPi)
			mRollAngle += NxTwoPi;

		//轮子转动
		rollMat.rotX(mRollAngle);

		wheelMat = steerMat * rollMat;
		wheelMat.toQuat(nxquat);

		NxWheelContactData wcd;
		NxShape* shape = mWheels[i].mWheel->getContact(wcd);

		if (shape)
		{
			NxReal radius = wheel->getRadius();

			wheelPos.y += radius - wcd.contactPosition;
		}
		else
		{
			NxReal suspensionTravel = wheel->getSuspensionTravel();
			wheelPos.y += -suspensionTravel;
		}

		mWheels[i].mSceneNode->setPosition(UtilityFunc::NxVec3_To_OgreVec3(wheelPos));
		mWheels[i].mSceneNode->setOrientation(UtilityFunc::NxQuat_ToOgre_Quat(nxquat));
		//mWheels[i].mSceneNode->rotate(Ogre::Vector3(1, 0, 0), Ogre::Radian(mRollAngle));
	}
}

//---------------------------------------------------------------------------------------------------------

void Vehicle::updataCar(NxReal timeSinceLastFrame)
{
	if(NxMath::abs(mTurnLeft) > 0.001f || NxMath::abs(mTurnRight) > 0.001f)
	{
		mAngleDelta = mTurnLeft + mTurnRight;
	}
	else
		// 车在松开按键的时候会自动回正
		if(NxMath::abs(mAngle) > 0.0001f)
		{
			if(mAngle > 0.0001f)
				mAngleDelta = -mVehicleParam.m_sVehicleParam.SteerSpeed;
			else
				mAngleDelta = mVehicleParam.m_sVehicleParam.SteerSpeed;
		}
		else
			mAngleDelta = 0.f;

	mAngle += mAngleDelta;

	//将轮子角度限制
	mAngle = NxMath::clamp(mAngle, mVehicleParam.m_sVehicleParam.SteerAngleRange,
							-mVehicleParam.m_sVehicleParam.SteerAngleRange);

	Ogre::Real radian = mAngle * 0.01745329f;
	mWheels[TOP_LEFT].mWheel->setSteerAngle(radian);
	mWheels[TOP_RIGHT].mWheel->setSteerAngle(radian);

	mWheels[TOP_LEFT].mWheel->setMotorTorque(mSpeed);
	mWheels[TOP_RIGHT].mWheel->setMotorTorque(mSpeed);
	mWheels[Bottom_LEFT].mWheel->setMotorTorque(mSpeed);
	mWheels[Bottom_RIGHT].mWheel->setMotorTorque(mSpeed);

	//mActor->addForce(tforce, NX_IMPULSE);
	mActor->addTorque(ttortue, NX_IMPULSE);

	if (isJet)
		jet(800);
}

//---------------------------------------------------------------------------------------------------------

VehicleParam Vehicle::getVehicleParam()
{
	return mVehicleParam;
}

//---------------------------------------------------------------------------------------------------------

void Vehicle::setVehicleParam(VehicleParam vp)
{ 
	mVehicleParam = vp;
	refreshParameter();
	
}

//---------------------------------------------------------------------------------------------------------

NxReal Vehicle::getSpeed()
{
	return mActor->getLinearVelocity().magnitude();
}

//---------------------------------------------------------------------------------------------------------

bool Vehicle::injectMouseMoved( const OIS::MouseEvent &e )
{
	if (mVehicleCamer->injectMouseMoved(e))
		return true;
	return true;
}

//---------------------------------------------------------------------------------------------------------

bool Vehicle::injectMousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(id == OIS::MB_Left)
		mVehicleCamer->releaseCam(true);
	return true;
}

//---------------------------------------------------------------------------------------------------------

bool Vehicle::injectMouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
		mVehicleCamer->releaseCam(false);
	return true;
}

//---------------------------------------------------------------------------------------------------------

bool Vehicle::injectKeyPressed(const OIS::KeyEvent &arg)
{
	switch(arg.key)
	{
	case OIS::KC_LEFT:
	case OIS::KC_A:	
		turnLeft(true);
		break;
	case OIS::KC_RIGHT:
	case OIS::KC_D:
		turnRight(true);
		break;

	case OIS::KC_UP:
	case OIS::KC_W:
		mSpeed = mVehicleParam.m_sVehicleParam.MotorTorque;
		break;

	case OIS::KC_DOWN:
	case OIS::KC_S:	
		mSpeed = -mVehicleParam.m_sVehicleParam.MotorTorque;
		break;
	
	case OIS::KC_SPACE:
		brake(mVehicleParam.m_sVehicleParam.MotorTorque);
		break;

	case OIS::KC_F:
		isJet = true;
		CompositorManager::getSingleton().setCompositorEnabled(mWindow->getViewport(0), "Radial Blur", true);
		break;
	case  OIS::KC_H:
		tforce += NxVec3(0, 0, 2000);
		break;;
	case OIS::KC_J:
		ttortue += NxVec3(1000, 0, 0);
		break;
	case  OIS::KC_L:
		ttortue += NxVec3(0, 0, 2000);
		break;;
	case OIS::KC_R:
		resetCarPos();
		break;
	}

	return true;
}

//---------------------------------------------------------------------------------------------------------

bool Vehicle::injectKeyReleased(const OIS::KeyEvent &arg)
{
	switch(arg.key)
	{
	case OIS::KC_LEFT:
	case OIS::KC_A:	
		turnLeft(false);
		break;
	case OIS::KC_RIGHT:
	case OIS::KC_D:
		turnRight(false);
		break;

	case OIS::KC_UP:
	case OIS::KC_W:
		mSpeed = 0;
		break;

	case OIS::KC_DOWN:
	case OIS::KC_S:
		mSpeed = 0;
		break;

	case OIS::KC_SPACE:
		brake(0);
		break;

	case OIS::KC_F:
		isJet = false;
		CompositorManager::getSingleton().setCompositorEnabled(mWindow->getViewport(0), "Radial Blur", false);
		break;
	case  OIS::KC_H:
		tforce -= NxVec3(0, 0, 2000);
		break;;
	case OIS::KC_J:
		ttortue -= NxVec3(1000, 0, 0);
		break;
	case  OIS::KC_L:
		ttortue -= NxVec3(0, 0, 2000);
		break;;
	}

	return true;
}

//---------------------------------------------------------------------------------------------------------

void Vehicle::refreshParameter()
{

	VehicleParamStruct vps = mVehicleParam.m_sVehicleParam;

	mWheels[TOP_LEFT].mWheel->setLongitudalTireForceFunction(vps.FrontLongtitudal);
	mWheels[TOP_LEFT].mWheel->setLateralTireForceFunction(vps.FrontLateral);
	mWheels[TOP_LEFT].mWheel->setSuspension(vps.FrontSuspension);
	mWheels[TOP_LEFT].mWheel->setSuspensionTravel(vps.FrontSuspensionTravel);

	mWheels[TOP_RIGHT].mWheel->setLongitudalTireForceFunction(vps.FrontLongtitudal);
	mWheels[TOP_RIGHT].mWheel->setLateralTireForceFunction(vps.FrontLateral);
	mWheels[TOP_RIGHT].mWheel->setSuspension(vps.FrontSuspension);
	mWheels[TOP_RIGHT].mWheel->setSuspensionTravel(vps.FrontSuspensionTravel);

	mWheels[Bottom_LEFT].mWheel->setLongitudalTireForceFunction(vps.RearLongtitudal);
	mWheels[Bottom_LEFT].mWheel->setLateralTireForceFunction(vps.RearLateral);
	mWheels[Bottom_LEFT].mWheel->setSuspension(vps.RearSuspension);
	mWheels[Bottom_LEFT].mWheel->setSuspensionTravel(vps.RearSuspensionTravel);

	mWheels[Bottom_RIGHT].mWheel->setLongitudalTireForceFunction(vps.RearLongtitudal);
	mWheels[Bottom_RIGHT].mWheel->setLateralTireForceFunction(vps.RearLateral);
	mWheels[Bottom_RIGHT].mWheel->setSuspension(vps.RearSuspension);
	mWheels[Bottom_RIGHT].mWheel->setSuspensionTravel(vps.RearSuspensionTravel);
}

//---------------------------------------------------------------------------------------------------------

void Vehicle::resetCarPos()
{

	NxVec3 vec3(mOriginalPos.x, mOriginalPos.y, mOriginalPos.z);
	NxQuat quat;
	quat.setWXYZ(mOriginalQuat.w, mOriginalQuat.x, mOriginalQuat.y, mOriginalQuat.z);

	mActor->setGlobalPose(NxMat34(quat, vec3));

	vec3.zero();
	mActor->setLinearVelocity(vec3);
	mActor->setAngularVelocity(vec3);

}

//---------------------------------------------------------------------------------------------------------

void Vehicle::loadScene(const std::string &fileName)
{

	//车的父节点
	mBaseCarNode			= mSceneMgr->getRootSceneNode()->createChildSceneNode(fileName + "BaseCarNode");

	//DotSceneLoader* dsl = new DotSceneLoader();//rel
	//dsl->parseDotScene(fileName + ".scene", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mSceneMgr, mBaseCarNode);

	mVehicleRenderable = new VehicleRenderable(mSceneMgr, mBaseCarNode);
	mVehicleRenderable->load(fileName + ".vrf");

	//车身节点
	mBodyNode				= static_cast<Ogre::SceneNode*>(mBaseCarNode->getChild(fileName + "Body"));

	//轮子节点
	mWheels[TOP_LEFT].mSceneNode	= static_cast<Ogre::SceneNode*>(mBaseCarNode->getChild(fileName + "LTWheel"));
	mWheels[TOP_LEFT].mName			= fileName + "LTWheel";

	mWheels[TOP_RIGHT].mSceneNode	= static_cast<Ogre::SceneNode*>(mBaseCarNode->getChild(fileName + "RTWheel"));
	mWheels[TOP_RIGHT].mName		= fileName + "RTWheel";

	mWheels[Bottom_LEFT].mSceneNode	= static_cast<Ogre::SceneNode*>(mBaseCarNode->getChild(fileName + "LBWheel"));
	mWheels[Bottom_LEFT].mName		= fileName + "LBWheel";

	mWheels[Bottom_RIGHT].mSceneNode  = static_cast<Ogre::SceneNode*> (mBaseCarNode->getChild(fileName + "RBWheel"));
	mWheels[Bottom_RIGHT].mName		  = fileName + "RBWheel";

	//设置整体参数
	NxBodyDesc bodyDes;
	//bodyDes.wakeUpCounter	= 1E8;
	bodyDes.mass			= 12000;//mVehicleInfo.gMass;
	//bodyDes.massLocalPose.t	= NxVec3(mVehicleInfo.gMassLocalPose.x, mVehicleInfo.gMassLocalPose.y, mVehicleInfo.gMassLocalPose.z);

	bodyDes.angularDamping	= 0.03f;
	bodyDes.linearDamping	= 0.08f;

	//写入车身的shapeDesc
	mBoundingBox		= mBodyNode->getAttachedObject(0)->getBoundingBox();
	NxBoxShapeDesc boxDes;
	NxVec3 dimen		(mBoundingBox.getHalfSize().x, mBoundingBox.getHalfSize().y, mBoundingBox.getHalfSize().z);
	NxVec3 localPos		(mBodyNode->getPosition().x, mBodyNode->getPosition().y, mBodyNode->getPosition().z);

	////车身到轮子的距离
	//NxReal d1 = NxMath::abs(mBodyNode->getPosition().y - mWheels[0].mSceneNode->getPosition().y);
	////包围盒半径之和
	//NxReal d2 = mBoundingBox.getHalfSize().y + mWheels[0].mSceneNode->getAttachedObject(0)->getBoundingBox().getHalfSize().y;
	////判断轮子是否与车身的包围盒重叠
	//if(d1 < d2)
	//{
	//	//设置为最大适合高度
	//	dimen.y = (mBoundingBox.getSize().y - (d2 - d1)) / 2.0f;
	//}
	boxDes.dimensions.set(dimen);
	boxDes.localPose.t	= localPos;

	NxActorDesc actorDesc;
	actorDesc.body			= &bodyDes;
	actorDesc.globalPose.t	= NxVec3(mOriginalPos.x, mOriginalPos.y, mOriginalPos.z);
	NxQuat quat;
	quat.setWXYZ(mOriginalQuat.w, mOriginalQuat.x, mOriginalQuat.y, mOriginalQuat.z);
	actorDesc.globalPose.M.fromQuat(quat);
	actorDesc.shapes.pushBack(&boxDes);
	mActor		= mNxScene->createActor(actorDesc);
	mActor->setCMassOffsetLocalPosition(NxVec3(0, -mBoundingBox.getSize().y, 0));
	mBodyShape  = static_cast<NxBoxShape*>(mActor->getShapes()[0]);

	//写入轮子的shapeDesc

	createWheelShapeDesc(&mWheels[TOP_LEFT], true);
	createWheelShapeDesc(&mWheels[TOP_RIGHT], true);
	createWheelShapeDesc(&mWheels[Bottom_LEFT], false);
	createWheelShapeDesc(&mWheels[Bottom_RIGHT], false);

	//mWheels[TOP_LEFT].mWheelDesc.localPose.t = NxVec3(mBoundingBox.getHalfSize().x, 2, 0);
	//mWheels[TOP_RIGHT].mWheelDesc.localPose.t = NxVec3(-mBoundingBox.getHalfSize().x, 2, 0);
	//mWheels[Bottom_LEFT].mWheelDesc.localPose.t = NxVec3(mBoundingBox.getHalfSize().x, -mBoundingBox.getHalfSize().y, 0);
	//mWheels[Bottom_RIGHT].mWheelDesc.localPose.t = NxVec3(-mBoundingBox.getHalfSize().x, -mBoundingBox.getHalfSize().y, 0);

	//创建轮子
	mWheels[TOP_LEFT].mWheel = static_cast<NxWheelShape*>(mActor->createShape(mWheels[TOP_LEFT].mWheelDesc));
	mWheels[TOP_RIGHT].mWheel = static_cast<NxWheelShape*>(mActor->createShape(mWheels[TOP_RIGHT].mWheelDesc));
	mWheels[Bottom_LEFT].mWheel	= static_cast<NxWheelShape*>(mActor->createShape(mWheels[Bottom_LEFT].mWheelDesc));
	mWheels[Bottom_RIGHT].mWheel = static_cast<NxWheelShape*>(mActor->createShape(mWheels[Bottom_RIGHT].mWheelDesc));

	NxMaterial* mat = mNxScene->getMaterialFromIndex(0);
	mat->setFrictionCombineMode(NX_CM_MULTIPLY);
	mat->setStaticFriction(300.2f);
	mat->setDynamicFriction(100.5f);
}

//---------------------------------------------------------------------------------------------------------

void Vehicle::createWheelShapeDesc(VehicleWheel* vw, bool isFront)
{

	//轮子参数的定义
	NxWheelShapeDesc tWheelDes;
	tWheelDes.localPose.t	= UtilityFunc::OgreVec3_To_NxVec3(vw->mSceneNode->getPosition());
	tWheelDes.radius		= vw->mSceneNode->getAttachedObject(0)->getBoundingBox().getHalfSize().y;

	tWheelDes.localPose.t.y -= 0.1f;

	VehicleParamStruct vps = mVehicleParam.m_sVehicleParam;

	//创建的轮子是前轮还是后轮
	if (isFront)
	{
		tWheelDes.longitudalTireForceFunction	= vps.FrontLongtitudal;
		tWheelDes.lateralTireForceFunction		= vps.FrontLateral;
		tWheelDes.suspension					= vps.FrontSuspension;
		tWheelDes.suspensionTravel				= vps.FrontSuspensionTravel;

	}
	else
	{
		tWheelDes.longitudalTireForceFunction	= vps.RearLongtitudal;
		tWheelDes.lateralTireForceFunction		= vps.RearLateral;
		tWheelDes.suspension					= vps.RearSuspension;
		tWheelDes.suspensionTravel				= vps.RearSuspensionTravel;

	}

	//static NxMaterial* wsm = 0; 
	//if (!wsm)
	//{
	//	NxMaterialDesc m;
	//	m.restitution = 0;
	//	m.staticFriction = 3.2f;
	//	m.dynamicFriction = 0.5f;
	//	m.staticFrictionV = 3.0f;
	//	m.dynamicFrictionV = 0.5f;
	//	m.dirOfAnisotropy.set(0, 0, 1);
	//	m.flags |= NX_MF_ANISOTROPIC;
	//	m.frictionCombineMode = NX_CM_MULTIPLY;

	//	wsm = mNxScene->createMaterial(m);
	//}

	//tWheelDes.materialIndex					= wsm->getMaterialIndex();
	tWheelDes.wheelFlags					|= NX_WF_WHEEL_AXIS_CONTACT_NORMAL | 0
												| 0 | NX_WF_CLAMPED_FRICTION;

	vw->mWheelDesc			= tWheelDes;
	
	//NxShapeDesc重载了=符号，必须逐一赋值
	vw->mWheelDesc.localPose		= tWheelDes.localPose;
	vw->mWheelDesc.materialIndex	= tWheelDes.materialIndex;
}

//---------------------------------------------------------------------------------------------------------

void Vehicle::jet(NxReal stiff)
{
	Ogre::Quaternion quat = UtilityFunc::NxQuat_ToOgre_Quat(mActor->getGlobalOrientationQuat());
	NxVec3 nxv3			  = UtilityFunc::OgreVec3_To_NxVec3(quat.zAxis());

	nxv3.normalize();
	mActor->addForce(nxv3 * stiff, NX_IMPULSE);
}

//---------------------------------------------------------------------------------------------------------

void Vehicle::createPeriphery()
{
	Ogre::Real offet = mBoundingBox.getHalfSize().y + 1;
	Ogre::Vector2 ortsize(mBoundingBox.getSize().x, mBoundingBox.getSize().z);
	//mDecalShadow = new DecalShadow(mSceneMgr, mBaseCarNode, offet, "decalShadow.png", ortsize);
}

//---------------------------------------------------------------------------------------------------------
