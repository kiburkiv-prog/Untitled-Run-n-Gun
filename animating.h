#include <raylib.h>
#include <string>

using namespace std;

string itStr(int);

class Animater{
public:
    int timer = 0;
    int frame = 1;
    int curr_anim = 0;
    int curr_len = 0;
	int curr_delay = 25;
    Texture2D** animations;
    Texture2D* load_anim(const char*, const char*, int);
    void set_new_anim(int, int, int);
};



string get_frame(const char*, int);
int animate(Animater&);
