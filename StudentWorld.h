#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actors;
class peach;
class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
    ~StudentWorld();
    bool check_overlap(Actors* &object, Actors* a, double X1, double Y1); //checks overlap between items and returns overlap status.
    bool check_blocking_object(Actors* &object, Actors* a, double X1, double Y1);
    bool check_overlap_peach(Actors* &object, Actors* a, double X1, double Y1);
    bool chech_damageable_object(Actors* &object, Actors* a, double X1, double Y1);
    bool chech_damageable_object_other_than_peach(Actors* &object, Actors* a, double X1, double Y1);
    bool check_overlap_fireball(Actors* &object, Actors* a, double X1, double Y1);
    void addtoVector(Actors* x){actors.push_back(x);}
    peach* getPeach(){return thepeach;}
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void displayyGameText();
    void setlevelFinishedStatus(){is_level_finished = true;}
    void setgameFinishedStatus(){is_game_finished = true;}

private:
    peach* thepeach;
    std::vector<Actors*> actors;
    bool is_level_finished;
    bool is_game_finished;
    
    
};

#endif // STUDENTWORLD_H_
