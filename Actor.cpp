#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//constructors to classes
Actors::Actors(StudentWorld* Sworld, int imageID, int startX, int startY, int dir, int depth, double size) : GraphObject(imageID, startX, startY, dir, depth, size)
{
    alive = true;
    m_world = Sworld;
}

platforms::platforms(StudentWorld* Sworld, int imageID, int startX, int startY) : Actors(Sworld, imageID, startX, startY, 0, 2, 1.0){
    ;
}
block::block(StudentWorld* Sworld, int startX, int startY, bool Stargoodiestatus, bool Mushgoodiestatus, bool Flowergoodiestatus, bool Emptystatus) : platforms(Sworld, IID_BLOCK, startX, startY){Stargoodie = Stargoodiestatus; Mushgoodie = Mushgoodiestatus; Flowergoodie = Flowergoodiestatus; Empty = Emptystatus;}

pipe::pipe(StudentWorld* Sworld, int startX, int startY) : platforms(Sworld, IID_PIPE, startX, startY){;}


goodies::goodies(StudentWorld* Sworld, int imageID,  int startX, int startY) : Actors(Sworld, imageID, startX, startY, 0, 1, 1){;}

Flower::Flower(StudentWorld* Sworld, int startX, int startY) : goodies(Sworld, IID_FLOWER, startX, startY){;}
Mushroom::Mushroom(StudentWorld* Sworld, int startX, int startY) : goodies(Sworld, IID_MUSHROOM, startX, startY){;}
Star::Star(StudentWorld* Sworld, int startX, int startY) : goodies(Sworld, IID_STAR, startX, startY){;}

objectives::objectives(StudentWorld* Sworld, int imageID,  int startX, int startY) : Actors(Sworld, imageID, startX, startY, 0, 1, 1){;}

Flag::Flag(StudentWorld* Sworld, int startX, int startY) : objectives(Sworld, IID_FLAG, startX, startY){;}
Mario::Mario(StudentWorld* Sworld, int startX, int startY) : objectives(Sworld, IID_MARIO, startX, startY){;}

projectiles::projectiles(StudentWorld* Sworld, int imageID,  int startX, int startY, int dir): Actors(Sworld, imageID, startX, startY, dir, 1, 1){;}

Piranha_fired_fireballs::Piranha_fired_fireballs(StudentWorld* Sworld,  int startX, int startY, int dir): projectiles(Sworld, IID_PIRANHA_FIRE, startX, startY, dir){;}
Peach_fired_fireballs::Peach_fired_fireballs(StudentWorld* Sworld,  int startX, int startY, int dir): projectiles(Sworld, IID_PEACH_FIRE, startX, startY, dir){;}
Shell::Shell(StudentWorld* Sworld,  int startX, int startY, int dir): projectiles(Sworld, IID_SHELL, startX, startY, dir){;}


enemies::enemies(StudentWorld* Sworld, int imageID,  int startX, int startY, int dir): Actors(Sworld, imageID, startX, startY, dir, 1, 0){;}

koopa::koopa(StudentWorld* Sworld,  int startX, int startY, int dir): enemies(Sworld, IID_KOOPA, startX, startY, dir){;}
goomba::goomba(StudentWorld* Sworld,  int startX, int startY, int dir): enemies(Sworld, IID_GOOMBA, startX, startY, dir){;}
piranha::piranha(StudentWorld* Sworld,  int startX, int startY, int dir): enemies(Sworld, IID_PIRANHA, startX, startY, dir){firing_delay =0;}


peach::peach(StudentWorld* Sworld, int imageID, int startX, int startY) : Actors(Sworld, IID_PEACH, startX, startY, 0, 0, 1.0){health = 1; starpower_ticks =0; jumppower=false; shootpower= false; invincibility_ticks=0; time_to_recharge_before_next_fire =0; remaining_jump_distance = 0; CanShootFireball = false; jumpinitiated = false;}


