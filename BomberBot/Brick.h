#ifndef brick
#define brick
#include "SpriteSheet.h"
#include "Hero.h"
using namespace std;
//Aaron Purinton
class Brick : public Sprite
{
public:
	Brick();
	~Brick();
	void InitSprites(int width, int height);
	bool spawn(int x, int y);
	void DrawSprites(int xoffset, int yoffset);
	void dead() { if (alive) { 
		playSFX();
		alive = false;
	} }
	bool isAlive() { return alive; }

	bool playerColide(int xx, int yy);
private:

	bool alive = false;
};

#endif // !spritesheet
