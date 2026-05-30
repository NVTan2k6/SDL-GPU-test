//glslc TextureAlphaDiscard.frag -o TextureAlphaDiscard.frag.spv
#version 450
//------------------------------------------------------------------Input------------------------------------------------------------------
layout(location = 0) in vec2 fragUV;

layout(set = 2, binding = 0) uniform sampler2D tex;

//------------------------------------------------------------------Output-----------------------------------------------------------------
layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(tex, fragUV); 
	if(outColor.w<0.5){
		discard;
	}
}