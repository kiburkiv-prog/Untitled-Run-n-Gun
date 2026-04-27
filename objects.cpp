#include "objects.h"
#include <raylib.h>
#include <iostream>






Collision::Collision(int* xn, int* yn, float wn, float hn){
	this->x = xn;
	this->y = yn;
	this->w = wn;
	this->h = hn;
}

Rectangle Collision::collide(Collision* col){
	//x crossings
	Collision* pivot = col->w >= this->w ? col : this;
	Collision* pivot_small = col->w < this->w ? col : this;
	float x1 = -1;
	float x2 = -1;
	bool is_x_set = false;
	
	for(int i = *pivot->x; i <= *pivot_small->x + pivot_small->w - 1 && i <= *pivot->x + pivot->w - 1; i++){
		if(i >= *pivot_small->x && i <= *pivot_small->x + pivot_small->w - 1 && !is_x_set){
			x1 = i;
			is_x_set = true;
		}
		if(i == *pivot_small->x + pivot_small->w - 1){
			x2 = i;
		}
		if(i == *pivot->x + pivot->w - 1){
			x2 = i;
		}
	}
	
	//y crossings
	Collision* pivot_y = col->h >= this->h ? col : this;
	Collision* pivot_y_small = col->h < this->h ? col : this;
	float y1 = -1;
	float y2 = -1;
	bool is_y_set = false;
	
	
	for(int i = *pivot_y->y; i <= *pivot_y_small->y + pivot_y_small->h - 1 && i <= *pivot_y->y + pivot_y->h - 1; i++){
		if(i >= *pivot_y_small->y && i <= *pivot_y_small->y + pivot_y_small->h - 1 && !is_y_set){
			y1 = i;
			is_y_set = true;
		}
		if(i == *pivot_y_small->y + pivot_y_small->h - 1){
			y2 = i;
		}
		if(i == *pivot_y->y + pivot_y->h - 1){
			y2 = i;
		}
	}
	//returning a rectangle
	float w;
	float h;
	
	if(!is_x_set || !is_y_set){
		return {0,0,0,0};
	}
	w = x2 - x1;
	h = y2 - y1;
	return {x1, y1, w + 1, h + 1};
	
}

void Collision::draw(){
	DrawRectangleRec({*this->x, *this->y, this->w, this->h}, WHITE);
}


Cam::Cam(){
	this->collisions[2] = new Collision(&this->x1, &this->y1, 440, 1);
	this->collisions[3] = new Collision(&this->x1, &this->y2, 440, 1);
	this->collisions[0] = new Collision(&this->x1, &this->y1, 1, 360);
	this->collisions[1] = new Collision(&this->x2, &this->y1, 1, 360);
}

void Cam::draw(){
	for(int i = 0; i < 4; i++){
		this->collisions[i]->draw();
	}
}




//Camera movement
//Maybe the most difficult part of the code yet


