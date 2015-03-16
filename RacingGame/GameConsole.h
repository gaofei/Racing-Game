#ifndef __GAMECONSOLE_H__
#define __GAMECONSOLE_H__

//#include <windows.h>
#include "Vehicle.h"
/*
��Ϸ����̨������
*/

class GameConsole
{

private:
	GameConsole(){}

public:
	static GameConsole* getSingleton();
	
	//��ʼ��
	//����_vehicle���ڻ�ȡ���ĳ�ʼ��Ϣ�����Բ���
	void init(Vehicle* _vehicle = NULL);

	//���¸ó���VehicleInfo����
	void updata(Vehicle* _vehicle);

	//����Ƿ���յ�������
	bool isReceiveData(){return isReceive;}

private:
	//�����̼߳���GameCosole����
	void listen();

	//��������
	void receive();

	static DWORD WINAPI threadPro(LPVOID arg);

	static void wrapToFloatArray(float* float_ar);
	static void wrapFromFloatArray(float* float_ar);

private:

	static bool							isReceive;

	static VehicleInfo						mVehicleInfo;

	static DWORD							mThreadID;

public:

	static GameConsole*				mGameConsole;

	static const int						mDataSize;
};

#endif