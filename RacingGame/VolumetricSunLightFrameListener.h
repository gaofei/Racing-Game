#pragma once

/////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////

class VolumetricSunLightFrameListener	:	public Ogre::CompositorInstance::Listener
{
public:
	
	//-----------------------------------------------------------------------------------
	VolumetricSunLightFrameListener(Ogre::SceneManager * pSceneManager, Ogre::Camera * mCamera, Ogre::Viewport * pViewport)
	{
		m_pSceneManager = pSceneManager;
		m_pSunLight = NULL;
		m_pMainCamera = mCamera;
		m_pMainViewport = pViewport;

		alphaXZ = 0;
		alphaZY = 0;
	}
	//-----------------------------------------------------------------------------------
	~VolumetricSunLightFrameListener()
	{

	}
	//-----------------------------------------------------------------------------------
	void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
	{
		if( pass_id == 1 )
		{
			fpParams = mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
		}
	}
	//-----------------------------------------------------------------------------------
	void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr & mat)
	{
		if( pass_id == 1 )
		{
			if( !m_pSunLight )
				m_pSunLight = m_pSceneManager->getLight("SunLight");

			//if( !m_pMainCamera )
			//	m_pMainCamera = m_pSceneManager->getCamera(MAIN_CAMERA_NAME);

			if( m_pSunLight && m_pMainCamera && m_pMainViewport )
			{
				Ogre::Vector3 vCamPos = m_pMainCamera->getRealPosition();
				Ogre::Vector3 vSunPos = m_pSunLight->getDerivedPosition();
				Ogre::Vector3 camToLightVector = vSunPos - vCamPos;

				Ogre::Vector3 vCameraDir = m_pMainCamera->getDerivedDirection();
				Ogre::Radian rAngle = vCameraDir.angleBetween(camToLightVector);

				Ogre::Vector3 lightInCamSpace;
				Ogre::Vector3 hcsPosition = m_pMainCamera->getProjectionMatrix() * m_pMainCamera->getViewMatrix() * camToLightVector; 
				lightInCamSpace.x = 0.5 - hcsPosition.x * 0.5f;
				lightInCamSpace.x = 1.0 - lightInCamSpace.x;
				lightInCamSpace.y = hcsPosition.y * 0.5f + 0.5;
				lightInCamSpace.y = 1.0 - lightInCamSpace.y;

				//if( lightInCamSpace.x < -1 )
				//{
				//	lightInCamSpace.x = 1 + lightInCamSpace.x;
				//	lightInCamSpace.y = 1 - lightInCamSpace.y;
				//}

				lightInCamSpace.z = camToLightVector.z;
				fpParams->setNamedConstant("screenLightPos", lightInCamSpace);



				float fDensity;
				Ogre::Real fAngle = rAngle.valueDegrees();
				if( fAngle < 90 )
				{
					fDensity = Ogre::Math::Sin(Ogre::Math::HALF_PI + Ogre::Radian(Ogre::Degree(fAngle)).valueRadians());
					float delta = fAngle * (float)(0.1f / 90.0f);
					fDensity += delta;
				}
				else
				{
					fDensity = Ogre::Math::Sin(Ogre::Math::HALF_PI + Ogre::Radian(Ogre::Degree(fAngle)).valueRadians());
					float delta = fAngle * (float)(0.1f / 90.0f);
					fDensity += delta;
					if( fDensity < 0.1 )
						fDensity = 0.1;
				}

				fpParams->setNamedConstant("density", fDensity);
			}			
		}
	}
	//-----------------------------------------------------------------------------------

protected:
	Ogre::GpuProgramParametersSharedPtr fpParams;
	
	Ogre::SceneManager *	m_pSceneManager;
	Ogre::Light *			m_pSunLight;
	Ogre::Camera *			m_pMainCamera;
	Ogre::Viewport *		m_pMainViewport;

	float alphaXZ;
	float alphaZY;
};

/////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////