#define _USE_MATH_DEFINES
#include "ball.h"
#include <raylib.h>
#include <math.h>
#include <iostream>

Ball::Ball() 
    : pos((Vector2){0.0f, 0.0f})
    , spd((Vector2){0.0f, 0.0f})
    , acc((Vector2){0.0f, 0.0f})
    , radius(15){

}
Ball::Ball(Vector2 p, Vector2 s, Vector2 a, float r){
    pos = p;
    spd = s;
    acc = a;
    radius = r;
}

Ball::~Ball(){}

void Ball::isOutOfScreen(Vector2 s){
    if(pos.x >= s.x-radius || pos.x <= radius){ 
        spd.x *= -1.0f;
    }
    if(pos.y >= s.y-radius || pos.y <= radius){ 
        spd.y *= -1.0f;
    }
}

void Ball::modPos(Vector2 p){ 
    pos.x += p.x; 
    pos.y += p.y;
}

void Ball::modSpd(Vector2 s){ 
    spd.x += s.x; 
    spd.y += s.y;
}
void Ball::modAcc(Vector2 a){ 
    acc.x += a.x; 
    acc.y += a.y;
}
void Ball::modRad(float r){ radius += r; }

void Ball::Update(int cmd, float sW, float sH){
    Vector2 mov = (Vector2){0.0f, 0.0f};
    switch (cmd){
    case 0b0001:
        mov.x += max_acc;
        break;
    case 0b0010:
        mov.x -= max_acc;
        break;
    case 0b0100:
        mov.y += max_acc;
        break;
    case 0b1000:
        mov.y -= max_acc;
        break;
    case 0b0101:
        mov.x += max_acc;
        mov.y += max_acc;
        break;
    case 0b0110:
        mov.x -= max_acc;
        mov.y += max_acc;
        break;
    case 0b1001:
        mov.x += max_acc;
        mov.y -= max_acc;
        break;
    case 0b1010:
        mov.x -= max_acc;
        mov.y -= max_acc;
        break;
    default:
        acc.x *= inertia;
        acc.y *= inertia;
        spd.x *= friction;
        spd.y *= friction;

        break;
    }
    
    acc.x += mov.x;
    acc.y += mov.y;

    float mag = sqrt(pow(acc.x, 2)+pow(acc.y, 2));
    float ang = atan2f(acc.y, acc.x);
    if(mag >= max_acc){
        acc = (Vector2){max_acc*cosf(ang), max_acc*sinf(ang)};
    }

    spd.x += acc.x;
    spd.y += acc.y;
    
    mag = sqrt(pow(spd.x, 2)+pow(spd.y, 2));
    ang = atan2f(spd.y, spd.x);
    if(mag >= max_spd){
        spd = (Vector2){max_spd*cosf(ang), max_spd*sinf(ang)};
    }

    isOutOfScreen((Vector2){sW, sH});
    pos.x += spd.x;
    pos.y += spd.y;
}

void Ball::Draw(Color c){
    DrawCircleV(pos, radius*1.075f, ColorFromHSV(0.0f,0.0f,0.0f));
    DrawCircleV(pos, radius, c);
    //DrawText(TextFormat("spd: %03.2f, %03.2f", spd.x, spd.y), pos.x, pos.y - 30, 10, WHITE);
    
}

Vector2 Ball::getPos(){ return pos; }
Vector2 Ball::getSpd(){ return spd; }
Vector2 Ball::getAcc(){ return acc; }
float Ball::getRad(){ return radius; }

void    Ball::setPos(Vector2 p){ pos = p; }
void    Ball::setSpd(Vector2 s){ spd = s; }
void    Ball::setAcc(Vector2 a){ acc = a; }
void    Ball::setRad(float r){ radius = r; }
