#include "Board.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* Board::createScene()
{
    return Board::create();
}

bool Board::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    initTiles();
    initPlayer();
    initClickListener();
    startDice();
    

    return true;
}

void Board::initTiles()
{
    Size screenSize = Director::getInstance()->getVisibleSize();
    
    float yPosition = screenSize.height/3;
    
    //First tile
    auto stoneTile = Sprite::create("stone.png");
    stoneTile->setScale(0.85, 0.5);
    
    stoneTile->setPosition(Vec2(stoneTile->getContentSize().width / 2, yPosition));
    
    addChild(stoneTile);
    
    
    firstTileSize = stoneTile->getContentSize();
    firstTilePosition = stoneTile->getPosition();
    
    //6 tiles
    for(int i = 1; i < 3; i++)
    {
        Sprite* tile = Sprite::create("grass.png");
        
        tile->setScale(0.85, 0.5);
        
        float xPosition = screenSize.width / 3 * i + tile->getContentSize().width / 2;
        
        tile->setPosition(Vec2(xPosition, yPosition));
        
        addChild(tile);
        
        //SceneLabel
        auto label = Label::create();
		auto maximapuntuacion = Label::create();

        label->setScale(1.f/0.85, 2.f);
        label->setPosition(Vec2(tile->getContentSize().width/2, 0));

		maximapuntuacion->setScale(1.f / 0.85, 2.f);
		maximapuntuacion->setPosition(Vec2((tile->getContentSize().width/2) , -20));
        
        tile->addChild(label);
		tile->addChild(maximapuntuacion);
        label->setString(sceneNames[i-1]);
		maximapuntuacion->setString(scenePunctuation[i - 1]);
    }
}

void Board::initPlayer()
{
    playerSprite = Sprite::create("p_stand.png");
    playerSprite->setScale(0.5);

	playerjumpSprite = Sprite::create("p_jump.png");
	playerjumpSprite->setScale(0.5);
	playerjumpSprite->setVisible(false);
	

    
    float xPosition = firstTilePosition.x;
    float yPosition = firstTilePosition.y + playerSprite->getContentSize().height/2 - 6;
    
    playerSprite->setPosition(Vec2(xPosition, yPosition));
	playerjumpSprite->setPosition(Vec2(xPosition, yPosition));
    
	addChild(playerjumpSprite);
    addChild(playerSprite);
}

void Board::initClickListener()
{
    auto mouseListener = EventListenerTouchOneByOne::create();
    
    mouseListener->onTouchBegan = [=](Touch* touch, Event* event){
        stopDiceAndMove();
        return true;
    };
    mouseListener->onTouchMoved = [=](Touch* touch, Event* event){};
    
    mouseListener->onTouchEnded = [=](Touch* touch, Event* event){};
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Board::stopDiceAndMove()
{
    stopDice();
	

    Size screenSize = Director::getInstance()->getVisibleSize();
    
    Vec2 finalPosition = Vec2(screenSize.width / 3 * actualNumber + firstTileSize.width / 2, playerSprite->getPosition().y);
	Vec2 finalPositionjump = Vec2(screenSize.width / 3 * actualNumber + firstTileSize.width / 2, playerjumpSprite->getPosition().y);

    auto jumps = JumpTo::create(actualNumber * 0.6, finalPosition, 60, actualNumber);
	auto jumps2 = JumpTo::create(actualNumber * 0.6, finalPosition, 60, actualNumber);
    
	playerjumpSprite->setVisible(true);
	

    playerSprite->runAction(jumps);
	playerjumpSprite->runAction(jumps2);
    
    schedule([=](float dt){
        Director::getInstance()->pushScene(sceneConstructors[actualNumber-1]());
    }, actualNumber, 1, 0, "changeScene");
	playerSprite->setVisible(false);
}

void Board::startDice()
{
    Size screenSize = Director::getInstance()->getVisibleSize();
    //auto diceLabel = Label::create();
    //
    //diceLabel->setPosition(Vec2(screenSize/3.f * 2.f));
    //diceLabel->setSystemFontSize(40);
    //
    //addChild(diceLabel);
    	

    schedule([=](float dt){



        actualNumber %= sceneConstructors.size();
        actualNumber++; //counter
        

		if (actualNumber == 1)
		{
			dice1 = Sprite::create("dice1.png");
			dice1->setScale(1);
			dice1->setPosition(Vec2(screenSize / 3.f * 2.f));
			addChild(dice1);
		}
		if (actualNumber == 2)
		{
			dice1 = Sprite::create("dice2.png");
			dice1->setScale(1);
			dice1->setPosition(Vec2(screenSize / 3.f * 2.f));
			addChild(dice1);
		}
		
		//if (actualNumber == 6)
		//{
		//	dice1 = Sprite::create("dice6.png");
		//	dice1->setScale(1);
		//	/*float xPosition = firstTilePosition.x;
		//	float yPosition = firstTilePosition.y + dice1->getContentSize().height / 2 - 6;
		//	dice1->setPosition(Vec2(xPosition, yPosition));*/
		//	dice1->setPosition(Vec2(screenSize / 3.f * 2.f));
		//	addChild(dice1);
		//}

		

        string text = "";
        text.push_back(actualNumber+'0');
        /*diceLabel->setString(text);*/
        
    }, 0.1f, -1, 0, "changeDiceNumber");
    
}

void Board::stopDice()
{
    unschedule("changeDiceNumber");
}

