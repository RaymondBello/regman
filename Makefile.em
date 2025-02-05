
CC = emcc
CXX = em++

BUILD_DIR = build-em
EXE = $(BUILD_DIR)/index.html

SOURCE_DIR = src
OPENGL_SRC = $(SOURCE_DIR)/OpenGL
SCENE_SRC = $(SOURCE_DIR)/Scene

ASSETS_DIR = assets

IMGUI_DIR  = lib/imgui
IMPLOT_DIR = lib/implot
GLM_DIR    = lib/glm
ENTT_DIR   = lib/entt

# Create build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR))

# Source Files
SOURCES =  $(SOURCE_DIR)/main.cpp $(SOURCE_DIR)/app.cpp
SOURCES += $(OPENGL_SRC)/opengl_buffer.cpp
SOURCES += $(OPENGL_SRC)/opengl_shader.cpp
# Scene Sources
SOURCES += $(SCENE_SRC)/camera.cpp $(SCENE_SRC)/editor.cpp
SOURCES += $(SCENE_SRC)/entity.cpp $(SCENE_SRC)/scene.cpp
# ImGui Files
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl2.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
# ImPlot Files
SOURCES += $(IMPLOT_DIR)/implot.cpp $(IMPLOT_DIR)/implot_items.cpp $(IMPLOT_DIR)/implot_demo.cpp

# OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.cpp=.o)))

UNAME_S := $(shell uname -s)
CPPFLAGS = 
LDFLAGS = 
EMS =

##---------------------------------------------------------------------
## EMSCRIPTEN OPTIONS
##---------------------------------------------------------------------

EMS += -sUSE_SDL=2 
EMS += -s DISABLE_EXCEPTION_CATCHING=1
LDFLAGS += -s WASM=1  -s ALLOW_MEMORY_GROWTH=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1 -s MIN_WEBGL_VERSION=2
# LDFLAGS += -s EXPORTED_FUNCTIONS=_malloc

# Build as single file (binary text encoded in .html file)
# LDFLAGS += -sSINGLE_FILE

# Uncomment next line to fix possible rendering bugs with Emscripten version older then 1.39.0 (https://github.com/ocornut/imgui/issues/2877)
#EMS += -s BINARYEN_TRAP_MODE=clamp
#EMS += -s SAFE_HEAP=1    ## Adds overhead

# The Makefile for this example project suggests embedding the misc/fonts/ folder into our application, it will then be accessible as "/fonts"
# See documentation for more details: https://emscripten.org/docs/porting/files/packaging_files.html
# (Default value is 0. Set to 1 to enable file-system and include the misc/fonts/ folder as part of the build.)
USE_FILE_SYSTEM ?= 1
ifeq ($(USE_FILE_SYSTEM), 0)
LDFLAGS += -s NO_FILESYSTEM=1
CPPFLAGS += -DIMGUI_DISABLE_FILE_FUNCTIONS
endif
ifeq ($(USE_FILE_SYSTEM), 1)
LDFLAGS += --no-heap-copy 
LDFLAGS += --preload-file $(ASSETS_DIR)
endif

##---------------------------------------------------------------------
## FINAL BUILD FLAGS
##---------------------------------------------------------------------

CPPFLAGS += -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(IMGUI_DIR)/examples/libs/emscripten
CXXFLAGS += -I$(IMPLOT_DIR)
CXXFLAGS += -I$(GLM_DIR)
CXXFLAGS += -I$(OPENGL_SRC) -I$(SCENE_SRC)
CXXFLAGS += `pkg-config --cflags entt`
CXXFLAGS += `pkg-config --cflags assimp`

#CPPFLAGS += -g
## Ignore unused parameters
#CPPFLAGS += -Wno-unused-parameter
## Show all warnings
CPPFLAGS += -Wall 
CPPFLAGS += -Wformat -Os $(EMS)
LDFLAGS += --shell-file $(IMGUI_DIR)/examples/libs/emscripten/shell_minimal.html
LDFLAGS += $(EMS)

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(OPENGL_SRC)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SCENE_SRC)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(IMPLOT_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(EXE)

$(BUILD_DIR):
	mkdir $@

serve: all
	python3 -m http.server -d $(BUILD_DIR)

rerun: clean
	make -f Makefile.em serve -j8

$(EXE): $(OBJS) $(BUILD_DIR)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -rf $(OBJS) $(BUILD_DIR)
