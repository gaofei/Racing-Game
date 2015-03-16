
#ifndef __VEHICLECAMERA_H__
#define __VEHICLECAMERA_H__

#include <OIS.h>
//#include "Util.h"

/**
	赛车缓冲摄像机原理:
		使用两个SceneNode, 一个绑定摄像机,另一个用来绑定代表赛车的节点, 赛车移动的时候
		摄像机的位置始终以一定速度向赛车节点靠近，这个速度跟摄像机与赛车子节点的全局坐标距离成正比.
		并不断跟新两节点位置.

		例如: 在这里每20帧跟新一次两节点坐标(与赛车同步),并更新节点相对位置.

		// 根据当前赛车速度和帧数来设定缓冲大小
		if (isBoundingCamera())
		{
		      float speed = getSpeed() / 30;

		      // 提供刹车时的缓冲
		      if (speed < 1.0f)
		           speed = 1.0f;

		      // 时时跟新摄像机,
		      mVehicleCamer->update(timeSinceLastFrame * speed) ;
		}
*/
using namespace Ogre;
class VehicleCamera
{
public:
	VehicleCamera(const std::string &playerName, Ogre::RenderWindow* win, Ogre::SceneManager* sm);

	~VehicleCamera();

	// 设置摄像机缓冲程度值越大越硬,越小越柔和
	void setTightness ( Real tightness );

	float getTightness ( ) const;

	// 设定绑定节点, cameraNode为赛车上摄像机位置的节点, lookAtNode为赛车上车体的节点
	void setTarget ( Ogre::SceneNode* cameraNode, Ogre::SceneNode* lookAtNode );

	// 手动更新节点位置, 与赛车同步
	void instantUpdate ( );

	void update ( float timeSinceLastFrame );

	bool injectMouseMoved(const OIS::MouseEvent &arg);

	void setCameraNodePitchAndYaw( Ogre::Real deltaYaw, Ogre::Real deltaPitch );

	Ogre::SceneNode* getCameraNode();

	void releaseCam(bool _isRelease);

private:

	Ogre::SceneNode*		mBaseCamerNode;
	Ogre::SceneNode*		mBaseLookedAtNode;

	//当前摄像机节点
	Ogre::SceneNode*		mCameraLookAtNode;
	Ogre::SceneNode*		mCameraNode;

	//#~当Renderable的node发生变化时会出现问题
	Ogre::SceneNode*		mCameraAroundNode;

	// 设置摄像机缓冲程度值越大越硬,越小越柔和
	Ogre::Real				mTightness;
	Ogre::RenderWindow* mWin;
	Ogre::SceneManager* mSm;

	bool				isRelease;
};


#endif



