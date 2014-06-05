#include "MainScene.h"
#include  "SimpleAudioEngine.h"  
#include <stdlib.h>
#include <algorithm>
#include<string.h>
#define CCRANDOM_0_1() ((float)rand()/RAND_MAX)
USING_NS_CC;
using namespace std;
static float myRand_0_1()
{
	cc_timeval psv;  
	CCTime::gettimeofdayCocos2d( &psv, NULL );    // 获取本地时间
	//根据时间产生随机种子
	//(这里加上一个rand函数，可解决在循环内(时间间隔极短)取随机数时，
	//连续取得的几个值相同的问题)
	unsigned int tsrans = psv.tv_sec * 1000 + psv.tv_usec / 1000 + rand();
	srand(tsrans);    //设定随机数种子
	return CCRANDOM_0_1()*0.6+0.2;
}
CCScene* MainScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainScene *layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
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
	
    CCTexture2D* number = CCTextureCache::sharedTextureCache()->addImage("number.png"); 
	CCSize numbersize = number->getContentSize();
	n1 = CCSprite::createWithTexture(number,CCRectMake(0,0,numbersize.width/5,numbersize.height/2));
	n1->setPosition(ccp(visibleSize.width/2,3*visibleSize.height/4));
	this->addChild(n1,1);

	n2 = CCSprite::createWithTexture(number,CCRectMake(0,0,numbersize.width/5,numbersize.height/2));
	n2->setPosition(ccp(visibleSize.width/2-numbersize.width/5,3*visibleSize.height/4));
	n2->setVisible(false);
	this->addChild(n2,1);

	n3 = CCSprite::createWithTexture(number,CCRectMake(0,0,numbersize.width/5,numbersize.height/2));
	n3->setPosition(ccp(visibleSize.width/2-numbersize.width*2/5,3*visibleSize.height/4));
	n3->setVisible(false);
	this->addChild(n3,1);

	bird = NULL;
	CCSpriteFrame *frame[3];
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("bird.png");  
	float w = texture->getContentSize().width;    
	float h = texture->getContentSize().height/ 3;   
	animation = CCAnimation::create();    
	animation->setDelayPerUnit(0.2f);    
	for(int i = 0; i < 3; i ++) {
		//animation->addSpriteFrameWithTexture(texture,  CCRectMake(i * w, 0, w, h));  
		frame[i]=CCSpriteFrame::createWithTexture(texture,CCRectMake(0,i * h, w, h));
		animation->addSpriteFrame(frame[i]);
	}
	animate = CCAnimate::create(animation);  
	bird = CCSprite::create("bird0.png");
	bird->setPosition(ccp(visibleSize.width/4 + origin.x, visibleSize.height/2 + origin.y));
	fly = CCRepeatForever::create(animate);
	bird->runAction(fly);    
	this->addChild(bird,1);

	CCTexture2D* up = CCTextureCache::sharedTextureCache()->addImage("up.png");  
	CCTexture2D* down = CCTextureCache::sharedTextureCache()->addImage("down.png");  
	pillarHeight = up->getContentSize().height;
	pillarWidth = up->getContentSize().width;
	up1 = CCSprite::createWithTexture(up);
	up2 = CCSprite::createWithTexture(up);
	down1 = CCSprite::createWithTexture(down);
	down2 = CCSprite::createWithTexture(down);
	this->addChild(up1, 0);
	this->addChild(up2, 0);
	this->addChild(down1,0);
	this->addChild(down2,0);
	setFirst();
	setSecond();
	//CCLOG("%f %f",visibleSize.width); 
	//CCLOG("%f %f",down1->getPositionX(),down2->getPositionX()); 
	flag1 = false;
	flag2 = false;
	die = false;
	index=0;
	high = 0;
	score = 0;
	m_state = kSpriteStateUngrabbed;
	this->scheduleUpdate();
	schedule(schedule_selector(MainScene::updatebird),0.3f); 
	this->setTouchEnabled(true);

	CCMenuItemImage *startItem = CCMenuItemImage::create(
		"restart.png",
		"restart.png",
		this,
		menu_selector(MainScene::startCallback));

	startItem->setPosition(ccp(origin.x + visibleSize.width/4 ,
		origin.y +  ground1->getContentSize().height+startItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	pMenu = CCMenu::create(startItem, NULL);
	pMenu->setPosition(CCPointZero);
	pMenu->setVisible(false);
	this->addChild(pMenu, 2);

	CCMenuItemImage *rankItem = CCMenuItemImage::create(
		"rank.png",
		"rank.png",
		this,
		menu_selector(MainScene::rankCallback));

	rankItem->setPosition(ccp(origin.x + 3*visibleSize.width/4 ,
		origin.y +  ground1->getContentSize().height+startItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	rMenu = CCMenu::create(rankItem, NULL);
	rMenu->setPosition(CCPointZero);
	rMenu->setVisible(false);
	this->addChild(rMenu, 1);
	
	 gameover = CCSprite::create("gameover.png");
	gameover->setPosition(ccp(visibleSize.width/2 + origin.x, 2*visibleSize.height/3 + origin.y));
	gameover->setVisible(false);
	this->addChild(gameover,2);

	result = CCSprite::create("result.png");
	result->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	result->setVisible(false);
	this->addChild(result,2);
    return true;
}
void MainScene::setFirst(){
	a = myRand_0_1();
	//CCLog("%f",a);
	
	up1->setTextureRect(CCRect(0,pillarHeight*a,pillarWidth,pillarHeight*(1-a)));
	up1->setPosition(ccp(visibleSize.width+pillarWidth/2 + origin.x,
		visibleSize.height-up1->getContentSize().height/2 + origin.y));
	
	down1->setTextureRect(CCRect(0,0,pillarWidth,pillarHeight*a));
	down1->setPosition(ccp(visibleSize.width+pillarWidth/2 + origin.x,
		ground1->getContentSize().height+down1->getContentSize().height/2 + origin.y));

}
void MainScene::setSecond(){
	b = myRand_0_1();
	//CCLog("%f",b);
	up2->setTextureRect(CCRect(0,pillarHeight*b,pillarWidth,pillarHeight*(1-b)));
	up2->setPosition(ccp(visibleSize.width/2+up1->getPositionX()+pillarWidth/2,
		visibleSize.height-up2->getContentSize().height/2 + origin.y));

	down2->setTextureRect(CCRect(0,0,pillarWidth,pillarHeight*b));
	down2->setPosition(ccp(visibleSize.width/2+up1->getPositionX()+pillarWidth/2,
		ground1->getContentSize().height+down2->getContentSize().height/2 + origin.y));
}
void MainScene::updatebird(float delta){
	
	CCTexture2D* bird1 = CCTextureCache::sharedTextureCache()->addImage("bird1.png"); 
	CCTexture2D* bird3 = CCTextureCache::sharedTextureCache()->addImage("bird3.png"); 
	if(index++>1){
	     //animate->stop(); 
		//bird->stopAllActions();
		CCDirector::sharedDirector()->getActionManager()->pauseTarget(bird);
		//bird->stopAction(fly);
		bird->setTexture(bird3);
		bird->setTextureRect(CCRectMake(0,0,bird3->getContentSize().width,bird3->getContentSize().height));
	}
	/*
	else{
		bird->setTexture(bird1);
		bird->setTextureRect(CCRectMake(0,0,bird1->getContentSize().width,bird1->getContentSize().height));
	}
	*/
	if(bird->getPositionY()-bird->getContentSize().height/2<= ground1->getContentSize().height){
		this->unschedule(schedule_selector(MainScene::updatebird));
		GameOver();
	}
	else{
		if(die){
			high = bird->getContentSize().height/2;
		}
		else{
			high += bird->getContentSize().height/2;
		}
		if(bird->getPositionY()-high<= ground1->getContentSize().height)
			bird->setPositionY(ground1->getContentSize().height);
		else
			bird->setPositionY(bird->getPositionY()-high);
	}
	if(bird->getPositionY()>visibleSize.height){
		bird->setPositionY(visibleSize.height);
	}
	
}
bool compare(int a,int b)
{
	return a>b;   //升序排列，如果改为return a>b，则为降序
}

void MainScene::GameOver(){
	this->unscheduleUpdate();
	this->setTouchEnabled(false);
	if(!flag2){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_hit.wav");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_die.wav");
	}
	pMenu->setVisible(true);
	rMenu->setVisible(true);
	result->setVisible(true);
	gameover->setVisible(true);
	n1->setVisible(false);
	n2->setVisible(false);
	n3->setVisible(false);
	stringstream ss4; 
	ss4<< score;
	pLabel4 = CCLabelTTF::create(ss4.str().c_str(), "Felt", 18);
	pLabel4->setPosition(ccp(origin.x + result->getPositionX()+result->getContentSize().width/3,
		                      result->getPositionY()+result->getContentSize().height/6));
	pLabel4->setColor(ccc3(0, 0, 0));
	this->addChild(pLabel4, 2);

	isHaveFile();
	int highestScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("HighestScore");
	if(score>highestScore){
		CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",score);
		highestScore = score;
	}
	stringstream ss5; 
	ss5<< highestScore;
	pLabel5 = CCLabelTTF::create(ss5.str().c_str(), "Felt",18);
	pLabel5->setPosition(ccp(origin.x + result->getPositionX()+result->getContentSize().width/3,
		origin.y + result->getPositionY()-result->getContentSize().height/4));
	pLabel5->setColor(ccc3(0, 0, 0));
	this->addChild(pLabel5, 2);

}

void MainScene::isHaveFile(){
	if(!CCUserDefault::sharedUserDefault()->getBoolForKey("isHaveSaveFile")){
		CCUserDefault::sharedUserDefault()->setBoolForKey("isHaveSaveFile",true);
		CCUserDefault::sharedUserDefault()->setIntegerForKey("HighestScore",0);
		CCUserDefault::sharedUserDefault()->flush();
	}
}

void MainScene::update(float delta){
	float X1 = ground1->getPositionX();
	float X2 = ground2->getPositionX();

	float F1 = up1->getPositionX();
	float F2 = up2->getPositionX();
	float speed=1.8f;
	X1-=speed;
	X2-=speed;
	F1-=speed;
	F2-=speed;
	CCSize size = ground1->getContentSize();
	CCSize size1 = up1->getContentSize();
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
	if (F1<-size1.width/2)
	{
		setFirst();
		F1=up1->getPositionX();
	}
	if (F2<-size1.width/2)
	{
		setSecond();
		F2=up2->getPositionX();
	}
	ground1->setPositionX(X1);
	ground2->setPositionX(X2);
	up1->setPositionX(F1);
	down1->setPositionX(F1);
	up2->setPositionX(F2);
	down2->setPositionX(F2);
	CheckCollision();
	CCLOG("%f %f",down1->getPositionX(),down2->getPositionX()); 
	if((down1->getPositionX() - visibleSize.width/4>-speed/2&&down1->getPositionX() - visibleSize.width/4<=speed/2)||
		(down2->getPositionX() - visibleSize.width/4>-speed/2&&down2->getPositionX() - visibleSize.width/4<=speed/2)){
			//CCLOG("%f %f %f",down1->getPositionX(),down2->getPositionX(),visibleSize.width/4); 
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_point.wav");
			score += 1;
			if(score>=10){
				if(score>=100){
					n3->setVisible(true);
				}
				else{
					n2->setVisible(true);
				}
			}
			setScore();
	}
}

void MainScene::setScore(){
	CCTexture2D* number = CCTextureCache::sharedTextureCache()->addImage("number.png"); 
	int numberwidth = number->getContentSize().width/5;
	int numberheight = number->getContentSize().height/2;
	if(score<10){
		int col = score/5;
		int row = score%5;
		n1->setTexture(number);
		n1->setTextureRect(CCRectMake(row*numberwidth,col*numberheight,numberwidth,numberheight));
	}
	else if(score<100){
		int score2 = score/10;
		int score1 = score%10;
		int col1 = score1/5;
		int row1 = score1%5;
		int col2 = score2/5;
		int row2 = score2%5;
		n1->setTexture(number);
		n1->setTextureRect(CCRectMake(row1*numberwidth,col1*numberheight,numberwidth,numberheight));
		n2->setTexture(number);
		n2->setTextureRect(CCRectMake(row2*numberwidth,col2*numberheight,numberwidth,numberheight));
	}
	else if(score<1000){
		int score1 = score%10;
		int scoret = score/10;
		int score2 = scoret%10;
		int score3 = scoret/10;
		int col1 = score1/5;
		int row1 = score1%5;
		int col2 = score2/5;
		int row2 = score2%5;
		int col3 = score3/5;
		int row3 = score3%5;
		n1->setTexture(number);
		n1->setTextureRect(CCRectMake(row1*numberwidth,col1*numberheight,numberwidth,numberheight));
		n2->setTexture(number);
		n2->setTextureRect(CCRectMake(row2*numberwidth,col2*numberheight,numberwidth,numberheight));
		n3->setTexture(number);
		n3->setTextureRect(CCRectMake(row3*numberwidth,col3*numberheight,numberwidth,numberheight));
	}

}
void MainScene::CheckCollision(){
	CCPoint  relativeup1 =bird->convertToNodeSpaceAR(up1->getPosition());
	CCPoint  relativeup2 =bird->convertToNodeSpaceAR(up2->getPosition());
	CCPoint  relativedown1 =bird->convertToNodeSpaceAR(down1->getPosition());
	CCPoint  relativedown2 =bird->convertToNodeSpaceAR(down2->getPosition());
	CCSize BirdSize = bird->getContentSize();
	CCRect BirdRect = CCRectMake(-BirdSize.width/2, -BirdSize.height/2, BirdSize.width, BirdSize.height);
	CCSize up1size = up1->getContentSize();
	CCSize down1size = down1->getContentSize();
	CCSize up2size = up2->getContentSize();
	CCSize down2size = down2->getContentSize();
	CCRect up1Rect = CCRectMake(-up1size.width/2+relativeup1.x, -up1size.height/2+relativeup1.y, 
		                         up1size.width, up1size.height);
	CCRect down1Rect = CCRectMake(-down1size.width/2+relativedown1.x, -down1size.height/2+relativedown1.y, 
		                       down1size.width, down1size.height);
	CCRect up2Rect = CCRectMake(-up2size.width/2+relativeup2.x, -up2size.height/2+relativeup2.y, 
		                       up2size.width, up2size.height);
	CCRect down2Rect = CCRectMake(-down2size.width/2+relativedown2.x, -down2size.height/2+relativedown2.y, 
		                       down2size.width, down2size.height);
	if ( BirdRect.intersectsRect(up1Rect)||BirdRect.intersectsRect(up2Rect)||
		 BirdRect.intersectsRect(down1Rect)||BirdRect.intersectsRect(down2Rect))

	{
		this->unscheduleUpdate();
		this->setTouchEnabled(false);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_hit.wav");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_die.wav");
		die = true;
		this->unschedule(schedule_selector(MainScene::updatebird));
		schedule(schedule_selector(MainScene::updatebird),0.05f); 
		flag2 =  true;
		
	}
	
}
void MainScene::onEnter(){  

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,false);  
	CCLayer::onEnter();  
}  

