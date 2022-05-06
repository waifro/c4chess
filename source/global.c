// third-party libraries
#include <stdio.h>
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

// functions https://www.learn-c.org/en/Linked_lists
void *GLOBAL_HookArray_Init(uintptr_t **array) {
    array = malloc(sizeof(char*) * 1);
    int size = 0; *array[0] = size;
    return (array);
}

size_t GLOBAL_HookArray_Size(void **array) {
    size_t size = *(int*)array[0];
    return (size);
}

size_t GLOBAL_HookArray_Reference(void **array, void *ptr) {
    size_t size = *(int*)GLOBAL_HookArray_Size(array);
    if (size == (size_t)-1) return -1;

    size += 1;
    array = realloc(array, size);

    array[size] = ptr;

    array[0] += (int)1;

    //printf("sizeof array: %d = %p\n", (int)*array[0], array[size] );

    return (size);
}

size_t GLOBAL_HookArray_Dereference(uintptr_t **array) {
    size_t size = GLOBAL_HookArray_Size(array);
    if (size == (size_t)-1) return -1;

    array[size] = NULL;
    size -= 1;
    array = realloc(array, size);
    *array[0] -= 1;

    return (size);
}


