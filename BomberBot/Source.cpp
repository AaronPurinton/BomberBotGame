#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "SpriteSheet.h"
#include "SpriteSheet.h"
#include "mappy_A5.h"

#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

#include "Hero.h"
#include "Bomb.h"
#include "Brick.h"

#include <iostream>
using namespace std;
//Aaron Purinton
int collided(int x, int y);  //Tile Collision
bool endValue( int x, int y ); //End Block with the User Value = 8

void spawnBricks();//spwns the bricks on a map
bool wall(int x, int y);//returns true if it is a wall tile
bool brickHit(int x, int y); //collided with a brick sprite
void destroyBrick(int x, int y);//finds the block for a given xy point and destroys the brick if present

int max=150;//number of blocks on the map 0-150
Brick bricks[150];//array of bricks may be uysed up to 150 at a time

int main(void)
{
	const int WIDTH = 950;
	const int HEIGHT = 500;
	bool keys[] = {false, false, false, false, false};
	enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE};
	//variables
	bool done = false;
	bool render = false;
	//Player Variable
	Hero player; 
	//We may be able to place more than one bomb at a time
	Bomb bag[10];
	
	//time for the level
	float time = 0;
	//time we are the color red
	float redTime = 0;
	//the amount of time we cannot place a bomb
	float coolDown = 1.5;


	//allegro variable
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_SAMPLE *sampl = NULL;
	ALLEGRO_SAMPLE *failTune = NULL;
	ALLEGRO_SAMPLE *passTune = NULL;
	//program init
	if(!al_init())										//initialize Allegro
		return -1;

	if (!al_install_audio()) {
		return -1;
	}

	if (!al_init_acodec_addon()) {
		return -1;
	}

	if (!al_reserve_samples(5)) {//NUMBER OF AUDIO FILES ... IMPORTANT
		return -1;
	}

	sampl = al_load_sample("background.ogg");//background
	failTune = al_load_sample("loss.wav");//lose game
	passTune = al_load_sample("good.wav");//win level

	if (!sampl || !failTune || !passTune) {
		exit(9);
	}

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if(!display)										//test display object
		return -1;

	//addon init
	al_install_keyboard();
	al_init_image_addon();
	al_init_primitives_addon();

	al_init_font_addon();//added fonts
	al_init_ttf_addon();

	ALLEGRO_FONT *font = al_load_font("Droid.ttf", 45, 0);

	//initialize our sprites
	player.InitSprites(WIDTH,HEIGHT);
	for (int i = 0; i < 10; i++) {
		bag[i].InitSprites(WIDTH, HEIGHT);
	}

	for (int i = 0; i < 150; i++) {
		bricks[i].InitSprites(WIDTH, HEIGHT);
	}

	int xOff = 0;
	int yOff = 0;

	char sample[] = { 'A','L','P','H','A','.','f','m','p','\0' };

	//sprint(samle, "DELTA.fmp");
	if (MapLoad(sample, 1))
		return -5;

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	while (!done) {//intro screen
		ALLEGRO_EVENT pv;
		

		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2, 30, ALLEGRO_ALIGN_CENTRE, "Welcome to BomberBot");
		al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2, 90, ALLEGRO_ALIGN_CENTRE, "blow up the brick walls to get to the exit");
		al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2, 150, ALLEGRO_ALIGN_CENTRE, "[Arrow Keys] to Move");
		al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2, 210, ALLEGRO_ALIGN_CENTRE, "[Space] to drop a bomb");
		al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2, 270, ALLEGRO_ALIGN_CENTRE, "help.txt for more info");
		al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2, 330, ALLEGRO_ALIGN_CENTRE, "Press Any Key to Continue");

		al_flip_display();
		al_wait_for_event(event_queue, &pv);

		if (pv.type == ALLEGRO_EVENT_KEY_DOWN) {
			done = true;
		}
	}
	done = false;
	
	//SPAWN THE BLOCKS
	spawnBricks();

	MapInitAnims ();//map animations

	bool levelCom = false;//completed current level
	bool fail = false;//timed out
	bool finished = false;//full clear
	int levelCount = 0;//level number
	float timeLeft = 60;//time the level is required to be beaten by
	float TotalTime = 0;//tottal time the player has been in a match
	
	al_start_timer(timer);
	//draw the background tiles
	MapDrawBG(xOff,yOff, 0, 0, WIDTH-1, HEIGHT-1);

	//draw foreground tiles
	MapDrawFG(xOff,yOff, 0, 0, WIDTH-1, HEIGHT-1, 0);
	player.DrawSprites(0,0);

	//cout << max;

	for (int i = 0; i < max; i++) {
		//cout << i << "\n";
		bricks[i].DrawSprites(0, 0);
	}
	//draw the bricks thate are to be played at start
	al_flip_display();
	al_clear_to_color(al_map_rgb(0,0,0));

	al_play_sample(sampl, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);//music

	while(!done)
	{
		if (time >= timeLeft || player.getHP()==0) {
			//game over
			done = true;
			fail = true;
		}
		
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			//animations of map
			MapUpdateAnims();

			time += 1.0 / 60.0;
			if (redTime > 0) {
				redTime -= 1.0 / 60.0;
			}
			else if (redTime < 0) { redTime = 0; player.setRed(false); }

			render = true;
			if(keys[UP])
				player.UpdateSprites(WIDTH, HEIGHT, 2);
			else if(keys[DOWN])
				player.UpdateSprites(WIDTH, HEIGHT, 3);
			else if(keys[LEFT])
				player.UpdateSprites(WIDTH,HEIGHT,0);
			else if(keys[RIGHT])
				player.UpdateSprites(WIDTH,HEIGHT,1);


			for(int i = 0; i < 10; i++)
				bag[i].updatesplosion();

			if (player.CollisionEndBlock()) {
				levelCom = true;
				levelCount++;
			}
				//cout<<"Hit an End Block\n";
			render = true;

		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE://plac3e bomb if able
				if (redTime <= 0) {
					redTime = coolDown;
					for (int i = 0; i < 10; i++) {
						if (bag[i].spawn(player.getX() + player.getWidth() / 2, player.getY() + (3 * player.getHeight() / 4)))
							break;
					}
					player.setRed(true);
				}
				keys[SPACE] = true;

			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}
		if(render && al_is_event_queue_empty(event_queue))
		{
			render = false;

			//update the map scroll position
			xOff = player.getX()+player.getWidth() - WIDTH/2 ;
			yOff = player.getY()+player.getHeight()   - HEIGHT/2 ;

			//avoid moving beyond the map edge
			if (xOff < 0) xOff = 0;

			if (xOff > (mapwidth * mapblockwidth - WIDTH))
				xOff = mapwidth * mapblockwidth - WIDTH;
			if (yOff < 0) 
				yOff = 0;
			if (yOff > (mapheight * mapblockheight - HEIGHT)) 
				yOff = mapheight * mapblockheight - HEIGHT;

			//draw the background tiles
			MapDrawBG(xOff,yOff, 0, 0, WIDTH, HEIGHT);

			//draw foreground tiles
			MapDrawFG(xOff,yOff, 0, 0, WIDTH, HEIGHT, 0);
			

			for (int i = 0; i < 10; i++) {//draw the bombs and thier explosions
				if (bag[i].isAlive()) { bag[i].DrawSprites(xOff, yOff); }
				if (bag[i].isExploding()) { bag[i].drawExplostion(xOff, yOff, player); }
			}

			for (int i = 0; i < max; i++) {//draw the not blownup bricks
				if (bricks[i].isAlive()) {
					bricks[i].DrawSprites(xOff,yOff);
				}
			}
			//player health ad sprite
			player.drawHP();
			player.DrawSprites(xOff, yOff);

			
			//finished level
			if (levelCom) {
				al_play_sample(passTune, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				al_stop_timer(timer);
				TotalTime += time;
				levelCom = false;
				al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Level Complete");
				al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2 + 50, ALLEGRO_ALIGN_CENTRE, "TIME: %.2f", time);
				
				al_flip_display();
				al_rest(1);
				
				player.setLoc(64*3,64*2);//always start in the top levft corner of the screen

				if (levelCount == 1) {//level 2
					char next[] = { 'B','E','T','A','.','f','m','p','\0' };
					if (MapLoad(next, 1))
						return -5;
					timeLeft = 90;
				}
				else if (levelCount == 2) {//level 3
					char last[] = { 'C','H','A','R','L','I','E','.','f','m','p','\0' };
					if (MapLoad(last, 1))
						return -5;
					timeLeft = 150;
				}//ADD ELSE IF LEVEL WHATEVER HERE FOR MORE MAPS #MOD_SUPPORT
				else {//otherwise we done
					done = true;
					finished = true;
				}
				al_rest(1);

				for (int i = 0; i < max; i++) {//destroy all bricks on screen
					bricks[i].dead();
				}
				spawnBricks();//spwan a new set of bricks
				time = 0;//set the timer back to zero
				al_start_timer(timer);
			}
			
			//UI
			al_draw_textf(font, al_map_rgb(255, (int)(255-((time/timeLeft)*255)), (int)(255-((time/timeLeft)*255))), 4.3*WIDTH/7, 25, ALLEGRO_ALIGN_LEFT, "Time left: %.2f", timeLeft-time);//time left

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}

	if (fail) {//failure message
		TotalTime += time;
		al_draw_textf(font, al_map_rgb(255, 0, 0), WIDTH/2, HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "FAILURE");
		al_draw_textf(font, al_map_rgb(0, 0, 255), WIDTH / 2, HEIGHT / 2+55, ALLEGRO_ALIGN_CENTRE, "Total Time: %.2f", TotalTime);
		al_play_sample(failTune, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		al_flip_display();
	}
	if (finished) {// we did good message
		al_draw_textf(font, al_map_rgb(0, 0, 255), WIDTH / 2, HEIGHT / 2-55, ALLEGRO_ALIGN_CENTRE, "BomberBot can now launch Nuclear Domination!");
		al_draw_textf(font, al_map_rgb(0, 0, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "Total Time: %.2f", TotalTime);
		al_draw_textf(font, al_map_rgb(0, 0, 255), WIDTH / 2, HEIGHT / 2+55, ALLEGRO_ALIGN_CENTRE, "HP left: %i", player.getHP());
		//al_play_sample(passTune, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		al_flip_display();
	}
	al_rest(8);
	MapFreeMem();
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						//destroy our display object
	al_destroy_sample(sampl);

	return 0;
}


//have we collied with a collision tile
int collided(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	
	return blockdata->tl;
}

//have we collieded with the end tile
bool endValue( int x, int y )
{

	BLKSTR* data;
	data = MapGetBlock( x/mapblockwidth, y/mapblockheight );

	if( data->user1 == 8 )
	{
		return true;
	}else
		return false;
}

//is this x/y coridniate on a wall tile
bool wall(int x, int y)
{

	BLKSTR* data;
	if (y / mapblockheight >= mapheight || x / mapblockwidth >= mapwidth || y/mapblockheight <= 0|| x/mapblockwidth <= 0) {
		return true;
	}

	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	if (data->user1 == 1)
	{
		
		return true;
	}
	else
		return false;
}

//destroy a brick if there is a brick here
void destroyBrick(int x, int y) {
	BLKSTR* data;
	if (y / mapblockheight >= mapheight || x / mapblockwidth >= mapwidth || y / mapblockheight <= 0 || x / mapblockwidth <= 0) {
		return;
	}

	data = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	//cout << x / mapblockwidth << " " << y / mapblockheight << "\n";
	if (data->user1 == 3)
	{
		for (int i = 0; i < max; i++) {
			//cout << (int)(bricks[i].getX() / 64) << " " << (int)(x / 64) << " " << (int)(bricks[i].getY() / 64) << " " << (int)(y / 64) << "\n";
			if ((int)(bricks[i].getX() / 64) == (int)(x / 64) && (int)(bricks[i].getY() / 64) == (int)(y / 64)) {
				bricks[i].dead();
				return;
			}
		}
	}
}

//load in the bricks for this map
void spawnBricks()
{
	//cout << "Running...\n";
	int counter = 0;
	BLKSTR* data;
	for (int y = 0; y < mapheight; y++) {
		for (int x = 0; x<mapwidth;x++){
			data = MapGetBlock(x, y);
			//cout << x << " " << y << " "<< data->user1 << "\n";
			if (data->user1 == 3)
			{
				bricks[counter].spawn(x * 64, y * 64);
				counter++;
			}
		}
	}
	max = counter+1;
}

//has the player collied with a brick?
bool brickHit(int x, int y) {
	for (int i = 0; i < max; i++) {
		if (bricks[i].playerColide(x,y)) {
			return true;
		}
	}
	return false;
}