void Cam::move(Player &p, int& offx, int& offy){
	//checking collision 1
	this->is_x = false;
	this->is_y = false;
	p.const_y = false;
	float x_pivot;
	float y_pivot;
	if(p.facing == 1){
		int offset_x_piv =p.collision->w;
		int lastx = p.lastx + offset_x_piv - 1;
		int curr_x = p.x + offset_x - 1;
		int w = abs(lastx - curr_x + 1);
		int h = abs(p.lasty - p.y + 1);

		int newx = lastx < curr_x ? lastx : curr_x;
		int newy = p.lasty < p.y ? p.lasty : p.y;

		Collision col(&newx, &newy, w, h);

		if(p.x + offset_x_piv - 1 > *this->collisions[1]->x){
			x_pivot = p.global_x;
			this->is_x = true;
		}

		//if(this->collisions[1]->collide(&col).width != 0){
			//p.x = this->collisions[1]->collide(&col).x - offset_x_piv + 1;
		//}
	}


	if(p.facing == -1){
		int offset_x_piv = 0 ;
		int lastx = p.lastx;
		int curr_x = p.x;
		int w = abs(lastx - curr_x + 1);
		int h = abs(p.lasty - p.y + 1);

		int newx = lastx < curr_x ? lastx : curr_x;
		int newy = p.lasty < p.y ? p.lasty : p.y;

		Collision col(&newx, &newy, w, h);

		if(p.x < *this->collisions[0]->x){
			x_pivot = p.global_x;
			this->is_x = true;
		}

		//if(this->collisions[0]->collide(&col).width != 0){
			//p.x = this->collisions[0]->collide(&col).x - offset_x_piv + 1;
		//}
	}

	if(p.up_speed == 0){
		int offset_y_piv = p.collision->h;
		int lastx = p.lastx;
		int curr_x = p.x;
		int w = abs(lastx - curr_x + 1);
		int h = abs(p.lasty + offset_y_piv - p.y - offset_y_piv + 1);

		int newx = lastx < curr_x ? lastx : curr_x;
		int newy = p.lasty + offset_y_piv - 1 < p.y + offset_y_piv - 1 ? p.lasty + offset_y_piv - 1: p.y + offset_y_piv - 1;

		Collision col(&newx, &newy, w, h);

		if(p.y + offset_y_piv - 1 >= *this->collisions[3]->y){
			y_pivot = p.global_y;
			this->is_y = true;
		}
	}

	if(this->is_x){
		float new_offset_x = x_pivot - p.lastgx;
		this->offset_x  -= new_offset_x;
	}
	if(this->is_y){
		float new_offset_y = y_pivot - p.lastgy;
		this->offset_y  -= new_offset_y;
	}

	offx = this->offset_x;
	offy = this->offset_y;
	//cout << p.y <<"\n";

}
















Surface::Surface(float* xn, float* yn, int wn, int hn, string t, int i){
	this->x = xn;
	this->y = yn;
	this->w = wn;
	this->h = hn;
	this->type = t;
	this->collision = new Collision(&this->intx, &this->inty, wn, hn);
	this->index = i;
}

void Surface::update(Player& p, int offx, int offy){
	this->intx = *this->x - offx;
	this->inty = *this->y - offy;
	static bool is_standing = false;
	if(this->type == "str"){
		if(p.up_speed == 0){
			int lastx = p.global_x >= this->intx ? p.lastgx : p.lastgx + p.collision->w - 1;
			int nx = p.global_x >= this->intx ? p.global_x : p.global_x + p.collision->w - 1;
			
			
			int newx = nx > lastx ? lastx : nx;
			int newy = p.global_y + p.collision->h - 1 > p.lastgy + p.collision->h - 1 ? p.lastgy + p.collision->h - 1  : p.global_y + p.collision->h - 1;
			
			int width = abs(nx - lastx) + 1 > 0 ? abs(nx - lastx) + 1 : 1;
			int height = abs(p.global_y - p.lastgy) + 1> 0 ? abs(p.global_y - p.lastgy) + 1 : 1;
			
			Collision pivot(&newx, &newy, width, height);
			Rectangle rec = this->collision->collide(&pivot);
			if(is_standing == 0){
				if(rec.width != 0){
					p.standing_index = this->index;
				}
			}
			
			if(p.standing_index == this->index){
				if(rec.width != 0){
					if(!is_standing){
						p.global_x = rec.x - (p.global_x >= this->intx ? 0 : p.collision->w - 1);
						p.global_y = rec.y - p.collision->h + 1;
						is_standing = true;
					}
					p.contact_surf = true;
					p.const_y = false;
					//cout << rec.width << "\n";
					//cout << rec.height  << "\n";

				}
				else{
					p.contact_surf = false;
					is_standing = false;
					
				}
			}
			
			
		}
	}
	//this->collision->draw();
}


void GameObject::render(){
    Texture2D &texture = this->animater.animations[this->animater.curr_anim][this->animater.frame - 1];
	Rectangle source = {0.0f,0.0f, float(texture.width * this->facing), float(texture.height)};
	Rectangle dest = {this->x, this->y,float(texture.width), float(texture.height)};
	DrawTexturePro(texture, source, dest, {0.0f, 0.0f}, this->angle, WHITE);
}

