#pragma once
#define _USE_MATH_DEFINES
#include "ball.h"
#include <raylib.h>
#include <math.h>
#include <iostream>


class PhySystem{
    private:
        const int maxBalls = 10;
        int numOfBalls;
        int* isAttr;
        Ball** listBall; //array of pointers to Balls
        Vector2 size;
        //---- Physics constants
        const float attraction = 6.6743e-3f;
        const float maxForce   = 2.5f;
        const float minDist    = 15.0f;
    public:
        PhySystem(Vector2);
        ~PhySystem();
        int  numBall();
        Ball* fndBall(int);
        void addBall(Vector2, int);
        void cpyBall(Ball*, Ball*);
        void delBall(Ball*);

        void collision(Ball*, Ball*, float, float);
        void updSyst();
};