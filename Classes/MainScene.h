#ifndef __MainScene_SCENE_H__
#define __MainScene_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;
typedef enum tagSpriteState 
{
	kSpriteStateGrabbed,
	kSpriteStateUngrabbed
} SpriteState;
class MainScene : public cocos2d::CCLayer
{
	float a;
	float b;
	CCNode* ground1;
	CCNode* ground2;
	CCSprite *up1;
	CCSprite *up2;
	CCSprite*down1;
	CCSprite *down2;
	CCSprite* bird;
	CCSprite *n1;
	CCSprite *n2;
	CCSprite *n3;
	float pillarHeight ;
	float pillarWidth ;
	CCSize visibleSize ;
	CCPoint origin;
	SpriteState m_state;
	CCMenu* pMenu;
	CCMenu* rMenu;
	CCSprite* gameover;
	CCSprite* result;
	int index;
	int high;
	int score;
	bool flag1;
	bool flag2;
	bool die;
	CCLabelTTF* pLabel4;
	CCLabelTTF* pLabel5;
	CCAction* fly;
	CCAnimation* animation;
	CCAction* mAction;
	CCAnimate *animate;
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void startCallback(CCObject* pSender);
	void rankCallback(CCObject* pSender);
	void setFirst();
	void setSecond();
	void update(float delta);
	void updatebird(float delta);
	void CheckCollision ();
	void setScore();
	void GameOver();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void onEnter();  
	virtual void onExit();  
	void isHaveFile();
	// implement the "static node()" method manually
	CREATE_FUNC(MainScene);
};

#endif // __MainScene_SCENE_H__