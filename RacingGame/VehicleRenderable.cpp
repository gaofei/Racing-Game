#include "Head.h"
#include "VehicleRenderable.h"

//---------------------------------------------------------------------------------------------------------

VehicleRenderable::VehicleRenderable(Ogre::SceneManager* sm, Ogre::SceneNode* sn)
{
	mSceneNodes.clear();
	mEntities.clear();
	mSceneMgr	= sm;
	mAttachNode = sn;
}

//---------------------------------------------------------------------------------------------------------

VehicleRenderable::~VehicleRenderable()
{
	destroy();

	mSceneMgr	= NULL;
	mAttachNode = NULL;
}

//---------------------------------------------------------------------------------------------------------

bool VehicleRenderable::load(Ogre::String fileName, Ogre::String groupName)
{
	//open resource from exist group if specify group or file does not exist
	Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(fileName, groupName);
	
	//create a tinyXml object by stream
	TinyXml::TiXmlDocument* doc = new TinyXml::TiXmlDocument;
	try
	{
		doc->Parse(stream->getAsString().c_str());
	}
	catch (...)
	{
		Ogre::LogManager::getSingleton().
			logMessage("VehicleRenderable Error : Parse <" + fileName + "> fail!");
		delete doc;
		return false;
	}

	//check if scene file can parse
	Ogre::String str = Ogre::String(doc->RootElement()->Value());
	if (str != "scene")
	{
		Ogre::LogManager::getSingleton().
			logMessage("VehicleRenderable Error : <" + fileName + "> is not a scene file!");
		delete doc;
		return false;
	}

	TinyXml::TiXmlElement* ele;

	// process scene node
	ele = doc->RootElement()->FirstChildElement("nodes");
	if (!loadSceneNodes(ele))
	{
		Ogre::LogManager::getSingleton().
			logMessage("VehicleRenderable Error : errors occurred in parsing nodes!");
		delete doc;
		return false;
	}

	Ogre::LogManager::getSingleton().
		logMessage("VehicleRenderable succeed!");

	delete doc;
	return true;
}

//---------------------------------------------------------------------------------------------------------

bool VehicleRenderable::destroy()
{
	while(!mEntities.empty())
	{
		Ogre::Entity* ent = mEntities.back();

		//detach and remove all entities the object created
		ent->detachFromParent();
		mSceneMgr->destroyEntity(ent);
		mEntities.pop_back();
	}

	while(!mSceneNodes.empty())
	{
		Ogre::SceneNode* sn = mSceneNodes.back();
		
		//detach and remove all scene node only the object created
		sn->detachAllObjects();
		mSceneMgr->destroySceneNode(sn);
		mSceneNodes.pop_back();
	}

	return true;
}

//---------------------------------------------------------------------------------------------------------

bool VehicleRenderable::loadSceneNodes(TinyXml::TiXmlElement* ele)
{
	TinyXml::TiXmlElement* nodeEle = ele->FirstChildElement("node");
	while (nodeEle)
	{
		Ogre::String name = getAttrib(nodeEle, "name");
		Ogre::SceneNode* node = NULL;

		if (name.empty())
			node = mAttachNode->createChildSceneNode();
		else
			node = mAttachNode->createChildSceneNode(name);

		mSceneNodes.push_back(node);

		Ogre::LogManager::getSingleton().
			logMessage("VehicleRenderable : parsing <" + name + "> node...");

		//process position
		TinyXml::TiXmlElement* pos = nodeEle->FirstChildElement("position");
		node->setPosition(parseVector3(pos));
		node->setInitialState();

		//process scale
		TinyXml::TiXmlElement* scale = nodeEle->FirstChildElement("scale");
		node->setScale(parseVector3(scale));
		node->setInitialState();

		//process rotate
		TinyXml::TiXmlElement* rotate = nodeEle->FirstChildElement("rotation");
		node->setOrientation(parseQuaternion(rotate));
		node->setInitialState();

		//process entities
		TinyXml::TiXmlElement* entity = nodeEle->FirstChildElement("entity");
		while (entity)
		{
			if (!loadEntity(entity, node))
				return false;

			entity = entity->NextSiblingElement("entity");
		}

		nodeEle = nodeEle->NextSiblingElement("node");
	}

	return true;
}

//---------------------------------------------------------------------------------------------------------

bool VehicleRenderable::loadEntity(TinyXml::TiXmlElement* ele, Ogre::SceneNode* parent)
{
	Ogre::String name			= getAttrib(ele, "name");
	Ogre::String castShadow		= getAttrib(ele, "castShadows");
	Ogre::String receiveShadow	= getAttrib(ele, "receiveShadows");
	Ogre::String meshFile		= getAttrib(ele, "meshFile");
	Ogre::String materialFile	= getAttrib(ele, "materialFile");

	try
	{
		Ogre::Entity* ent = mSceneMgr->createEntity(name, meshFile);
		mEntities.push_back(ent);

		ent->setCastShadows(Ogre::StringConverter::parseBool(castShadow));

		if (!materialFile.empty())
			ent->setMaterialName(materialFile);

		parent->attachObject(ent);
	}
	catch (...)
	{
		Ogre::LogManager::getSingleton().logMessage("VehicleRenderable Error : parsing <" + name + "> error!");
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------------------------------------

Ogre::Vector3 VehicleRenderable::parseVector3(TinyXml::TiXmlElement *XMLNode)
{
	return Ogre::Vector3(
		Ogre::StringConverter::parseReal(XMLNode->Attribute("x")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("y")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("z"))
		);
}

//---------------------------------------------------------------------------------------------------------

Ogre::Quaternion VehicleRenderable::parseQuaternion(TinyXml::TiXmlElement *XMLNode)
{
	return Ogre::Quaternion(
		Ogre::StringConverter::parseReal(XMLNode->Attribute("qw")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("qx")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("qy")),
		Ogre::StringConverter::parseReal(XMLNode->Attribute("qz"))
		);
}

//---------------------------------------------------------------------------------------------------------

Ogre::String VehicleRenderable::getAttrib(TinyXml::TiXmlElement* ele, const Ogre::String& des)
{
	const Ogre::String* str = ele->Attribute(des);
	if (str)
		return *str;

	Ogre::String blank = "";
	return blank;
}