void GameObject::render(int offx, int offy){
	Texture2D &texture = this->animater.animations[this->animater.curr_anim][this->animater.frame - 1];
	Rectangle source = {0.0f,0.0f, float(texture.width * this->facing), float(texture.height)};
	Rectangle dest = {this->x + offx, this->y + offy,float(texture.width), float(texture.height)};
	DrawTexturePro(texture, source, dest, {0.0f, 0.0f}, this->angle, WHITE);
	this->draw_x = this->x + offx;
	this->draw_y = this->y + offy;
}

//Bullet class
Bullet::Bullet(float xn, float yn, string t, signed char face){
	this->x = xn;
	this->y = yn;
	this->type = t;
	this->animater.animations = new Texture2D* [1];
	this->animater.animations[0] = this->animater.load_anim("assets/animations/sfx/", "bullet/", 1);
	this->facing = face;
}


void Bullet::move(){
	if(this->type == "str"){
		this->x += 12 * this->facing;
	}
	else if(this->type == "up"){
		this->y -= 12;
		this->angle = 90;
	}
	else{
		this->y += 12;
		this->angle = 270;
	}
}


void Bullet::exist(){
	this->move();
	animate(this->animater);
	this->render();
}

//Player class

Player::Player(){
	this->collision = new Collision(&xint, &yint, 60, 80);
	this->lastx = x;
	this->lasty = y;
	this->controller = SDL_OpenGamepad(1);
}

void Player::load_textures(){
    string animations[] = {string("idle"), string("run")};
    int anim_len = 2;
    int lens_h[] = {4, 6};
    int lens_l[] = {1, 6};

    this->animater.animations = new Texture2D* [anim_len + 6];
    this->animater_legs.animations = new Texture2D* [anim_len + 4];

    for(int i = 0; i < anim_len; i++){
        this->animater.animations[i] = this->animater.load_anim("assets/animations/marco/", (animations[i] + string("/")).c_str(), lens_h[i]);
        this->animater_legs.animations[i] = this->animater_legs.load_anim("assets/animations/marco/", (animations[i] + string("legs/")).c_str(), lens_l[i]);
    }
	this->animater_legs.animations[2] = this->animater_legs.load_anim("assets/animations/marco/", "jumplegs/", 6);
	this->animater_legs.animations[3] = this->animater_legs.load_anim("assets/animations/marco/", "idlec/", 4);
	this->animater_legs.animations[4] = this->animater_legs.load_anim("assets/animations/marco/", "runc/", 7);
	this->animater_legs.animations[5] = this->animater_legs.load_anim("assets/animations/marco/", "shootc/", 10);
	this->animater.animations[2] = this->animater.load_anim("assets/animations/marco/", "shoot/", 10);
	this->animater.animations[3] = this->animater.load_anim("assets/animations/marco/", "up/", 1);
	this->animater.animations[4] = this->animater.load_anim("assets/animations/marco/", "upp/", 1);
	this->animater.animations[5] = this->animater.load_anim("assets/animations/marco/", "shootup/", 9);
	this->animater.animations[6] = this->animater.load_anim("assets/animations/marco/", "jumpc/", 1);
	this->animater.animations[7] = this->animater.load_anim("assets/animations/marco/", "shootj/", 6);
}

