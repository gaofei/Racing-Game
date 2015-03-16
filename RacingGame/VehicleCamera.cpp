#include "Head.h"
#include "VehicleCamera.h"

//---------------------------------------------------------------------------------------------------------

VehicleCamera::VehicleCamera(const std::string &playerName, Ogre::RenderWindow* win, Ogre::SceneManager* sm) : 
																			mTightness(4.0), 
																			mCameraNode(0),
																			mCameraLookAtNode(0),
																			mBaseLookedAtNode(0),
																			mBaseCamerNode(0),
																			isRelease(false)
{
	mSm = sm;
	mWin = win;

	// �����������Ľڵ�
	mCameraLookAtNode = mSm->getRootSceneNode ()->createChildSceneNode ();
	mCameraNode =mSm->getRootSceneNode ()->createChildSceneNode ();

	mCameraAroundNode = mCameraLookAtNode->createChildSceneNode();

	// ʹ��������Ľڵ��Զ�׷�������ڵ�
	mCameraNode->setAutoTracking ( true, mCameraLookAtNode );
	mCameraAroundNode->setAutoTracking(true, mCameraLookAtNode);

	// ����ֻ����Y����ת,
	mCameraNode->setFixedYawAxis ( true );

	//Util::addCompositor(viewport, "Bloom");
	//Util::setCompositorEnabled(viewport,"Bloom",true);*/
}

//---------------------------------------------------------------------------------------------------------

VehicleCamera::~VehicleCamera()
{
	mCameraNode->detachAllObjects ();

	if (mCameraNode)
	{
		mSm->destroySceneNode ( mCameraNode );
		mCameraNode = NULL;
	}

	if (mCameraAroundNode)
	{
		mSm->destroySceneNode ( mCameraAroundNode );
		mCameraAroundNode = NULL;

	}

}

//---------------------------------------------------------------------------------------------------------

void VehicleCamera::setTightness ( Real tightness )
{
	mTightness = tightness;
}

//---------------------------------------------------------------------------------------------------------

float VehicleCamera::getTightness ( ) const
{
	return mTightness;
}

//---------------------------------------------------------------------------------------------------------

// �趨�󶨽ڵ�, cameraNodeΪ�����������λ�õĽڵ�, lookAtNodeΪ�����ϳ���Ľڵ�
void VehicleCamera::setTarget ( Ogre::SceneNode* cameraNode, Ogre::SceneNode* lookAtNode )
{
	 mBaseCamerNode = cameraNode;
	 mBaseLookedAtNode = lookAtNode;

	instantUpdate ( );
}

//---------------------------------------------------------------------------------------------------------

// �ֶ����½ڵ�λ��, ������ͬ��
void VehicleCamera::instantUpdate ( )
{
	mCameraNode->setPosition ( mBaseCamerNode->_getDerivedPosition() );
	mCameraLookAtNode->setPosition ( mBaseLookedAtNode->_getDerivedPosition() );
	mCameraAroundNode->setPosition( mBaseCamerNode->_getDerivedPosition());
}

//---------------------------------------------------------------------------------------------------------

void VehicleCamera::update ( float timeSinceLastFrame )
{
	Ogre::Vector3 displacement;
	float rate = mTightness * timeSinceLastFrame;

	if(!isRelease)
	{
		if (mCameraNode)
		{
			displacement = ( mBaseCamerNode->_getDerivedPosition() - mCameraNode->_getDerivedPosition() );
			mCameraNode->translate ( displacement  * rate);

			displacement = ( mBaseLookedAtNode->_getDerivedPosition() - mCameraLookAtNode->_getDerivedPosition() );
			mCameraLookAtNode->translate ( displacement  * rate);
			mCameraLookAtNode->setOrientation( mBaseLookedAtNode->_getDerivedOrientation());
		}
	}
	else
	{
		mCameraLookAtNode->setPosition(mBaseLookedAtNode->_getDerivedPosition());

		mCameraNode->setPosition( mCameraAroundNode->_getDerivedPosition());
	}
}

//---------------------------------------------------------------------------------------------------------

bool VehicleCamera::injectMouseMoved(const OIS::MouseEvent &arg)
{
	if(isRelease)
	{
		setCameraNodePitchAndYaw(-0.5f * arg.state.X.rel, -0.5f * arg.state.Y.rel);
		/*Ogre::Vector3 displacement = mBaseLookedAtNode->_getDerivedPosition() - mCameraAroundNode->_getDerivedPosition();*/
		mCameraAroundNode->translate( mCameraAroundNode->getPosition() * arg.state.Z.rel * 0.001f * -0.5f);
	}
	return true;
}

//---------------------------------------------------------------------------------------------------------

void VehicleCamera::setCameraNodePitchAndYaw(Ogre::Real deltaYaw, Ogre::Real deltaPitch)
{
	mCameraLookAtNode->pitch( -Ogre::Degree(deltaPitch));
	mCameraLookAtNode->yaw( (Ogre::Degree(deltaYaw)));
}

//---------------------------------------------------------------------------------------------------------

Ogre::SceneNode* VehicleCamera::getCameraNode()
{
	return mCameraNode;
}

//---------------------------------------------------------------------------------------------------------

void VehicleCamera::releaseCam(bool _isRelease)
{
	isRelease = _isRelease;
}
