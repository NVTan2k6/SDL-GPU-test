//g++ *.cpp -Wno-narrowing -I"D:/C++ libraries/SDL3/include" -L"D:/C++ libraries/SDL3/lib" -lmingw32 -lSDL3 -o test.exe
#include <algorithm>
#include <iostream>
#include "Environment.hpp"
#include "Math.hpp"
#include "Structs.hpp"
using namespace std;
std::ostream& operator<<(std::ostream& os, const mat4& a) {
    os <<a.m11<<" "<<a.m12<<" "<<a.m13<<" "<<a.m14<<"\n"<<a.m21<<" "<<a.m22<<" "<<a.m23<<" "<<a.m24<<"\n"<<a.m31<<" "<<a.m32<<" "<<a.m33<<" "<<a.m34<<"\n"<<a.m41<<" "<<a.m42<<" "<<a.m43<<" "<<a.m44<<"\n";
    return os;
}
float mdx, mdy;
float yaw=0, pitch=0;
vec3 campos = {0, 0, 0};
float msen=0.01;
PositionTextureVertex vertices[] = {
    // +X (Right)
	{ 1,-1,-1,  0.5, float(2)/3},
    { 1, 1,-1,  0.5, float(1)/3},
    { 1, 1, 1,  0.75, float(1)/3},
    { 1,-1, 1,  0.75, float(2)/3},

    // -X (Left)		
	{ -1,-1, 1, 0, float(2)/3},
    { -1, 1, 1, 0, float(1)/3},
    { -1, 1,-1, 0.25, float(1)/3},
    { -1,-1,-1, 0.25, float(2)/3},

    // +Y (Top)
	{ -1, 1,-1, 0.5, 0},
    { -1, 1, 1, 0.5, float(1)/3},
    {  1, 1, 1, 0.25, float(1)/3},
    {  1, 1,-1, 0.25, 0},

    // -Y (Bottom)
	{ -1,-1, 1, 0.5, float(2)/3},
    { -1,-1,-1, 0.5, 1},
    {  1,-1,-1, 0.25, 1},
    {  1,-1, 1, 0.25, float(2)/3},
	
	  // +Z (Front)
    { -1,-1, 1, 0.5, float(2)/3},
    {  1,-1, 1, 0.25, float(2)/3},
    {  1, 1, 1, 0.25, float(1)/3},
    { -1, 1, 1, 0.5, float(1)/3},

    // -Z (Back)
    {  1,-1,-1, 1, float(2)/3},
    { -1,-1,-1, 0.75, float(2)/3},
    { -1, 1,-1, 0.75, float(1)/3},
    {  1, 1,-1, 1, float(1)/3}
};
Uint16 indices[] = {

    // +X
    0,1,2,  0,2,3,

    // -X
    4,5,6,  4,6,7,

    // +Y
    8,9,10, 8,10,11,

    // -Y
    12,13,14, 12,14,15,

    // +Z
    16,17,18, 16,18,19,

    // -Z
    20,21,22, 20,22,23
};
//Model is Rotate*Translate
Solid_instance_data instances[] = {
	{Rotation(0, 0, 0)*Translate( 0, 0,-5)},
	{Rotation(0, 0, 0)*Translate( 0, 0, 5)},
	{Rotation(0, 0, 0)*Translate( 5, 0, 0)},
	{Rotation(0, 0, 0)*Translate(-5, 0, 0)},
	{Rotation(0, 0, 0)*Translate( 5, 0, 5)},
	{Rotation(0, 0, 0)*Translate( 5, 0,-5)},
	{Rotation(0, 0, 0)*Translate(-5, 0, 5)},
	{Rotation(0, 0, 0)*Translate(-5, 0,-5)},
	{Rotation(0, 0, 0)*Translate( 0, 5,-5)},
	{Rotation(0, 0, 0)*Translate( 0, 5, 5)},
	{Rotation(0, 0, 0)*Translate( 5, 5, 0)},
	{Rotation(0, 0, 0)*Translate(-5, 5, 0)},
	{Rotation(0, 0, 0)*Translate( 5, 5, 5)},
	{Rotation(0, 0, 0)*Translate( 5, 5,-5)},
	{Rotation(0, 0, 0)*Translate(-5, 5, 5)},
	{Rotation(0, 0, 0)*Translate(-5, 5,-5)},
	{Rotation(0, 0, 0)*Translate( 0, 5, 0)},
	{Rotation(0, 0, 0)*Translate( 0,-5,-5)},
	{Rotation(0, 0, 0)*Translate( 0,-5, 5)},
	{Rotation(0, 0, 0)*Translate( 5,-5, 0)},
	{Rotation(0, 0, 0)*Translate(-5,-5, 0)},
	{Rotation(0, 0, 0)*Translate( 5,-5, 5)},
	{Rotation(0, 0, 0)*Translate( 5,-5,-5)},
	{Rotation(0, 0, 0)*Translate(-5,-5, 5)},
	{Rotation(0, 0, 0)*Translate(-5,-5,-5)},
	{Rotation(0, 0, 0)*Translate( 0,-5, 0)}
};

