#ifndef __COMPOSITOR_LOGIC_H__
#define __COMPOSITOR_LOGIC_H__
/*
ListenerFactoryLogic»ùÀà
*/

#include "OgreCompositorLogic.h"

class ListenerFactoryLogic : public Ogre::CompositorLogic
{
public:
	/** @copydoc CompositorLogic::compositorInstanceCreated */
	virtual void compositorInstanceCreated(Ogre::CompositorInstance* newInstance) 
	{
		Ogre::CompositorInstance::Listener* listener = createListener(newInstance);
		newInstance->addListener(listener);
		mListeners[newInstance] = listener;
	}

	/** @copydoc CompositorLogic::compositorInstanceDestroyed */
	virtual void compositorInstanceDestroyed(Ogre::CompositorInstance* destroyedInstance)
	{
		delete mListeners[destroyedInstance];
		mListeners.erase(destroyedInstance);
	}

protected:
	virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance) = 0;
private:
	typedef std::map<Ogre::CompositorInstance*, Ogre::CompositorInstance::Listener*> ListenerMap;
	ListenerMap mListeners;

};

/*
HDRLogic
*/

class HDRLogic : public ListenerFactoryLogic
{
protected:
	/** @copydoc ListenerFactoryLogic::createListener */
	virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance);
};

/*
HDRListener
*/

class HDRListener: public Ogre::CompositorInstance::Listener
{
protected:
	int mVpWidth, mVpHeight;
	int mBloomSize;
	// Array params - have to pack in groups of 4 since this is how Cg generates them
	// also prevents dependent texture read problems if ops don't require swizzle
	float mBloomTexWeights[15][4];
	float mBloomTexOffsetsHorz[15][4];
	float mBloomTexOffsetsVert[15][4];
public:
	HDRListener();
	virtual ~HDRListener();
	void notifyViewportSize(int width, int height);
	void notifyCompositor(Ogre::CompositorInstance* instance);
	virtual void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
	virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
};

#endif //__COMPOSITOR_LOGIC_H__