#include "Brick.h"
Brick::Brick()
{
	image = NULL;
}

Brick::~Brick()
{
	//cout << "this time\n";
	//al_destroy_bitmap(image);
}

void Brick::InitSprites(int width, int height) {

	maxFrame = 1;
	curFrame = 0;

	frameCount = 0;
	frameDelay = 8;

	frameWidth = 64;
	frameHeight = 64;
	sampl = al_load_sample("bricks.wav");
	image = al_load_bitmap("brick.png");
	if (!sampl) {
		exit(9);
	}
	//al_convert_mask_to_alpha(image, al_map_rgb(255,0,255));
}

bool Brick::spawn(int x, int y) {
	if (!alive) {
		alive = true;

		x = x / 64;
		y = y / 64;
		setLoc(x * 64, y * 64);//snaps the brick to the grid
		return true;
	}
	return false;
}

bool Brick::playerColide(int xx, int yy) {
	//cout << (int)(player.getX() / block) << " " << (int)(player.getY() + (3 * player.getHeight() / 4)) / block << " : " << Tilex / block << " " << Tiley / block << "\n";
	return(((int)(xx / block) == (int)(x / block)) && ((int)(yy  / block) == (int)(y / block)) && alive);
}

void Brick::DrawSprites(int xoffset, int yoffset) {
	int fx = 0;
	int fy = 0;
	if (alive) {
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
	}
}