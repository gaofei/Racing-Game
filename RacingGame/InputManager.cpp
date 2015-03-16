#include "Head.h"
#include "InputManager.h"

#include "GUIManager.h"

InputManager* InputManager::mInputManager = 0;

//---------------------------------------------------------------------------------------------------------

InputManager::InputManager(Ogre::RenderWindow* win, StatesManager* sm)
{
	mWindow = win;
	mStateManager = sm;
	createInputSystem();
	createInputObject();
	setEventCallback();
	setMouseClip();
}

//---------------------------------------------------------------------------------------------------------

InputManager::~InputManager()
{
	if( mOISInputManager )
	{
		mOISInputManager->destroyInputObject( mMouse );
		mOISInputManager->destroyInputObject( mKeyboard );

		OIS::InputManager::destroyInputSystem(mOISInputManager);
		mOISInputManager = 0;
	}
}

//---------------------------------------------------------------------------------------------------------

void InputManager::Initialize(Ogre::RenderWindow* win, StatesManager* sm)
{
	mInputManager = new InputManager(win, sm);

}

//---------------------------------------------------------------------------------------------------------

InputManager* InputManager::getSingleton()
{
	if(mInputManager)
		return mInputManager;
	else
		return 0;
}


//---------------------------------------------------------------------------------------------------------

void InputManager::createInputSystem()
{

	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));

	mOISInputManager = OIS::InputManager::createInputSystem( pl );

}

//---------------------------------------------------------------------------------------------------------

void InputManager::createInputObject()
{
	mKeyboard = static_cast<OIS::Keyboard*>(mOISInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mOISInputManager->createInputObject( OIS::OISMouse, true ));
}

//---------------------------------------------------------------------------------------------------------

void InputManager::setEventCallback()
{
		mMouse->setEventCallback(this);
		mKeyboard->setEventCallback(this);
}

//---------------------------------------------------------------------------------------------------------

void InputManager::setMouseClip()
{
	unsigned int width, height, depth;
	int left, top;
	mWindow->getMetrics(width, height, depth, left, top);

	//设置鼠标裁剪大小
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

//---------------------------------------------------------------------------------------------------------

void InputManager::capture()
{
	mMouse->capture();
	mKeyboard->capture();
}

//---------------------------------------------------------------------------------------------------------

bool InputManager::mouseMoved( const OIS::MouseEvent &arg )
{
	if(!mStateManager->injectMouseMoved(arg))
		return false;

	if (!GUIManager::getSingleton()->injectMouseMoved(arg))
		return false;

	return true;
}

//-------------------------------------------------------------------------------------

bool InputManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(!mStateManager->injectMousePressed(arg, id))
		return false;

	if (!GUIManager::getSingleton()->injectMousePressed(arg, id))
		return false;

	return true;
}

//-------------------------------------------------------------------------------------

bool InputManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(!mStateManager->injectMouseReleased(arg, id))
		return false;

	if (!GUIManager::getSingleton()->injectMouseReleased(arg, id))
		return false;

	return true;
}

//-------------------------------------------------------------------------------------

bool InputManager::keyPressed( const OIS::KeyEvent &arg )
{
	if(!mStateManager->injectKeyPressed(arg))
		return false;

	if (!GUIManager::getSingleton()->injectKeyPressed(arg))
		return false;

	return true;
}

//-------------------------------------------------------------------------------------

bool InputManager::keyReleased( const OIS::KeyEvent &arg )
{
	if(!mStateManager->injectKeyReleased(arg))
		return false;

	if (!GUIManager::getSingleton()->injectKeyReleased(arg))
		return false;

	return true;
}