void piranha::dosomething(){
    Actors* x;
    if(isalive()==false) //if dead, return immediately
    {
        return;
    }
    increaseAnimationNumber();
    if(getWorld()-> check_overlap_peach(x, this, getX(), getY())){
        x -> bonked();
        return;
    }
    if(abs(getWorld() -> getPeach() -> getY() - getY()) > 1.5*SPRITE_HEIGHT){
        return;
    }
    else
    {
        int d = getWorld() -> getPeach() -> getDirection();
        if(d == 0)
            setDirection(180);
        if(d == 180)
            setDirection(0);
        if(firing_delay > 0)
        {
            firing_delay--;
            return;
        }
        if(firing_delay <=0)
        {
            if(abs(getWorld() -> getPeach() -> getX() - getX()) < 8*SPRITE_WIDTH){
                getWorld() -> addtoVector(new Piranha_fired_fireballs(getWorld(), getX(), getY(), getDirection()));
                getWorld() -> playSound(SOUND_PIRANHA_FIRE);
                firing_delay = 40;
            }
            
        }
            
        
        
    }
    
    
}


bool Actors::switchdirectionifneeded(int targetX, int targetY){ //if you cant go to target destination, return true and switch direction, if you can go to target destination return false and do nothing.
    Actors* z;
    if(getWorld()-> check_blocking_object(z, this, targetX, targetY))
    {
        int x;
        x = getDirection();
        if(x == 0)
            setDirection(180);
        if(x == 180)
            setDirection(0);
        return true;
    }
    return false;
}

void enemies::bonk_commonality(){ //both have the same bonk method
    Actors* x;
    if(getWorld()-> check_overlap_peach(x, this, getX(), getY()) == false)
    {
        ;
    }
    else  //if not bonked by peach, ignore, but if you are, increase score, set dead, and play sound
    {
        if(getWorld()-> getPeach() -> starpowerstatus() == true)
        {
            getWorld()-> playSound(SOUND_PLAYER_KICK);
            getWorld()-> increaseScore(100);
            setdead();
            
        }
        
    }
        
}


void enemies::koopa_goomba_commonality(){ //do something functions for both of these are the same
    Actors* x;
    if(isalive()==false) //if dead, return immediately
    {
        return;
    }
    if(getWorld()-> check_overlap_peach(x, this, getX(), getY())){ //if overlap with peach bonk her and return.
        x -> bonked();
        return;
    }
    //check to see, step can be taken, if so check to see if you'd still be on a platform. Switch direction if you can't
    if(getDirection() == 0)
    {
        if(getWorld() -> check_blocking_object(x, this, getX()+1, getY()))
            setDirection(180);
        else
        {
            if(getWorld() -> check_blocking_object(x, this, getX()+1, getY()-1) == false)
                setDirection(180);
        }
            
    }
    if(getDirection() == 180)
    {
        if(getWorld() -> check_blocking_object(x, this, getX()-1, getY()))
            setDirection(0);
        else
        {
            if(getWorld() -> check_blocking_object(x, this, getX(), getY()-1) == false)
                setDirection(0);
        }
            
    }
    //get direction, now see if there's a blocking object in the location you want to go to. If there isn't move there.
    int d = getDirection();
    if(d == 0)
    {
    if(getWorld() -> check_blocking_object(x, this, getX()+1, getY()))
        return;
    else
        moveTo(getX()+1, getY());
    }
    if(d == 180)
    {
    if(getWorld() -> check_blocking_object(x, this, getX()-1, getY()))
        return;
    else
        moveTo(getX()-1, getY());
    }
    
}

void koopa::dosomething(){
    koopa_goomba_commonality(); //using the common functions they both share.
}
void goomba::dosomething(){
    koopa_goomba_commonality();
}

void koopa::bonked(){
    bonk_commonality();
}
void goomba::bonked(){
    bonk_commonality();
}

void piranha::bonked(){
    bonk_commonality();
}

