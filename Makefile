
CXX = g++

BUILD_DIR = build
EXE = regman

SOURCE_DIR = src
OPENGL_SRC = $(SOURCE_DIR)/OpenGL
SCENE_SRC = $(SOURCE_DIR)/Scene

ASSETS_DIR = assets

IMGUI_DIR  = lib/imgui
IMPLOT_DIR = lib/implot
GLM_DIR    = lib/glm
ENTT_DIR   = lib/entt
SDL_DIR    = /remote/us01home59/bello/bin/bin

# Create build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR))

# Source Files
SOURCES = $(SOURCE_DIR)/main.cpp $(SOURCE_DIR)/app.cpp
# OpenGL API
SOURCES += $(OPENGL_SRC)/opengl_buffer.cpp
SOURCES += $(OPENGL_SRC)/opengl_shader.cpp
# Scene Sources
SOURCES += $(SCENE_SRC)/camera.cpp 
SOURCES += $(SCENE_SRC)/entity.cpp $(SCENE_SRC)/scene.cpp
# ImGui Files
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl2.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
# ImPlot Files
SOURCES += $(IMPLOT_DIR)/implot.cpp $(IMPLOT_DIR)/implot_items.cpp $(IMPLOT_DIR)/implot_demo.cpp
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.cpp=.o)))
UNAME_S = $(shell uname -s)

CXXFLAGS = -std=c++17
CXXFLAGS += -g -Wall -Wformat
CXXFLAGS += -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(IMGUI_DIR)/examples/libs/emscripten
CXXFLAGS += -I$(IMPLOT_DIR)
CXXFLAGS += -I$(GLM_DIR)
CXXFLAGS += `pkg-config --cflags entt`
CXXFLAGS += `pkg-config --cflags assimp`

# Source Includes
CXXFLAGS += -I$(OPENGL_SRC) -I$(SCENE_SRC)

LIBS = `pkg-config --libs assimp`

# Build flags per platform
ifeq ($(UNAME_S), Linux)
    ECHO_MESSAGE = "Linux"
	LIBS += -lGL -ldl `sdl2-config --libs`
    CXXFLAGS += `sdl2-config --cflags`
    # LIBS += -lGL -ldl `$$(SDL_DIR)/sdl2-config --libs`
    # CXXFLAGS += `$$(SDL_DIR)/sdl2-config --cflags`
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib

	CXXFLAGS += `sdl2-config --cflags`
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

	CXXFLAGS += `pkg-config --cflags sdl2`
	CFLAGS = $(CXXFLAGS)
endif

# Build rules
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(OPENGL_SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SCENE_SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(IMPLOT_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(BUILD_DIR)/$(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

run: all
	./$(BUILD_DIR)/$(EXE)

rerun: clean
	make run -j8
	
$(BUILD_DIR)/$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -rf $(BUILD_DIR)
