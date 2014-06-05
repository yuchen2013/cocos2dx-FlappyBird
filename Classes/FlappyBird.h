#ifndef __FlappyBird_SCENE_H__
#define __FlappyBird_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;
class FlappyBird : public cocos2d::CCLayer
{
	CCNode* ground1;
	CCNode* ground2;
	CCSize visibleSize;
	CCPoint origin ;
	CCSprite* wSprite;
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	void startCallback(CCObject* pSender);
	void rankCallback(CCObject* pSender);
	void update(float delta);
	// implement the "static node()" method manually
	CREATE_FUNC(FlappyBird);
};

#endif // __FlappyBird_SCENE_H__