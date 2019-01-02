//#include "SpriteSheet.h"
#include "Hero.h"
#include "Brick.h"

int collided(int x, int y);
bool endValue(int x, int y);
bool brickHit(int x,int y);
//Aaron Purinton
Hero::Hero()
{
	image = NULL;
	health = NULL;
}
Hero::~Hero()
{
	al_destroy_bitmap(health);
}
void Hero::InitSprites(int width, int height)
{

	setLoc(block * 3, block * 2);

	maxFrame = 4;//only need swap between 4 frames x-wise
	curFrame = 0;

	frameCount = 0;
	frameDelay = 15;

	frameWidth = 64;
	frameHeight = 128;

	animationColumns = 3;
	//animationRows = 4;
	animationDirection = 1;

	image = al_load_bitmap("HeroBot.png");
	health = al_load_bitmap("healthbar.png");
	sampl = al_load_sample("hurt.wav");

	if (!sampl) {
		exit(9);
	}
	//al_convert_mask_to_alpha(image, al_map_rgb(255,0,255));
}

void Hero::setRed(bool isRed) {
	if (isRed) {
		red = 1;
	}
	else {
		red = 0;
	}
}

void Hero::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;
	if (Icounter > 0) {
		Icounter -= 1.0 / 60.0;
	}
	else {
		Iframe = false;
		Icounter = 0;
	}
	//update positiion and frame
	if (dir == 1) { //right key
		animationDirection = 1;
		//sidways = 2;
		x += speed;
		//if (++frameCount > frameDelay)
		//{
		//propeller++;
		//frameCount=0;
		//if (++curFrame > maxFrame)
		//curFrame=0;
		//}
	}
	else if (dir == 0) { //left key
		animationDirection = 0;
		//sidways = 2;
		x -= speed;
		//if (++frameCount > frameDelay)
		//{
		//frameCount=0;
		//propeller++;
		//if (++curFrame > maxFrame)
		//				curFrame=0;
		//}
	}
	else if (dir == 2) { //up key
		animationDirection = 2;
		///sidways = 0;
		y -= speed;
		//if (++frameCount > frameDelay)
		//{
		//frameCount = 0;
		//propeller++;
		//if (++curFrame > maxFrame)
		//curFrame = 0;
		//}
	}
	else if (dir == 3) { //down key
		animationDirection = 3;
		//sidways = 0;
		y += speed;
		//if (++frameCount > frameDelay)
		//{
		//frameCount = 0;
		//propeller++;
		//if (++curFrame > maxFrame)
		//curFrame = 0;
		//}
	}

	//collision
	if (animationDirection == 0)
	{
		if (collided(x + 15, y + 3 * frameHeight / 4) || brickHit(x + 15, y + 3 * frameHeight / 4)) { //collision detection to the left
			x = oldx;
			y = oldy;
		}

	}
	else if (animationDirection == 1)
	{
		if (collided(x + frameWidth - 15, y + 3 * frameHeight / 4) || brickHit(x + frameWidth - 15, y + 3 * frameHeight / 4)) { //collision detection to the right
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 2)
	{
		if (collided(x + frameWidth / 2, y + frameHeight / 2 + 15) || brickHit(x + frameWidth / 2, y + frameHeight / 2 + 15)) { //collision detection to the above
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 3)
	{
		if (collided(x + frameWidth / 2, y + frameHeight - 10) || brickHit(x + frameWidth / 2, y + frameHeight - 10)) { //collision detection to the bellow
			x = oldx;
			y = oldy;
		}
	}
}

bool Hero::CollisionEndBlock()
{
	if (endValue(x + frameWidth / 2, y + 3*frameHeight / 4))
		return true;
	else
		return false;
}

void Hero::DrawSprites(int xoffset, int yoffset)
{
	if (++frameCount > frameDelay)
	{
		frameCount = 0;
		if (++curFrame > maxFrame) {
			curFrame = 0;
		}
	}
	if (Iframe && curFrame % 2) { return; }

	int fx = (curFrame % animationColumns) * frameWidth;//0,1,2
	int fy = frameHeight * red;// (propeller + sidways) * frameHeight;

	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);

}

void Hero::drawHP() {
	al_draw_bitmap_region(health, 0, (6-HP)*64, frameWidth, 64, 0, 0, 0);
}

void Hero::hurt() {
	if (Iframe) {
		return;
	}
	Iframe = true;
	playSFX();
	//cout << "ow\n";
	Icounter = Iframes;
	HPhit();
}