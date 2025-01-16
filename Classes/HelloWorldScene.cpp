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

#include "HelloWorldScene.h"
#include "audio/include/AudioEngine.h"
#include <chrono>
#include <thread>

USING_NS_CC;

float SCROLL_INTERVAL = 100.00;
float START_X = 480.00;
float START_Y = 550.00;
float SHIFT = 600;

std::string ANIMATION_MOVE_DOWN = "moveDown",
            ANIMATION_MOVE_UP = "moveUp",
            ANIMATION_MOVE_RIGHT = "moveRight",
            ANIMATION_SPEARGOBLIN_MOVE_RIGHT = "spearGoblinMoveRight";

Scene* HelloWorld::createScene()
{
    //auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );

    //auto layer = HelloWorld::createWithPhysics();
    //return HelloWorld::createWithPhysics();
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    //this->_physicsWorld->setGravity(Vec2(0, 0));
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    /*
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    */
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label std::to_string((int)var)
    std::string labelStr = std::to_string((int)this->unitsCount);
    auto label = Label::createWithTTF("Sprites #", "fonts/arial.ttf", 62);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - label->getContentSize().height));

    this->unitsCountLabel = Label::createWithTTF(labelStr, "fonts/arial.ttf", 62);
    this->unitsCountLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 + label->getContentSize().width,
        origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 3);

    this->addChild(this->unitsCountLabel, 3);
    /*
    
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }*/

    // add "HelloWorld" splash screen"
    /*
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        //sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        //this->addChild(sprite, 0);
    }*/

    this->addUnitPosX = START_X;
    this->addUnitPosY = START_Y + SHIFT;

    auto map = TMXTiledMap::create("strategy_map.tmx");
    //auto map = TMXTiledMap::create("big_map/map_100.tmx");
    Size mapSize = map->getMapSize();
    Size tileSize = map->getTileSize();
    this->mapWidth = (int)mapSize.width * (int)tileSize.width;
    this->mapHeight = (int)mapSize.height * (int)tileSize.height;
    //log("map size w tiles: %f", mapSize.width);
    //log("map size w tile px: %f", tileSize.width);
    //log("map size w: %i", this->mapWidth);
    addChild(map, 0); // with a tag of '99'

    //auto groundL = map->getLayer("ground");
    //auto gSprite = groundL->getTileAt()

    auto _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
    _mouseListener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
    _mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


    auto _contactListener = EventListenerPhysicsContact::create();
    _contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_contactListener, this);

    addStartUnitsAndBuildings();
    return true;
}

