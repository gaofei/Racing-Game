
#ifndef __VEHICLECAMERA_H__
#define __VEHICLECAMERA_H__

#include <OIS.h>
//#include "Util.h"

/**
	�������������ԭ��:
		ʹ������SceneNode, һ���������,��һ�������󶨴��������Ľڵ�, �����ƶ���ʱ��
		�������λ��ʼ����һ���ٶ��������ڵ㿿��������ٶȸ�������������ӽڵ��ȫ��������������.
		�����ϸ������ڵ�λ��.

		����: ������ÿ20֡����һ�����ڵ�����(������ͬ��),�����½ڵ����λ��.

		// ���ݵ�ǰ�����ٶȺ�֡�����趨�����С
		if (isBoundingCamera())
		{
		      float speed = getSpeed() / 30;

		      // �ṩɲ��ʱ�Ļ���
		      if (speed < 1.0f)
		           speed = 1.0f;

		      // ʱʱ���������,
		      mVehicleCamer->update(timeSinceLastFrame * speed) ;
		}
*/
using namespace Ogre;
class VehicleCamera
{
public:
	VehicleCamera(const std::string &playerName, Ogre::RenderWindow* win, Ogre::SceneManager* sm);

	~VehicleCamera();

	// �������������̶�ֵԽ��ԽӲ,ԽСԽ���
	void setTightness ( Real tightness );

	float getTightness ( ) const;

	// �趨�󶨽ڵ�, cameraNodeΪ�����������λ�õĽڵ�, lookAtNodeΪ�����ϳ���Ľڵ�
	void setTarget ( Ogre::SceneNode* cameraNode, Ogre::SceneNode* lookAtNode );

	// �ֶ����½ڵ�λ��, ������ͬ��
	void instantUpdate ( );

	void update ( float timeSinceLastFrame );

	bool injectMouseMoved(const OIS::MouseEvent &arg);

	void setCameraNodePitchAndYaw( Ogre::Real deltaYaw, Ogre::Real deltaPitch );

	Ogre::SceneNode* getCameraNode();

	void releaseCam(bool _isRelease);

private:

	Ogre::SceneNode*		mBaseCamerNode;
	Ogre::SceneNode*		mBaseLookedAtNode;

	//��ǰ������ڵ�
	Ogre::SceneNode*		mCameraLookAtNode;
	Ogre::SceneNode*		mCameraNode;

	//#~��Renderable��node�����仯ʱ���������
	Ogre::SceneNode*		mCameraAroundNode;

	// �������������̶�ֵԽ��ԽӲ,ԽСԽ���
	Ogre::Real				mTightness;
	Ogre::RenderWindow* mWin;
	Ogre::SceneManager* mSm;

	bool				isRelease;
};


#endif



