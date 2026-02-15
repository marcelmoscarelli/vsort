CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra

SDL_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL_LIBS := $(filter-out -mwindows,$(shell pkg-config --libs sdl2))

IMGUI_DIR := imgui
IMGUI_BACKENDS := $(IMGUI_DIR)/backends

IMGUI_SRC := \
	$(IMGUI_DIR)/imgui.cpp \
	$(IMGUI_DIR)/imgui_demo.cpp \
	$(IMGUI_DIR)/imgui_draw.cpp \
	$(IMGUI_DIR)/imgui_tables.cpp \
	$(IMGUI_DIR)/imgui_widgets.cpp \
	$(IMGUI_BACKENDS)/imgui_impl_sdl2.cpp \
	$(IMGUI_BACKENDS)/imgui_impl_sdlrenderer2.cpp

SRC := vsort.cpp $(IMGUI_SRC)
OBJ := $(SRC:.cpp=.o)

BIN_DIR := bin
TARGET := $(BIN_DIR)/vsort

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

run-dgpu: $(TARGET)
	__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./$(TARGET)

$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SDL_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) -I$(IMGUI_DIR) -I$(IMGUI_BACKENDS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

.PHONY: all clean run run-dgpu