void HelloWorld::addStartUnitsAndBuildings() {

    //SpriteFrame* frame = SpriteFrame::create("./images/MiniWorldSprites/Characters/Monsters/Orcs/FarmerGoblin.png", Rect(0.00, 0.00, 160.00, 224.00));

    //auto atlas = TextureAtlas::create("./images/MiniWorldSprites/Characters/Monsters/Orcs/FarmerGoblin.png", 32);
    auto texture = Director::getInstance()->getTextureCache()->addImage("./images/MiniWorldSprites/Characters/Monsters/Orcs/FarmerGoblin.png");
    auto frame1 = SpriteFrame::createWithTexture(texture, Rect(0, 0, 32, 32));

    Vector<SpriteFrame*> framesMoveDown = {
        SpriteFrame::createWithTexture(texture, Rect(0, 0, 32, 32)),
        SpriteFrame::createWithTexture(texture, Rect(32, 0, 32, 32)),
        SpriteFrame::createWithTexture(texture, Rect(64, 0, 32, 32)),
        SpriteFrame::createWithTexture(texture, Rect(96, 0, 32, 32)),
        SpriteFrame::createWithTexture(texture, Rect(128, 0, 32, 32))
    };

    Vector<SpriteFrame*> framesMoveUp = {
        SpriteFrame::createWithTexture(texture, Rect(0, 32, 32, 32)),
        SpriteFrame::createWithTexture(texture, Rect(32, 32, 32, 32)),
        SpriteFrame::createWithTexture(texture, Rect(64, 32, 32, 32)),
        SpriteFrame::createWithTexture(texture, Rect(96, 32, 32, 32)),
        SpriteFrame::createWithTexture(texture, Rect(128, 32, 32, 32))
    };

    Vector<SpriteFrame*> framesMoveRight = {
       SpriteFrame::createWithTexture(texture, Rect(0, 64, 32, 32)),
       SpriteFrame::createWithTexture(texture, Rect(32, 64, 32, 32)),
       SpriteFrame::createWithTexture(texture, Rect(64, 64, 32, 32)),
       SpriteFrame::createWithTexture(texture, Rect(96, 64, 32, 32)),
       SpriteFrame::createWithTexture(texture, Rect(128, 64, 32, 32))
    };

    auto textureSpearGoblin = Director::getInstance()->getTextureCache()->addImage("./images/MiniWorldSprites/Characters/Monsters/Orcs/SpearGoblin.png");

    Vector<SpriteFrame*> framesSpearGoblinMoveRight = {
        SpriteFrame::createWithTexture(textureSpearGoblin, Rect(0, 64, 32, 32)),
        SpriteFrame::createWithTexture(textureSpearGoblin, Rect(32, 64, 32, 32)),
        SpriteFrame::createWithTexture(textureSpearGoblin, Rect(64, 64, 32, 32)),
        SpriteFrame::createWithTexture(textureSpearGoblin, Rect(96, 64, 32, 32)),
        SpriteFrame::createWithTexture(textureSpearGoblin, Rect(128, 64, 32, 32))
    };

    animationFrames[ANIMATION_MOVE_DOWN] = framesMoveDown;
    animationFrames[ANIMATION_MOVE_UP] = framesMoveUp;
    animationFrames[ANIMATION_MOVE_RIGHT] = framesMoveRight;
    animationFrames[ANIMATION_SPEARGOBLIN_MOVE_RIGHT] = framesSpearGoblinMoveRight;

    auto sprite = Sprite::createWithSpriteFrame(frame1);
    if (sprite == nullptr)
    {
        problemLoading("'./images/MiniWorldSprites/Characters/Monsters/Orcs/FarmerGoblin.png'");
    }
    else
    {
        //sprite->setTextureRect(Rect(0.00, 0.00, 32.00, 32.00));
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(START_X, START_Y));
        sprite->setName("playerUnit");
        
        //sprite->setAtlasIndex(8);
        // add the sprite as a child to this layer
        this->addChild(sprite, 1);
    }
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::onMouseDown(Event* event)
{
    // to illustrate the event....
    EventMouse* e = (EventMouse*)event;

    //log("%d", e->getMouseButton());

    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
        // try to select sprite
        float x = e->getCursorX() - this->getPositionX();
        float y = e->getCursorY() - this->getPositionY();
        /*
        log("<<---------------click----------------->>");
        log("X: %f", x);
        log("Y: %f", y);
        log("posX %f", this->getPositionX());
        log("posY %f", this->getPositionY());
        log("*****************************************");
        */
        auto children = this->getChildren();

        for (auto child : children) {
            if (child->getName() == "playerUnit") {
                if (child->getBoundingBox().containsPoint(Vec2(x, y))) {
                    log("orc selected!!");
                    this->removeSelections();
                    AudioEngine::play2d("audio/gruntnogold1.mp3");
                    Vec2 centerPos = child->getPosition();
                    centerPos.x -= child->getAnchorPointInPoints().x;
                    centerPos.y -= child->getAnchorPointInPoints().y;
                    auto node = DrawNode::create();
                    cocos2d::Color4F white(1, 1, 1, 1);
                    node->drawRect(Vec2(0, 0), Vec2(32, 0), Vec2(32, 32), Vec2(0, 32), white);
                    node->setPosition(centerPos);
                    node->setName("selection");
                    this->addChild(node, 2);
                    this->selectedUnits.push_back(child);
                    this->selectedUnits.push_back(node);
                    break;
                }
                else {
                    if (this->getChildByName("selection") != nullptr) {
                        for (auto unit : this->selectedUnits) {
                            this->moveUnitTo(unit, x, y);

                        }
                    }
                }
            }
        }
    }
}

void HelloWorld::onMouseUp(Event* event)
{
    // to illustrate the event....
    EventMouse* e = (EventMouse*)event;
    log("%d", e->getMouseButton());
}

void HelloWorld::onMouseMove(Event* event)
{
    // to illustrate the event....
    EventMouse* e = (EventMouse*)event;

    //log("X: %f", e->getCursorX());
    //log("Y: %f", e->getCursorY());
}

