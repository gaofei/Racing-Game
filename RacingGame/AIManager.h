#ifndef __AIMANAGER__H__
#define __AIMANAGER__H__

#include <vector>
#include "AIObject.h"

/*
AI������
ʵ��������AI�ĳ�ʼ����ִ��
�ݶ�Ϊ������
*/

class AIManager
{
protected:
	AIManager();
	~AIManager();

public:
	static AIManager* getSingleton();

	void addAIObject(AIObject* pAIObject);
	void deleteAIObject(AIObject* pAIObject);
	void deleteAIObject(int index);
	void cleanAll();

	void run();

protected:

	static AIManager*		m_pAIManager;

	std::vector<AIObject*>	m_pAIObjectVector;
};


#endif //__AIMANAGER__H__