void Player::jump(){
	static double energy = 7;
	static bool is_jumping = false;
	this->y += this->down_speed;
	this->global_y += this->down_speed;
	if(this->to_jump and this->state_legs != "shootc"){
		is_jumping = true;
	}
	if(this->down_speed > 0){
		this->state_legs = "jump";
		if(this->crouching){
			this->state_head = "jumpc";
			this->crouching = false;
		}
		this->set_new_speed = true;
		this->set_speed = 2;
	}
	if(is_jumping){
		if(this->crouching){
			this->state_head = "jumpc";
			this->crouching = false;
		}
		this->state_legs = "jump";
		this->set_new_speed = true;
		this->set_speed = 2;
		if(energy > 0){
			this->up_speed = energy;
			this->y -= energy;
			this->global_y -= energy;
			energy-=0.15;
			this->down_speed = 0;
			SDL_RumbleGamepad(this->controller, 6400, 6400, 5);
		}
		else{
			energy = 7;
			is_jumping = false;
			this->up_speed = 0;
			SDL_RumbleGamepad(this->controller, 0, 0, 5);
		}
	}
	
}
void Player::shoot(){
	static bool is_shooting = false;
	static string state_piv;
	static bool is_set = false;
	static bool is_played = false;
	static bool is_crouching = false;
	static bool isnt_crouching = false;
	static bool was_crouching = false;
	static string state_shoot;
	static char cooldown = 0;
	if(this->to_shoot == true && cooldown == 0){
		is_shooting = true;
		is_set = false;
		is_played = false;
		if(this->crouching){
			this->animater_legs.frame = 1;
			was_crouching = true;
			state_shoot = this->state_legs;
			is_crouching = true;
			isnt_crouching = false;
		}
		else{
			this->animater.frame = 1;
			was_crouching = false;
			state_shoot = this->state_head;
			isnt_crouching = true;
			is_crouching = false;
		}
	}

	if(is_crouching){
		if(this->crouching != was_crouching){
			is_shooting = false;
			SDL_RumbleGamepad(this->controller, 0, 0, 5);
			cooldown = 0;
		}
	}

	if(isnt_crouching){
		if(this->crouching != was_crouching){
			is_shooting = false;
			SDL_RumbleGamepad(this->controller, 0, 0, 5);
			cooldown = 0;
		}
	}


	if(is_shooting){
		SDL_RumbleGamepad(this->controller, 48000, 48000, 5); // 0.5 сек
		if(is_crouching){
			this->crouching = true;
			this->null_speed = true;
		}
		if(isnt_crouching){
			this->crouching = false;
			this->set_new_speed = true;
			this->set_speed = 2;
		}
		if(!is_set){
			if(this->state_head == "up" || this->state_head == "upp"){
				state_piv = "shootup";
				is_set = true;
			}
			else if(this->crouching){
				state_piv = "shootc";
				is_set = true;
			}
			else if(this->state_head == "jumpc"){
				state_piv = "shootj";
				is_set = true;
			}
			else{
				state_piv = "shoot";
				is_set = true;
			}
		}
		if(this->crouching){
			this->state_legs = state_piv;
			is_crouching = true;
		}
		else{
			this->state_head = state_piv;
			this->crouching = false;
			isnt_crouching = true;
		}
		
		if((this->animater.frame == 1 || this->animater_legs.frame == 1) && (this->animater.curr_anim == 2 || this->animater.curr_anim == 5 || this->animater_legs.curr_anim == 5 || this->animater.curr_anim == 7) && !is_played){
			Sound* pshot = new Sound;
			cooldown = 14;
			*pshot = LoadSoundFromWave(LoadWave("assets/sounds/shoot.wav"));
			SetSoundVolume(*pshot,0.25);
			PlaySound(*pshot);
			delete pshot;
			if(state_piv == "shoot"){
				this->bullets[this->bullet_len] = new Bullet(this->x + 76 * this->facing, this->y + 21 + this->offset_y, "str", this->facing);
			}
			else if(state_piv == "shootup"){
				this->bullets[this->bullet_len] = new Bullet(this->x + 20 + this->offset_x, this->y + this->offset_y_h + 41 + this->offset_y, "up", this->facing);
			}
			else if(state_piv == "shootc"){
				this->bullets[this->bullet_len] = new Bullet(this->x + 74 * this->facing, this->y + 44 + this->offset_y + 15, "str", this->facing);
			}
			else if(state_piv == "shootj"){
				this->bullets[this->bullet_len] = new Bullet(this->x + (this->facing==1 ? 25 : 15), this->y + offset_y_h + 70, "down", this->facing);
			}
			this->bullet_len++;
			is_played = true;
			
		}
		if(this->animater.frame == 10 && this->animater.curr_anim == 2){
			is_shooting = false;
			is_set = false;
			is_played = false;
			isnt_crouching = 0;
			SDL_RumbleGamepad(this->controller, 0, 0, 5); // 0.5 сек
		}
		else if(this->animater_legs.frame == 10 && this->animater_legs.curr_anim == 5){
			is_shooting = false;
			is_set = false;
			is_played = false;
			is_crouching = 0;
			SDL_RumbleGamepad(this->controller, 0, 0, 5); // 0.5 сек
		}
		else if(this->animater.frame == 9 && this->animater.curr_anim == 5){
			is_shooting = false;
			is_set = false;
			is_played = false;
			isnt_crouching = 0;
			SDL_RumbleGamepad(this->controller, 0, 0, 5); // 0.5 сек
		}
		else if(this->animater.frame == 6 && this->animater.curr_anim == 7){
			is_shooting = false;
			is_set = false;
			is_played = false;
			isnt_crouching = 0;
			SDL_RumbleGamepad(this->controller, 0, 0, 5); // 0.5 сек
		}
		if(cooldown > 0){
			cooldown--;
		}
	}

}



