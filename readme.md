# SpaceStuff

SpaceStuff is a work-in-progress visualisation of various solar system objects. It is written in raw c++, openGL and GLSL. This project has a few goals: play around with the solar system dynamics, improve my c++ and learn a bit of grapical programming.

Currently, the app includes:
 - Accurate major body position
 - Major body rotation (currently broken, the reference frame is not the same as body position)
 - Earth artificial satelite data
 - Gas giant rings

# body rotations
 - 2009 report: (earth only, not accurate) https://apps.dtic.mil/sti/tr/pdf/ADA538254.pdf
 - 2015 report: https://www.usgs.gov/publications/report-iau-working-group-cartographic-coordinates-and-rotational-elements-2015
 - 2015 report correction: https://link.springer.com/content/pdf/10.1007/s10569-019-9925-1.pdf

# TODO list / future ideas
 - compute major body position from horizon's orbital elements rather than vector tables
 - render Ephemeris component 
 - fix major body orientation relative to xz plane
 - better management of "global objects" (textures, meshes, scenes, ecs tables...)
 - render scene to texture => enable multi-views
 - "async-like" texture/data loading
 - elite dangerous like grid (maybe even with entity snapping)
 - render atmospheres / shadows 