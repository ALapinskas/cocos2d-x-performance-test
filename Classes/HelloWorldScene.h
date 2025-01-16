/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
private:
    int mapWidth;
    int mapHeight;

    bool isScroll = false;
    bool isSpawnUnits = false;
    int addUnitPosX = 0;
    int addUnitPosY = 0;
    size_t unitsCount = 0;
    cocos2d::Label* unitsCountLabel;
    std::map<cocos2d::EventKeyboard::KeyCode, bool> keyPressed;
    std::vector<Node*> selectedUnits;
    std::map<std::string, cocos2d::Vector<cocos2d::SpriteFrame*>> animationFrames;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void onMouseMove(cocos2d::Event*);
    virtual void onMouseUp(cocos2d::Event*);
    virtual void onMouseDown(cocos2d::Event*);
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    virtual bool onContactBegin(cocos2d::PhysicsContact& contact);
    virtual void addStartUnitsAndBuildings();
    virtual void moveScreen(float dt);
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void removeSelections();
    void moveUnitTo(Node*, float x, float y);
    void addUnitAndMoveThread();
    void addUnitAndMove(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    ~HelloWorld()
    {
        isScroll = false;
    }
};

#endif // __HELLOWORLD_SCENE_H__
