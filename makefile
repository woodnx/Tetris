CC := g++
DXLIBDIR=dxlib
INCDIR=include
LIBDIR=lib

SRC_DIR = src
SRCS = $(wildcard src/*.cpp)

default:
	$(CC) -g -O2 -o game -I $(DXLIBDIR) -L $(DXLIBDIR) $(SRCS) \
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