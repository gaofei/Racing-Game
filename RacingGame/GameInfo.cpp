#include "Head.h"
#include "GameInfo.h"

#include "GameConfig.h"

//---------------------------------------------------------------------------------------------------------

VehicleInfo::VehicleInfo()
{
	//正向TFF
	gLongitudalExtremumSlip = 0.2f;		//滑行极值变量
	gLongitudalExtremumValue = 0.02f;		//极值因变量
	gLongitudalAsymptoteSlip = 2.0f;		//渐近变量
	gLongitudalAsymptoteValue = 0.5f;		//渐近因变量
	gLongitudalStiffnessFactor = 155000.f;	//硬度因子

	//侧面TFF
	gLateralExtremumSlip = 0.04f;		//滑行极值变量
	gLateralExtremumValue = 0.02f;		//极值因变量
	gLateralAsymptoteSlip = 2.0f;		//渐近变量
	gLateralAsymptoteValue = 0.01f;		//渐近因变量
	gLateralStiffnessFactor = 3000.f;	   //硬度因子:车子原位置抓握能力，防滑所添加的力，越大越抓紧但是越容易翻车

	gSuspensionSpring = 0.1f;	//弹簧劲度系数
	gSuspensionDamping = 0.1f;	//弹簧阻尼
	gSuspensionTravel = 0.05f;	//悬挂距离
	gTargetValues = 0.01f;		//零点弹性
	gAngularDamping = 0.f;
	gLinearDamping = 0.f;

	initRealVec();

	gMass = 1500.f;
	gMassLocalPose = Ogre::Vector3(0, -3, 0);
	gSpeed = 90.f;

	gAngle = 30.f;
	gAngleSpeed = 1.5f;
	gIsEasyScene = 0.f;

}

//---------------------------------------------------------------------------------------------------------

void VehicleInfo::loadFromFile(Ogre::String filename)
{
	GameConfig			mConfigFile;
	mConfigFile.loadFromFile(filename);
	GameConfig::ConfigMap cm = mConfigFile.getConfigMap();
	GameConfig::ConfigMap::iterator it;

	//将参数读取到变量中
	unsigned int i = 0;
	for (it=cm.begin();it!=cm.end();it++)
	{
		if(i >= gRealVec.size())
			break;
		*gRealVec[i] = it->second;
		i++;
	}

	gSpeed = it->second;
	it++;
	gAngle = it->second;
	it++;
	gAngleSpeed = it->second;
	it++;
	gIsEasyScene = it->second;


}

//---------------------------------------------------------------------------------------------------------

void VehicleInfo::initRealVec()
{
	gRealVec.clear();
	gRealVec.push_back(&gLongitudalExtremumSlip);
	gRealVec.push_back(&gLongitudalExtremumValue);
	gRealVec.push_back(&gLongitudalAsymptoteSlip);
	gRealVec.push_back(&gLongitudalAsymptoteValue);
	gRealVec.push_back(&gLongitudalStiffnessFactor);

	gRealVec.push_back(&gLateralExtremumSlip);
	gRealVec.push_back(&gLateralExtremumValue);
	gRealVec.push_back(&gLateralAsymptoteSlip);
	gRealVec.push_back(&gLateralAsymptoteValue);
	gRealVec.push_back(&gLateralStiffnessFactor);

	gRealVec.push_back(&gSuspensionSpring);
	gRealVec.push_back(&gSuspensionDamping);
	gRealVec.push_back(&gSuspensionTravel);
	gRealVec.push_back(&gTargetValues);
	gRealVec.push_back(&gAngularDamping);
	gRealVec.push_back(&gLinearDamping);
}

//---------------------------------------------------------------------------------------------------------

std::vector<Ogre::Real*> VehicleInfo::getRealVec()
{
	return gRealVec;
}