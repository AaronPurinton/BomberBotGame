#ifndef heroo
#define heroo


#include "SpriteSheet.h"

class Hero : public Sprite {
	friend bool brickHit(int x, int y);
public:
	//OVERRIDE
	Hero();
	~Hero();
	void InitSprites(int width, int height);
	void UpdateSprites(int width, int height, int dir); //dir 1 = right, 0 = left, 2 = Standing Still
	void DrawSprites(int xoffset, int yoffset);
	void HPhit() { HP--; }
	int getHP() { return HP; }
	void drawHP();
	void hurt();
	
	//ADDED
	bool CollisionEndBlock();
	void setRed(bool isRed);

private:
	//Flag for red mode
	const int Iframes = 45;
	int Icounter = 0;
	bool Iframe = false;

	int red = 0;//0 or 1
	int HP = 6;
	ALLEGRO_BITMAP* health;
};

#endif // heroo