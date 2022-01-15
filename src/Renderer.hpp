#pragma once

#include <GL/glew.h>

//------------------------------------------------------------------------

class Renderer;

// Some resources:
// https://www.opengl.org/wiki/Vertex_Specification_Best_Practices
// https://www.youtube.com/watch?v=-bCeNzgiJ8I&t=1920s

// - should maybe keep track of programs, textures, materials
// - data structure holding key-value pairs where key is like, e.g., here:
//   https://realtimecollisiondetection.net/blog/?p=86
// - sort according to the keys before looping through it and making the draw calls
// - or maybe separate queues for different types of objects?

//------------------------------------------------------------------------
