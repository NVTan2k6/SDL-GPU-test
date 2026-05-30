//glslc PositionTextureVertex.vert -o PositionTextureVertex.vert.spv
#version 450
//------------------------------------------------------------------Input------------------------------------------------------------------
//Vertex
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUV;

// Instance matrix
layout(location = 2) in vec4 iM0;
layout(location = 3) in vec4 iM1;
layout(location = 4) in vec4 iM2;
layout(location = 5) in vec4 iM3;


layout(set = 1, binding = 0) uniform VertexUniform{
    mat4 ProjView;
};


//-----------------------------------------------------------------Output-----------------------------------------------------------------
layout(location = 0) out vec2 fragUV;


void main(){
    mat4 Model = mat4(iM0, iM1, iM2, iM3);
	fragUV = inUV;
    gl_Position = ProjView * Model * vec4(inPos, 1.0);
}