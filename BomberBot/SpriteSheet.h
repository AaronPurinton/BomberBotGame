#ifndef spritesheet
#define spritesheet



#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

#include <iostream>

using namespace std;
//Aaron Purinton
class Sprite
{
	friend int collided(int x, int y);
	friend bool endValue(int x, int y);
	friend bool wall(int x, int y);
	friend void destroyBrick(int x, int y);
public:
	Sprite();
	~Sprite();
	void InitSprites(int width, int height);
	void UpdateSprites(int width, int height, int dir); //dir 1 = right, 0 = left, 2 = Standing Still
	void DrawSprites(int xoffset, int yoffset);
	float getX() {return x;}
	float getY() {return y;}
	int getWidth() {return frameWidth;}
	int getHeight() {return frameHeight;}
	void setLoc(int tox, int toy);
	void playSFX();

protected:
	float x;
	float y;

	int maxFrame=0;
	int curFrame=0;

	int frameCount=0;
	int frameDelay=0;

	int frameWidth=64;
	int frameHeight=64;
	
	int animationColumns;
	int animationRows;
	
	int animationDirection;

	const int block = 64;// size of tile for spwan help
	const int speed = 5;// how fast we move

	//int red = 0;//0 for not, 1 for is

	ALLEGRO_BITMAP *image;
	ALLEGRO_SAMPLE *sampl;
};

#endif // !spritesheet
