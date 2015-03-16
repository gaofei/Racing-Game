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
	//��ȡcfg�ļ����ݵ�mStoreMap��
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

	//�����ַ�����ø�ֵ
	Ogre::Real getElement(Ogre::String para_name)
	{
		//#~����δ�������Ҳ����ַ���
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