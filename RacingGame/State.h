#ifndef __STATE_H__
#define __STATE_H__

//#include "GlobalManager.h"

/*
��ʾ����״̬�Ļ���
�̳��Ա�д��״̬�����д���
�޷�ʵ��������
*/

class State
{
	
public:
	enum RUN_STATE{INIT, LOOP};

	State():mRunState(INIT), isInit(false){}
	//״̬����
	virtual void run(Ogre::RenderWindow* win);

	//״̬�����ͼ����¼�
	virtual bool mouseMoved( const OIS::MouseEvent &e ) { return true; }
	virtual bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )  { return true; }
	virtual bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )  { return true; }

	virtual bool keyPressed( const OIS::KeyEvent &e )  { return true; }
	virtual bool keyReleased( const OIS::KeyEvent &e )  { return true; }


	//���õ�ǰ����״̬
	virtual void setRunState(RUN_STATE rs) { mRunState = rs; }

	//ÿ��״̬�������̶ֹ�����״̬
	//�ֱ��ǳ�ʼ����ѭ���ͽ�������
	//����ʹ��setRunState()�����ǵ���������������

	//��ʼ����
	virtual void init() = 0; 

	//ѭ��
	virtual void loop() = 0;

	//��������
	virtual void exit() = 0;

public:
	bool isInit;

protected:
	RUN_STATE mRunState;
	Ogre::RenderWindow* mWindow;
};

#endif