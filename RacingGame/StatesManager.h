#ifndef __STATEMANGER_H__
#define __STATEMANGER_H__

#include <vector>
#include "State.h"

/*
״̬��������
*/

class StatesManager
{
public:
	StatesManager(){}
	StatesManager(Ogre::RenderWindow* win);
	~StatesManager();

	static StatesManager* getSingleton();

	//״̬��ת
	bool run();

	//ע���������ͼ����¼�
	bool injectMouseMoved( const OIS::MouseEvent &e );
	bool injectMousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
	bool injectMouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

	bool injectKeyPressed( const OIS::KeyEvent &e );
	bool injectKeyReleased( const OIS::KeyEvent &e );


	//�ı䵱ǰ״̬
	void changeCurrentState(State* pState);

	//���ã���ʼ������ǰ״̬
	void resetCurrentState();

	//�ڵ�ǰ״̬�����½���״̬
	void buildNewState(State* pState);

	//������һ��״̬�������ֳ���
	void resumeLastState();

	//�������״̬�������ر�
	void clearAllState();

	//����������
	void attachWindow(Ogre::RenderWindow* win);

public:

	static StatesManager* mStateManger;

private:

	std::vector<State*>			mStateStack;

	Ogre::RenderWindow*			 mWindow;
};

#endif