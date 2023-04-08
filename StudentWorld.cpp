#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include "Actor.h"
#include <cmath>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    thepeach = nullptr;
    is_level_finished = false;
    is_game_finished = false;
}

StudentWorld::~StudentWorld(){
   cleanUp();
    
}

bool StudentWorld:: check_overlap(Actors* &object, Actors* a, double X1, double Y1){ //checks overlap between objects
    std::vector<Actors*>:: iterator it; //for all of the overlap functions, this code iterates through the vector of objects and sees if the absolute value of the distance between them is less than sprite height and width. if it is they overlap.
    for(it = actors.begin(); it != actors.end(); it++){
        if(abs((*it)->getX() -X1) < SPRITE_WIDTH and abs( (*it)->getY() -Y1) < SPRITE_HEIGHT){
            if(*it != a)
            {
                object = (*it);
                return true;
            }
        }
        
    }
        
    return false;
}

bool StudentWorld:: check_blocking_object(Actors* &object, Actors *a, double X1, double Y1){ //checks to see if overlapping object can block you
    std::vector<Actors*>:: iterator it;
    for(it = actors.begin(); it != actors.end(); it++){
        if(abs((*it)->getX() -X1) < SPRITE_WIDTH and abs( (*it)->getY() -Y1) < SPRITE_HEIGHT){
            if(*it != a)
            {
                object = (*it);
                if((*it) -> CanBlock())
                    return true;
            }
        }
        
    }
            
    
    return false;
    
}
bool StudentWorld:: check_overlap_peach(Actors *&object, Actors *a, double X1, double Y1){ //checking to see if you overlap with peach
    std::vector<Actors*>:: iterator it;
    for(it = actors.begin(); it != actors.end(); it++){
        if(abs((*it)->getX() -X1) < SPRITE_WIDTH and abs( (*it)->getY() -Y1) < SPRITE_HEIGHT){
            if(*it != a)
            {
                object = (*it);
                if((*it) -> usesHealth())
                    return true;
            }
        }
        
    }
    return false;
}

  

bool StudentWorld:: check_overlap_fireball(Actors *&object, Actors *a, double X1, double Y1){ //checking to see if you overlap with a projectile
    std::vector<Actors*>:: iterator it;
    for(it = actors.begin(); it != actors.end(); it++){
        if(abs((*it)->getX() -X1) < SPRITE_WIDTH and abs( (*it)->getY() -Y1) < SPRITE_HEIGHT){
            if(*it != a)
            {
                object = (*it);
                if((*it) -> isprojectile())
                    return true;
            }
        }
        
    }
    return false;
    
}
bool StudentWorld::chech_damageable_object(Actors* &object, Actors* a, double X1, double Y1){ //checking to see if overlapping with a damageable object
    std::vector<Actors*>:: iterator it;
    for(it = actors.begin(); it != actors.end(); it++){
        if(abs((*it)->getX() -X1) < SPRITE_WIDTH and abs( (*it)->getY() -Y1) < SPRITE_HEIGHT){
            if(*it != a)
            {
                object = (*it);
                if((*it) -> Damageable())
                    return true;
            }
        }
        
    }
    return false;
    
}
bool StudentWorld::chech_damageable_object_other_than_peach(Actors* &object, Actors* a, double X1, double Y1){ //checking to see if overlapping with a damageable object
    std::vector<Actors*>:: iterator it;
    for(it = actors.begin(); it != actors.end(); it++){
        if(abs((*it)->getX() -X1) < SPRITE_WIDTH and abs( (*it)->getY() -Y1) < SPRITE_HEIGHT){
            if(*it != a and *it != thepeach)
            {
                object = (*it);
                if((*it) -> Damageable())
                    return true;
            }
        }
        
    }
    return false;
    
}


