#ifndef bomb
#define bomb
#include "SpriteSheet.h"
#include "Hero.h"
using namespace std;
//Aaron Purinton
class Bomb : public Sprite
{
public :
	Bomb();
	~Bomb();
	void InitSprites(int width, int height);
	bool spawn(int x, int y);
	int explode(int rangeLeft, int dir);
	void drawExplostion(int xoffset, int yoffset, Hero &player);
	void DrawSprites(int xoffset, int yoffset);
	void dead() { 
		if (alive) {
			alive = false;
		}
	}
	bool isAlive() { return alive; }
	bool isExploding() { return exploding; }
	float getLast() { return current; }
	void updatesplosion();

	bool playerColide(Hero &player, int Tilex, int Tiley, int xoff, int yoff);
private:

	int extra = 0;
	bool alive = false, exploding = false;
	float countdown;
	ALLEGRO_BITMAP* explosion;
	int range = 3;
	const float RAD = 3.14159 / 2.00;
	const float lasting = 0.98;
	float current = 0;
};

#endif // !spritesheet
