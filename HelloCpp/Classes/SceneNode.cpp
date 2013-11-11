

#include "SceneNode.h"
#include <iostream>
#include <map>
#include "support/ccUtils.h"
#include "myFunc.h"
using namespace cocos2d ;
using namespace std ;


bool SceneNode::init(string texFileName)
{
    //set projection to 2D (default is 3D). if use 3D projection, may cause tiny projection error, in some situation, if the error accumulated across frames, may cause effect wrong.
    CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);
    //init this CCSprite
    this->initWithFile(texFileName.c_str());
    //create spriteBatchNode
    spriteBatchNode=CCSpriteBatchNode::createWithTexture(this->getTexture());
    spriteBatchNode->setAnchorPoint(ccp(0.5,0.5));
    spriteBatchNode->setPosition(ccp(0,0));
    this->addChild(spriteBatchNode);
    //reset shatter
    this->resetShatter();
   	return true ;
}
void SceneNode::startShatter(){
    //prepare *this
    this->unscheduleUpdate();//in order to not schedule twice (cause assert failure), we call unschedule first
    this->scheduleUpdate();
    this->setIsAllParticleDisapear(false);
    this->showThis=false;
    //prepare batchNode
    this->spriteBatchNode->setVisible(true);
    //prepare particles
    int m=(int)grid.size();
    int n=m?(int)grid[0].size():0;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            Cparticle*particle=grid[i][j];
            particle->unscheduleUpdate();
            particle->scheduleUpdate();
            particle->setScale(particleScale0);
            particle->setIsDisapear(false);
        }
    }
}
void SceneNode::resetShatter(){
    if(d_new!=d){//recreate particles
        d=d_new;
        //clear grid
        grid.clear();
        //release all particles
        spriteBatchNode->removeAllChildrenWithCleanup(true);
        //break this sprite into particles
        CCSize contentSize=this->getContentSize();
        CCLOG("contentSize:%f,%f",contentSize.width,contentSize.height);
        int m=contentSize.height/d;//grid row count
        int n=contentSize.width/d;//grid col count
        CCLOG("m n:%i,%i",m,n);
        //resize grid based on m,n
        grid.resize(m);
        for(int i=0;i<m;i++){
            grid[i].resize(n);
        }
        //create particles and add them to spriteBatchNode and grid
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                float x=j*d;
                float y_OatLU=i*d;
                Cparticle*particle=new Cparticle();
                //rect of texture use O-at-upleft coordinate
                particle->initWithTexture(this->getTexture(), CCRect(x,y_OatLU,d,d));
                particle->setAnchorPoint(ccp(0.5,0.5));
                grid[i][j]=particle;
                spriteBatchNode->addChild(particle);
                particle->release();//release
            }
        }
    
    }
    //reset *this
    this->resetTime();
    this->unscheduleUpdate();
    this->showThis=true;
    //reset batchNode
    this->spriteBatchNode->setVisible(false);
    //reset particles
    CCSize contentSize=this->getContentSize();
    int m=(int)grid.size();
    int n=m?(int)grid[0].size():0;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            Cparticle*particle=grid[i][j];
            float x=j*d;
            float y_OatLD=contentSize.height-i*d;
            //particle relative to father node use O-at-downleft coordinate
            //and no matter what the anchorPoint is, origin of father space is at the downleft corner of father sprite.
            particle->setPosition(ccp(x+0.5*d,y_OatLD+0.5*d));
            particle->setVelocity(ccp(0,0));
            particle->setAccel(ccp(0,0));
            particle->setOpacity(255);
            particle->resetTime();
            particle->unscheduleUpdate();
            particle->setScale(1);
        }
    }

}
void SceneNode::update(float t) {
    //update time
    time+=t;
    if(time>1000000)time=0;
    //update particles
    CCSize contentSize=this->getContentSize();
    CCPoint center=CCPoint(contentSize.width/2,contentSize.height/2);
    float R=ccpLength(ccp(contentSize.width,contentSize.height))/2;//radius of srounding circle
    float Rcur=R+time*50;//crrent R
    float R_opacity=R;
    float R_scale=R;
    float R_rand=R;
    int nDisapear=0;
    int m=(int)grid.size();
    int n=m?(int)grid[0].size():0;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            Cparticle*particle=grid[i][j];
            if (particle->getIsDisapear()){
                nDisapear++;
                continue;
            }
            CCPoint pos=particle->getPosition();
            //how far from particle to center
            float r=ccpLength(pos-center);
            //how far from particle to srounding circle edge
            float d=Rcur-r;
            //add random to d
            d+=particle->getRandomNumber()%(int)R_rand-R_rand/2;
            //we move 10 percent of d
            float vLen=d/30;
            //dir
            CCPoint dir=ccpNormalize(pos-center);
            //v
            CCPoint v=ccpMult(dir, vLen);
            //set v to particle
            particle->setVelocity(v);
            //a
            CCPoint a=CCPoint(0,0);
            //set a to particle
            particle->setAccel(a);
            //update opacity
            float opacity=MAX(0,255-r*255/R_opacity);
            particle->setOpacity(opacity);
            //update scale
            particle->setScale(MAX(0, particleScale0-r*particleScale0/R_scale));
        }
    }
    if(nDisapear==m*n){//if all particles are disapeared, reset
        resetShatter();
    }

    
}
void SceneNode::draw()
{
    if(showThis)CCSprite::draw();
 
}