int StudentWorld::init()
{
    int x = randInt(0, 180); //getting random directions for the Koops and goomba
    if(x < 90)
        x = 0;
    if(x >= 90)
        x =180;
    int y = randInt(0, 180);
    if(y < 90)
        y = 0;
    if(y >= 90)
        y =180;
    int z = randInt(0, 180);
    if(z < 90)
        z = 0;
    if(z >= 90)
        z =180;
    is_level_finished = false; //level hasn't been finished yet
    int level = getLevel(); //loading level
    if(level == 100)
        return GWSTATUS_PLAYER_WON;
    
    Level lev(assetPath());
    ostringstream TheLevel;
    TheLevel << "level";
    TheLevel.fill('0');
    TheLevel << setw(2) << level;
    TheLevel << ".txt";
   string level_file = TheLevel.str();
    Level::LoadResult result = lev.loadLevel(level_file); //if levels don't load return appropriate GW status
    if (result == Level::load_fail_file_not_found)
    {
        cerr << "Could not find " << level_file << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_fail_bad_format)
    {
        cerr << level_file << " is improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success)
    {
    cerr << "Successfully loaded level" << endl;
    Level::GridEntry ge;
    for(int i = 0; i < 32; i++)
        {
            for(int j = 0; j < 32; j++) //filling in the grid
            {
                ge = lev.getContentsOf(i,j); // x=5, y=10
                switch (ge)
                {//for each type of object, push it into the vector containing all the objects.
                    case Level::empty:
                     break;
                    case Level::koopa:
                    actors.push_back(new koopa(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, x));
                     break;
                    case Level::goomba:
                        actors.push_back(new goomba(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, y));
                     break;
                    case Level::peach: //star, mush, flower, empty
                        thepeach = new peach(this, IID_PEACH, i*SPRITE_WIDTH, j*SPRITE_HEIGHT);
                        actors.push_back(thepeach);
                        break;
                    case Level::flag:
                        actors.push_back(new Flag(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT));
                        break;
                    case Level::block:
                        actors.push_back(new block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, false, false, false, true));
                        break;
                    case Level::star_goodie_block:
                        actors.push_back(new block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, true, false, false, false));
                        break;
                    case Level::piranha:
                        actors.push_back(new piranha(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, z));
                        break;
                    case Level::mushroom_goodie_block:
                        actors.push_back(new block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, false, true, false, false));
                        break;
                    case Level::flower_goodie_block:
                        actors.push_back(new block(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, false, false, true, false));
                        break;
                    case Level::pipe:
                        actors.push_back(new pipe(this, i*SPRITE_WIDTH, j*SPRITE_HEIGHT));
                        break;
                    case Level::mario:
                        actors.push_back(new Mario(this,i*SPRITE_WIDTH, j*SPRITE_HEIGHT));
                        break;
                }
                
            }
    }
}
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::displayyGameText(){
    int points = getScore();
    int lives = getLives();
    int level = getLevel();
    //creating string to display standard game text
    ostringstream TextFormat;
    TextFormat << "Lives: ";
    TextFormat.fill(' ');
    TextFormat << setw(1) << lives << "  ";
    
    TextFormat << "Level: ";
    TextFormat.fill('0');
    TextFormat << setw(2) << level << "  ";
    
    TextFormat << "Points: ";
    TextFormat.fill('0');
    TextFormat <<setw(6) << points << " ";
//game text for when peach obtains powers
    if(thepeach -> starpowerstatus())
        TextFormat << "StarPower!" << " ";
    if(thepeach -> shootpowerstatus())
        TextFormat <<"ShootPower!" << " ";
    if(thepeach -> jumppowerstatus())
        TextFormat << "JumpPower!" << " ";
    string Display_Text = TextFormat.str();
    setGameStatText(Display_Text);
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    std::vector<Actors*>:: iterator it; //give everyone a chance to do something, and return appropriate GW status
    for(it = actors.begin(); it != actors.end(); it++)
    {
        if((*it) -> isalive())
        {
        (*it) -> dosomething();
            if(thepeach -> isalive() == false){ //if peach dies, decrement the lvies remaining and return proper GW status
                playSound(SOUND_PLAYER_DIE);
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if(is_game_finished == true) //return GW status for game ending
            {
                playSound(SOUND_GAME_OVER);
                return GWSTATUS_PLAYER_WON;
            }
            if(is_level_finished == true) //returnoing GW status for level ending
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    for(it = actors.begin(); it != actors.end();) //deleting all objects that have died
    {
        if((*it)-> isalive() == false)
        {
            delete (*it);
            it = actors.erase(it);
        }
        else
            it++;
    }
    
        
    displayyGameText(); //display game text
    
    return GWSTATUS_CONTINUE_GAME; //if peach hasn't died yet keep the game going.
}

void StudentWorld::cleanUp()
{
    
    thepeach = nullptr; //set peach pointer to null
    std::vector<Actors*>:: iterator it; //delete all of the items in the vector
    for(it = actors.begin(); it != actors.end(); it++){
        delete *it;
    }
    actors.clear();
}

