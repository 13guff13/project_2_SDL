CXX = g++
# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install
SDL_IMG_LIB = /usr/lib/x86_64-linux-gnu/libSDL2_image-2.0.so.0 #I don't know why this shit didn't work from standart lib dir fuck all morons who write the manual
SDL_LIB = -L./ -L/usr/lib/x86_64-linux-gnu $(SDL_IMG_LIB) -lm -lSDL2 -lpthread -ldl -lrt -Wl,-rpath=./SDL2-2.0.10
# -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I./SDL2-2.0.10/include
# You may need to change -std=c++11 to -std=c++0x if your compiler is a bit older
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
EXE = SDL_Lesson0

all: $(EXE)

$(EXE): main.o
	$(CXX) $< $(LDFLAGS) -o $@

main.o: main.cpp
	echo "[LOG]$(CXX) $(CXXFLAGS) $< -o $@"
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
