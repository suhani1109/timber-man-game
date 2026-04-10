#include<sstream>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
using namespace sf;

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

void updateBranches(int seed);

//where is the player/branch? left or right
enum class side{LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];



int main(){
    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Timber!!!");
    View view(FloatRect(0, 0, 1920, 1080));
    window.setView(view);
    
    //background sprite
    Texture textureBackground;
    textureBackground.loadFromFile("asset/graphics/background.png");
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0,0);
    
    //Tree sprite
    Texture textureTree;
    textureTree.loadFromFile("asset/graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810,0);
    
    //Bee sprite
    Texture textureBee;
    textureBee.loadFromFile("asset/graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0,800);
    bool beeActive = false;
    float beeSpeed = 0.0f;
    
    //draw cloud sprite
    Texture textureCloud;
    textureCloud.loadFromFile("asset/graphics/cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteCloud1.setPosition(0,0);
    spriteCloud2.setPosition(0,250);
    spriteCloud3.setPosition(0,500);
    bool cloud1Active=false;
    bool cloud2Active=false;
    bool cloud3Active=false;
    
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;
    
    Clock clock;
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920/2.0f) - (timeBarStartWidth/2.0f), 980);
    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth/timeRemaining;
    
    
    
    bool paused = true;
    
    int score = 0;
    Text messageText;
    Text scoreText;
    
    Font font;
    font.loadFromFile("asset/font/KOMIKAP_.ttf");
    
    messageText.setFont(font);
    scoreText.setFont(font);
    messageText.setString("Press Enter to start!!");
    scoreText.setString("Score = 0");
    
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);
    
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920/2.0f, 1080/2.0f);
    scoreText.setPosition(20,20);
    
    //prepare the branches
    Texture textureBranch;
    textureBranch.loadFromFile("asset/graphics/branch.png");
    for(int i=0; i<NUM_BRANCHES; i++){
         branches[i].setTexture(textureBranch);
         branches[i].setPosition(-2000, -2000);
         branches[i].setOrigin(220, 20);
    }
    
    //Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("asset/graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    side playerSide = side::LEFT;
    
    //Prepare the gravestone(RIP)
    Texture textureRIP;
    textureRIP.loadFromFile("asset/graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 600);
    
    //Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("asset/graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);
    
    //line the axe with tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;
    
    //Prepare flying log
    Texture textureLog;
    textureLog.loadFromFile("asset/graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);
    
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;
    bool acceptInput = false;
    

    //Prepare sound
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("asset/sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);
    
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("asset/sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);
    
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("asset/sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);

    while(window.isOpen()){
         Event event;
         while(window.pollEvent(event)){
             if(event.type==Event::KeyReleased && !paused){
                 acceptInput = true;
                 spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
             }
         }
   
         
         while(window.pollEvent(event)){
             if(event.type==Event::Closed){
                 window.close();
             }
         }
   
         if(Keyboard::isKeyPressed(Keyboard::Escape)){
              window.close();
         }
         
         if(Keyboard::isKeyPressed(Keyboard::Return)){
              paused = false;
              score = 0;
              timeRemaining = 6;
              //make all branches disappear
              for(int i=0; i<NUM_BRANCHES; i++){ //i=1 cuz we have to disappear from 2nd branch
                 branchPositions[i] = side::NONE;
              }
              
	      for(int i=0; i<NUM_BRANCHES; i++){ 
                 updateBranches(i);
              }
              spriteRIP.setPosition(675, 2000);
              spritePlayer.setPosition(580, 720);
              acceptInput = true;
         }
         
         //update the scene
         if(acceptInput){
	if(Keyboard::isKeyPressed(Keyboard::Right)){
          playerSide=side::RIGHT;
          score++;
          timeRemaining+=(2/score)+.15;
          spriteAxe.setPosition(AXE_POSITION_RIGHT,spriteAxe.getPosition().y);
          spritePlayer.setPosition(1200,720);
          updateBranches(score);
          spriteLog.setPosition(810,720);
          logSpeedX=-5000;
          logActive=true;
          acceptInput=false;
          chop.play();
	}	
	if(Keyboard::isKeyPressed(Keyboard::Left)){
          playerSide=side::LEFT;
          score++;
          timeRemaining+=(2/score)+.15;
          spriteAxe.setPosition(AXE_POSITION_LEFT,spriteAxe.getPosition().y);
          spritePlayer.setPosition(580,720);
          updateBranches(score);
          spriteLog.setPosition(810,720);
          logSpeedX=5000;
          logActive=true;
          acceptInput=false;
          chop.play();
         }	
         }
         
 
         
         if(!paused){
            Time dt = clock.restart();
             
            timeRemaining -= dt.asSeconds();
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeRemaining));
            
            if(timeRemaining <= 0.0f){
                paused = true;
                messageText.setString("Out of time!");
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920/2.0f, 1080/2.0f);
                outOfTime.play();
                
            }
            
            //setup the bee
           if(!beeActive){
              srand((int)time(0));
              beeSpeed = (rand()%200+400); //200-399
             
              srand((int)time(0)*10);
              float height = (rand()%500) + 500; //500-999
              spriteBee.setPosition(2000, height);
             
              beeActive = true;
             
           }
           else{
              //move the bee
              spriteBee.setPosition(spriteBee.getPosition().x-(beeSpeed*dt.asSeconds()), spriteBee.getPosition().y);
             
              if(spriteBee.getPosition().x < -100){
                   beeActive = false;
              }
           }
         
           //Managing clouds
           //cloud1
           if(!cloud1Active){
              srand((int)time(0)*10);
              cloud1Speed = (rand()%200+100); //100-299
             
              srand((int)time(0)*10);
              float height = (rand()%150); //500-999
              spriteCloud1.setPosition(-300, height);
             
              cloud1Active = true;
             
           }
           else{
              //move the cloud1
              spriteCloud1.setPosition(spriteCloud1.getPosition().x+(cloud1Speed*dt.asSeconds()), spriteCloud1.getPosition().y);
             
              if(spriteCloud1.getPosition().x > 1920){
                   cloud1Active = false;
              }
          }
         
          //cloud2
          if(!cloud2Active){
              srand((int)time(0)*20);
              cloud2Speed = (rand()%200+100); //100-299
             
              srand((int)time(0)*20);
              float height = (rand()%300)-150; //500-999
              spriteCloud2.setPosition(-300, height);
             
              cloud2Active = true;
             
          }
          else{
              //move the cloud2
              spriteCloud2.setPosition(spriteCloud2.getPosition().x+(cloud2Speed*dt.asSeconds()), spriteCloud2.getPosition().y);
             
              if(spriteCloud2.getPosition().x > 1920){
                   cloud2Active = false;
              }
          }
         
          //cloud3
          if(!cloud3Active){
              srand((int)time(0)*30);
              cloud3Speed = (rand()%200+100); //100-299
             
              srand((int)time(0)*30);
              float height = (rand()%450)-150; //500-999
              spriteCloud3.setPosition(-300, height);
             
              cloud3Active = true;
             
           }
           else{
              //move the cloud3
              spriteCloud3.setPosition(spriteCloud3.getPosition().x+(cloud3Speed*dt.asSeconds()), spriteCloud3.getPosition().y);
             
              if(spriteCloud3.getPosition().x > 1920){
                   cloud3Active = false;
              }
           }
           //score++;
           std::stringstream ss;
           ss << "Score = " << score;
           scoreText.setString(ss.str());
           
           //update the branch sprite
           for(int i=0; i<NUM_BRANCHES; i++){
               float height = i*150;
               if(branchPositions[i] == side::LEFT){
                   branches[i].setPosition(610, height);
                   branches[i].setRotation(180);
               }
               else if(branchPositions[i] == side::RIGHT){
                   branches[i].setPosition(1330, height);
                   branches[i].setRotation(0);
               }
               else{
                   branches[i].setPosition(3000, height);
               }
           }
           
           if(logActive){
              spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
           
           }
           if(spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000){
                logActive = false;
                spriteLog.setPosition(810, 720);
               
           }
           
           //death
           if(branchPositions[5] == playerSide){
              paused = true;
              acceptInput = false;
              spriteRIP.setPosition(525, 760);
              spritePlayer.setPosition(2000, 660);
              spriteAxe.setPosition(2000, 660);
              
              messageText.setString("SQUISHED!");
              FloatRect textRect = messageText.getLocalBounds();
              messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
              death.play();
              
            
           }
           
           }//end of if(!paused)
         
           window.clear();
           //draw the scene
           window.draw(spriteBackground);
           window.draw(spriteCloud1);
           window.draw(spriteCloud2);
           window.draw(spriteCloud3);
         
           for(int i=0; i<NUM_BRANCHES; i++){
               window.draw(branches[i]);
           }
           
           window.draw(spriteTree);
           window.draw(spritePlayer);
           window.draw(spriteLog);
           window.draw(spriteAxe);
           window.draw(spriteRIP);
           window.draw(spriteBee);
           
           window.draw(timeBar);
         
           window.draw(scoreText);
           if(paused){
              window.draw(messageText);
           }
         
         
           //show everything what you have drawn
           window.display();
    }
}

void updateBranches(int seed){
     for(int j = NUM_BRANCHES - 1; j > 0; j--){
         branchPositions[j] = branchPositions[j-1];
     }
     srand((int)time(0) + seed);
     int r = (rand()%5); //to generate random numbers from 0 to 4
     
     switch(r){
         case 0: branchPositions[0] = side::LEFT;
         break;
         case 1: branchPositions[0] = side::RIGHT;
         break;
         default: branchPositions[0] = side::NONE;
         break;
     }
}