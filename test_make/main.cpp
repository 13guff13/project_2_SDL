#include <iostream>
#include <string>
#include <SDL.h>
#include "res_path.h"

int main(int argc, char **argv){
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
    std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
    return 1;
  }
  std::cout << "Resource path is: " << getResourcePath() << std::endl;
  std::cout << "Resource path for some is: " << getResourcePath("some/shit") << std::endl;

  SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
  if (win == nullptr){
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr){
    SDL_DestroyWindow(win);
    std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  // ------adding bmp---------
  std::string imagePath = getResourcePath() + "cs8x8.bmp";
  SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
  if (bmp == nullptr){
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
  SDL_FreeSurface(bmp);
  if (tex == nullptr){
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }
  // ---- draw the texture -------
  //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
  for (int i = 0; i < 7; ++i){
    //First clear the renderer
    SDL_RenderClear(ren);
    //Draw the texture      
    SDL_RenderCopy(ren, tex, NULL, NULL);
    //Update the screen
    SDL_RenderPresent(ren);
    //Take a quick break after all that hard work
    SDL_Delay(1000);
  }

  
  // todo: make clean up on each stage that can encounter an erro!
  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