int main(int argc, char* argv[]){
	Init();	
	//-----------------------------------------------------------------Depth texture-------------------------------------------------------
	SDL_GPUTextureCreateInfo depthcreateinfo = {
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_D24_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
		.width = WIN_W,
		.height = WIN_H,
		.layer_count_or_depth = 1,
		.num_levels = 1};	
	SDL_GPUTexture* DepthTexture=SDL_CreateGPUTexture(gpu, &depthcreateinfo);
	//--------------------------------------------------------------Texture test--------------------------------------------------------------
	SDL_Surface* surf = LoadSurface("Assets/Textures/cube-a.png");
	SDL_GPUTextureCreateInfo texcreateinfo = {
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
		.width = surf->w,
		.height = surf->h,
		.layer_count_or_depth = 1,
		.num_levels = 1};
	SDL_GPUTexture* texture = SDL_CreateGPUTexture(gpu, &texcreateinfo);
	//-----------------------------------Sampler, nearest and clamp to edge, suitable for pixel art on quad------------------------------------------
	SDL_GPUSamplerCreateInfo samplercreateinfo = {
		.min_filter = SDL_GPU_FILTER_NEAREST,
		.mag_filter = SDL_GPU_FILTER_NEAREST,
		.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
		.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
		.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
	};
	SDL_GPUSampler* texSampler = SDL_CreateGPUSampler(gpu, &samplercreateinfo);
	
	//--------------------------------------------------------------------------Create pipeline----------------------------------------------------------------
	// Vertex buffers description
	SDL_GPUVertexBufferDescription VertBuffDesc[2]{};
		//Vertex buffer
		VertBuffDesc[0].slot = 0;
		VertBuffDesc[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
		VertBuffDesc[0].instance_step_rate = 0;
		VertBuffDesc[0].pitch = sizeof(PositionTextureVertex);
		//Instance buffer
		VertBuffDesc[1].slot = 1;
		VertBuffDesc[1].input_rate = SDL_GPU_VERTEXINPUTRATE_INSTANCE;
		VertBuffDesc[1].instance_step_rate = 0;
		VertBuffDesc[1].pitch = sizeof(Solid_instance_data);
		
	//Vertex attributes
	SDL_GPUVertexAttribute VertAttr[6]{};
		//PositionTextureVertex
			//Posistion: 3 floats
			VertAttr[0].location = 0;
			VertAttr[0].buffer_slot = 0;
			VertAttr[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
			VertAttr[0].offset = 0;
			//UV coord: 2 floats
			VertAttr[1].location = 1;
			VertAttr[1].buffer_slot = 0;
			VertAttr[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
			VertAttr[1].offset = sizeof(float)*3;
		//Solid_instance_data
			// Instance matrix column 0
			VertAttr[2].location    = 2;
			VertAttr[2].buffer_slot = 1;
			VertAttr[2].format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
			VertAttr[2].offset      = 0;
			// Column 1
			VertAttr[3].location    = 3;
			VertAttr[3].buffer_slot = 1;
			VertAttr[3].format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
			VertAttr[3].offset      = sizeof(float) * 4;
			// Column 2
			VertAttr[4].location    = 4;			
			VertAttr[4].buffer_slot = 1;
			VertAttr[4].format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
			VertAttr[4].offset      = sizeof(float) * 8;
			// Column 3
			VertAttr[5].location    = 5;
			VertAttr[5].buffer_slot = 1;
			VertAttr[5].format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
			VertAttr[5].offset      = sizeof(float) * 12;
			

	// Vertex input state
	SDL_GPUVertexInputState vertexInputState = {
		.vertex_buffer_descriptions = VertBuffDesc,
		.num_vertex_buffers = 2,
		.vertex_attributes = VertAttr,
		.num_vertex_attributes = sizeof(VertAttr)/sizeof(SDL_GPUVertexAttribute)};

	// Color target	
	SDL_GPUColorTargetDescription color_target_desc = {};
		color_target_desc.format = SDL_GetGPUSwapchainTextureFormat(gpu, win);
		
		color_target_desc.blend_state.enable_blend = true;
		color_target_desc.blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
		color_target_desc.blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
		color_target_desc.blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
		color_target_desc.blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
		color_target_desc.blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
		color_target_desc.blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;

	// Pipeline info
	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.target_info.num_color_targets = 1;
		pipelineCreateInfo.target_info.color_target_descriptions = &color_target_desc;
		
		pipelineCreateInfo.target_info.has_depth_stencil_target=true; 
		pipelineCreateInfo.target_info.depth_stencil_format=SDL_GPU_TEXTUREFORMAT_D24_UNORM;
		
		pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;//SDL_GPU_FILLMODE_LINE for wireframe, SDL_GPU_FILLMODE_FILL for raster
		pipelineCreateInfo.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_BACK;//SDL_GPU_CULLMODE_BACK for culling
		pipelineCreateInfo.rasterizer_state.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;

		pipelineCreateInfo.vertex_input_state = vertexInputState;
		pipelineCreateInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
		
		pipelineCreateInfo.depth_stencil_state = (SDL_GPUDepthStencilState){
			.compare_op = SDL_GPU_COMPAREOP_LESS,
			.enable_depth_test = true,
			.enable_depth_write = false};

		pipelineCreateInfo.vertex_shader = LoadShader("Shaders/PositionTextureVertex.vert.spv",SDL_GPU_SHADERSTAGE_VERTEX, 0, 1);
		pipelineCreateInfo.fragment_shader = LoadShader("Shaders/Texture.frag.spv",SDL_GPU_SHADERSTAGE_FRAGMENT, 1, 0);

	// Create pipeline
	SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(gpu, &pipelineCreateInfo);

	if (!pipeline)
	{
		SDL_Log("Pipeline failed: %s", SDL_GetError());
		return -1;
	}
	SDL_ReleaseGPUShader(gpu, pipelineCreateInfo.vertex_shader);
	SDL_ReleaseGPUShader(gpu, pipelineCreateInfo.fragment_shader);
	//----------------------------------------------------------------Start creating buffer--------------------------------------------------------
	//Vertex buffer
	SDL_GPUBufferCreateInfo verbufinfo = {
		.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
		.size  = sizeof(vertices)};
	SDL_GPUBuffer* VertexBuffer = SDL_CreateGPUBuffer(gpu, &verbufinfo);
	//Index buffer
	SDL_GPUBufferCreateInfo indbufinfo{
        .usage = SDL_GPU_BUFFERUSAGE_INDEX,
        .size = sizeof(indices)};
	SDL_GPUBuffer* IndexBuffer = SDL_CreateGPUBuffer(gpu, &indbufinfo);
	//Instance buffer
	SDL_GPUBufferCreateInfo insbufinfo{
		.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
		.size = sizeof(instances)};
	SDL_GPUBuffer* InstanceBuffer = SDL_CreateGPUBuffer(gpu, &insbufinfo);

	//Vertex transfer buffer
	SDL_GPUTransferBufferCreateInfo vertTransferInfo{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size  = sizeof(vertices)+sizeof(indices)+sizeof(instances)};	
	SDL_GPUTransferBuffer* vtbuf = SDL_CreateGPUTransferBuffer(gpu, &vertTransferInfo);
	
	void* vptr = SDL_MapGPUTransferBuffer(gpu, vtbuf, false);
	memcpy(vptr, vertices, sizeof(vertices));
	memcpy((char*)vptr + sizeof(vertices), indices, sizeof(indices));
	memcpy((char*)vptr + sizeof(vertices) + sizeof(indices), instances, sizeof(instances));

	SDL_UnmapGPUTransferBuffer(gpu, vtbuf);
	
	//Texture transfer buffer
	SDL_GPUTransferBufferCreateInfo texTransferInfo{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = surf->w * surf->h * 4};
	SDL_GPUTransferBuffer* ttbuf = SDL_CreateGPUTransferBuffer(gpu, &texTransferInfo);
	
	void* tptr = SDL_MapGPUTransferBuffer(gpu, ttbuf, false);
	memcpy(tptr, surf->pixels, surf->w * surf->h * 4);
	SDL_UnmapGPUTransferBuffer(gpu, ttbuf);
	
	//Upload transfer buffer
	SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(gpu); 
	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);
	//Upload vertex buffer
	SDL_GPUTransferBufferLocation versrc{
		.transfer_buffer = vtbuf,
		.offset = 0};
	SDL_GPUBufferRegion verdst{
		.buffer = VertexBuffer,
		.offset = 0,
		.size   = sizeof(vertices)};
	SDL_UploadToGPUBuffer(copyPass,&versrc,&verdst,false);
	//Upload index buffer
	SDL_GPUTransferBufferLocation indsrc{
		.transfer_buffer = vtbuf,
		.offset = sizeof(vertices)};
	SDL_GPUBufferRegion inddst{
		.buffer = IndexBuffer,
		.offset = 0,
		.size   = sizeof(indices)};
	SDL_UploadToGPUBuffer(copyPass,&indsrc,&inddst,false);
	//Upload instance buffer
	SDL_GPUTransferBufferLocation inssrc{
		.transfer_buffer = vtbuf,
		.offset = sizeof(vertices)+sizeof(indices)};
	SDL_GPUBufferRegion insdst{
		.buffer = InstanceBuffer,
		.offset = 0,
		.size   = sizeof(instances)};
	SDL_UploadToGPUBuffer(copyPass,&inssrc,&insdst,false);
	//Upload texture
	SDL_GPUTextureTransferInfo texsrc{
		.transfer_buffer = ttbuf,
		.offset = 0};
	SDL_GPUTextureRegion texdst{
		.texture = texture,
		.w = surf->w,
		.h = surf->h,
		.d = 1};
	SDL_UploadToGPUTexture(copyPass,&texsrc,&texdst,false);
	//Release system RAM side temp buffer
	SDL_EndGPUCopyPass(copyPass);
	SDL_SubmitGPUCommandBuffer(uploadCmdBuf);
	SDL_ReleaseGPUTransferBuffer(gpu, vtbuf);
	SDL_ReleaseGPUTransferBuffer(gpu, ttbuf);
	SDL_DestroySurface(surf);
	
	while (running) {
		while (SDL_PollEvent(&e)){
			switch(e.type){
				case SDL_EVENT_QUIT:
					running=false;
					break;
				case SDL_EVENT_WINDOW_RESIZED:
					SDL_ReleaseGPUTexture(gpu, DepthTexture);
					SDL_GetWindowSizeInPixels(win, &WIN_W, &WIN_H);
					depthcreateinfo.width=WIN_W;
					depthcreateinfo.height=WIN_H;
					DepthTexture=SDL_CreateGPUTexture(gpu, &depthcreateinfo);
					break;
				case SDL_EVENT_KEY_DOWN:
					if(e.key.scancode==SDL_SCANCODE_W){
						campos=campos+(vec3){cos(pitch)*sin(yaw), sin(-pitch), -cos(pitch)*cos(yaw)};
					}
					else if(e.key.scancode==SDL_SCANCODE_S){
						campos=campos-(vec3){cos(pitch)*sin(yaw), sin(-pitch), -cos(pitch)*cos(yaw)};
					}
					
					if(e.key.scancode==SDL_SCANCODE_A){
						campos=campos-(vec3){cos(yaw), 0, sin(yaw)};
					}
					else if(e.key.scancode==SDL_SCANCODE_D){
						campos=campos+(vec3){cos(yaw), 0, sin(yaw)};
					}
					break;
				default:
					break;
			}
		}
		SDL_GetRelativeMouseState(&mdx, &mdy);
		yaw+=mdx*msen;
		if(yaw<0) yaw+=2*pi;
		else if (yaw>2*pi) yaw-=2*pi;
		
		pitch=clamp(pitch+mdy*msen, -pi/2+0.01f, pi/2-0.01f);
		
		//Get GPU command buffer, which store command to be sent to the GPU for execution
		SDL_GPUCommandBuffer *cmdbuf= SDL_AcquireGPUCommandBuffer(gpu);

		SDL_GPUTexture *swapchainTexture;
		SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, win, &swapchainTexture, NULL, NULL);
		if(swapchainTexture){
			SDL_GPUColorTargetInfo colorTargetInfo = {0};
			colorTargetInfo.texture = swapchainTexture;
			colorTargetInfo.clear_color = (SDL_FColor){ (float)50/255, (float)50/255, (float)50/255, 1};
			colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
			colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
			
			SDL_GPUDepthStencilTargetInfo depthInfo = {0};
			depthInfo.texture = DepthTexture;
			depthInfo.clear_depth = 1.0f;
			depthInfo.load_op = SDL_GPU_LOADOP_CLEAR;
			depthInfo.store_op = SDL_GPU_STOREOP_DONT_CARE;
			
			SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, &depthInfo);

			// Bind pipeline
			SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
			//Vertex
			SDL_GPUBufferBinding verbufbind{
				.buffer = VertexBuffer, 
				.offset = 0 };
			SDL_BindGPUVertexBuffers(renderPass,0,&verbufbind, 1);
			//Instance
			SDL_GPUBufferBinding insbufbind{
				.buffer = InstanceBuffer, 
				.offset = 0};
			SDL_BindGPUVertexBuffers(renderPass,1,&insbufbind, 1);
			//Index
			SDL_GPUBufferBinding ibufbind{
				.buffer = IndexBuffer, 
				.offset = 0};
			SDL_BindGPUIndexBuffer(renderPass,&ibufbind,SDL_GPU_INDEXELEMENTSIZE_16BIT);
			//Fragment sampler
			SDL_GPUTextureSamplerBinding texsambind{ 
				.texture = texture, 
				.sampler = texSampler};
			SDL_BindGPUFragmentSamplers(renderPass, 0, &texsambind, 1);
			
			//Vertex Uniform
			mat4 view = View(campos, yaw, pitch);
			mat4 proj = OnePointPerspective(70.0f * 3.14159f/180.0f, (float)WIN_W/WIN_H, 0.1f, 600.0f);
			
			Vertex_uniform vu = {view*proj};
			SDL_PushGPUVertexUniformData(cmdbuf, 0, &vu, sizeof(Vertex_uniform));

			// Draw call
			SDL_DrawGPUIndexedPrimitives(renderPass,sizeof(indices)/sizeof(Uint16),sizeof(instances)/sizeof(Solid_instance_data),0,0,0);

			SDL_EndGPURenderPass(renderPass);
		}

		SDL_SubmitGPUCommandBuffer(cmdbuf);
	}
	for(int i = 0; i < 26; i++) std::cout<<instances[i].id<<"\n";
	SDL_ReleaseGPUGraphicsPipeline(gpu, pipeline);
	SDL_ReleaseGPUBuffer(gpu, VertexBuffer);
	SDL_ReleaseGPUBuffer(gpu, InstanceBuffer);
	SDL_ReleaseGPUBuffer(gpu, IndexBuffer);
	SDL_ReleaseGPUTexture(gpu, DepthTexture);
	SDL_ReleaseGPUTexture(gpu, texture);
	SDL_ReleaseGPUSampler(gpu, texSampler);
	SDL_ReleaseWindowFromGPUDevice(gpu, win);
	SDL_DestroyWindow(win);
	SDL_DestroyGPUDevice(gpu);
	return 0;
}
