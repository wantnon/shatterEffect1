#include "HelloWorldScene.h"
#include "AppMacros.h"
USING_NS_CC;
#include "SceneNode.h"
#include "CCControlButton.h"
using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...
    //enable touch
	setTouchEnabled( true );
    
    
    
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();//winSize equals to designResolutionSize
    
    //show frame rate info
    CCDirector::sharedDirector()->setDisplayStats(true);
    
    
    //sceneNode
    SceneNode*sceneNode=new SceneNode();
    sceneNode->init("fish.png");
    sceneNode->setAnchorPoint(ccp(0.5,0.5));
    sceneNode->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(sceneNode,1);
    pSceneNodeList.push_back(sceneNode);
    sceneNode->release();
 
    
    
    //slider
	{
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		CCControlSlider *slider = CCControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(1.8f); // Sets the min value of range
		slider->setMaximumValue(10); // Sets the max value of range
        this->m_initialValue=pSceneNodeList[0]->getd_new();
		slider->setValue(m_initialValue);
		slider->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 4.0f));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::sliderAction), CCControlEventValueChanged);
		m_pSliderCtl=slider;
		addChild(m_pSliderCtl,100);
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("particle size ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);
	}
	//slider2
    {
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		CCControlSlider *slider = CCControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
		slider->setAnchorPoint(ccp(0.5f, 0.5f));
		slider->setMinimumValue(1.0f); // Sets the min value of range
		slider->setMaximumValue(10); // Sets the max value of range
        this->m_initialValue2=pSceneNodeList[0]->getParticleScale0();
		slider->setValue(m_initialValue2);
		slider->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 6.0f));
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::sliderAction2), CCControlEventValueChanged);
		m_pSliderCtl2=slider;
		addChild(m_pSliderCtl2,100);
		//title
		CCLabelTTF* pLabel = CCLabelTTF::create("particle scale ", "Arial", 40);
		pLabel->setPosition(ccp(slider->getPositionX()-slider->getContentSize().width/2-pLabel->getContentSize().width/2, slider->getPositionY()));
        this->addChild(pLabel, 1);
	}
    //resetShatter control button
    {
        cocos2d::extension::CCScale9Sprite* btnUp=cocos2d::extension::CCScale9Sprite::create("button.png");
        cocos2d::extension::CCScale9Sprite* btnDn=cocos2d::extension::CCScale9Sprite::create("button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("reset", "Helvetica", 30);
        cocos2d::extension::CCControlButton* controlButton=cocos2d::extension::CCControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,cocos2d::extension::CCControlStateHighlighted);
        controlButton->setPreferredSize(CCSize(100,100));
        controlButton->setPosition(ccpMult(m_pSliderCtl->getPosition()+m_pSliderCtl2->getPosition(),0.5)
                                   +ccp(m_pSliderCtl->getContentSize().width/2+controlButton->getContentSize().width/2+70 ,0));
        controlButton->addTargetWithActionForControlEvents(this, (cocos2d::extension::SEL_CCControlHandler)(&HelloWorld::resetSliders), cocos2d::extension::CCControlEventTouchDown);
        addChild(controlButton);
        controlButton->setVisible(true);
        m_controlButton=controlButton;
    }

    //----menu0
    {
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        
        CCLabelTTF* pLabel0 = CCLabelTTF::create("circle", "Arial", 30);
        CCLabelTTF* pLabel1 = CCLabelTTF::create("heart", "Arial", 30);
        
        pMenuItem0 = CCMenuItemImage::create("btn.png","btn_dn.png",this,menu_selector(HelloWorld::menu0Callback));
        pMenuItem0->setPosition(CCPointZero);
        pMenuItem0->addChild(pLabel0,10);
        pLabel0->setPosition(ccp(pMenuItem0->getContentSize().width/2,pMenuItem0->getContentSize().height/2));
        
        pMenuItem1 = CCMenuItemImage::create("btn.png","btn_dn.png",this,menu_selector(HelloWorld::menu1Callback));
        pMenuItem1->setPosition(pMenuItem0->getPosition()-ccp(0,pMenuItem0->getContentSize().height));
        pMenuItem1->addChild(pLabel1,10);
        pLabel1->setPosition(ccp(pMenuItem1->getContentSize().width/2,pMenuItem1->getContentSize().height/2));
        
        pMenu0 = CCMenu::create(pMenuItem0, pMenuItem1,NULL);
        pMenu0->setPosition(ccp(screenSize.width*(3.0/4), screenSize.height*(3.0/4)));
        this->addChild(pMenu0, 1);
        
        checkPic = CCSprite::create();
        checkPic->initWithFile("check.png");
        checkPic->setPosition(ccp(pMenuItem0->getContentSize().width+checkPic->getContentSize().width/2,pMenuItem0->getContentSize().height/2));
        pMenuItem0->addChild(checkPic);
        CCLOG("checkPic retainCount:%i",checkPic->retainCount());
        

        
    }
    
    
    // author info
    {
        CCLabelTTF* pLabel = CCLabelTTF::create("by yang chao (wantnon) 2013-11-10", "Arial", 30);
        pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - pLabel->getContentSize().height-60));
        this->addChild(pLabel, 1);
    }
    
    // add a label shows "Hello World"
    // create and initialize a label
 
    CCLabelTTF* pLabel = CCLabelTTF::create("Shatter Effect 1", "Arial", 45);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);
  
    
    // add "HelloWorld" splash screen"
