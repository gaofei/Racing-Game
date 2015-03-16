#ifndef __VEHICLERENDERABLE__H__
#define __VEHICLERENDERABLE__H__

#include "tinyxml/tinyxml.h"

/*
赛车可视体类
读取.vrf文件
负责读取SceneNode、Entity
只支持一层子节点的读取
不支持SubEntity的脚本读取
*/

class VehicleRenderable
{

public:

	VehicleRenderable(Ogre::SceneManager* sm, Ogre::SceneNode* sn);
	~VehicleRenderable();

	bool load(Ogre::String fileName, 
		Ogre::String groupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	bool destroy();

protected:

	bool loadSceneNodes(TinyXml::TiXmlElement* ele);

	bool loadEntity(TinyXml::TiXmlElement* ele, Ogre::SceneNode* parent);

	Ogre::Vector3 parseVector3(TinyXml::TiXmlElement* ele);

	Ogre::Quaternion parseQuaternion(TinyXml::TiXmlElement* ele);

	Ogre::String getAttrib(TinyXml::TiXmlElement* ele, const Ogre::String& des);

protected:

	Ogre::SceneManager*				mSceneMgr;
	Ogre::SceneNode*				mAttachNode;
	std::vector<Ogre::SceneNode*>	mSceneNodes;
	std::vector<Ogre::Entity*>		mEntities;
};

#endif