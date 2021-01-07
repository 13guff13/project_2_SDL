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

void reset_impediments(SDL_Rect* impediments, int amount) {
  for (int i =0; i < amount; i++) {
    impediments[i].x = orig_impediments_x[i];
  }
}

void move_impediments(SDL_Renderer * renderer, SDL_Rect* impediments, int amount) {
  for (int i =0; i < amount; i++) {
    //todo: add width to x position
    //todo: add repeat impediments method
    if(impediments[i].x + impediments[i].w < 0) {
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

void logSDLError(std::ostream &os, const std::string &msg){
  os << msg << " error: " << SDL_GetError() << std::endl;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
  float scale = 1;
  int w, h;
  SDL_QueryTexture(tex, NULL, NULL, &w, &h);
  // actual width - w actual height - h
  // printf("renderTexture: (x=%d,y=%d,w=%d,h=%d, scale=%.2f)", x, y, w, h, scale);
  // renderTexture(tex, ren, x, y, (int)w*scale, (int)h*scale);
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = (int)w*scale;
  dst.h = (int)h*scale;
  SDL_RenderCopy(ren, tex, NULL, &dst);
}

SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
  //Open the font
  TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
  if (font == nullptr){
    logSDLError(std::cout, "TTF_OpenFont");
      // std::cout << "[JORA1]" << std::endl;
    return nullptr;
  }	
  //We need to first render to a surface as that's what TTF_RenderText
  //returns, then load that surface into a texture
  SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
  if (surf == nullptr){
    TTF_CloseFont(font);
    logSDLError(std::cout, "TTF_RenderText");
    // std::cout << "[JORA2]" << std::endl;    
    return nullptr;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
  if (texture == nullptr){
    // std::cout << "[JORA3]" << std::endl;    
    logSDLError(std::cout, "CreateTexture");
    // return nullptr;    
  }
  //Clean up the surface and font
  SDL_FreeSurface(surf);
  TTF_CloseFont(font);
  return texture;
}

void view_foreground_text(SDL_Renderer *renderer, SDL_Window *window, const char* label)
{
  // SDL_Color color = { 255, 255, 255, 255 };
  SDL_Color color = { 30, 30, 30, 255 };  
  SDL_Texture *text = renderText(label, "./res/linux-libertine/LinLibertine_aBS.ttf",
				  color, 64, renderer);

  if (text == nullptr){
    // cleanup(renderer, window);
    std::cout << "[JORA]" << std::endl;    
    TTF_Quit();
    SDL_Quit();
    return ;
  }
  //Get the texture w/h so we can center it in the screen
  int iW, iH;
  SDL_QueryTexture(text, NULL, NULL, &iW, &iH);

  int x = SCREEN_WIDTH / 2 - iW / 2;
  int y = 0;
  
  renderTexture(text, renderer, x, y);  
}
std::string int_to_str(int x) {
   std::stringstream ss;
   ss << x;
   return ss.str();
}

int main(int argc, char ** argv)
{
  // variables

  std::cout << impediments[0].x << "::" << impediments[1].x << std::endl;
  bool quit = false;
  SDL_Event event;
  int x = 10;
  int y = FLOOR - 100;



  // init SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window * window = SDL_CreateWindow("Unicorn su*k",
					 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

  //------ init SDL_TTF lib --------
  if (TTF_Init() != 0){
    logSDLError(std::cout, "TTF_Init");
    SDL_Quit();
    return 1;
  }
  

  SDL_Rect ground = { 0, FLOOR, SCREEN_WIDTH, 70};

  SDL_Rect rect1 = { x, y, 100, 100 };
  SDL_Rect rect2 = { 0, FLOOR - 100, 70, 100 };

  int horizont = SCREEN_WIDTH;
  
  // handle events
  int hope_height = 190;
  float jump_position = 0.0;
  float Pi = 3.14;
  int j_y = y;

  // .c_str()  
  // concat_str = my_str + int_to_str(x);
  while (!quit)
    {
      SDL_Delay(30);
      SDL_PollEvent(&event);
      
      horizont += GAME_SPEED;
      // rect2.x -= GAME_SPEED;
      std::string label = "elapsed time: " + int_to_str(horizont);

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
	  std::cout << "keycode: " << event.key.keysym.sym << std::endl;
	  if(event.key.keysym.sym == SDLK_ESCAPE) quit = true;
	  if(event.key.keysym.sym == 110){
	    reset_impediments(impediments, IMPEDIMENTS_AMOUNT);
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

      
      /* SDL_RenderFillRect(renderer, &rect2); */

      view_foreground_text(renderer, window, label.c_str());

      SDL_RenderPresent(renderer);
    }

  // cleanup SDL

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
