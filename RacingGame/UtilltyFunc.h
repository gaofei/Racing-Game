#ifndef __UTILITYFUNCTION__H__
#define __UTILITYFUNCTION__H__

class UtilityFunc
{
public:
	//vec3从ogre到nx的转换
	static NxVec3 OgreVec3_To_NxVec3(const Ogre::Vector3 &ogre_vec3)
	{
		NxVec3 nxvec3(ogre_vec3.x, ogre_vec3.y, ogre_vec3.z);
		return nxvec3;
	}

	inline static Ogre::Vector3 NxVec3_To_OgreVec3(const NxVec3 &nxvec3)
	{
		Ogre::Vector3 ogre_vec3(nxvec3.x, nxvec3.y, nxvec3.z);
		return ogre_vec3;
	}

	inline static NxQuat OgreQuat_To_NxQuat(const Ogre::Quaternion &ogre_quat)
	{
		NxQuat nxquat;
		nxquat.setWXYZ(ogre_quat.w, ogre_quat.x, ogre_quat.y, ogre_quat.z);
		return nxquat;
	}

	inline static Ogre::Quaternion NxQuat_ToOgre_Quat(const NxQuat &nxquat)
	{
		Ogre::Quaternion ogre_quat(nxquat.w, nxquat.x, nxquat.y, nxquat.z);
		return ogre_quat;
	}

	static void elapseTick()
	{
		Ogre::Real deltaTime, curTime;
		curTime		= Ogre::Root::getSingleton().getTimer()->getMilliseconds() * 0.001f;
		deltaTime	= curTime - mLastElapse;

		mLastElapse = curTime;
		mElapseTime = deltaTime;
	}

	//获得时间差
	static Ogre::Real getElapseLastFrame()
	{
		if(!mElapseTime)
			mElapseTime = 0.1f;
		return mElapseTime;
	}

	//全局坐标转局部坐标2D
	inline static Ogre::Vector2 GlobalAxis_To_LocalAxis2D(Ogre::Vector2 axisPos, Ogre::Vector2 pointPos, 
														Ogre::Radian sita)
	{
		Ogre::Vector2 dirPos2D	= pointPos - axisPos;
		Ogre::Real locX			= dirPos2D.x * Ogre::Math::Cos(sita) + dirPos2D.y * Ogre::Math::Sin(sita);
		Ogre::Real locY			= -dirPos2D.x * Ogre::Math::Sin(sita) + dirPos2D.y * Ogre::Math::Cos(sita);

		return Ogre::Vector2(locX, locY);
	}

	static void showFPS(Ogre::RenderWindow* win);

	static Ogre::SceneManager* mSceneManger;

	static Ogre::Real mLastElapse;

	static Ogre::Real mElapseTime;
};

#endif