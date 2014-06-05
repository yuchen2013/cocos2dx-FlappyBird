#include "FlappyBird.h"
#include "MainScene.h"
#include <stdlib.h>
#include <algorithm>
#include<string.h>

USING_NS_CC;
using namespace std;

CCScene* FlappyBird::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	FlappyBird *layer = FlappyBird::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool FlappyBird::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite* pSprite = CCSprite::create("day.png");

	// position the sprite on the center of the screen
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);

	ground1 = CCSprite::create("ground.png");
	ground2 = CCSprite::create("ground.png");
	// position the sprite on the center of the screen
	ground1->setPosition(ccp(visibleSize.width/2 + origin.x, ground1->getContentSize().height/2 + origin.y));
	ground2->setPosition(ccp(3*visibleSize.width/2 + origin.x, ground2->getContentSize().height/2 + origin.y));
	// add the sprite as a child to this layer
	this->addChild(ground1, 0);
	this->addChild(ground2, 0);

	CCSprite* bird = NULL;
	CCSpriteFrame *frame[3];
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("bird.png");  
	float w = texture->getContentSize().width;    
	float h = texture->getContentSize().height/ 3;   
	CCAnimation* animation = CCAnimation::create();    
	animation->setDelayPerUnit(0.2f);    
	for(int i = 0; i < 3; i ++) {
		//animation->addSpriteFrameWithTexture(texture,  CCRectMake(i * w, 0, w, h));  
		frame[i]=CCSpriteFrame::createWithTexture(texture,CCRectMake(0,i * h, w, h));
		animation->addSpriteFrame(frame[i]);
	}
	CCAnimate* animate = CCAnimate::create(animation);  
	bird = CCSprite::createWithSpriteFrame(frame[0]);
	bird->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	bird->runAction(CCRepeatForever::create(animate));    
	this->addChild(bird);

	wSprite = CCSprite::create("flappybird.png");
	wSprite->setPosition(ccp(visibleSize.width/2 + origin.x,3*visibleSize.height/4 + origin.y));
	// add the sprite as a child to this layer
	this->addChild(wSprite);


	this->scheduleUpdate();


	CCMenuItemImage *startItem = CCMenuItemImage::create(
		"restart.png",
		"restart.png",
		this,
		menu_selector(FlappyBird::startCallback));

	startItem->setPosition(ccp(origin.x + visibleSize.width/4 ,
		origin.y +  ground1->getContentSize().height+startItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(startItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 2);

	CCMenuItemImage *rankItem = CCMenuItemImage::create(
		"rank.png",
		"rank.png",
		this,
		menu_selector(FlappyBird::rankCallback));

	rankItem->setPosition(ccp(origin.x + 3*visibleSize.width/4 ,
		origin.y +  ground1->getContentSize().height+startItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	CCMenu *rMenu = CCMenu::create(rankItem, NULL);
	rMenu->setPosition(CCPointZero);
	this->addChild(rMenu, 1);

	CCLabelTTF* pLabel = CCLabelTTF::create("Copyright@ YuChen", "Felt", 18);

	// position the label on the center of the screen
	pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + ground1->getContentSize().height - pLabel->getContentSize().height*1.5));
	pLabel->setColor(ccc3(0, 0, 0));
	// add the label as a child to this layer
	this->addChild(pLabel, 1);

	return true;
}


void FlappyBird::update(float delta)
{
	int X1 = ground1->getPositionX();
	int X2 = ground2->getPositionX();
	int speed=2;
	X1-=speed;
	X2-=speed;
	CCSize size = ground1->getContentSize();
	if (X1<-size.width/2)
	{
		X2 = size.width/2;
		X1=size.width/2+size.width;
	}
	if (X2<-size.width/2)
	{
		X1 = size.width/2;
		X2=size.width/2+size.width;
	}
	ground1->setPositionX(X1);
	ground2->setPositionX(X2);
}


void FlappyBird::startCallback(CCObject* pSender)
{
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	MainScene *layer = MainScene::create();
	scene->addChild(layer);
	// add layer as a child to scene
	CCDirector::sharedDirector()->replaceScene(scene); 

}

void FlappyBird::rankCallback(CCObject* pSender)
{
	CCSprite* pSprite = CCSprite::create("ranks.png");
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	// add the sprite as a child to this layer
	this->addChild(pSprite,2);
	wSprite->setVisible(false);
	
	if(!CCUserDefault::sharedUserDefault()->getBoolForKey("isHaveSaveFile")){
		CCUserDefault::sharedUserDefault()->setBoolForKey("isHaveSaveFile",true);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",0);
		CCUserDefault::sharedUserDefault()->flush();
	}
	int highestScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("HighestScore");
	stringstream score; 
	score<< highestScore;
	CCLabelTTF* pLabel1 = CCLabelTTF::create(score.str().c_str(), "Felt", 38);
	pLabel1->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + 3*visibleSize.height/4 ));
	pLabel1->setColor(ccc3(0, 0, 0));
	this->addChild(pLabel1, 2);
}