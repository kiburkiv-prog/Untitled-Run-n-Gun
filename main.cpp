#include <cstdio>
#include <raylib.h>
#include <iostream>
#include "objects.h"







int main(){
	InitAudioDevice();
	SetMasterVolume(100);
    InitWindow(1280,720,"Igra dlya bratiev");
	SDL_Init(SDL_INIT_GAMEPAD);
    SetTraceLogLevel(LOG_NONE);
    SetTargetFPS(120);
    Player legs;
	GameObject lavka1;
	lavka1.animater.animations = new Texture2D* [1];
	lavka1.animater.animations[0] = lavka1.animater.load_anim("assets/animations/sfx/", "lavka/", 1);
	GameObject lavka2;
	lavka2.animater.animations = new Texture2D* [1];
	lavka2.animater.animations[0] = lavka2.animater.load_anim("assets/animations/sfx/", "lavka/", 1);
	GameObject lavka3;
	lavka3.animater.animations = new Texture2D* [1];
	lavka3.animater.animations[0] = lavka3.animater.load_anim("assets/animations/sfx/", "lavka/", 1);
    legs.x = 0;
    legs.y = 0;
    legs.load_textures();
	float xs = 0;
	float ys = 400;
	float xs1 = 400;
	float ys1 = 600;
	float xs2 = 700;
	float ys2 = 480;
	lavka1.x = xs;
	lavka1.y = ys - 20;
	lavka2.x = xs1;
	lavka2.y = ys1 - 20;
	lavka3.x = xs2;
	lavka3.y = ys2 - 20;
	Surface surf(&xs, &ys, 400,1, "str", 1);
	Surface surf2(&xs1, &ys1, 400,1, "str", 2);
	Surface surf3(&xs2, &ys2, 400,1, "str", 3);
	int x = 400;
	int y = 280;
	Collision col(&x, &y, 300,50);
	cout << GetGamepadName(0) << "\n";

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(GREEN);
		SetGamepadVibration(0, 0.5, 0.5, 1.0);
        DrawFPS(100,100);
		cout << GetGamepadButtonPressed() <<"\n";
		surf.update(legs);
		surf2.update(legs);
		surf3.update(legs);
		lavka1.render();
		lavka2.render();
		lavka3.render();
        legs.exist();
		//col.draw();
		Rectangle collis = legs.collision->collide(surf.collision);
		/*if(collis.height != 0){
			DrawRectangleRec(collis, BLACK);
		}*/
        EndDrawing();

    }
    CloseWindow();

    return 0;
}
