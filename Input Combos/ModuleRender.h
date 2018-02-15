#ifndef __ModuleRenderer_H__
#define __ModuleRenderer_H__

#include "Module.h"

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Rect;

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PostUpdate();
	update_status PreUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, float x, float y, SDL_Rect* section = nullptr, int w = 0, int h = 0, int modifier = 0);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
	
	
};

#endif //__ModuleRenderer_H__