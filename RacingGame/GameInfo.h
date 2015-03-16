#ifndef _GAMEINFO_H
#define _GAMEINFO_H

class VehicleInfo
{
public:

	//ÕýÏòTFF
	Ogre::Real gLongitudalExtremumSlip;
	Ogre::Real gLongitudalExtremumValue; 
	Ogre::Real gLongitudalAsymptoteSlip; 
	Ogre::Real gLongitudalAsymptoteValue;
	Ogre::Real gLongitudalStiffnessFactor;

	//²àÃæTFF
	Ogre::Real gLateralExtremumSlip;
	Ogre::Real gLateralExtremumValue;
	Ogre::Real gLateralAsymptoteSlip;
	Ogre::Real gLateralAsymptoteValue;
	Ogre::Real gLateralStiffnessFactor; 

	Ogre::Real gSuspensionSpring;
	Ogre::Real gSuspensionDamping;
	Ogre::Real gSuspensionTravel;
	Ogre::Real gTargetValues;
	Ogre::Real gAngularDamping;
	Ogre::Real gLinearDamping ;

	//Force
	//^		extremum
	//|    _*_
	//|   ~   \     asymptote
	//|  /     \~__*______________
	//| /
	//|/
	//---------------------------> Slip

	//vecor index 0~15
	std::vector<Ogre::Real*> gRealVec;

	Ogre::Real				gMass;

	Ogre::Vector3			gMassLocalPose;

	Ogre::Real				gSpeed;

	Ogre::Real				gAngle;

	Ogre::Real				gAngleSpeed;

	Ogre::Real				gIsEasyScene;

	VehicleInfo();

	void loadFromFile(Ogre::String filename);

	void initRealVec();

	std::vector<Ogre::Real*> getRealVec();
};


#endif