#include "Head.h"
#include "VehicleParam.h"

VehicleParam::VehicleParam()
{
	m_sVehicleParam.LinearDamping = 0.0f;
	m_sVehicleParam.AngularDamping = 0.05f;
	m_sVehicleParam.MotorTorque = 90.f;
	m_sVehicleParam.SteerAngleRange = 30.f;
	m_sVehicleParam.SteerSpeed = 1.f;

	m_sVehicleParam.FrontLongtitudal.setToDefault();
	m_sVehicleParam.FrontLateral.setToDefault();
	m_sVehicleParam.FrontSuspension.setToDefault();
	m_sVehicleParam.FrontSuspensionTravel = 0.5f;

	m_sVehicleParam.RearLongtitudal.setToDefault();
	m_sVehicleParam.RearLateral.setToDefault();
	m_sVehicleParam.RearSuspension.setToDefault();
	m_sVehicleParam.RearSuspensionTravel = 0.5f;
}

bool VehicleParam::setVehicleParam(VehicleParamStruct vps)
{

	if (!vps.isValid())
		return false;

	m_sVehicleParam = vps;
	return true;
}

bool VehicleParam::isValid()
{
	return m_sVehicleParam.isValid();
}

void VehicleParam::cutSpace(std::string& str)
{
	//cut space character which start with nonspace,end with space 
	int left = str.find_first_not_of(" ");
	int right = str.find(" ", left);

	if (right == -1)
		str = str.substr(left);
	else
		str = str.substr(left, right-left);
}

bool VehicleParam::extractValue(std::string str, std::string identifyName, float& outValue)
{
	//identify value name and extract its value to float
	//name and value split by '='
	//return false if value name differ from indentifyName

	int mid = str.find("=");
	std::string name = str.substr(0, mid);
	std::string val = str.substr(mid+1);

	cutSpace(name);
	cutSpace(val);

	if (name != identifyName)
		return false;

	outValue = (float)atof(val.c_str());

	return true;
}

bool VehicleParam::readLine(std::ifstream& ifs, std::string identifyName, float& outValue)
{
	std::string str;
	do 
	{
		std::getline(ifs, str);
		if (ifs.eof())
			return false;

	} while (str.empty());//jump over the null line

	return extractValue(str, identifyName, outValue);
}

bool VehicleParam::loadFromFile(std::string fileName)
{
	std::ifstream ifs(fileName.c_str());
	if (ifs.bad())
		return false;

	//std paramer

	if (!readLine(ifs, "LinearDamping", m_sVehicleParam.LinearDamping))
		return false;
	if (!readLine(ifs, "AngularDamping", m_sVehicleParam.AngularDamping))
		return false;;
	if (!readLine(ifs, "MotorTorque", m_sVehicleParam.MotorTorque))
		return false;;
	if (!readLine(ifs, "SteerAngleRange", m_sVehicleParam.SteerAngleRange))
		return false;;
	if (!readLine(ifs, "SteerSpeed", m_sVehicleParam.SteerSpeed))
		return false;;

	std::string str;
	for (int i=0;i<2;i++)
	{
		str = i==0 ? "Front" : "Rear";
		for (int j=0;j<2;j++)
		{
			for (int k=0;k<5;k++)
			{
				std::string ttfStr = j==0 ? "LNG" : "LAT";
				switch(k)
				{
				case 0:
					ttfStr += "extremumSlip";
					break;
				case 1:
					ttfStr += "extremumValue";
					break;
				case 2:
					ttfStr += "asymptoteSlip";
					break;
				case 3:
					ttfStr += "asymptoteValue";
					break;
				case 4:
					ttfStr += "stiffnessFactor";
				}
				if ( !readLine(ifs, str + ttfStr, getTTF(i, j, k)))
					return false;
			}
		}

		//deal with suspension

		NxSpringDesc& sup = i == 0 ? m_sVehicleParam.FrontSuspension:
									m_sVehicleParam.RearSuspension;
		if (!readLine(ifs, str+"SuspensionSpring", sup.spring))
			return false;
		if (!readLine(ifs, str+"SuspensionDamping", sup.damper))
			return false;
		if (!readLine(ifs, str+"SuspensionTargetValue", sup.targetValue))
			return false;

		float& supTra = i==0 ? m_sVehicleParam.FrontSuspensionTravel:
								m_sVehicleParam.RearSuspensionTravel;
		if (!readLine(ifs, str+"SuspensionTravel", supTra))
			return false;
	}

	ifs.close();

	return true;
}

