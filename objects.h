#include "animating.h"
#include <SDL3/SDL.h>


class Collision{
public:
	int* x;
	int* y;
	float w;
	float h;
	Collision(int*, int*, float, float);
	void update();
	Rectangle collide(Collision*);
	void draw();
};





class GameObject{
public:
    float x;
    float y;
	float angle = 0;
    char facing = 1;
    Animater animater;
    void render();

};





class Bullet : public GameObject{
public:
	string type;
	void move();
	Bullet(float, float, string, signed char);
	void exist();
};

class Player : public GameObject{
public:
	Player();
    Animater animater_legs;
	Collision* collision;
	double speed = 2;
	int xint;
	int yint;
	int lastx;
	int lasty;
	int standing_index = 0;
	bool null_speed = false;
	Bullet** bullets = new Bullet* [400];
	int bullet_len = 0;
	bool crouching = false;
	bool set_new_speed = false;
	double set_speed = 0;
	Sound pshoot = LoadSound("assets/sounds/pshoot.mp3");
	bool to_jump = false;
	bool to_shoot = false;
	bool to_offset = false;
	int offset_x = 0;
	int offset_y = 0;
	int offset_y_h = 0;
	int head_r_off = 0;
	string state_head = "idle";
	string state_legs = "idle";
	float up_speed = 0;
	SDL_Gamepad* controller;
	float down_speed = 0;
	bool contact_surf = false;
    void load_textures();
	void jump();
	void shoot();
    void render();
	void null_all();
    void move();
	void set_offsets();
    void exist();
};



class Surface{
public:
	float* x;
	float* y;
	int w;
	int h;
	int intx;
	int inty;
	int index;
	string type;
	Collision* collision;
	Surface(float*, float*, int, int, string, int);
	void update(Player&);
};
