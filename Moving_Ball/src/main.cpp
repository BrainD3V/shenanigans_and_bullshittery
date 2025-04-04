#include <raylib.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "ball.h"
#include "physics.h"

const int MAX_SIZE = 100;
int main() 
{
    const Color darkGreen = {20, 160, 133, 255};
    
    const int screenWidth = 800;
    const int screenHeight = 600;

    Vector2 sSize = (Vector2){screenWidth, screenHeight};
    PhySystem ps(sSize);

    InitWindow(screenWidth, screenHeight, "My first RAYLIB program!");
    SetTargetFPS(60);
    
    //int cmd = 0;
    int relr = 1;
    int rell = 1;
    int ballSize = 1;
    int pause = 0;
    while(!WindowShouldClose()){
        ballSize += GetMouseWheelMove();
        ballSize = (ballSize < 1)? MAX_SIZE : ((ballSize > MAX_SIZE)? 1 : ballSize);
        DrawText(TextFormat("Size: %i", ballSize), screenWidth - 100, screenHeight - 50, 20, WHITE);
        DrawText(TextFormat("N.Balls: %i", ps.numBall()), screenWidth - 100, screenHeight - 25, 20, WHITE);
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && rell == 1){
            ps.addBall(GetMousePosition(), -1);
            ps.fndBall(ps.numBall()-1)->setRad(ballSize);
            ballSize = 1;
            rell = 0;
        } 
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && rell == 0){
            rell = 1;
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && relr == 1){
            ps.addBall(GetMousePosition(), 1);
            ps.fndBall(ps.numBall()-1)->setRad(ballSize);
            ballSize = 1;
            relr = 0;
        } 
        if(IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && relr == 0){
            relr = 1;
        }
        if(IsKeyPressed(KEY_SPACE)) pause = (pause)? 0: 1;
        BeginDrawing();
            ClearBackground(darkGreen);
            if(pause == 0){
                for(int i = 0; i < ps.numBall(); i++){
                    ps.fndBall(i)->Draw(ColorFromHSV(((float)(i+1)/(float)ps.numBall())*360.0f, 1.0f, .75f) );
                    if(ps.numBall() <= 1){ break; } 
                    ps.updSyst();
                    ps.fndBall(i)->Update(0b0000, sSize.x, sSize.y);
                }
            } else {
                for(int i = 0; i < ps.numBall(); i++){
                    ps.fndBall(i)->Draw(ColorFromHSV(((float)(i+1)/(float)ps.numBall())*360.0f, 1.0f, .75f) );
     
                    DrawLineV(       ps.fndBall(i)->getPos(),
                         (Vector2)  {ps.fndBall(i)->getPos().x+100.0f*ps.fndBall(i)->getAcc().x, 
                                     ps.fndBall(i)->getPos().y+100.0f*ps.fndBall(i)->getAcc().y},
                                     WHITE);
                }
            }
        EndDrawing();
        //cmd = 0;
    }
    
    CloseWindow();
}