void koopa::damage(){ //if damaged, increase score, die, then introduce shell projectile
    getWorld()-> increaseScore(100);
    getWorld() -> addtoVector(new Shell(getWorld(), getX(), getY(), getDirection()));
    setdead();
    
}
void goomba::damage(){ //same as above, but don't introduce shell
    getWorld()-> increaseScore(100);
    setdead();
}

void piranha::damage(){
    getWorld()-> increaseScore(100);
    setdead();
}
void projectiles::doprojectilebehavior(string z){ //both projectiles share similar functionality, aside from minor adjustments. Depending on if you give "ally" projectile or "enemy" projectile, the funciton will do the respective functionality.
    if(z == "ally")
    {
        Actors* x;
        if(getWorld() -> chech_damageable_object_other_than_peach(x, this, getX(), getY())) //if overlapping woth a damageable object other than yourself and peach, alert that object's damage method, set dead, then return.
        {
                x-> damage();
                setdead();
                return;
        }
        else //if not overlapping with damageable object
        {
            if(getWorld() -> check_blocking_object(x, this, getX(), getY()-2) == false){ //check to see if you can move 2 pixels down
                if(x != getWorld() -> getPeach())
                    moveTo(getX(), getY()-2);
            }
                int d = getDirection(); //then get direction and see if you can move 2 pixels in that direction.
                if(d == 0)
                {
                    if(getWorld()-> check_blocking_object(x, this, getX()+2, getY()))
                    {
                        setdead();
                    }
                    else
                        moveTo(getX()+2, getY());
                }
                if(d == 180)
                {
                    if(getWorld()-> check_blocking_object(x, this, getX()-2, getY()))
                    {
                        setdead();
                    }
                    else
                        moveTo(getX()-2, getY());
                }
        }
    }
    if(z == "enemy")
    {
        Actors* x;
        if(getWorld() -> check_overlap_peach(x, this, getX(), getY())) //same as above except for this condition right here: if overlapping with peach, then damage her, set dead, and return.
        {
            x-> damage();
            setdead();
            return;
        }
        else
        {
            if(getWorld() -> check_blocking_object(x, this, getX(), getY()-2) == false)
                moveTo(getX(), getY()-2);
            
                int d = getDirection();
                if(d == 0)
                {
                    if(getWorld()-> check_blocking_object(x, this, getX()+2, getY()))
                    {
                        setdead();
                    }
                    else
                        moveTo(getX()+2, getY());
                    }
                if(d == 180)
                {
                    if(getWorld()-> check_blocking_object(x, this, getX()-2, getY()))
                    {
                        setdead();
                    }
                    else
                        moveTo(getX()-2, getY());
                }
            }
        }
        
}


void Piranha_fired_fireballs::dosomething()
{
    doprojectilebehavior("enemy");
    
}
//using above function and passing paramater to indicate which type of projectile
void Shell::dosomething()
{
    doprojectilebehavior("ally");
}
void Peach_fired_fireballs::dosomething()
{
    doprojectilebehavior("ally");

}


void objectives::docommonality3(string x){ //function that does one commonality of mario and flag. This function does the part where the flag or mario needs to notify the student world that the level has ended depending on which peach overlapped with.
    Actors* s;
    if(this -> isalive() == false)
        ; //if dead, do nothing
    else
    { //if alive and overlapping with peach, set score, set dead, then based on if it's mario or flag, tell the studentworld
        if(getWorld()->check_overlap_peach(s, this, getX(), getY()))
        {
        getWorld() -> increaseScore(1000);
        setdead();
        if(x == "final")
            getWorld() -> setgameFinishedStatus();
        if(x == "next level")
            getWorld() -> setlevelFinishedStatus();
        }
    }
}


void Flag::dosomething(){
    docommonality3("next level");
    
}
void Mario::dosomething(){
    docommonality3("final");
}





