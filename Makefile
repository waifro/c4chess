# Makefile

OBJS := source/main.c source/game.c source/core.c source/input.c source/debug.c source/fs_management.c source/logic.c source/pp4m/pp4m.c source/pp4m/pp4m_io.c source/pp4m/pp4m_draw.c source/pp4m/pp4m_image.c source/pp4m/pp4m_ttf.c source/pp4m/pp4m_net.c

OUTPUT := $(notdir $(CURDIR))

LIBS := -lSDL2 -lSDL2_ttf -lSDL2_image

all : $(OBJS)
	gcc $(OBJS) $(sdl2-config --cflags) -Wall -Wextra $(LIBS) -o $(OUTPUT)

clean :
	rm *.o $(OUTPUT)
