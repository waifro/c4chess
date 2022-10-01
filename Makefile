# Makefile

# uncomment this line if your on windows
LIBMMAN := "-L. -lmman" -lws2_32 # -mwindows

# uncomment this line if you want to sanitize
SANITIZE := -O0 -g # -fsanitize=address #-static-libasan
COMPILER := gcc #x86_64-w64-mingw32-gcc # decide what compiler to use

dir_sources := source

ifeq ($(OS), Windows_NT)
	subpath := $(wildcard $(dir_sources) $(foreach var, $(dir_sources), $(shell dir /b /ad /S $(var))))
else
	subpath := $(wildcard $(patsubst %:, %, $(shell ls -R $(dir_sources) | grep :)))
endif

c_files_buf := $(foreach var, $(subpath), $(wildcard $(var)/*.c))

empty :=
foo := /
bar := $(empty)\$(empty)

ifeq ($(OS), Windows_NT)
	c_files := $(foreach var, $(c_files_buf), $(wildcard $(patsubst $(foo), $(bar), $(var))))
else
	c_files := $(c_files_buf)
endif

objects := $(patsubst %.c, %.o, $(wildcard $(c_files)))

ODIR := output
dummy_build_folder := $(shell mkdir $(ODIR))
OUTPUT := $(ODIR)/$(notdir $(CURDIR))

LIBS := -lSDL2 -lSDL2_ttf -lSDL2_image

all: echoing compile
	@echo done

echoing:
	@echo ##################################
	@echo system: $(OS)
	@echo bar: $(bar)
	@echo foo: $(foo)
	@echo subpath: $(subpath)
	@echo c_files: $(c_files)
	@echo objects: $(objects)
	@echo ##################################

compile : $(objects)
	$(COMPILER) $(objects) $(sdl2-config --cflags) $(LIBMMAN) -Wall -Wextra $(SANITIZE) $(LIBS) -o $(OUTPUT)

.PHONY : clean

clean:
	rm $(objects) $(output)
	@echo done
