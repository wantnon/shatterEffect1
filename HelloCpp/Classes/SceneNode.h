

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
        isDisapear=false;
        
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
        if(this->getOpacity()==0||this->getScale()==0){
            setIsDisapear(true);
        }
        if(getIsDisapear()){
            unscheduleUpdate();
        }
        CCPoint pos=this->getPosition();
        velocity=velocity+accel;
        pos=pos+velocity;
        this->setPosition(pos);
        //
        time+=t;
        if(time>1000000)time=0;
    }
    bool getIsDisapear()const {return isDisapear;}
    void setIsDisapear(float value){isDisapear=value;}
    int getRandomNumber()const {return randomNumber;}
    float getTime()const {return time;}
    void resetTime(){time=0;};
private:
    int randomNumber;
    CCPoint velocity;
    CCPoint accel;
    float time;
    bool isDisapear;
};
class SceneNode : public CCSprite{//here SceneNode is a CCSprite
public:
	SceneNode() {
        d_new=2.7;
        d=-d_new;//we must ensure d!=d_new at initial time
        time=0;
        particleScale0=4.5;
        showThis=true;
        isAllParticleDisapear=false;
        
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
private:
	
	CGLProgramWithMyUnifos program;
    float d;//grid step length
    float d_new;//if d_new!=d then we need to recreate particles
    vector<vector<Cparticle*> > grid;
    CCSpriteBatchNode*spriteBatchNode;
    float time;
    float particleScale0;//particle initial scale
    bool showThis;
    bool isAllParticleDisapear;
};
#endif
