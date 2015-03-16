#ifndef __GAMECONSOLE_H__
#define __GAMECONSOLE_H__

//#include <windows.h>
#include "Vehicle.h"
/*
游戏控制台工具类
*/

class GameConsole
{

private:
	GameConsole(){}

public:
	static GameConsole* getSingleton();
	
	//初始化
	//参数_vehicle用于获取车的初始信息，可以不填
	void init(Vehicle* _vehicle = NULL);

	//更新该车的VehicleInfo数据
	void updata(Vehicle* _vehicle);

	//检测是否接收到新数据
	bool isReceiveData(){return isReceive;}

private:
	//创建线程监听GameCosole窗口
	void listen();

	//接收数据
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