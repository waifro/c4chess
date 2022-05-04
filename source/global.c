// third-party libraries
#include <stdlib.h>
#include <SDL2/SDL.h>

// local libraries
#include "global.h"
#include "pp4m/pp4m.h"

// declared global variables
SDL_Window *glo_window = NULL;
SDL_Renderer *glo_render = NULL;

int glo_screen_w = 1280;
int glo_screen_h = 720;

// functions
size_t GLOBAL_HookArray_Size(uintptr_t **array) {
    for (int n = 0; n < BUF_MAX; n++)
        if (array[n] == NULL) return (n);
    return -1;
}

size_t GLOBAL_HookArray_Reference(uintptr_t **array, uintptr_t *ptr) {
    size_t index = GLOBAL_HookArray_Size(array);
    if (index == -1) return -1;

    array = realloc(array, index+1);
    array[index] = ptr;

    return (index);
}

size_t GLOBAL_HookArray_Dereference(uintptr_t **array) {
    size_t index = GLOBAL_HookArray_Size(array);
    if (index == -1) return -1;

    array[index-1] = NULL;
    array = realloc(array, index-1); // truncates size of array

    return (index-1);
}

uintptr_t **GLOBAL_HookArray_Init(void) {
    uintptr_t **array = malloc(sizeof(void*) * 1);
    array[0] = NULL;
    return (array);
}
