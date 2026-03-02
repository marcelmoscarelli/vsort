CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
DEPFLAGS := -MMD -MP

SDL_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL_LIBS := $(shell pkg-config --libs sdl2)

# Only add -mwindows on Windows builds
ifneq ($(shell uname -s),Linux)
SDL_LIBS += -mwindows
endif

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

SRC := vsort.cpp sorting_algo.cpp $(IMGUI_SRC)
BUILD_DIR := build
OBJ := $(addprefix $(BUILD_DIR)/,$(SRC:.cpp=.o))
DEP := $(OBJ:.o=.d)

BIN_DIR := bin
TARGET := $(BIN_DIR)/vsort

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

ifeq ($(shell uname -s),Linux)
run-nvidia: $(TARGET)
	__NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia ./$(TARGET)
else
run-nvidia:
	@echo "run-nvidia is only available on Linux"
endif

$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SDL_LIBS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(SDL_CFLAGS) -I$(IMGUI_DIR) -I$(IMGUI_BACKENDS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

.PHONY: all clean run run-nvidia

-include $(DEP)