// Implementation of the keyboard event callback function prototype
void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    log("Key with keycode %d pressed", keyCode);
    bool isMoveScreenPressed = false;
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW] = true;
        isMoveScreenPressed = true;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW] = true;
        isMoveScreenPressed = true;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
        this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW] = true;
        isMoveScreenPressed = true;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        //this->setAnchorPoint(Vec2(0.4, 0.4));
        this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW] = true;
        isMoveScreenPressed = true;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
        //this->setAnchorPoint(Vec2(0.4, 0.4));
        this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_SPACE] = true;
        log("Start spawn thread");
        //auto scheduler = Director::getInstance()->getScheduler();
        //scheduler->schedule(CC_CALLBACK_0(HelloWorld::addUnitAndMove, this), )
        //scheduler->performFunctionInCocosThread(CC_CALLBACK_0(HelloWorld::addUnitAndMove, this));
        this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::addUnitAndMove), 0.002f);
        //std::thread thr_add(&HelloWorld::addUnitAndMoveThread, this);
        //log("Stop spawn thread");
        //thr_add.detach();
        break;
    }
    
    if (isMoveScreenPressed == true && this->isScroll == false) {
        //std::thread thr(&HelloWorld::startScroll, this);

        //thr.detach();
        //auto scheduler = Director::getInstance()->getScheduler();

        this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::moveScreen), 0.1f);
    }
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    log("Key with keycode %d released", keyCode);
    this->keyPressed[keyCode] = false;
    if (
        this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW] == false
        && this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW] == false
        && this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW] == false
        && this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW] == false) {
            this->isScroll = false;
    }
    if (this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_SPACE] == false) {
        this->isSpawnUnits = false;
        this->addUnitPosX = START_X;
        this->addUnitPosY = START_Y + SHIFT;
    }
    this->unscheduleAllCallbacks();
}

bool HelloWorld::onContactBegin(cocos2d::PhysicsContact& contact) {
    PhysicsBody* a = contact.getShapeA()->getBody();
    PhysicsBody* b = contact.getShapeB()->getBody();

    log("collision on");

    return true;
}

void HelloWorld::addUnitAndMoveThread() {
    if (this->isSpawnUnits == false) {
        this->isSpawnUnits = true;
        while (this->isSpawnUnits == true) {
            log("add new unit");
            auto sprite = Sprite::createWithSpriteFrame(*animationFrames[ANIMATION_MOVE_RIGHT].begin());
            //auto sprite2 = Sprite::createWithSpriteFrame(*animationFrames[ANIMATION_SPEARGOBLIN_MOVE_RIGHT].begin());
            log("sprite created");
            if (sprite == nullptr)
            {
                problemLoading("'./images/MiniWorldSprites/Characters/Monsters/Orcs/FarmerGoblin.png'");
            }
            else
            {
                log("set sprite coords");
                //sprite->setTextureRect(Rect(0.00, 0.00, 32.00, 32.00));
                // position the sprite on the center of the screen
                sprite->setPosition(Vec2(this->addUnitPosX, this->addUnitPosY));
                this->addUnitPosY -= 16.00;
                sprite->setName("playerUnit");

                auto spriteBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0, 0, 0));
                //spriteBody->setCollisionBitmask(0);
               // spriteBody->setContactTestBitmask(true);
                sprite->setPhysicsBody(spriteBody);

                this->addChild(sprite, 1);
                this->moveUnitTo(sprite, 2500, this->addUnitPosY);

                ++this->unitsCount;
                this->unitsCountLabel->setString(std::to_string((int)this->unitsCount));
                //++this->unitsCount;
                log("added unit, units number: %i", this->unitsCount);
                //});
                std::this_thread::sleep_for(std::chrono::milliseconds((int)SCROLL_INTERVAL));
            }
        }
    }
}

void HelloWorld::addUnitAndMove(float dt) {
    log("add new unit");
    auto sprite = Sprite::createWithSpriteFrame(*animationFrames[ANIMATION_MOVE_RIGHT].begin());
    //auto sprite2 = Sprite::createWithSpriteFrame(*animationFrames[ANIMATION_SPEARGOBLIN_MOVE_RIGHT].begin());
    log("sprite created");
    if (sprite == nullptr)
    {
        problemLoading("'./images/MiniWorldSprites/Characters/Monsters/Orcs/FarmerGoblin.png'");
    }
    else
    {
        log("set sprite coords x: %f", this->addUnitPosX, " y: %f", this->addUnitPosY);
        //sprite->setTextureRect(Rect(0.00, 0.00, 32.00, 32.00));
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(this->addUnitPosX, this->addUnitPosY));
        this->addUnitPosY -= 16.00;
        sprite->setName("playerUnit");
            
        //auto spriteBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0, 0, 0));
        //spriteBody->setCollisionBitmask(0);
        // spriteBody->setContactTestBitmask(true);
        //sprite->setPhysicsBody(spriteBody);

        this->addChild(sprite, 1);
        this->moveUnitTo(sprite, 2500, this->addUnitPosY);

        //sprite2->setPosition(Vec2(this->addUnitPosX, this->addUnitPosY));

        //this->addUnitPosY -= 32.00;
           
        //sprite2->setName("playerUnit1");
        //auto spriteBody2 = PhysicsBody::createBox(sprite2->getContentSize(), PhysicsMaterial(0, 0, 0));
        //sprite2->setPhysicsBody(spriteBody2);

        //sprite->setAtlasIndex(8);
        // add the sprite as a child to this layer
        //log("add sprite to the map");
        //Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
            
        //this->addChild(sprite2, 1);

        //log("move unit");
            
        //this->moveUnitTo(sprite2, 2500, this->addUnitPosY);

        ++this->unitsCount;
        this->unitsCountLabel->setString(std::to_string((int)this->unitsCount));
        //++this->unitsCount;
        log("added unit, units number: %i", this->unitsCount);
        //});
    }
}

