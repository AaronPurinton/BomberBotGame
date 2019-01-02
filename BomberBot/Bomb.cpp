#include "Bomb.h"
#include "Hero.h"

int collided(int x, int y);
bool endValue(int x, int y);
bool wall(int x, int y);
void destroyBrick(int x, int y);
//Aaron Purinton
Bomb::Bomb()
{
	image = NULL;
	explosion = NULL;
}

Bomb::~Bomb()
{
	//al_destroy_bitmap(image);
	al_destroy_bitmap(explosion);
}

void Bomb::InitSprites(int width, int height) {

	maxFrame = 16;
	curFrame = 0;

	frameCount = 0;
	frameDelay = 8;

	countdown = (maxFrame - 1)*frameDelay;//number of frames before the bomb explodes

	frameWidth = 64;
	frameHeight = 64;

	image = al_load_bitmap("Bomb.png");
	explosion = al_load_bitmap("beam.png");
	sampl = al_load_sample("xplode.wav");
	//al_convert_mask_to_alpha(image, al_map_rgb(255,0,255));
}

bool Bomb::spawn(int x, int y) {
	if (!alive  && !exploding) {
		alive = true;

		x = x / 64;
		y = y / 64;
		setLoc(x * 64, y * 64);//snaps the bomb to the grid
		return true;
	}
	return false;
}

//Returns the number of tiles that explode in a given direction
int Bomb::explode(int rangeLeft, int dir) {
	if (rangeLeft > 0) {
		switch (dir)
		{
		case 0://left
			if (wall(x - (block*(range-rangeLeft) ), y+32)) {
				return 0;
			}
			break;
		case 1://right
			if (wall(x + (block*(range - rangeLeft)), y+32)) {
				return 0;
			}
			break;
		case 2://up
			if (wall(x+32, y - (block*(range - rangeLeft)))) {
				return 0;
			}
			break;
		case 3://down
			if (wall(x+32, y + (block*(range - rangeLeft)))) {
				return 0;
			}
			break;
		default:
			break;
		}
		return explode(rangeLeft - 1, dir) + 1;
	}
	else {
		return 0;
	}
}

//draws the bomb
void Bomb::DrawSprites(int xoffset, int yoffset) {
	if (++frameCount > frameDelay)
	{
		frameCount = 0;
		if (++curFrame > maxFrame) {
			curFrame = 0;
			alive = false;
		}
	}
	
	int fx = curFrame*frameWidth;
	int fy = 0;
	if (alive) {
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
	}
}

//draws the explosion of the bomb
void Bomb::drawExplostion(int xoffset, int yoffset, Hero &player) {
	if (exploding) {
		int going = 0;
		float scale = 1;
		int interation = 0;
		int r = rand() % 256; int g = rand() % 256; int b = rand() % 256;
		for (int dir = 0; dir < 4; dir++) {
			going = explode(range, dir)-1;
			interation = 0;
			for (int in = going * block; in > 0; in -= 64, interation++) {
				extra = (extra + 1) % 2;
				int fx = (extra*frameWidth);
				int fy = 0;
				
				switch (interation) {
				case 0:
					scale = 1.0;
					break;
				case 1:
					scale = 1.05;
					break;
				case 2:
					scale = 1.05;
					break;
				case 3:
					scale = 1.15;
					break;
				default:
					scale = 1;
					break;

				}
				switch (dir)
				{
				case 0://left
					al_draw_tinted_scaled_rotated_bitmap_region(explosion, fx, fy, frameWidth, frameHeight,al_map_rgb(r,g,b),32,32, ((x - xoffset)+32)-in, (y - yoffset)+32, scale, scale,RAD*0,0);
					if (playerColide(player, x + 32 - in, y + 32, xoffset, yoffset)) {
						player.hurt();
					}
					destroyBrick(x + 32 - in, y + 32);
					break;
				case 1://right
					al_draw_tinted_scaled_rotated_bitmap_region(explosion, fx, fy, frameWidth, frameHeight, al_map_rgb(r, g, b), 32, 32, ((x - xoffset) + 32)+in, (y - yoffset) + 32, scale, scale, RAD * 0, ALLEGRO_FLIP_HORIZONTAL);
					if (playerColide(player, x + 32 + in, y  + 32, xoffset, yoffset)) {
						player.hurt();
					}
					destroyBrick(x + 32 + in, y + 32);
					break;
				case 2://up
					al_draw_tinted_scaled_rotated_bitmap_region(explosion, fx, fy, frameWidth, frameHeight, al_map_rgb(r, g, b), 32, 32, (x - xoffset) + 32, ((y - yoffset) + 32) - in, scale, scale, RAD, ALLEGRO_FLIP_VERTICAL);
					if (playerColide(player, x  + 32, y + 32 - in, xoffset, yoffset)) {
						player.hurt();
					}
					destroyBrick(x + 32, y + 32 - in);
					break;
				case 3://down
					al_draw_tinted_scaled_rotated_bitmap_region(explosion, fx, fy, frameWidth, frameHeight, al_map_rgb(r, g, b), 32, 32, (x - xoffset) + 32, ((y - yoffset) + 32) + in, scale, scale, RAD, 0);
					if (playerColide(player, x  + 32, y + 32 + in, xoffset, yoffset)) {
						player.hurt();
					}
					destroyBrick(x + 32, y + 32 + in);
					break;
				default:
					break;
				}
			}
		}
	}
}

//used witht the explosion, will see if player needs to be hurt();
bool Bomb::playerColide(Hero &player, int Tilex, int Tiley, int xoff, int yoff) {
	//cout << (int)(player.getX() / block) << " " << (int)(player.getY() + (3 * player.getHeight() / 4)) / block << " : " << Tilex / block << " " << Tiley / block << "\n";
	return( ( (int)(player.getX() / block) == (int)(Tilex / block)) && ( (int)(player.getY() + (3*player.getHeight()/4)) / block == (int)(Tiley / block)));
}

//progess the explosion as needed
void Bomb::updatesplosion() {
	if (alive) {
		countdown -= 1; //(maxFrame - 3)*frameDelay;
			if (countdown <= 0) {
				exploding = true;
			}
			else if (countdown == 24) {
				playSFX();
			}
	}
	if (exploding) {
		current += 1.0 / 60.0;
		if (current >= lasting) {
			exploding = false;
			countdown = (maxFrame - 1)*frameDelay;
			current = 0;
		}
	}
}