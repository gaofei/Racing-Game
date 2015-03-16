#include "Head.h"
#include "UtilltyFunc.h"

#include <OgreFontManager.h>
#include <OgreTextAreaOverlayElement.h>



Ogre::SceneManager* UtilityFunc::mSceneManger = 0;

Ogre::Real UtilityFunc::mLastElapse = 0;

Ogre::Real UtilityFunc::mElapseTime = 0;

void UtilityFunc::showFPS(Ogre::RenderWindow* win)
{
	static bool bIsFirst = true;

	if (bIsFirst)
	{
		bIsFirst = false;

		using namespace Ogre;

		FontManager& fontMgr = FontManager::getSingleton();
		ResourcePtr font = fontMgr.create("UtilltyFPSfont", "General");
		font->setParameter("type", "truetype");
		font->setParameter("source", "solo5.ttf");
		font->setParameter("size", "16");
		font->setParameter("resolution", "96");
		font->load();

		OverlayManager& overlayManager = OverlayManager::getSingleton();

		// Create a panel
		OverlayContainer* panel = static_cast<OverlayContainer*>(
			overlayManager.createOverlayElement("Panel", "PanelName"));
		panel->setMetricsMode(Ogre::GMM_PIXELS);
		panel->setPosition(10, 10);
		panel->setDimensions(100, 100);

		// Create a text area
		TextAreaOverlayElement* textArea = static_cast<TextAreaOverlayElement*>(
			overlayManager.createOverlayElement("TextArea", "TextAreaName"));
		textArea->setMetricsMode(Ogre::GMM_PIXELS);
		textArea->setPosition(0, 0);
		textArea->setDimensions(100, 100);
		textArea->setCaption("Hello, World!");
		textArea->setCharHeight(16);
		textArea->setFontName("UtilltyFPSfont");
		textArea->setColourBottom(ColourValue(0.3, 0.5, 0.3));
		textArea->setColourTop(ColourValue(0.5, 0.7, 0.5));

		// Create an overlay, and add the panel
		Overlay* overlay = overlayManager.create("OverlayName");
		overlay->add2D(panel);

		// Add the text area to the panel
		panel->addChild(textArea);

		// Show the overlay
		overlay->show();
	}

	Ogre::TextAreaOverlayElement* over = 
		(Ogre::TextAreaOverlayElement*)Ogre::OverlayManager::getSingleton().getOverlayElement("TextAreaName");
	over->setCaption(Ogre::StringConverter::toString(win->getLastFPS()));
}