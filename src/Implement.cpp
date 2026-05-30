#include "Environment.hpp"
#include <iostream>

bool Init(){
	if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)){
		std::cout<<"SDL subsystem init failed, check .dll file\n";
		return false;
	}
	win = SDL_CreateWindow("Test", WIN_W, WIN_H, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
	SDL_SetWindowRelativeMouseMode(win, true);
	gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, NULL);
	SDL_ClaimWindowForGPUDevice(gpu, win);
	SDL_SetGPUSwapchainParameters(gpu,win,SDL_GPU_SWAPCHAINCOMPOSITION_SDR,SDL_GPU_PRESENTMODE_VSYNC);
	return true;
}

SDL_GPUShader* LoadShader(const char* path, SDL_GPUShaderStage stage,Uint32 samplerCount,Uint32 uniformBufferCount,Uint32 storageBufferCount,Uint32 storageTextureCount){
    // Load SPIR-V file
    size_t codeSize = 0;
    void* code = SDL_LoadFile(path, &codeSize);
	
    if (!code){
        SDL_Log("Failed to load shader: %s", path);
        return NULL;
    }

    // Fill create info
    SDL_GPUShaderCreateInfo info = {
		.code_size = codeSize,
		.code = (Uint8*)code,
		.entrypoint = "main",
		.format = SDL_GPU_SHADERFORMAT_SPIRV,
		.stage = stage,

		.num_samplers = samplerCount,
		.num_storage_textures = storageTextureCount,
		.num_storage_buffers = storageBufferCount,
		.num_uniform_buffers = uniformBufferCount,

		.props = 0,
	};
    // Create shader
    SDL_GPUShader* shader = SDL_CreateGPUShader(gpu, &info);
	SDL_free(code);
	
    if (!shader) std::cout<<"Failed to create GPU shader: "<<path<<"\n";

    return shader;
}
//Loading SDL_Surface
SDL_Surface* LoadSurface(const char* path){
	SDL_Surface *surf;
	surf = SDL_LoadPNG(path);
	if(!surf){
		std::cout<<"Failed to load image: "<<path<<"\n";
		return NULL;
	}
	if(surf->format != SDL_PIXELFORMAT_ABGR8888){
		SDL_Surface *alt = SDL_ConvertSurface(surf, SDL_PIXELFORMAT_ABGR8888);
		SDL_DestroySurface(surf);
		surf = alt;
	}
	return surf;
}