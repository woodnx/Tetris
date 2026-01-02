CC := g++
DXLIBDIR=dxlib
INCDIR=include
LIBDIR=lib

SRC_DIR = src
OBJ_DIR = obj
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

default: $(OBJ_DIR) game

$(OBJ_DIR):
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"

game: $(OBJS)
	$(CC) -std=c++17 -g -O0 -o game.exe $(OBJS) \
		-L $(DXLIBDIR) \
		-lDxLib\
		-lDxUseCLib\
		-lDxDrawFunc\
		-ljpeg\
		-lpng\
		-lzlib\
		-ltiff\
		-ltheora_static\
		-lvorbis_static\
		-lvorbisfile_static\
		-logg_static\
		-lbulletdynamics\
		-lbulletcollision\
		-lbulletmath\
		-lopusfile\
		-lopus\
		-lsilk_common\
		-lcelt\
		-mwindows

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -g -O0 -c $< -o $@ -I $(DXLIBDIR)

.PHONY: clean
clean:
	@if exist "$(OBJ_DIR)" rmdir /S /Q "$(OBJ_DIR)"
	@if exist game.exe del /F /Q game.exe
	@if exist game del /F /Q game