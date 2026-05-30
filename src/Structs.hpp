#pragma once

#include "Math.hpp"

struct PositionTextureVertex{
    float x, y, z;
    float u, v;
};
struct Solid_instance_data{
	mat4 id;
};
struct Vertex_uniform{
	mat4 vp;
};
