# Makefile

# uncomment this line if your on windows
LIBMMAN := "-L. -lmman" -lws2_32 # -mwindows

# uncomment this line if you want to sanitize
SANITIZE := -O0 -g # -fsanitize=address #-static-libasan
COMPILER := gcc #x86_64-w64-mingw32-gcc # decide what compiler to use

SOURCE_MAIN := 		source/main.c source/global.c source/config.c source/mman.c source/lang.c source/gen.c
SOURCE_ANIMATION := 	source/animation/animation.c
SOURCE_CHESS := 	source/chess/core.c source/chess/middle.c source/chess/chess.c source/chess/dot.c source/chess/event.c source/chess/fen.c source/chess/archive.c
SOURCE_DASHBOARD := 	source/dashboard/gui.c source/dashboard/gui_alias.c source/dashboard/menu.c source/dashboard/popup.c source/dashboard/ingame.c source/dashboard/textbox.c source/dashboard/destroy.c
SOURCE_SECURITY	:= 	source/security/sec.c source/security/debug.c

SOURCE_PROJECT := $(SOURCE_MAIN) $(SOURCE_ANIMATION) $(SOURCE_CHESS) $(SOURCE_DASHBOARD) $(SOURCE_SECURITY)

ODIR := output
dummy_build_folder := $(shell mkdir $(ODIR))
OUTPUT := $(ODIR)/$(notdir $(CURDIR))

DEP_PP4M := source/pp4m/pp4m.c source/pp4m/pp4m_io.c source/pp4m/pp4m_draw.c source/pp4m/pp4m_image.c source/pp4m/pp4m_ttf.c source/pp4m/pp4m_net.c source/pp4m/pp4m_input.c
DEP_C4NETWORK := source/c4network/net.c source/c4network/net_utils.c source/c4network/send.c source/c4network/recv.c source/c4network/client.c source/c4network/server.c source/c4network/lobby.c

LIBS := -lSDL2 -lSDL2_ttf -lSDL2_image

Release : $(SOURCE_PROJECT) $(DEP_PP4M) $(DEP_C4NETWORK)
	$(COMPILER) $(SOURCE_PROJECT) $(DEP_PP4M) $(DEP_C4NETWORK) $(sdl2-config --cflags) $(LIBMMAN) -Wall -Wextra $(SANITIZE) $(LIBS) -o $(OUTPUT)

all : $(SOURCE_PROJECT) $(DEP_PP4M) $(DEP_C4NETWORK)
	$(COMPILER) $(SOURCE_PROJECT) $(DEP_PP4M) $(DEP_C4NETWORK) $(sdl2-config --cflags) $(LIBMMAN) -Wmsvc-not-found -Wall -Wextra $(SANITIZE) $(LIBS) -o $(OUTPUT)

clean :
	rm *.o $(OUTPUT)
