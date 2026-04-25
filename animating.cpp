#include "animating.h"



string itStr(int a){
    string result;
    while(a > 0){
        int pivot = a % 10;
        result = char(pivot + 48) + result;
        a /= 10;
    }
    return result;
}



Texture2D* Animater::load_anim(const char* folder, const char* type, int len){
    Texture2D* anim = new Texture2D[len];
    for(int i = 1; i <= len; i++){
        string file = string(folder) + string(type) + itStr(i) + ".png";
        anim[i - 1] = LoadTexture(file.c_str());
    }
    return anim;
}

void Animater::set_new_anim(int anim, int delay, int frames){
    if(this->curr_anim == anim){
        this->curr_len = frames;
        return;
    }
    else{
        this->frame = 1;
        this->timer = 0;
        this->curr_anim = anim;
        this->curr_len = frames;
		this->curr_delay = delay;
    }
}




int animate(Animater& animater){
    animater.timer++;
    if(animater.timer >= animater.curr_delay){
        animater.timer = 0;
        animater.frame += 1;
    }
    if(animater.frame > animater.curr_len){
        animater.frame = 1;
    }
    return animater.frame;
}
