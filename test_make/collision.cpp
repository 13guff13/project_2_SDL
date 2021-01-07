#include <iostream>
#include <SDL.h>
#include <math.h>

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int FLOOR = SCREEN_HEIGHT - 60;
const float GAME_SPEED = 5;
const int LEVEL_LENGTH = 1000;

const int IMPEDIMENTS_AMOUNT = 3;

int orig_impediments_x[30] = {
				   SCREEN_WIDTH + 920,
				   SCREEN_WIDTH + 770,
				   SCREEN_WIDTH + 370,
};



SDL_Rect impediments[30] = {
			    { SCREEN_WIDTH + 920, FLOOR - 50, 80, 50 },
			    { SCREEN_WIDTH + 470, FLOOR - 60, 80, 60 },
			    { SCREEN_WIDTH + 270, FLOOR - 60, 80, 60 },
};

// from 0 to 3.14

void move_impediments(SDL_Renderer * renderer, SDL_Rect* impediments, int amount) {
  for (int i =0; i < amount; i++) {
    //todo: add width to x position
    //todo: add repeat impediments method
    if(impediments[i].x < 0) {
      impediments[i].x = orig_impediments_x[i];
    }
    impediments[i].x -= GAME_SPEED;
  }
}

void rend_impediments(SDL_Renderer * renderer, SDL_Rect* impediments, int amount) {
  for (int i =0; i < amount; i++) {
    SDL_RenderFillRect(renderer, &impediments[i]);
  }
}

int collision_with_impediments(SDL_Renderer* renderer, SDL_Rect* character, SDL_Rect* impediments, int amount) {
  for (int i =0; i < amount; i++) {
    if (SDL_HasIntersection(character, &impediments[i]) == true) {
      return 1;
    }
  }
  return 0;
}


int main(int argc, char ** argv)
{
  // variables

  std::cout << impediments[0].x << "::" << impediments[1].x << std::endl;
  bool quit = false;
  SDL_Event event;
  int x = 288;
  int y = FLOOR - 100;



  // init SDL

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window * window = SDL_CreateWindow("SDL2 Bounding Box Collision Detection",
					 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_Rect ground = { 0, FLOOR, SCREEN_WIDTH, 70};

  SDL_Rect rect1 = { x, y, 100, 100 };
  SDL_Rect rect2 = { 0, FLOOR - 100, 70, 100 };

  int horizont = SCREEN_WIDTH;
  
  // handle events
  int hope_height = 190;
  float jump_position = 0.0;
  float Pi = 3.14;
  int j_y = y;
  while (!quit)
    {
      SDL_Delay(30);
      SDL_PollEvent(&event);
      
      horizont += GAME_SPEED;
      // rect2.x -= GAME_SPEED;
      
      move_impediments(renderer, impediments, IMPEDIMENTS_AMOUNT);

      // jump
      if (rect1.y < y) {
      	jump_position += 0.07;
      	rect1.y = y - sin(jump_position) * hope_height;
      }
      if (rect1.y > y) {
	jump_position = 0;
	rect1.y = y;//todo: remove this for Nick!
      }
      std::cout << "jump" << jump_position << " " << rect1.y<< " " << y << std::endl;
      
      switch (event.type)
        {
	case SDL_QUIT:
	  quit = true;
	  break;
	case SDL_KEYDOWN:
	  if(event.key.keysym.sym == SDLK_ESCAPE) quit = true;
	  switch (event.key.keysym.sym)
	    {
	    case SDLK_LEFT:  rect1.x--; break;
	    case SDLK_RIGHT: rect1.x++; break;
	    case SDLK_UP:    rect1.y--; break;
	    case SDLK_DOWN:  rect1.y++; break;
	    }
	  break;
        }

      SDL_bool collision = SDL_HasIntersection(&rect1, &rect2);
      int imp_collision = collision_with_impediments(renderer, &rect1, impediments, IMPEDIMENTS_AMOUNT);

      SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
      SDL_RenderClear(renderer);
      
      if (collision)
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      else
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

      if (imp_collision == 1) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      }
      else {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      }

      SDL_RenderFillRect(renderer, &rect1);
      SDL_RenderFillRect(renderer, &ground);
      rend_impediments(renderer, impediments, IMPEDIMENTS_AMOUNT);

      if (collision)
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      else
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

      
      SDL_RenderFillRect(renderer, &rect2);

      SDL_RenderPresent(renderer);
    }

  // cleanup SDL

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
