HEADERS = -Iimgui -Iimgui/backends  -I. -Isrc
LIBS =  -lGL `pkg-config sdl3  --libs` -lGLEW
CXXFLAGS = `pkg-config sdl3 --cflags` -g -DGLM_ENABLE_EXPERIMENTAL
IMGUI_DIR = imgui

SOURCES = src/main.cpp src/scene/scene.cpp src/shapes/cube.cpp src/scene/camera.cpp 
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl3.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

all: build run

build:
	g++ $(SOURCES) -o out $(HEADERS) $(CXXFLAGS) $(LIBS)

run:
	./out

clean:
	rm out imgui.ini