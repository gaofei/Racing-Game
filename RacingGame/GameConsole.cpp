#include "Head.h"
#include "GameConsole.h"

GameConsole* GameConsole::mGameConsole = 0;
bool GameConsole::isReceive = false;
DWORD GameConsole::mThreadID = 0;
VehicleInfo GameConsole::mVehicleInfo;
const int GameConsole::mDataSize = 128;

//---------------------------------------------------------------------------------------------------------

GameConsole* GameConsole::getSingleton()
{
	if(!mGameConsole)
		mGameConsole = new GameConsole();
	return mGameConsole;
}

//---------------------------------------------------------------------------------------------------------

void GameConsole::init(Vehicle* _vehicle)
{
	if (_vehicle)
	{
		/*mVehicleInfo = */
		_vehicle->getVehicleInfo(mVehicleInfo);
		/*mVehicleInfo.initRealVec();*/
	}

	listen();
}

//---------------------------------------------------------------------------------------------------------

void GameConsole::updata(Vehicle* _vehicle)
{
	if (isReceiveData())
	{
		_vehicle->setVehicleInfo(mVehicleInfo);
		_vehicle->resetCarParameter();
	}
}

//---------------------------------------------------------------------------------------------------------

void GameConsole::listen()
{

	HANDLE hTh = CreateThread(0, 0, &GameConsole::threadPro, (LPVOID)0, 0, &mThreadID);
	CloseHandle(hTh);
}

//---------------------------------------------------------------------------------------------------------

DWORD WINAPI  GameConsole::threadPro(LPVOID arg)
{
	HANDLE hNamePipe = CreateNamedPipe("\\\\.\\Pipe\\RC",PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 1, 0, 0, 1000, NULL );

	while (true)
	{
		ConnectNamedPipe(hNamePipe, NULL);

		char recv_data[mDataSize];
		DWORD recv_size;
		float recv_data_f[mDataSize/4];

		//将当前数据发送给控制台
		wrapToFloatArray(recv_data_f);
		memcpy(recv_data, recv_data_f, sizeof(recv_data_f));
		WriteFile(hNamePipe, recv_data, sizeof(recv_data), &recv_size, 0);

		while (true)
		{
			//循环接收
			bool isOutline = ReadFile(hNamePipe, recv_data, sizeof(recv_data), &recv_size, 0);
			if(!isOutline)
				break;

			memcpy(recv_data_f, recv_data, sizeof(recv_data));
			wrapFromFloatArray(recv_data_f);

			isReceive = true;
		}
	}
}

//---------------------------------------------------------------------------------------------------------

void GameConsole::wrapToFloatArray(float* float_ar)
{
	for (int i=0;i<mDataSize/4;i++)
	{
		if (i >= mVehicleInfo.getRealVec().size())
			break;
		float_ar[i] = *mVehicleInfo.getRealVec()[i];
	}
}

//---------------------------------------------------------------------------------------------------------

void GameConsole::wrapFromFloatArray(float* float_ar)
{
	for (int i=0;i<mDataSize/4;i++)
	{
		if (i >= mVehicleInfo.getRealVec().size())
			break;
		*mVehicleInfo.getRealVec()[i] = float_ar[i];
	}
}

//---------------------------------------------------------------------------------------------------------