#ifndef _VEHICLECONFIG_H
#define _VEHICLECONFIG_H

#include <map>
#include <string>
#include <Ogre.h>

class GameConfig
{

public:
	typedef std::map<std::string, Ogre::Real> ConfigMap;

public:
	//读取cfg文件数据到mStoreMap中
	void loadFromFile(Ogre::String filename)
	{
		mConfigFile.load(filename);

		Ogre::ConfigFile::SectionIterator seci = mConfigFile.getSectionIterator();
		Ogre::String parName, parValue;
		Ogre::Real realValue;
		while (seci.hasMoreElements())
		{
			seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				parName = i->first;
				parValue = i->second;
				realValue = Ogre::StringConverter::parseReal(parValue);
				mStoreMap.insert(std::pair<Ogre::String, Ogre::Real>(parName, realValue));
			}
		}
	}

	//利用字符串获得该值
	Ogre::Real getElement(Ogre::String para_name)
	{
		//#~错误未处理，若找不到字符串
		if(mStoreMap.find(para_name) != mStoreMap.end())
		{
			ConfigMap::iterator it = mStoreMap.find(para_name);
			Ogre::Real para_value;
			para_value = it->second;
			return para_value;
		}
	}

	ConfigMap getConfigMap()
	{
		return mStoreMap;
	}

private:
	Ogre::ConfigFile mConfigFile;
	ConfigMap mStoreMap;
};

#endif