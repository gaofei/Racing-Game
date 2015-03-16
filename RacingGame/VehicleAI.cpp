#include "Head.h"
#include "VehicleAI.h"

#include "UtilltyFunc.h"

//---------------------------------------------------------------------------------------------------------

VehicleAI::VehicleAI()
{
	m_eVehicleState		= STAY;
	m_eVehicleDirction	= MIDDLE;
	m_ePredatorMode		= DEGREE;
	m_iNavigationRest	= 0;
	m_iNavigationCurrent	= 0;
	m_fPreciDistance		= 1.2f;
	m_cPreciDegree			= Ogre::Degree(2.5f);

	m_pPredator			= NULL;
	m_cOrbit			= Ogre::Vector2::ZERO;
	m_cLastTarget		= Ogre::Vector2::ZERO;
	m_cDirect			= Ogre::Vector2::ZERO;
	m_bIsFirst			= true;
	m_iLap				= 0;
}

//---------------------------------------------------------------------------------------------------------

VehicleAI::~VehicleAI()
{

}

//---------------------------------------------------------------------------------------------------------

void VehicleAI::init()
{
	m_eVehicleState		= STAY;
	m_eVehicleDirction	= MIDDLE;
	m_iNavigationRest	= 0;
	m_iNavigationCurrent	= 0;

	m_cOrbit			= Ogre::Vector2::ZERO;
	m_cLastTarget		= Ogre::Vector2::ZERO;
	m_cDirect			= Ogre::Vector2::ZERO;
	m_bIsFirst			= true;
	m_bIsOver			= false;
}

//---------------------------------------------------------------------------------------------------------

void VehicleAI::advance()
{
	m_eVehicleState		= STAY;
	m_eVehicleDirction	= MIDDLE;

	//�����ܵĲ�����
	if (!m_pPredator)	//׷����Ϊ��
		return;
	if (!m_cNavigationPointsVector.size())	//Ŀ��Ϊ��
		return;
	if (m_iNavigationCurrent >= m_cNavigationPointsVector.size() || m_iNavigationCurrent < 0)	//��ǰĿ�겻����
	{
		m_bIsOver = true;
		return;
	}

	//���ͨ����ǰ��
	m_eVehicleState = ADVANCE;

	Ogre::Vector3 tDir = m_pPredator->getOrientation().zAxis();
	tDir.y = 0;

	Ogre::Real raSign = tDir.x < 0 ? 1 : -1;

	//�����XZƽ����Ŀ����׷���ߵľֲ�����
	Ogre::Radian tRadian		= tDir.angleBetween(Ogre::Vector3::UNIT_Z) * raSign;
	Ogre::Vector3 tPrePos3D		= m_pPredator->getPosition();
	Ogre::Vector3 tTargetPos3D	= m_cNavigationPointsVector[m_iNavigationCurrent];

	Ogre::Vector2 tPrePos2D		= Ogre::Vector2(tPrePos3D.x, tPrePos3D.z);
	Ogre::Vector2 tTargetPos2D	= Ogre::Vector2(tTargetPos3D.x, tTargetPos3D.z);
	
	m_cDirect		= tTargetPos2D - tPrePos2D;
	m_cDirect.normalise();

	Ogre::Vector2 localAxis		= UtilityFunc::GlobalAxis_To_LocalAxis2D(tPrePos2D, tTargetPos2D, tRadian);

	//���㵽Ŀ��Ĺ켣����
	if (m_bIsFirst)
	{
		m_cLastTarget = tPrePos2D;
		m_bIsFirst = false;
	}

	m_cOrbit = tTargetPos2D - m_cLastTarget;

	//����׷��
	if (m_ePredatorMode == DISTANCE)
	{
		//���ݾֲ������֪׷���������������һ���ֱ��
		if (localAxis.x > m_fPreciDistance)
			m_eVehicleDirction = LEFT;
		else if (localAxis.x < -m_fPreciDistance)
			m_eVehicleDirction = RIGHT;
		else
			m_eVehicleDirction = MIDDLE;

	}
	//����׷��
	else if(m_ePredatorMode == DEGREE)
	{
		Ogre::Vector2 orbitNormal	= m_cOrbit.normalisedCopy();
		Ogre::Vector2 dir2D			  (tDir.x, tDir.z);
		Ogre::Vector2 dirNormal		= dir2D.normalisedCopy();
		Ogre::Real	signY			= dirNormal.y > 0 ? 1 : -1;

		//׷������Ŀ�귽��н�
		Ogre::Degree degree_Ori_dir(Ogre::Math::ACos(orbitNormal.dotProduct(dirNormal)));

		//�����ˣ�
		if (degree_Ori_dir > m_cPreciDegree)
		{

			//�ж�������׷���ߵ������ң��Ӷ��жϽ������
			if (signY * orbitNormal.x > signY * dirNormal.x)
				m_eVehicleDirction = LEFT;
			else
				m_eVehicleDirction = RIGHT;

		}
		else
			m_eVehicleDirction = MIDDLE;

	}
	else
	{
		m_ePredatorMode = DEGREE;
	}

	raSign = m_cOrbit.x < 0 ? 1 : -1;

	//�Ƿ��Ѿ�Խ��Ŀ��㣬����ݽ�����һ��Ŀ��
	Ogre::Vector3 orbit3D			= Ogre::Vector3(m_cOrbit.x, 0, m_cOrbit.y);
	Ogre::Radian tRadianOrbit		= orbit3D.angleBetween(Ogre::Vector3::UNIT_Z) * raSign;
	Ogre::Vector2 localAxisOribit	= UtilityFunc::GlobalAxis_To_LocalAxis2D(tPrePos2D, tTargetPos2D, tRadianOrbit);

	if (localAxisOribit.y <= 0)
	{
		m_cLastTarget = V3_To_V2_XZ(m_cNavigationPointsVector[m_iNavigationCurrent]);
		m_iNavigationCurrent++;
	}
}

