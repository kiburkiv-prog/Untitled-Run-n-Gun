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
	float draw_x;
	float draw_y;
	float angle = 0;
    char facing = 1;
    Animater animater;
    void render();
	void render(int, int);

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
	bool const_y = false;
	float global_x = 0;
	float global_y = 0;
	float lastgx = 0;
	float lastgy = 0;
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
    void render(int, int);
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
	void update(Player&, int, int);
};

class Cam{
public:
	float offset_x = 0;
	float offset_y = 0;
	int x1 = 200;
	int y1 = 200;
	int y2 = 560;
	int x2 = 640;
	bool is_x = false;
	bool is_y = false;
	Collision** collisions = new Collision* [4];
	Cam();
	void draw();
	void move(Player&, int&, int&);
};
