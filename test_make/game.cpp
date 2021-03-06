#include "./SDL2_image-2.0.5/SDL2_image-2.0.5/SDL_image.h"
#include "./SDL2_ttf-2.0.15/SDL2_ttf-2.0.15/SDL_ttf.h"
#include <SDL.h>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include "res_path.h"
#include "cleanup.h"
#include "helpers.h"

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
// const float GAME_SPEED = 7;
const float SCALE_TILE = 0.2;
const int TILE_WIDTH = (int) 148 * SCALE_TILE;
const int TILE_HEIGHT = (int) 125 * SCALE_TILE;
const float SCALE_CARACTER = 0.2;
const int STEP = 3;

const int FLOOR = SCREEN_HEIGHT - 60;
const float GAME_SPEED = 7;
const int LEVEL_LENGTH = 1000;

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderClippedTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr);
SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer);

// impediments --------------------------------------------------------------------------

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
  std::cout << "impediments were reseted !!!!" << std::endl;
}
void move_impediments(SDL_Renderer * renderer, SDL_Rect* impediments, int amount) {
  if(impediments[0].x + impediments[0].w < 0) {
    reset_impediments(impediments, amount);
  }
  for (int i =0; i < amount; i++) {
    impediments[i].x -= GAME_SPEED;
  }
}
void rend_impediments(SDL_Renderer * renderer, SDL_Rect* impediments, int amount) {
  for (int i =0; i < amount; i++) {
    SDL_RenderFillRect(renderer, &impediments[i]);
  }
  //inf: render ground
  SDL_Rect ground = { 0, FLOOR, SCREEN_WIDTH, 70};
  SDL_RenderFillRect(renderer, &ground);
}
int collision_with_impediments(SDL_Renderer* renderer, SDL_Rect* character, SDL_Rect* impediments, int amount) {
  for (int i =0; i < amount; i++) {
    if (SDL_HasIntersection(character, &impediments[i]) == true) {
      return 1;
    }
  }
  return 0;
}
// impediments end -----------------------------------------------------------------------------------------

/**
 * Log an SDL error
 */
void logSDLError(std::ostream &os, const std::string &msg){
  os << msg << " error: " << SDL_GetError() << std::endl;
}

/*
 * @\return An SDL_Texture containing the rendered message, or nullptr if something went wrong
 */
SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
  //Open the font
  TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
  if (font == nullptr){
    logSDLError(std::cout, "TTF_OpenFont");
    return nullptr;
  }	
  SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
  if (surf == nullptr){
    TTF_CloseFont(font);
    logSDLError(std::cout, "TTF_RenderText");
    return nullptr;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
  if (texture == nullptr){
    logSDLError(std::cout, "CreateTexture");
  }
  //Clean up the surface and font
  SDL_FreeSurface(surf);
  TTF_CloseFont(font);
  return texture;
}

void move_obj_position(int* x, int* y, int key_code)
{
  switch(key_code) {
    //arrow up
  case 82:
    *y -= STEP;
    break;
    //arrow down
  case 81:
    *y += STEP;
    break;
    //arrow left
  case 80:
    *x -= STEP;
    break;
    //arrow right
  case 79:
    *x += STEP;
    break;
  }
}
void view_foreground_text(SDL_Renderer *renderer, SDL_Window *window, const char* label = "jora")
{
  const std::string resPath = getResourcePath("linux-libertine");
  // std::cout << "path: " << resPath << std::endl;
  // return ;
  SDL_Color color = { 255, 255, 255, 255 };
  SDL_Texture *text = renderText(label, resPath + "LinLibertine_aBS.ttf",
				 color, 64, renderer);
  if (text == nullptr){
    cleanup(renderer, window);
    TTF_Quit();
    SDL_Quit();
    return ;
  }
  //Get the texture w/h so we can adjust its position
  int iW, iH;
  SDL_QueryTexture(text, NULL, NULL, &iW, &iH);
  int x = SCREEN_WIDTH / 2 - iW / 2;
  int y = 0;
  
  renderTexture(text, renderer, x, y);  
}

void view_foreground(SDL_Texture* image, SDL_Renderer *renderer, int* x, int* y)
{
  // rendred at give position if (passed(x,y))
  if (*x == NULL) {
    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
  }

  SDL_Rect clip;
  clip.x = 0;
  clip.y = 0;
  clip.w = 0;
  clip.h = 0;

  // SDL_RenderCopy(renderer, image, NULL, NULL);
    SDL_Rect DestR;

  SrcR.x = 0;
  SrcR.y = 0;
  SrcR.w = SHAPE_SIZE;
  SrcR.h = SHAPE_SIZE;

  DestR.x = 640 / 2 - SHAPE_SIZE / 2;
  DestR.y = 580 / 2 - SHAPE_SIZE / 2;
  DestR.w = SHAPE_SIZE;
  DestR.h = SHAPE_SIZE;

  renderClippedTexture(image, renderer, *x, *y, &clip);
  // renderTexture(image, renderer, *x, *y);
}

