# Makefile

SOURCE_MAIN := 			source/main.c source/global.c
SOURCE_ANIMATION := 	source/animation/animation.c
SOURCE_CHESS := 		source/chess/core.c source/chess/middle.c source/chess/touch.c source/chess/chess.c source/chess/dot.c source/chess/event.c
SOURCE_DASHBOARD := 	source/dashboard/gui.c source/dashboard/gui_alias.c
SOURCE_NETWORK	:=		source/network/net.c source/network/send.c source/network/recv.c
SOURCE_SECURITY	:=		source/security/sec.c source/security/debug.c

SOURCE_PROJECT := $(SOURCE_MAIN) $(SOURCE_ANIMATION) $(SOURCE_CHESS) $(SOURCE_DASHBOARD) $(SOURCE_NETWORK) $(SOURCE_SECURITY)

ODIR := output
dummy_build_folder := $(shell mkdir $(ODIR))
OUTPUT := $(ODIR)/$(notdir $(CURDIR))

DEP_PP4M := source/pp4m/pp4m.c source/pp4m/pp4m_io.c source/pp4m/pp4m_draw.c source/pp4m/pp4m_image.c source/pp4m/pp4m_ttf.c source/pp4m/pp4m_net.c
LIBS := -lSDL2 -lSDL2_ttf -lSDL2_image

all : $(SOURCE_PROJECT) $(DEP_PP4M)
	gcc $(SOURCE_PROJECT) $(DEP_PP4M) $(sdl2-config --cflags) -lws2_32 -Wall -Wextra $(LIBS) -o $(OUTPUT)

clean :
	rm *.o $(OUTPUT)
