#ifndef __VEHICLEPARAM__H__
#define __VEHICLEPARAM__H__

#include <NxWheelShapeDesc.h>
#include <string>
#include <iostream>

// 车的性能参数struct

struct VehicleParamStruct
{

	//前轮参数
	NxTireFunctionDesc FrontLongtitudal;
	NxTireFunctionDesc FrontLateral;

	NxSpringDesc FrontSuspension;
	float FrontSuspensionTravel;

	//后轮参数
	NxTireFunctionDesc RearLongtitudal;
	NxTireFunctionDesc RearLateral;

	NxSpringDesc RearSuspension;
	float RearSuspensionTravel;


	float	AngularDamping,
			LinearDamping;

	float	MotorTorque,
			SteerAngleRange,
			SteerSpeed;

	bool isValid()
	{
		bool bIsValid = false;
		bIsValid  = FrontLongtitudal.isValid() && FrontLateral.isValid() && FrontSuspension.isValid();
		bIsValid  = bIsValid && RearLongtitudal.isValid() && RearLateral.isValid() && RearSuspension.isValid();
		return bIsValid;
	}
};

class VehicleParam
{
public:
	VehicleParam();
public:

	bool setVehicleParam(VehicleParamStruct vps);
	bool loadFromFile(std::string fileName);
	bool saveToFile(std::string fileName);
	bool isValid();

protected:
	void cutSpace(std::string& str);
	bool extractValue(std::string str, std::string identifyName, float& outValue);
	bool readLine(std::ifstream& ifs, std::string identifyName, float& outValue);

	float& getTTF(int isFront, int isLongtitudal, int index);

public:
	//读取该值可以直接去成员
	//但是写该值必须使用setVehicleParam()
	VehicleParamStruct m_sVehicleParam;
};

#endif