void view_background(SDL_Texture* background, SDL_Renderer *renderer)
{
  //Determine how many tiles we'll need to fill the screen
  int xTiles = SCREEN_WIDTH / TILE_WIDTH;
  int yTiles = SCREEN_HEIGHT / TILE_HEIGHT;

  //Draw the tiles by calculating their positions
  for (int i = 0; i < xTiles * yTiles; ++i){
    int x = i % xTiles;
    int y = i / xTiles;
    renderTexture(background, renderer, x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
  }
}

/**
 * Loads an image into a texture on the rendering device
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
  SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
  if (texture == nullptr){
    logSDLError(std::cout, "LoadTexture");
  }
  return texture;
}

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
  //Setup the destination rectangle to be at the position we want
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopy(ren, tex, NULL, &dst);
}
/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
  float scale = 1;
  int w, h;
  SDL_QueryTexture(tex, NULL, NULL, &w, &h);
  renderTexture(tex, ren, x, y, (int)w*scale, (int)h*scale);
}
/**
 * Draw an SDL_Texture to an SDL_Renderer
 */
void renderClippedTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr)
{

  float scale = .6;
  int w, h;
  SDL_QueryTexture(tex, NULL, NULL, &w, &h);
  int clipw = 160,cliph = 160;
  
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = (int)clipw*scale;
  dst.h = (int)cliph*scale;

  SDL_Rect clip2;
  clip2.x = 0;
  clip2.y = 0;
  clip2.w = clipw;
  clip2.h = cliph;
  
  SDL_RenderCopy(ren, tex, &clip2, &dst);
}

int main(int argc, char **argv){
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
    logSDLError(std::cout, "SDL_Init");
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Lesson 2", 100, 100, SCREEN_WIDTH,
					SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == nullptr){
    logSDLError(std::cout, "CreateWindow");
    SDL_Quit();
    return 1;
  }
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
					      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr){
    logSDLError(std::cout, "CreateRenderer");
    cleanup(window);
    SDL_Quit();
    return 1;
  }

  //------ init SDL_IMG lib --------
  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
    logSDLError(std::cout, "IMG_Init");
    SDL_Quit();
    return 1;
  }

  //------ init SDL_TTF lib --------
  if (TTF_Init() != 0){
    logSDLError(std::cout, "TTF_Init");
    SDL_Quit();
    return 1;
  }

  // ------adding bmp---------
  const std::string resPath = getResourcePath();
  SDL_Texture *background = loadTexture(resPath + "mario-block.png", renderer);
  SDL_Texture *image = loadTexture(resPath + "mario_8bit-small.png", renderer);
  if (background == nullptr || image == nullptr){
    cleanup(background, image, renderer, window);    
    IMG_Quit();
    SDL_Quit();
    return 1;
  }
  // character default position
  int orig_x = 0;
  int orig_y = FLOOR - 100;
  SDL_Rect character = { orig_x, orig_y, 100, 100 };

  int progress = 0;
  int hope_height = 190;
  float jump_position = 0.0;
  
  // ---- draw the texture -------
  //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
  int x = NULL, y = NULL;
  SDL_Event e;
  bool quit = false;
  while (!quit){
    while (SDL_PollEvent(&e)){
      if (e.type == SDL_QUIT){
	quit = true;
      }
      if (e.type == SDL_KEYDOWN){
	int keycode = e.key.keysym.scancode;
	std::cout << "key pressed down = " << keycode << std::endl;
	if (keycode == 41) {
	  quit = true;
	}
	move_obj_position(&x, &y, keycode);
	if(e.key.keysym.sym == 110){
	  reset_impediments(impediments, IMPEDIMENTS_AMOUNT);
	  progress = 0;
	  x = orig_x;
	  y = orig_y;
	}

      }
      if (e.type == SDL_MOUSEBUTTONDOWN){
	quit = false;
      }
    }

    // trash start ---------------------------------------------------------------
    
    progress += GAME_SPEED;
    std::string label = "elapsed score: " + int_to_str(progress);

    // jump
    if (y < orig_y) {
      jump_position += 0.07;
      y = orig_y - sin(jump_position) * hope_height;
    }
    if (y > orig_y) {
      jump_position = 0;
      y = orig_y;
    }

    // trash end ---------------------------------------------------------------
    
    //Render the scene
    SDL_RenderClear(renderer);

    view_background(background, renderer);
    view_foreground(image, renderer, &x, &y);
    view_foreground_text(renderer, window, label.c_str());

    character.x = x;
    character.y = y;
    int collision = collision_with_impediments(renderer, &character, impediments, IMPEDIMENTS_AMOUNT);
    if (collision)
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    else
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    move_impediments(renderer, impediments, IMPEDIMENTS_AMOUNT);
    rend_impediments(renderer, impediments, IMPEDIMENTS_AMOUNT);
      
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
  }
  
  // todo: make clean up on each stage that can encounter an erro!
  cleanup(background, image, renderer, window);
  IMG_Quit();
  SDL_Quit();
  return 0;
}
