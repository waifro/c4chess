Scacchi: source/main.c source/game.c source/pp4m/pp4m.c source/pp4m/pp4m_io.c source/pp4m/pp4m_draw.c source/pp4m/pp4m_image.c source/pp4m/pp4m_ttf.c source/pp4m/pp4m_net.c
	gcc -Wall -o scacchi source/main.c source/game.c source/pp4m/pp4m.c source/pp4m/pp4m_io.c source/pp4m/pp4m_draw.c source/pp4m/pp4m_image.c source/pp4m/pp4m_ttf.c source/pp4m/pp4m_net.c -I. -lSDL2 -lSDL2_image -lSDL2_ttf