void Player::render(int xoff, int yoff){
    Texture2D &legt = this->animater_legs.animations[this->animater_legs.curr_anim][this->animater_legs.frame - 1];
    Texture2D &headt = this->animater.animations[this->animater.curr_anim][this->animater.frame - 1];
	this->x = this->global_x + xoff;
	this->y = this->global_y + yoff;
	
	float legx = this->x;
	float headx = this->x;

    if(facing == -1){
		headx = headx + this->head_r_off + this->offset_x;
		if(this->to_offset){
			if(this->animater.frame != 1){
				headx = headx - headt.width + this->animater.animations[this->animater.curr_anim][this->animater.frame - 2].width;
			}
		}
		legx = legx + + this->offset_x;
	}

    Rectangle lsource = {0,0,float(legt.width * this->facing), float(legt.height)};
    Rectangle ldest = {legx, this->y + 45 * 1 + this->offset_y, float(legt.width) * 1, float(legt.height) * 1};
    Rectangle hsource = {0,0,float(headt.width * this->facing), float(headt.height)};
    Rectangle hdest = {headx, this->y + this->offset_y + this->offset_y_h,float(headt.width) * 1, float(headt.height) * 1};
	
    DrawTexturePro(legt, lsource, ldest, {0,0}, 0, WHITE);
	if(!this->crouching){
		DrawTexturePro(headt, hsource, hdest, {0, 0}, 0, WHITE);
	}
    
	
}

void Player::null_all(){
	this->offset_y = 0;
	this->offset_y_h = 0;
	this->head_r_off = 0;
	this->to_jump = false;
	this->offset_x = 0;
	this->speed = 2;
}

