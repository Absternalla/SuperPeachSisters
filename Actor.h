#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include "GameConstants.h"
using namespace std;

class StudentWorld;
class Actors : public GraphObject{ //main class
public:
    virtual void dosomething() = 0; //do something function for ticks
    bool switchdirectionifneeded(int targetX, int targetY);
    Actors(StudentWorld* Sworld, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual bool usesHealth(){return false;}
    void doCommonality();
    virtual void bonked() = 0;
    virtual bool isprojectile(){return false;}

    virtual void damage(){;}
    virtual bool CanBlock()
    {
        return false;
    }
    virtual bool Damageable() //returns if damageable or not
    {
        return false;
    }
    virtual bool Bonkable() //returns if bonkable or not
    {
        return false;
        
    }
    bool isalive(){ //returns life status
        return alive;
    }
    void setdead(){ //lets you set life stataus
        alive = false;
    }
    
    StudentWorld* getWorld(){ //allows access to Student WWorld
        return m_world;
    }
   
    

private:
   
    bool alive; //alive or dead status
    StudentWorld* m_world;//gives access to studentworld
    
};


class peach: public Actors{
public:
    //major functions
    peach(StudentWorld* Sworld, int imageID, int startX, int startY); //constructor
    virtual void dosomething();
    virtual void bonked();
    virtual void damaged(){bonked();}
   
    
    //trivial functions
    virtual bool damageable(){return true;}
    virtual bool usesHealth(){return true;}
    int currenthealth(){return health;}
    
    bool starpowerstatus(){return status(starpower_ticks);}
    bool jumppowerstatus(){return jumppower;}
    bool shootpowerstatus(){return shootpower;}
    void setpowerstatus(string x, int y){
        if(x == "jumppower" and y == 0)
            jumppower = true;
        if(x == "shootpower" and y ==0)
            shootpower = true;
        if(x == "none" and y>0)
            starpower_ticks = y;
    }
   
    void sethealth(int x){health = x;}
    bool temporaryinvincibilitystatus(){return status(invincibility_ticks);}
    bool needsRecharge()
    {
        if(time_to_recharge_before_next_fire > 0)
            return true;
        else
            return false;
    }
    bool JumpStatus(){return status(remaining_jump_distance);}

private:
    bool status(int ticks){
        if(ticks > 0)
            return true;
        else
            return false;
    }
    int health;
    int starpower_ticks;
    bool jumppower;
    bool shootpower;
    int invincibility_ticks;
    int time_to_recharge_before_next_fire;
    int remaining_jump_distance;
    bool CanShootFireball;
    bool jumpinitiated;
 
};

class platforms: public Actors{ //sub class of Actors, includes pipes and blocks
public:
    platforms(StudentWorld* Sworld, int imageID, int startX, int startY);
    virtual void dosomething(){;}
    virtual bool Bonkable(){return true;}
    virtual bool CanBlock(){return true;}
    virtual void bonked(){;}

private:
    
    
};

class pipe : public platforms{
public:
    pipe(StudentWorld* Sworld,  int startX, int startY);
    virtual bool Bonkable(){return false;}
    
};
class block : public platforms{
public:
    block(StudentWorld* Sworld,  int startX, int startY, bool Stargoodie, bool Mushgoodie, bool Flowergoodie, bool Empty);
    virtual void bonked();
    bool Stargoodiestatus(){return Stargoodie;}
    bool Mushgoodiestatus(){return Mushgoodie;}
    bool Flowergoodiestatus(){return Flowergoodie;}
    void ReleaseGoodie(bool goodie){goodie = false;}
  
private:
    bool Stargoodie;
    bool Mushgoodie;
    bool Flowergoodie;
    bool Empty;
    bool Bonked = false;
    
 
};

class objectives: public Actors{ //includes flags and mario
public:
    objectives(StudentWorld* Sworld, int imageID,  int startX, int startY);
    virtual void bonked(){;}
    virtual void dosomething(){;}
    void docommonality3(string x);
    
private:
    
    
};

class Mario: public objectives{
public:
    Mario(StudentWorld* Sworld,  int startX, int startY);
    virtual void bonked(){;}
    virtual void dosomething();

private:
    
};

class Flag: public objectives{
public:
    Flag(StudentWorld* Sworld,  int startX, int startY);
    virtual void bonked(){;}
    virtual void dosomething();
    

private:
};

class goodies : public Actors{ //includes the flower, mushroom, and star goodies
public:
    goodies(StudentWorld* Sworld, int imageID,  int startX, int startY);
    bool doCommonality2(int sethealth, int setscore, string z, int setTicks);
    virtual void dosomething(){;}
    virtual void bonked(){;}
private:
    
    
};
class Flower : public goodies{
public:
    virtual void dosomething();
    virtual void bonked(){;}
    Flower(StudentWorld* Sworld,  int startX, int startY);
private:
};

class Mushroom : public goodies{
public:
    virtual void dosomething();
    virtual void bonked(){;}
    Mushroom(StudentWorld* Sworld,  int startX, int startY);
    
};
class Star : public goodies{
public:
    virtual void dosomething();
    virtual void bonked(){;}
    Star(StudentWorld* Sworld,  int startX, int startY);
};

class projectiles : public Actors{ //includes all the projectiles, so shell, and the fireballs
public:
    projectiles(StudentWorld* Sworld, int imageID, int startX, int startY, int dir);
    virtual void dosomething(){;}
    virtual void bonked(){;}
    void doprojectilebehavior(string z);
private:
};

class Peach_fired_fireballs : public projectiles{
public:
    Peach_fired_fireballs(StudentWorld* Sworld,  int startX, int startY, int dir);
    virtual void dosomething();
    virtual void bonked(){;}
    virtual bool isprojectile(){return true;}
private:
    
};

class Piranha_fired_fireballs: public projectiles{
public:
    Piranha_fired_fireballs(StudentWorld* Sworld,  int startX, int startY, int dir);
    virtual void dosomething();
    virtual void bonked(){;}
    
};

class Shell: public projectiles{
public: Shell(StudentWorld* Sworld, int startX, int startY, int dir);
    virtual void dosomething();
    virtual void bonked(){;}
    
    
};

class enemies : public Actors { //contains the enemies, koopa, piranha, goomba
public:
    enemies(StudentWorld* Sworld, int imageID, int startX, int startY, int dir);
    virtual void dosomething(){;}
    virtual void bonked(){;}
    virtual void koopa_goomba_commonality();
    virtual void bonk_commonality();
    virtual bool bonkable(){return true;}
    virtual bool damageable(){return true;}

private:
    
};

class goomba : public enemies{
public:
    goomba(StudentWorld* Sworld,  int startX, int startY, int dir);
    virtual void dosomething();
    virtual void bonked();
    virtual void damage();
    virtual bool damageable(){return true;}
    
private:
    
};

class koopa : public enemies{
public:
    koopa(StudentWorld* Sworld,  int startX, int startY, int dir);
    virtual void dosomething();
    virtual void bonked();
    virtual void damage();
    virtual bool damageable(){return true;}

private:
    
    
};

class piranha : public enemies{
public:
    piranha(StudentWorld* Sworld,  int startX, int startY, int dir);
    virtual void dosomething();
    virtual void bonked();
    virtual void damage();
private:
    int firing_delay = 0;
};
#endif // ACTOR_H_