void HelloWorld::moveScreen(float dt) {
    float x, y;
    float shift = 100.00;
    log("move map");
    if (this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW] == true) {
        x = this->getPositionX();
        log("move map left %f", x);
        if (x < 0) {
            this->setPositionX(x + shift);
        }
    }

    if (this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW] == true) {
        x = this->getPositionX();
        log("move map right %f", x);
        if (x > -(this->mapWidth)) {
            log("set pos %f", x - shift);
            this->setPositionX(x - shift);
        }
    }

    if (this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW] == true) {
        y = this->getPositionY();
        log("move map up %f", y);
        if (y > -(600 + this->mapHeight)) {

            this->setPositionY(y - shift);
        }
    }

    if (this->keyPressed[cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW] == true) {
        y = this->getPositionY();
        log("move map down %f", y);
        if (y < 0) {
            this->setPositionY(y + shift);
        }
    }
}

void HelloWorld::removeSelections() {
    this->removeChildByName("selection");
    this->selectedUnits.clear();
}

void HelloWorld::moveUnitTo(Node* node, float x, float y) {
    Vec2 currPos = node->getPosition();
    x += node->getAnchorPointInPoints().x;
    y += node->getAnchorPointInPoints().y;
    float distance = currPos.distance(Vec2(x, y));
    auto calcAngle = [](Vec2 pos1, Vec2 pos2) { return atan2((pos2.y - pos1.y), (pos2.x - pos1.x)); };
    float angle = calcAngle(currPos, Vec2(x, y));
    float moveTime = distance / 100.00;
    log("distance: %f", distance);
    log("time to reach: %f", moveTime);
    log("angle: %f", angle);
    auto moveTo = MoveTo::create(moveTime, Vec2(x, y));
    
    node->runAction(moveTo);
    
    if (node->getName() == "playerUnit") {
        std::string animationType = "none";
        auto sprite = dynamic_cast<Sprite*>(node);
        if ((M_PI / 4 >= angle) && (angle >= -1 * M_PI / 4)) {
            animationType = ANIMATION_MOVE_RIGHT;
            sprite->setFlippedX(false);
        }
        else if ((3 * M_PI / 4 >= angle) && (angle >= M_PI / 4)) {
            animationType = ANIMATION_MOVE_UP;
        }
        else if ((angle <= (- 1 * M_PI / 4)) && (angle >= (-3 * M_PI / 4))) {
            animationType = ANIMATION_MOVE_DOWN;
        }
        else if ((angle >= 3 * M_PI / 4) || (angle <= -3 * M_PI / 4)) {
            animationType = ANIMATION_MOVE_RIGHT;
            sprite->setFlippedX(true);
        }
        // set animation
        float animationTickTime = 0.3;
        float animationFrameTime = animationFrames[animationType].size() * animationTickTime;
        int animationRepeats = static_cast<int>(moveTime / animationFrameTime);
        auto animation = Animation::createWithSpriteFrames(animationFrames[animationType], animationTickTime);
        auto animate = Animate::create(animation);
        auto repeatAction = Sequence::create(animate, DelayTime::create(.0f), nullptr);
        auto repeatAnim = Repeat::create(repeatAction, animationRepeats);
        sprite->runAction(repeatAnim);
    }

    if (node->getName() == "playerUnit1") {
        std::string animationType = "none";
        auto sprite = dynamic_cast<Sprite*>(node);
        float animationTickTime = 0.3;
        float animationFrameTime = animationFrames[ANIMATION_SPEARGOBLIN_MOVE_RIGHT].size() * animationTickTime;
        int animationRepeats = static_cast<int>(moveTime / animationFrameTime);
        auto animation = Animation::createWithSpriteFrames(animationFrames[ANIMATION_SPEARGOBLIN_MOVE_RIGHT], animationTickTime);
        auto animate = Animate::create(animation);
        auto repeatAction = Sequence::create(animate, DelayTime::create(.0f), nullptr);
        auto repeatAnim = Repeat::create(repeatAction, animationRepeats);
        sprite->runAction(repeatAnim);
    }
}