void Actors::doCommonality(){ //this is a commonality many functions have especially the goodie class. All the goodies check to see if they can move down 2 pixels first. Then they check if direction needs to be switched depending on the direction looking to move in.
    Actors* x;
    if(getWorld()-> check_blocking_object(x, this, getX(), getY()-2) == false){
        moveTo(getX(), getY()-2);
    }
    
    if(switchdirectionifneeded(getX()+2, getY())){
        ;
    }
    else if(switchdirectionifneeded(getX()-2, getY())){
        ;
    }
   
    /*if(getWorld()-> check_blocking_object(x, this, getX()+2, getY()))
        {
            int x;
            x = getDirection();
            if(x == 0)
                setDirection(180);
            if(x == 180)
                setDirection(0);
        }
        
        else if(getWorld() -> check_blocking_object(x, this, getX()-2, getY()))
        {
            
            int x;
            x = getDirection();
            if(x == 0)
                setDirection(180);
            if(x == 180)
                setDirection(0);
        }
    */
        else //if they can make the move, move there
        {
            if(getDirection()== 0)
                moveTo(getX()+2, getY());
            else
                moveTo(getX()-2, getY());
        }
    

}
bool goodies::doCommonality2(int sethealth, int setscore, string z, int setTicks){ //this is a commonality between the goodies, and it implements the what happens if goodie is overlapping with peach. The first commonality is the second case, where peach and the goodie are not overlapping.
    Actors* x;
    peach* y;
    if(getWorld() -> check_overlap_peach(x, this, getX(), getY())){
        getWorld() -> increaseScore(setscore);
        y = getWorld() -> getPeach(); //get access to peach.
        y -> setpowerstatus(z, setTicks); //give the respective power
        if(sethealth > 0)
        {
        y -> sethealth(sethealth); //set health
        }
        setdead(); //set dead
        getWorld() -> playSound(SOUND_PLAYER_POWERUP); //play sound and return true
        return true;
    }
    return false;
}
//these 3 goodies combine the functions created representing there commonalities, with different parameters being passed to describe the differnt modification the goodies make to Peach.
void Mushroom::dosomething(){
    if(doCommonality2(2, 75, "jumppower", 0))
        ;
    else
       doCommonality();
    
}

void Flower::dosomething(){
    if(doCommonality2(2, 50, "shootpower", 0))
        ;
    else
        doCommonality();
}

void Star::dosomething(){
    if(doCommonality2(0, 100, "none", 150))
        ;
    else
        doCommonality();
}

void block::bonked(){
    if(Stargoodiestatus() == false and Mushgoodiestatus() == false and Flowergoodiestatus() == false){ //if block holds no goodies, just play bonk sound
        getWorld() -> playSound(SOUND_PLAYER_BONK);
    }
    if((Stargoodiestatus() or Mushgoodiestatus() or Flowergoodiestatus()) and Bonked == false){ //if it hasn't been bonked before, play sound, and introduce respective goodie. Release the goodie from the block to change status.
        getWorld() -> playSound(SOUND_POWERUP_APPEARS);
        if(Flowergoodiestatus())
        {
            getWorld() -> addtoVector(new Flower(getWorld(), getX(), getY()+8));
            Flowergoodie = false;
            Bonked = true;
        }
        else if(Mushgoodiestatus())
        {
            getWorld() -> addtoVector(new Mushroom(getWorld(), getX(), getY()+8));
            Mushgoodie = false;
            Bonked = true;
            
        }
        else if(Stargoodiestatus()){
            getWorld() -> addtoVector(new Star(getWorld(), getX(), getY()+8));
            Stargoodie = false;
            Bonked = true;
            
        }
            
    }
       
    

}

void peach::bonked()
{
 if(starpowerstatus() or temporaryinvincibilitystatus())
     ;
else //if doesn';t have invincibility, subtrat health, remove all powers if present, play sound if health is at 1, setdead if health >=0. decrement health.
{
    health--;
    invincibility_ticks = 10;
    if(shootpowerstatus())
        shootpower = false;
    if(jumppowerstatus())
        jumppower = false;
    if(health>=1)
        getWorld()-> playSound(SOUND_PLAYER_HURT);
    if(health <= 0)
    {
        setdead();
    }
        
}

}


