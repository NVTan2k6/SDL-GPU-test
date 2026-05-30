//glslc Texture.frag -o Texture.frag.spv
#version 450
//------------------------------------------------------------------Input------------------------------------------------------------------
layout(location = 0) in vec2 fragUV;

layout(set = 2, binding = 0) uniform sampler2D tex;
layout(set = 3, binding = 0) uniform FragmentUniform{
    vec4 lightDir;
};
//------------------------------------------------------------------Output-----------------------------------------------------------------
layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(tex, fragUV); 
}