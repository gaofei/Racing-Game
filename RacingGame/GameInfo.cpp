#include "Head.h"
#include "GameInfo.h"

#include "GameConfig.h"

//---------------------------------------------------------------------------------------------------------

VehicleInfo::VehicleInfo()
{
	//����TFF
	gLongitudalExtremumSlip = 0.2f;		//���м�ֵ����
	gLongitudalExtremumValue = 0.02f;		//��ֵ�����
	gLongitudalAsymptoteSlip = 2.0f;		//��������
	gLongitudalAsymptoteValue = 0.5f;		//���������
	gLongitudalStiffnessFactor = 155000.f;	//Ӳ������

	//����TFF
	gLateralExtremumSlip = 0.04f;		//���м�ֵ����
	gLateralExtremumValue = 0.02f;		//��ֵ�����
	gLateralAsymptoteSlip = 2.0f;		//��������
	gLateralAsymptoteValue = 0.01f;		//���������
	gLateralStiffnessFactor = 3000.f;	   //Ӳ������:����ԭλ��ץ����������������ӵ�����Խ��Խץ������Խ���׷���

	gSuspensionSpring = 0.1f;	//���ɾ���ϵ��
	gSuspensionDamping = 0.1f;	//��������
	gSuspensionTravel = 0.05f;	//���Ҿ���
	gTargetValues = 0.01f;		//��㵯��
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

	//��������ȡ��������
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