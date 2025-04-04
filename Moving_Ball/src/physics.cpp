#include "physics.h"
#include <raylib.h>
#include <math.h>
#include <iostream>

PhySystem::PhySystem(Vector2 s){
    numOfBalls = 0;
    size.x = s.x;
    size.y = s.y;
}

PhySystem::~PhySystem(){
    int i = numOfBalls-1;
    while( i >= 0){
        Ball* b = (Ball*)malloc(sizeof(Ball));
        b = fndBall(i);
        if(b == NULL){
            std::cout << "No Ball" << std::endl;
            break;
        }
        delBall(b);
        --i;
    }
}

int PhySystem::numBall(){ return numOfBalls; }

Ball* PhySystem::fndBall(int i){
    Ball* ret;
    if( i >= numOfBalls){
        std::cout << "wrong index" << std::endl;
        return NULL;
    }
    ret = listBall[i];
    return ret;
}

void PhySystem::addBall(Vector2 p, int a){
    if(numOfBalls >= maxBalls){
        std::cout << "Too many balls already" << std::endl;
        return;
    }
    if(a != 1 && a != -1) {
        std::cout << "Wrong attraction setting" <<std::endl;
    } 

    Ball* b = new Ball(p, (Vector2){0.0f, 0.0f}, (Vector2){0.0f, 0.0f}, 15.0f);
    if(numOfBalls == 0){
        listBall = (Ball**) malloc((numOfBalls+1)*sizeof(Ball*));
        isAttr = (int*) malloc((numOfBalls+1)*sizeof(int));
    } else if( numOfBalls > 0 && numOfBalls < maxBalls){
        listBall = (Ball**) realloc((void*)listBall, (numOfBalls+1)*sizeof(Ball*));
        isAttr = (int*) malloc((numOfBalls+1)*sizeof(int));
    }

        
    if(listBall == NULL){
        std::cout << "Re-allocation failed" << std::endl;
        return;
    }
    
    ++numOfBalls;
    listBall[numOfBalls-1] = b;
    isAttr[numOfBalls-1] = a;
}

void PhySystem::cpyBall(Ball* src, Ball* dst){
    if(src == NULL){
        std::cout << "Null source pointer" << std::endl;
        return;
    }
    if(dst == NULL){
        std::cout << "Null destination pointer" << std::endl;
        return;
    }
    dst->modPos(src->getPos());
    dst->modSpd(src->getSpd());
    dst->modAcc(src->getAcc());
    dst->modRad(src->getRad());
}

void PhySystem::delBall(Ball* b){
    if(b == NULL){
        std::cout << "Null pointer, no deletion in progress.\nReturning" << std::endl;
        return; 
    }
    delete b;
}

void PhySystem::updSyst(){
    //TODO: OPTIMIZE THIS SHIT
    for(int i = 0; i < numOfBalls; i++){
        Vector2 vecAcc = (Vector2){0.0f, 0.0f};
        Vector2 vecDis = (Vector2){0.0f, 0.0f};
        for(int j = 0; j < numOfBalls; j++){
            if(i == j) continue;
            //get distance
            vecDis.x = listBall[i]->getPos().x - listBall[j]->getPos().x;
            vecDis.y = listBall[i]->getPos().y - listBall[j]->getPos().y;
            //calculate the distance vector by its magnitude and its angle
            float ang = atan2f(vecDis.y, vecDis.x);
            float mag = sqrtf(pow(vecDis.x,2) + pow(vecDis.y,2));
            //if the distance vector is smaller than the sum of the balls radii
            //run the collision function
            if(mag <= listBall[i]->getRad() + listBall[j]->getRad()){ collision(listBall[i], listBall[j], mag, ang); }
            //otherwise calculate the usual acceleration 
            //pretty incorrect physics wise
            else {
                float forceMag = isAttr[i]*attraction*(listBall[j]->getRad())/powf(mag, 2);
                if(forceMag >= maxForce) forceMag = maxForce;
                vecAcc.x += forceMag*cosf(ang);
                vecAcc.y += forceMag*sinf(ang); 
            }
            //collision(listBall[i], listBall[j], mag, ang);
        }
        listBall[i]->modAcc(vecAcc);
        listBall[i]->Update(0b0000, size.x, size.y);
        
    }
}

void PhySystem::collision(Ball* a, Ball* b, float dist, float ang){
    //initialize a temporary vector that will hold the new speed
    //trivially I made it the opposite of the current speed
    Vector2 mod = (Vector2) {0.0f, 0.0f};
    //modify their position
    //--//dist = sqrt{[A.x-B.x^2]+[A.y-B.y]^2}
    //--//they should pushed outward by distance 
    //--//minus the other's radius
    float distA = dist - b->getRad();
    float distB = dist - a->getRad();
    a->modPos((Vector2){-distA*cosf(ang), -dist*sinf(ang)});
    mod.x = -a->getSpd().x;
    mod.y = -a->getSpd().y;
    a->setSpd(mod);
    b->modPos((Vector2){-distB*cosf(M_PI_2 - ang), -dist*sinf(M_PI_2 - ang)});
    mod.x = -a->getSpd().x;
    mod.y = -b->getSpd().y;
    b->setSpd(mod);
    a->Update(0b0000, size.x, size.y);
    b->Update(0b0000, size.x, size.y);

}