void peach:: dosomething(){
    int ch;
    Actors* x;
    bool overlap;
    if(isalive() == false) //if isn't alive, return immediately
        return;
    if(starpowerstatus()) //Peach must check if she is currently temporarily invincible, and if so, decrement the number of remaining game ticks before she loses temporary invincibility. If this tick count reaches zero, Peach must set her temporary invincibility status to false. (Peach gains temporary invincibility if she overlaps with an enemy while she has Jump Power or Fire Power.)
        starpower_ticks--;
    if(temporaryinvincibilitystatus()) //Peach must check if she is currently in “recharge” mode before she can fire again. If the number of time_to_recharge_before_next_fire ticks is greater than zero, she must decrement this tick count by one. If the tick count reaches zero, then Peach may again shoot a fireball (if she has Shoot Power).
        invincibility_ticks--;
    if(needsRecharge())
        time_to_recharge_before_next_fire--;
    if(shootpowerstatus() and needsRecharge() == false){
        CanShootFireball = true;
    }
    if(getWorld() -> check_overlap(x, this, getX(), getY())) //bonk object of overlaps and if they're bonkable.
    {
        if(x-> Bonkable())
            x-> bonked();
    }
    if(jumpinitiated and remaining_jump_distance > 0){  //if she is jumping
        if(getWorld() -> check_blocking_object(x, this, getX(), getY()+4))
        {
            x-> bonked();
            remaining_jump_distance = 0;
            
        }
        
        else
        {
            moveTo(getX(), getY()+4);
            remaining_jump_distance--;
        
        }
    }
    if(remaining_jump_distance == 0){ //if she is falling
        bool blocking_object = false;
        for(int i = 0 ; i < 4; i++){
            if(getWorld() -> check_blocking_object(x, this, getX(), getY()-i))
                blocking_object = true;
            }
        
        if(blocking_object == false){
            moveTo(getX(), getY()-4);
        }
            
    }
    
    if(getWorld()->getKey(ch)) //getting users inputs and doing something for each input respectively
    {
    // user hit a key during this tick! switch (ch)
        switch(ch)
        {
        case KEY_PRESS_LEFT: //set direction, try to move in given direction
            setDirection(180);
                overlap = getWorld() -> check_overlap(x, this, getX()-4, getY());
                if(overlap == false){
                    moveTo(getX()-4, getY());
                }
                else{
                    if(x -> CanBlock() == false)
                        moveTo(getX()-4, getY());
                    if(x-> Bonkable())
                        x-> bonked();
                }
            break;
       case KEY_PRESS_RIGHT:
                setDirection(0);
                if(getWorld() -> check_overlap(x, this, getX()+4, getY()) == false){
                    moveTo(getX()+4, getY());
                }
                else{
                    if(x -> CanBlock() == false)
                        moveTo(getX()+4, getY());
                    if(x-> Bonkable())
                        x-> bonked();
                }
            break;
        case KEY_PRESS_UP: //set jump distances
                if(getWorld() -> check_overlap(x, this, getX(), getY()-1) and x -> CanBlock())
                {
                    if(jumppowerstatus() == false)
                        remaining_jump_distance = 8;
                    else
                        remaining_jump_distance = 12;
                    
                    getWorld() -> playSound(SOUND_PLAYER_JUMP);
                    jumpinitiated = true;
                }
                break;
        case KEY_PRESS_SPACE: //create fireball
                if(shootpowerstatus() == false)
                    ;
                else if(needsRecharge())
                    ;
                else
                {
                    getWorld() -> playSound(SOUND_PLAYER_FIRE);
                    time_to_recharge_before_next_fire = 8;
                    int d = getDirection();
                    if(d == 0)
                        getWorld() -> addtoVector(new Peach_fired_fireballs(getWorld(), getX()+4, getY(), d));
                    if(d == 180)
                        getWorld() -> addtoVector(new Peach_fired_fireballs(getWorld(), getX()-4, getY(), d));
                    
                }
                break;
        
        }
    }

}
