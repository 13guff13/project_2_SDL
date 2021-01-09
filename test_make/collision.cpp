#include <iostream>
#include "./SDL2_ttf-2.0.15/SDL2_ttf-2.0.15/SDL_ttf.h"
#include <iostream>
#include <string>
#include <sstream>
#include <SDL.h>
#include <math.h>

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int FLOOR = SCREEN_HEIGHT - 60;
const float GAME_SPEED = 7;
const int LEVEL_LENGTH = 1000;

const int IMPEDIMENTS_AMOUNT = 3;

int orig_x[30] = {
		  SCREEN_WIDTH + 920,
		  SCREEN_WIDTH + 770,
		  SCREEN_WIDTH + 370,
};

SDL_Rect obstacle[30] = {
			 { SCREEN_WIDTH + 920, FLOOR - 50, 80, 50 },
			 { SCREEN_WIDTH + 470, FLOOR - 60, 80, 60 },
			 { SCREEN_WIDTH + 270, FLOOR - 60, 80, 60 },
};

void reset_obstacle(SDL_Rect* obstacle, int amount) {
  for (int i =0; i < amount; i++) {
    obstacle[i].x = orig_x[i];
  }
}
void move_obstacle(SDL_Renderer * renderer, SDL_Rect* obstacle, int amount) {
  for (int i =0; i < amount; i++) {
    if(obstacle[i].x + obstacle[i].w < 0) {
      obstacle[i].x = orig_x[i];
    }
    obstacle[i].x -= GAME_SPEED;
  }
}
void rend_obstacle(SDL_Renderer * renderer, SDL_Rect* obstacle, int amount) {
  for (int i =0; i < amount; i++) {
    SDL_RenderFillRect(renderer, &obstacle[i]);
  }
}
int collision_with_obstacle(SDL_Renderer* renderer, SDL_Rect* character, SDL_Rect* obstacle, int amount) {
  for (int i =0; i < amount; i++) {
    if (SDL_HasIntersection(character, &obstacle[i]) == true) {
      return 1;
    }
  }
  return 0;
}

void show_texture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
  float scale = 1;
  int w, h;
  SDL_QueryTexture(tex, NULL, NULL, &w, &h);
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = (int)w*scale;
  dst.h = (int)h*scale;
  SDL_RenderCopy(ren, tex, NULL, &dst);
}

SDL_Texture* show_text(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
  //Open the font
  TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
  if (font == nullptr){
    return nullptr;
  }	
  SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
  if (surf == nullptr){
    TTF_CloseFont(font);
    return nullptr;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_FreeSurface(surf);
  TTF_CloseFont(font);
  return texture;
}

void show_label(SDL_Renderer *renderer, SDL_Window *window, const char* label)
{
  SDL_Color color = { 30, 30, 30, 255 };  
  SDL_Texture *text = show_text(label, "./res/linux-libertine/LinLibertine_aBS.ttf",
				 color, 64, renderer);

  if (text == nullptr){
    TTF_Quit();
    SDL_Quit();
    return ;
  }
  int iW, iH;
  SDL_QueryTexture(text, NULL, NULL, &iW, &iH);

  int x = SCREEN_WIDTH / 2 - iW / 2;
  int y = 0;
  
  show_texture(text, renderer, x, y);  
}

void view_foreground(SDL_Texture* image, SDL_Renderer *renderer, int x, int y)
{
  if (x == NULL) {
    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
  }
  SDL_Rect DestR;
  const int SHAPE_W = 70;
  const int SHAPE_H = 100;
    
  DestR.x = x;
  DestR.y = y;
  DestR.w = SHAPE_W;
  DestR.h = SHAPE_H;
  
  SDL_RenderCopy(renderer, image, NULL, &DestR);
}

SDL_Texture* LDT(const std::string &file, SDL_Renderer *ren){
  SDL_Surface* bmp = SDL_LoadBMP(file.c_str());
  if (bmp == nullptr){
    std::cout << "[ERROR SDL] "<< SDL_GetError() << std::endl;
  }
  

  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
  return tex;
}

std::string int_to_str(int x) {
  std::stringstream ss;
  ss << x;
  return ss.str();
}

int main(int argc, char ** argv)
{
  bool quit = false;
  SDL_Event event;
  int x = 40;
  int y = FLOOR - 100;

  // init SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window * window = SDL_CreateWindow("Unicorn attack",
					 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

  //------ init SDL_TTF lib --------
  if (TTF_Init() != 0){
    SDL_Quit();
    return 1;
  }
  
  SDL_Texture *ch_image = LDT("./res/m1.bmp", renderer);
  if (ch_image == nullptr){
    return 0;
  }
  SDL_Rect ground = { 0, FLOOR, SCREEN_WIDTH, 70};

  SDL_Rect rect1 = { x, y, 70, 100 };

  int horizont = SCREEN_WIDTH;
  
  // handle events
  int hope_height = 190;
  float jump_position = 0.0;
  int j_y = y;

  while (!quit)
    {
      SDL_Delay(20);
      SDL_PollEvent(&event);

      horizont += GAME_SPEED;
      std::string label = "score: " + int_to_str(horizont);
      move_obstacle(renderer, obstacle, IMPEDIMENTS_AMOUNT);

      if (rect1.y < y) {
      	jump_position += 0.07;
      	rect1.y = y - sin(jump_position) * hope_height;
      }
      if (rect1.y > y) {
	jump_position = 0;
	rect1.y = y;
      }
      
      switch (event.type)
        {
	case SDL_QUIT:
	  quit = true;
	  break;
	case SDL_KEYDOWN:
	  if(event.key.keysym.sym == SDLK_ESCAPE) quit = true;
	  if(event.key.keysym.sym == 110){
	    reset_obstacle(obstacle, IMPEDIMENTS_AMOUNT);
	    horizont = 0;
	    rect1.x = x;
	    rect1.y = y;
	  }

	  switch (event.key.keysym.sym)
	    {
	    case SDLK_LEFT:  rect1.x--; break;
	    case SDLK_RIGHT: rect1.x++; break;
	    case SDLK_UP:    rect1.y--; break;
	    case SDLK_DOWN:  rect1.y++; break;
	    }
	  break;
        }

      int imp_collision = collision_with_obstacle(renderer, &rect1, obstacle, IMPEDIMENTS_AMOUNT);

      SDL_SetRenderDrawColor(renderer, 42, 2, 142, 255);
      SDL_RenderClear(renderer);
      
      if (imp_collision == 1) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      }
      else {
	SDL_SetRenderDrawColor(renderer, 100, 10, 200, 255);
      }

      SDL_RenderFillRect(renderer, &rect1);;
      SDL_RenderFillRect(renderer, &ground);
      rend_obstacle(renderer, obstacle, IMPEDIMENTS_AMOUNT);

      show_label(renderer, window, label.c_str());
      
      view_foreground(ch_image, renderer, rect1.x, rect1.y);

      SDL_RenderPresent(renderer);
    }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
