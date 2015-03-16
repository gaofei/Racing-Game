#include "AIManager.h"

AIManager* AIManager::m_pAIManager = NULL;

//-------------------------------------------------------------------------------------

AIManager::AIManager()
{
	m_pAIManager = this;
}

//-------------------------------------------------------------------------------------

AIManager::~AIManager()
{
	m_pAIManager = NULL;
}

//-------------------------------------------------------------------------------------

AIManager* AIManager::getSingleton()
{
	if (!m_pAIManager)
		m_pAIManager = new AIManager();
	return m_pAIManager;
}

//-------------------------------------------------------------------------------------

void AIManager::addAIObject(AIObject* pAIObject)
{
	if (!pAIObject)
		return;

	m_pAIObjectVector.push_back(pAIObject);
	pAIObject->init();
}

//-------------------------------------------------------------------------------------

void AIManager::deleteAIObject(AIObject* pAIObject)
{
	if (m_pAIObjectVector.empty())
		return;

	for (int i=0;i<m_pAIObjectVector.size();i++)
	{
		if (pAIObject == m_pAIObjectVector[i])
		{
			pAIObject->exit();
			m_pAIObjectVector.erase(m_pAIObjectVector.begin() + i);
			break;
		}
	}
}

//-------------------------------------------------------------------------------------

void AIManager::deleteAIObject(int index)
{
	if (index > m_pAIObjectVector.size() - 1)
		return;

	m_pAIObjectVector[index]->exit();
	m_pAIObjectVector.erase(m_pAIObjectVector.begin() + index);
}

//-------------------------------------------------------------------------------------

void AIManager::cleanAll()
{
	m_pAIObjectVector.clear();
}

//-------------------------------------------------------------------------------------

void AIManager::run()
{
	for (int i=0;i<m_pAIObjectVector.size();i++)
	{
		m_pAIObjectVector[i]->advance();
	}
}