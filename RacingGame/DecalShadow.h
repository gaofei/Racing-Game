#ifndef __DECALSHADOW__H__
#define __DECALSHADOW__H__


/*
贴花阴影类
要创建new一个实例
每一帧调用updata即可
*/

class DecalShadow
{
public:
	DecalShadow(Ogre::SceneManager* sm, Ogre::SceneNode* projParentNode, 
		Ogre::Real offset, Ogre::String texName, Ogre::Vector2 orthoSize);
	~DecalShadow();

	void updata();
	void updataProjectorNode();

	void dettachParentDecal();

protected:

	bool checkIsPassExist(Ogre::String matName);

	void makeMaterialReceiveDecal(Ogre::String matName);

public:

	Ogre::SceneManager*				m_pSceneManger;

	Ogre::Frustum*					m_pDecalFrustum;

	Ogre::MaterialPtr				m_pDecalMaterial;

	Ogre::Pass*						m_pDecalMatPass;

	Ogre::SceneNode*				m_pProjectorNode;

	Ogre::SceneNode*				m_pProjParentNode;

	Ogre::SceneNode*				m_pTransNode;

	Ogre::Real						m_fOffset;

	Ogre::RaySceneQuery*			m_pRaySceneQuery;

	Ogre::String					mTextureName;

	std::vector<Ogre::String>		m_cExistMaterial;
};

#endif