void MainScene::onExit(){  

	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
	CCLayer::onExit();  
}  
bool MainScene::ccTouchBegan(CCTouch* touch, CCEvent* event)
{ 
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect( "//sounds//sfx_wing.wav" );
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_wing.wav");
	CCTexture2D* bird2 = CCTextureCache::sharedTextureCache()->addImage("bird2.png"); 
	bird->setTexture(bird2);
	bird->setTextureRect(CCRectMake(0,0,bird2->getContentSize().width,bird2->getContentSize().height));
	high = 0;
	

	CCDirector::sharedDirector()->getActionManager()->resumeTarget(bird);

	index = 0;
	bird->setPositionY(bird->getPositionY()+bird->getContentSize().height);
	//if (m_state != kSpriteStateUngrabbed) return false;
	return true;
}
void MainScene::startCallback(CCObject* pSender)
{
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	MainScene *layer = MainScene::create();
	scene->addChild(layer);
	// add layer as a child to scene
	CCDirector::sharedDirector()->replaceScene(scene); 

}
void MainScene::rankCallback(CCObject* pSender)
{
	CCSprite* pSprite = CCSprite::create("ranks.png");
	pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	// add the sprite as a child to this layer
	this->addChild(pSprite,2);
	
	gameover->setVisible(false);
	result->setVisible(false);
	isHaveFile();
	int highestScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("HighestScore");
	stringstream score; 
	score<< highestScore;
	CCLabelTTF* pLabel1 = CCLabelTTF::create(score.str().c_str(), "Felt", 38);
	pLabel1->setPosition(ccp(origin.x + visibleSize.width/2,origin.y + 3*visibleSize.height/4 ));
	pLabel1->setColor(ccc3(0, 0, 0));
	this->addChild(pLabel1, 2);

	pLabel4->setVisible(false);
	pLabel5->setVisible(false);
}