

#ifndef HelloWorld_SceneNode_h
#define HelloWorld_SceneNode_h
#include "cocos2d.h"

#include <vector>
#include<iostream>
#include<map>
using namespace std;

#include "indexVBO.h"
using namespace cocos2d;
//if program1 and program2 have a uniform variable with the same name, the two variable's uniform ID (GLint) may be different. 
//so safe way is let each shader program hold his own uniform IDs.
class CGLProgramWithMyUnifos{
	CCGLProgram*program;
public:
	map<string,GLint> myUnifoMap;
    CGLProgramWithMyUnifos(){
		program=NULL;
	}
	virtual ~CGLProgramWithMyUnifos(){
		program->release();
	}
    void setProgram(CCGLProgram*_program){
        if(program){
            program->release();
        }
        program=_program;
        program->retain();
    }
    CCGLProgram*getProgram(){
        return program;
    }

};

class Cparticle:public CCSprite{
public:
    Cparticle(){
        velocity=CCPoint(0,0);
        accel=CCPoint(0,0);
        randomNumber=rand();
        isDisapear=true;
        
    }
    void setVelocity(const CCPoint&v){
        velocity=v;
    }
    void setAccel(const CCPoint&a){
        accel=a;
    }
    CCPoint getVelocity()const{return velocity;}
    CCPoint getAccel()const{return accel;}
    void update(float t){
        //judge if the particle is disapeared
        if(this->getOpacity()==0||this->getScale()==0){
            setIsDisapear(true);
        }
        //if the particle is disapeared, unschedule it.
        if(getIsDisapear()){
            unscheduleUpdate();
        }
        //update the position of the particle based on velocity and accel
        CCPoint pos=this->getPosition();
        velocity=velocity+accel;
        pos=pos+velocity;
        this->setPosition(pos);
        //update time
        time+=t;
        if(time>1000000)time=0;
    }
    bool getIsDisapear()const {return isDisapear;}
    void setIsDisapear(float value){isDisapear=value;}
    int getRandomNumber()const {return randomNumber;}
    void setRandomNumber(int number){randomNumber=number;}
    float getTime()const {return time;}
    void resetTime(){time=0;};
private:
    int randomNumber;//every particle pre-generate a random number
    CCPoint velocity;
    CCPoint accel;
    float time;//time eclipsed since the particle start to move
    bool isDisapear;//whether the particle is disapeared
};

#define SHAPETYPE_CIRCLE 0
#define SHAPETYPE_HEART 1

class SceneNode : public CCSprite{//here SceneNode is a CCSprite
public:
	SceneNode() {
        d_new=2.7;
        d=-d_new;//we must ensure d!=d_new at initial time in order to trigger the creation of particles and grid.
        time=0;
        particleScale0=4.5;
        showThis=true;
        isAllParticleDisapear=true;
        shapeType=SHAPETYPE_CIRCLE;
        
    }
	virtual ~SceneNode(){};
    bool init(string texFileName);
	void draw() ;
    void update(float t);
    void resetShatter();
    void startShatter();
    void resetTime(){time=0;}
    float getTime()const {return time;}
    bool getIsAllParticleDisapear()const {return isAllParticleDisapear;}
    void setIsAllParticleDisapear(bool value){isAllParticleDisapear=value;}
    void setd_new(float value){d_new=value;}
    float getd_new()const {return d_new;}
    float getParticleScale0()const {return particleScale0;}
    void setParticleScale0(float value){particleScale0=value;}
    void setShapeType(int _shapeType){shapeType=_shapeType;}
    int getShapeType()const {return shapeType;}
    
    float calculateR(const CCPoint&dir,float R_content);
private:
	
	CGLProgramWithMyUnifos program;//crrently unused
    float d;//particle size (grid step length)
    float d_new;//newly seted particle size, if d_new!=d then we need to recreate particles
    vector<vector<Cparticle*> > grid;//hold all the particles
    CCSpriteBatchNode*spriteBatchNode;//all particles added to this batchNode for quick drawing. batchNode added to *this
    float time;//time eclipsed since shatter start
    float particleScale0;//particle initial scale. we may want to make particles looks a litter bigger at the starting.
    bool showThis;//whether need to show *this. when shattering, *this should be hide
    bool isAllParticleDisapear;//whether all particles are disapeared
    int shapeType;
};
#endif