/*    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    */
    return true;
}

void HelloWorld::menu0Callback(CCObject* pSender){
    checkPic->retain();
    checkPic->removeFromParentAndCleanup(false);
    pMenuItem0->addChild(checkPic);
    checkPic->release();
    checkPic->setPosition(ccp(pMenuItem0->getContentSize().width+checkPic->getContentSize().width/2,pMenuItem0->getContentSize().height/2));
    this->pSceneNodeList[0]->setShapeType(0);

}

void HelloWorld::menu1Callback(CCObject* pSender){
    checkPic->retain();
    checkPic->removeFromParentAndCleanup(false);
    pMenuItem1->addChild(checkPic);
    checkPic->release();
    checkPic->setPosition(ccp(pMenuItem1->getContentSize().width+checkPic->getContentSize().width/2,pMenuItem1->getContentSize().height/2));
    this->pSceneNodeList[0]->setShapeType(1);
    
}

void HelloWorld::sliderAction(CCObject* sender, CCControlEvent controlEvent)
{
    CCControlSlider* pSlider = (CCControlSlider*)sender;
    float value=pSlider->getValue();
    CCLOG("slider value:%f",value);
    pSceneNodeList[0]->setd_new(value);
    
}
void HelloWorld::sliderAction2(CCObject* sender, CCControlEvent controlEvent)
{
    CCControlSlider* pSlider = (CCControlSlider*)sender;
    float value=pSlider->getValue();
    CCLOG("slider value:%f",value);
    pSceneNodeList[0]->setParticleScale0(value);
    
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
void HelloWorld::resetSliders(CCObject *senderz, cocos2d::extension::CCControlEvent controlEvent){
    this->m_pSliderCtl->setValue(m_initialValue);
    pSceneNodeList[0]->setd_new(m_initialValue);
    this->m_pSliderCtl2->setValue(m_initialValue2);
    pSceneNodeList[0]->setParticleScale0(m_initialValue2);
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSetIterator it;
    CCTouch* touch;
    
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        
        
    }
}
void HelloWorld::ccTouchesMoved(cocos2d::CCSet* touches , cocos2d::CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSetIterator it;
    CCTouch* touch;
    for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        
        
       

    
        
		
    }
    
}

void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSetIterator it;
    CCTouch* touch;
	for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        //CCLOG("loc_GLSpace:%f,%f",loc_GLSpace.x,loc_GLSpace.y);
       
        if (pSceneNodeList[0]->getIsAllParticleDisapear()) {
            pSceneNodeList[0]->resetShatter();//d_new may be changed, so we need to reset shatter to update particle size.
            pSceneNodeList[0]->startShatter();
        }
        
       
        
    }
}

