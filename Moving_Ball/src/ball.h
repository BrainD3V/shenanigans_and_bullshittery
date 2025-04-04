#pragma once
#include <raylib.h>
#include <math.h>

class Ball{
    private:
        Vector2 pos, spd, acc;
        const float max_acc = 0.25f;
        const float max_spd = 12.5f;
        const float friction = 0.975f;
        const float inertia = 0.9f;
        float radius;
    public:
        Ball();
        Ball(Vector2 p, Vector2 s, Vector2 a, float r);
        ~Ball();
        void isOutOfScreen(Vector2 s);
        Vector2 getPos();
        Vector2 getSpd();
        Vector2 getAcc();
        float   getRad();
        void    modPos(Vector2);
        void    modSpd(Vector2);
        void    modAcc(Vector2);
        void    modRad(float);
        void    setPos(Vector2);
        void    setSpd(Vector2);
        void    setAcc(Vector2);
        void    setRad(float);
        void Update(int cmd, float sW, float sH);
        void Draw(Color c);
};