void Player::move(){
	//setting states
	this->lastx = this->x;
	this->lasty = this->y;
	this->lastgy = this->global_y;
	this->lastgx = this->global_x;
	
	if(!this->contact_surf){
		this->down_speed += 0.15;
	}
	else{
		this->down_speed = 0;
	}
	
	this->crouching = false;
	if(IsKeyDown(KEY_S) || IsGamepadButtonDown(0, 3)){
		this->crouching = true;
		this->speed = 0.5;
	}
	if(this->null_speed){
		this->speed = 0;
	}
	if(this->set_new_speed){
		this->speed = this->set_speed;
	}
	
    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_A) || IsGamepadButtonDown(0,4) || IsGamepadButtonDown(0,2)){
		this->state_head = "run";
		this->state_legs = "run";
        if(IsKeyDown(KEY_D) || IsGamepadButtonDown(0,2)){
            this->x+=this->speed;
			this->global_x+=this->speed;
            this->facing = 1;
        }
        if(IsKeyDown(KEY_A) || IsGamepadButtonDown(0,4)){
            this->x-=this->speed;
			this->global_x-=this->speed;
            this->facing = -1;
        }
    }
    else{
		this->state_head = "idle";
		this->state_legs = "idle";
    }
	if(IsKeyDown(KEY_D) && IsKeyDown(KEY_A) || IsGamepadButtonDown(0,4) && IsGamepadButtonDown(0,2)){
		this->state_legs = "idle";
		this->state_head = "idle";
	}
	
	
	if(IsKeyPressed(KEY_RIGHT_CONTROL) || IsGamepadButtonPressed(0, 7)){
		this->to_shoot = true;
	}
	if((IsKeyDown(KEY_W) || IsGamepadButtonDown(0,1))  && !this->crouching){
		this->state_head = "up";
		if(IsKeyDown(KEY_A) || IsKeyDown(KEY_D)){
			this->state_head = "upp";
		}
	}
	if((IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(0, 6)) && this->down_speed == 0){
		this->to_jump = true;
	}
	this->null_speed = false;
	this->set_new_speed = false;
	this->set_speed = 0;
	this->jump();
	this->shoot();
	this->to_shoot = false;
	
	this->null_all();
	if(this->crouching){
		this->offset_y = -15;
	}
	
	//setting animations
	if(this->state_head == "idle"){
		this->animater.set_new_anim(0, 25, 4);
	}
	if(this->state_legs == "idle"){
		if(this->crouching){
			this->animater_legs.set_new_anim(3, 25, 4);
		}
		else{
			this->animater_legs.set_new_anim(0, 25, 1);
		}
	}
	if(this->state_head == "run"){
		this->animater.set_new_anim(1, 12, 6);
	}
	if(this->state_legs == "run"){
		if(!this->crouching){
			this->animater_legs.set_new_anim(1, 12, 6);
			this->offset_y = -10; 
		}
		else{
			this->animater_legs.set_new_anim(4, 12, 7);
		}
	}
	if(this->state_legs == "jump"){
		this->animater_legs.set_new_anim(2, 12, 1);
	}
	if(this->state_head == "shoot"){
		this->animater.set_new_anim(2, 7, 10);
		this->offset_y_h = 8;
	}
	if(this->state_head == "up"){
		this->animater.set_new_anim(3, 7, 1);
		this->offset_y_h = 5;
	}
	if(this->state_head == "upp"){
		this->animater.set_new_anim(4, 7, 1);
		this->offset_y_h = 5;
	}
	if(this->state_head == "shootup"){
		this->animater.set_new_anim(5, 7, 9);
	}
	if(this->state_legs == "shootc"){
		this->animater_legs.set_new_anim(5,7, 10);
	}
	if(this->state_head == "jumpc"){
		this->animater.set_new_anim(6, 1, 1);
		this->offset_y_h = 10;
	}
	if(this->state_head == "shootj"){
		this->animater.set_new_anim(7, 7, 6);
	}
	if(this->const_y){
		this->y = this->lasty;
	}

	xint = x;
	yint = y;
}

void Player::set_offsets(){
	if(this->animater.curr_anim == 2){
		switch(this->animater.frame){
			case 1:
			case 2:
			case 3:
				this->head_r_off = -60;
				break;
			
			default:
				this->head_r_off = -30;
		}
		this->to_offset = false;
		
	}
	if(this->animater.curr_anim == 1){
		this->head_r_off = -20;
		this->to_offset = true;
	}
	if(this->animater.curr_anim == 0){
		this->head_r_off = -24;
		this->to_offset = true;
	}
	if(this->animater.curr_anim == 3){
		this->head_r_off = -10;
	}
	if(this->animater.curr_anim == 4){
		this->head_r_off = -24;
	}
	if(this->animater.curr_anim == 5){
		switch(this->animater.frame){
			case 1:
			case 2:
			case 3:
				this->offset_y_h = -80;
				break;
			default:
				this->offset_y_h = -20;
		}
	}
	if(this->animater_legs.curr_anim == 3 && this->crouching){
		this->offset_x = -10;
	}
	if(this->animater_legs.curr_anim == 4 && this->crouching){
		this->offset_x = -10;
	}
	if(this->animater_legs.curr_anim == 5){
		switch(this->animater_legs.frame){
			case 1:
			case 2:
			case 3:
				this->offset_x = -40;
				break;
			
			default:
				this->offset_x = -15;
		}
		this->to_offset = false;
		this->offset_y -= 7;
	}
	if(this->animater.curr_anim == 7){
		switch(this->animater.frame){
			case 1:
			case 2:
			case 3:
				this->offset_y_h = 15;
				break;
			default:
				this->offset_y_h = 10;
		}
	}
	
	
}

void Player::exist(){
    this->move();
	animate(this->animater);
    animate(this->animater_legs);
	this->set_offsets();
	for(int i = 0; i < this->bullet_len; i++){
		this->bullets[i]->exist();
	}
	//this->collision->draw();
	
}
