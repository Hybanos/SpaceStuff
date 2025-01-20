HEADERS = -Iimgui -Iimgui/backends  -I. -Isrc
LIBS =  -lGL `pkg-config sdl3  --libs` -lGLEW -lm
CXXFLAGS = `pkg-config sdl3 --cflags` -g -DGLM_ENABLE_EXPERIMENTAL
IMGUI_DIR = imgui

PROG_SOURCES = src/main.cpp \
	src/scene/camera.cpp src/scene/scene.cpp \
	src/objects/object_mesh.cpp src/objects/object_faces.cpp src/objects/object_cubemap.cpp \
	src/objects/sphere.cpp src/objects/skybox.cpp
LIBS_SOURCES = $(IMGUI_DIR)/imgui.o $(IMGUI_DIR)/imgui_demo.o $(IMGUI_DIR)/imgui_draw.o $(IMGUI_DIR)/imgui_tables.o $(IMGUI_DIR)/imgui_widgets.o
LIBS_SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl3.o $(IMGUI_DIR)/backends/imgui_impl_opengl3.o

all: build run

build: libs
	g++ $(PROG_SOURCES) $(LIBS_SOURCES) -o out $(HEADERS) $(CXXFLAGS) $(LIBS)

libs: $(LIBS_SOURCES)

%.o: %.cpp
	g++ $< -c $(HEADERS) $(CXXFLAGS) $(LIBS) -o $@

run:
	./out

clean:
	rm out imgui.ini *.o $(LIBS_SOURCES)