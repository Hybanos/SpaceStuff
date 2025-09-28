# SpaceStuff

SpaceStuff is a work-in-progress visualization of various solar system objects. It is written in raw c++, openGL and GLSL. This project has a few goals: play around with the solar system dynamics, improve my c++ and learn a bit of grapical programming.

Currently, the app includes:
 - Accurate major body position
 - Major body rotation (currently broken, the reference frame is not the same as body position)
 - Earth artificial satelite data
 - Gas giant rings

![look how cool it looks!](https://github.com/Hybanos/SpaceStuff/blob/master/assets/github/readme_earth.png)

# Data and sofware used

### Body position
Major body position and physical caracteristics (mass, radius...) are gathered from [NASA's horizon system](https://ssd.jpl.nasa.gov/horizons/)

### Body rotation
Body rotations are from the IAU Working Group:
 - [2009 report](https://apps.dtic.mil/sti/tr/pdf/ADA538254.pdf) (earth only) 
 - [2015 report](https://www.usgs.gov/publications/report-iau-working-group-cartographic-coordinates-and-rotational-elements-2015)
 - [2015 report correction](https://link.springer.com/content/pdf/10.1007/s10569-019-9925-1.pdf)

### Gas giant rings
Everything is form [wikipedia](https://en.wikipedia.org/wiki/Ring_system)

### Satelite orbits (TLEs)
Big thanks to [Celestrak.org](https://celestrak.org/) for providing such an easy to use api.

## Libs
 - [opengl](https://www.opengl.org/)
 - [glm](https://github.com/g-truc/glm): easy opengl math
 - [imgui](https://github.com/ocornut/imgui): amazing imidiate-mode GUI
 - [SDL](https://github.com/libsdl-org/SDL): windows
 - [glew](https://github.com/nigels-com/glew): opengl glue
 - [fmt](https://github.com/fmtlib/fmt ): easier printing and string management
 - [cpr](https://github.com/libcpr/cpr.git): c++ curl wrapper for api requests
 - [nlohmann-json](https://github.com/nlohmann/json.git): amazing json lib
 - [gtest](https://github.com/google/googletest.git): tests
 - [nothings.org](https://nothings.org/stb): stb_image.h for loading images
 - [sqlite](https://sqlite.org/): i think you know this one


![look how cool it looks!](https://github.com/Hybanos/SpaceStuff/blob/master/assets/github/readme_saturn.png)

# TODO list / future ideas
 - fix cmake (building doesn't work unless a couple packages are installed on the system)
 - compute major body position from horizon's orbital elements rather than vector tables
 - render Ephemeris component 
 - fix major body orientation relative to xz plane
 - better management of "global objects" (textures, meshes, scenes, ecs tables...)
 - render scene to texture => enable multi-views
 - "async-like" texture/data loading
 - elite dangerous like grid (maybe even with entity snapping)
 - render atmospheres / shadows
