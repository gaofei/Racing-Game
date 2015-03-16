#include "Head.h"

#include "DecalShadow.h"


//---------------------------------------------------------------------------------------------------------

//构造函数，参数1是场景管理器，参数2是要绑定的投影矩阵父节点，即要投影的实体，参数3是子节点的位差，参数4是纹理名
DecalShadow::DecalShadow(Ogre::SceneManager* sm, Ogre::SceneNode* projParentNode, 
						 Ogre::Real offset, Ogre::String texNam, Ogre::Vector2 orthoSize)
{
	m_pSceneManger		= sm;
	m_pDecalFrustum		= new Ogre::Frustum;
	m_pDecalFrustum->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	m_pDecalFrustum->setOrthoWindow(orthoSize.x, orthoSize.y);
	
	m_pProjParentNode	= projParentNode;
	m_pProjectorNode	= m_pSceneManger->getRootSceneNode()->createChildSceneNode();
	m_pProjectorNode->attachObject(m_pDecalFrustum);

	m_fOffset			= offset;

	m_pTransNode		= m_pSceneManger->getRootSceneNode()->createChildSceneNode();

	updataProjectorNode();
	dettachParentDecal();

	m_pDecalFrustum->setQueryFlags(0);

	m_pRaySceneQuery	= sm->createRayQuery(Ogre::Ray());
	mTextureName		= texNam;

	m_pDecalMatPass		= NULL;
}

//---------------------------------------------------------------------------------------------------------

DecalShadow::~DecalShadow()
{
	if (m_pProjectorNode)
	{
		m_pProjectorNode->detachAllObjects();
		m_pSceneManger->destroySceneNode(m_pProjectorNode);
		m_pProjectorNode = NULL;
	}

	if (m_pTransNode)
	{
		m_pTransNode->detachAllObjects();
		m_pSceneManger->destroySceneNode(m_pTransNode);
		m_pTransNode = NULL;
	}

	if (m_pRaySceneQuery)
	{
		delete m_pRaySceneQuery;
		m_pRaySceneQuery = NULL;
	}

	if (m_pDecalMatPass)
	{
		//m_pDecalMatPass->getTextureUnitState(0)->setProjectiveTexturing(false);
		//m_pDecalMatPass->removeAllTextureUnitStates();
		m_pDecalMaterial->getTechnique(0)->removePass(m_pDecalMatPass->getIndex());
		m_pDecalMatPass = NULL;
	}

	if (m_pDecalFrustum)
	{
		delete m_pDecalFrustum;
		m_pDecalFrustum = NULL;
	}
}

//---------------------------------------------------------------------------------------------------------
void DecalShadow::updata()
{	
	updataProjectorNode();

	Ogre::Ray ray(m_pProjectorNode->getPosition(), Ogre::Vector3::NEGATIVE_UNIT_Y);
	m_pRaySceneQuery->setRay(ray);
	m_pRaySceneQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);
	m_pRaySceneQuery->setSortByDistance(true);

	//m_pRaySceneQuery->setWorldFragmentType();
	Ogre::RaySceneQueryResult &resu = m_pRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = resu.begin();
	//itr++;

	while (itr != resu.end())
	{
		Ogre::MovableObject *mo = itr->movable;
		Ogre::Entity *ent = static_cast<Ogre::Entity*>(mo);

		for(unsigned int i = 0; i < ent->getNumSubEntities(); i++)
		{
			Ogre::String matName(ent->getSubEntity(i)->getMaterialName().c_str());

			if(checkIsPassExist(matName))
				continue;

			makeMaterialReceiveDecal(matName);
			m_cExistMaterial.push_back(matName);
		}

		itr++;
	}
}

//---------------------------------------------------------------------------------------------------------

void DecalShadow::updataProjectorNode()
{
	Ogre::Vector3 pos	= m_pProjParentNode->_getDerivedPosition();
	pos.y				+= m_fOffset;
	m_pProjectorNode->_setDerivedPosition(pos);

	Ogre::Vector3 dir = m_pProjParentNode->_getDerivedOrientation().zAxis();
	dir.y			  = 0;

	m_pTransNode->setOrientation(Ogre::Quaternion::IDENTITY);
	m_pTransNode->setDirection(dir);
	m_pTransNode->rotate(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));

	m_pProjectorNode->_setDerivedOrientation(m_pTransNode->getOrientation());
}

//---------------------------------------------------------------------------------------------------------

void DecalShadow::dettachParentDecal()
{
	//被投影的实体上不会产生阴影

	for (int i=0;i<m_pProjParentNode->numChildren();i++)
	{
		Ogre::SceneNode* sn = static_cast<Ogre::SceneNode*>(m_pProjParentNode->getChild(i));
		for (int j=0;j<sn->numAttachedObjects();j++)
		{
			Ogre::Entity* ent = static_cast<Ogre::Entity*>(sn->getAttachedObject(j));
			ent->setQueryFlags(0);
		}
	}

}

//---------------------------------------------------------------------------------------------------------

bool DecalShadow::checkIsPassExist(Ogre::String matName)
{
	for (unsigned int i=0;i<m_cExistMaterial.size();i++)
	{
		if(m_cExistMaterial[i] == matName)
			return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------------------------

void DecalShadow::makeMaterialReceiveDecal(Ogre::String matName)
{
	m_pDecalMaterial = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(matName);
	m_pDecalMatPass = m_pDecalMaterial->getTechnique(0)->createPass();
	m_pDecalMatPass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	m_pDecalMatPass->setDepthBias(1);
	//pass->setLightingEnabled(false);

	Ogre::TextureUnitState *texState = m_pDecalMatPass->createTextureUnitState(mTextureName);
	texState->setProjectiveTexturing(true, m_pDecalFrustum);
	texState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	texState->setTextureFiltering(Ogre::FO_POINT, Ogre::FO_LINEAR, Ogre::FO_NONE);

	//texState = pass->createTextureUnitState("decal_filter.png");
	//texState->setProjectiveTexturing(true, mFilterFrustum);
	//texState->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	//texState->setTextureFiltering(TFO_NONE);
}