bool VehicleParam::saveToFile(std::string fileName)
{

	std::ofstream ofs(fileName.c_str());
	if (ofs.bad())
		return false;

	std::string content;

	char line[40];
	//std paramer
	sprintf_s(line, "LinearDamping=%f\n", m_sVehicleParam.LinearDamping);
	content.append(line);
	sprintf_s(line, "AngularDamping=%f\n", m_sVehicleParam.AngularDamping);
	content.append(line);
	sprintf_s(line, "MotorTorque=%f\n", m_sVehicleParam.MotorTorque);
	content.append(line);
	sprintf_s(line, "SteerAngleRange=%f\n", m_sVehicleParam.SteerAngleRange);
	content.append(line);
	sprintf_s(line, "SteerSpeed=%f\n", m_sVehicleParam.SteerSpeed);
	content.append(line);
	content.append("\n");

	//front/rear curve paramer

	std::string str;
	for (int i=0;i<2;i++)
	{
		str = i==0 ? "Front" : "Rear";
		for (int j=0;j<2;j++)
		{
			for (int k=0;k<5;k++)
			{
				char val[40];
				switch(k)
				{
				case 0:
					sprintf_s(val, "extremumSlip=%f\n", getTTF(i, j, k));
					break;
				case 1:
					sprintf_s(val, "extremumValue=%f\n", getTTF(i, j, k));
					break;
				case 2:
					sprintf_s(val, "asymptoteSlip=%f\n", getTTF(i, j, k));
					break;
				case 3:
					sprintf_s(val, "asymptoteValue=%f\n", getTTF(i, j, k));
					break;
				case 4:
					sprintf_s(val, "stiffnessFactor=%f\n", getTTF(i, j, k));
					break;
				}
				std::string ttfStr = j==0 ? "LNG" : "LAT";
				content.append(str + ttfStr + std::string(val));
			}

			content.append("\n");
		}

		//deal with suspension
		NxSpringDesc sup = i==0 ? m_sVehicleParam.FrontSuspension :
									m_sVehicleParam.RearSuspension;

		sprintf_s(line, "SuspensionSpring=%f\n", sup.spring);
		content.append(str + std::string(line));
		sprintf_s(line, "SuspensionDamping=%f\n", sup.damper);
		content.append(str + std::string(line));
		sprintf_s(line, "SuspensionTargetValue=%f\n", sup.targetValue);
		content.append(str + std::string(line));

		float supTra = i==0 ? m_sVehicleParam.FrontSuspensionTravel:
								m_sVehicleParam.RearSuspensionTravel;
		sprintf_s(line, "SuspensionTravel=%f\n", supTra);
		content.append(str + std::string(line));

		content.append("\n");
	}

	ofs.write(content.c_str(), content.length());
	ofs.close();

	return true;
}

float& VehicleParam::getTTF(int isRear, int isLateral, int index)
{

	//receive TireFunction member from specify boolean isFrong & isLongtitudal
	//with index

	NxTireFunctionDesc* pTFD = NULL;
	if (isRear)
	{
		pTFD = isLateral ?
		&m_sVehicleParam.RearLateral : &m_sVehicleParam.RearLongtitudal;
	}
	else
	{
		pTFD = isLateral ?
		&m_sVehicleParam.FrontLateral : &m_sVehicleParam.FrontLongtitudal;
	}

	NxTireFunctionDesc& fd = *pTFD;

	switch(index)
	{
	case 0:
		return fd.extremumSlip;
		break;
	case 1:
		return fd.extremumValue;
		break;
	case 2:
		return fd.asymptoteSlip;
		break;
	case 3:
		return fd.asymptoteValue;
		break;
	case 4:
		return fd.stiffnessFactor;
		break;;
	default:
		assert(0);//it's a wrong index
		return fd.extremumSlip;
	}
}