#ifndef __AIOBJECT__H__
#define __AIOBJECT__H__

/*
AIʵ����
������
�̳к�
ÿ����Ķ����Ƕ�����AIִ�ж���
���Զ������������ǽ���ʹ��AI����������
*/

class AIObject
{
public:
	AIObject();
	~AIObject();

public:
	virtual void init() = 0;		//��ʵ����뵽AI��������ʱ�����ĳ�ʼ������
	virtual void advance() = 0;		//��AI������ִ�е�AI����
	virtual void exit() = 0;		//��AI�������������ͷŲ���

};

#endif //__AIOBJECT__H__