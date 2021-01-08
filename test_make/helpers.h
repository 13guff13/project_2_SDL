#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <string>
#include <sstream>
#include <SDL.h>

std::string int_to_str(int x) {
  std::stringstream ss;
  ss << x;
  return ss.str();
}

#endif
