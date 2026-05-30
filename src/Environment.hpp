#pragma once

#include <SDL3/SDL.h>

inline int WIN_W=1600, WIN_H=1000;
inline bool running=true;

inline SDL_Window* win;
inline SDL_GPUDevice* gpu;
inline SDL_Event e;

extern bool Init();

extern SDL_GPUShader* LoadShader(const char* path, SDL_GPUShaderStage stage,Uint32 samplerCount = 0,Uint32 uniformBufferCount = 0,Uint32 storageBufferCount = 0,Uint32 storageTextureCount = 0);
extern SDL_Surface* LoadSurface(const char* path);