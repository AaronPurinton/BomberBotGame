#include "SpriteSheet.h"
int collided(int x, int y);
bool endValue(int x, int y);
void destroyBrick(int x, int y);
//Aaron Purinton
Sprite::Sprite()
{
	image=NULL;
}
Sprite::~Sprite()
{
	image = NULL;
	al_destroy_bitmap(image);
	sampl = NULL;
	al_destroy_sample(sampl);
	
}
void Sprite::InitSprites(int width, int height)//initalize sprites --ERROR IS DEFUALT IF NOT OVERRIDDEN
{
	image = al_load_bitmap("Error.png");
	sampl = al_load_sample("error.wav");
	setLoc(block * 3, block * 2);

	maxFrame = 1;
	curFrame = 0;

	frameWidth = 64;
	frameHeight = 64;
}

void Sprite::setLoc(int tox, int toy) {//set location
	x = tox;
	y = toy;
}

void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;

	//update positiion and frame
	if(dir == 1){ //right key
		animationDirection = 1; 

		x+=speed; 

	} else if (dir == 0){ //left key
		animationDirection = 0; 

		x-=speed; 

	}
	else if (dir == 2) { //up key
		animationDirection = 2;

		y -= speed;

	}
	else if (dir == 3) { //down key
		animationDirection = 3;

		y += speed;

	}

	//collision
	if (animationDirection==0)
	{ 
		if (collided(x, y + frameHeight/2)) { //collision detection to the left
			x = oldx; 
			y= oldy;
		} 

	}
	else if (animationDirection ==1)
	{ 
		if (collided(x + frameWidth, y + frameHeight/2)) { //collision detection to the right
			x = oldx; 
			y= oldy;
		}
	}
	else if (animationDirection == 2)
	{
		if (collided(x+frameWidth/2 , y+frameHeight/2)) { //collision detection to the above
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 3)
	{
		if (collided(x + frameWidth/2, y + frameHeight)) { //collision detection to the bellow
			x = oldx;
			y = oldy;
		}
	}
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{

	int fx = 0;
	int fy = 0;

	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);

	}

void Sprite::playSFX() {
	al_play_sample(sampl, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);//music
}