//---------------------------------------------------------------------------------------------------------

void VehicleAI::exit()
{

}

//---------------------------------------------------------------------------------------------------------

void VehicleAI::addNavigationPoint(Ogre::Vector3 pVector3)
{
	m_cNavigationPointsVector.push_back(pVector3);
	m_iNavigationRest++;
}

//---------------------------------------------------------------------------------------------------------

void VehicleAI::bindPredator(Ogre::SceneNode* pPredator)
{
	m_pPredator = pPredator;
}

//---------------------------------------------------------------------------------------------------------

VehicleAI::VEHICLE_STATE VehicleAI::getVehicleState()
{
	return m_eVehicleState;
}

//---------------------------------------------------------------------------------------------------------

VehicleAI::VEHICLE_DIRECT VehicleAI::getVehicleDirection()
{
	return m_eVehicleDirction;
}

//---------------------------------------------------------------------------------------------------------

Ogre::Vector3 VehicleAI::getCurrentNavigationPoint()
{
	return m_cNavigationPointsVector[m_iNavigationCurrent];
}
//---------------------------------------------------------------------------------------------------------

Ogre::Vector3 VehicleAI::getNavigationPoint(int index)
{
	return m_cNavigationPointsVector[index];
}
//---------------------------------------------------------------------------------------------------------

int VehicleAI::getNavigationCurrent()
{
	return m_iNavigationCurrent;
}

//---------------------------------------------------------------------------------------------------------

bool VehicleAI::isOver()
{
	return m_bIsOver;
}

//---------------------------------------------------------------------------------------------------------

void VehicleAI::reset()
{
	m_cNavigationPointsVector.clear();
	init();
}

//---------------------------------------------------------------------------------------------------------
unsigned int VehicleAI::getNavigationSize()
{
	return m_cNavigationPointsVector.size();
}

//---------------------------------------------------------------------------------------------------------

void VehicleAI::setPredatorMode(PREDATOR_MODE mode)
{
	m_ePredatorMode = mode;
}

//---------------------------------------------------------------------------------------------------------

void VehicleAI::setDistancePrecision(Ogre::Real d)
{
	m_fPreciDistance = d;
}

//---------------------------------------------------------------------------------------------------------

void VehicleAI::setDegreePrecision(Ogre::Degree degree)
{
	m_cPreciDegree = degree;
}

//---------------------------------------------------------------------------------------------------------
Ogre::Vector2 VehicleAI::V3_To_V2_XZ(Ogre::Vector3 v3)
{
	return Ogre::Vector2(v3.x, v3.z);
}