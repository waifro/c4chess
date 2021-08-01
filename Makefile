# Makefile

SOURCE_MAIN := 		source/main.c
SOURCE_ANIMATION := source/animation/animation.c
SOURCE_CHESS := 	source/chess/core.c
SOUCE_DASHBOARD := 	source/dashboard/menu.c

SOURCE_PROJECT := $(SOURCE_MAIN) $(SOURCE_ANIMATION) $(SOURCE_CHESS) $(SOUCE_DASHBOARD)

OUTPUT := $(notdir $(CURDIR))

DEP_PP4M := source/pp4m/pp4m.c source/pp4m/pp4m_io.c source/pp4m/pp4m_draw.c source/pp4m/pp4m_image.c source/pp4m/pp4m_ttf.c source/pp4m/pp4m_net.c
LIBS := -lSDL2 -lSDL2_ttf -lSDL2_image

all : $(SOURCE_PROJECT) $(DEP_PP4M)
	gcc $(SOURCE_PROJECT) $(DEP_PP4M) $(sdl2-config --cflags) -Wall -Wextra $(LIBS) -o $(OUTPUT)

clean :
	rm *.o $(OUTPUT)
