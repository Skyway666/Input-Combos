#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

ModuleTextures::ModuleTextures() : Module()
{
	int i = 0;
	while (i != 50)
	{
		textures[i] = nullptr;
		++i;
	}

}

// Destructor
ModuleTextures::~ModuleTextures()
{}

// Called before render is available
bool ModuleTextures::Init()
{
	LOG("Init Image library");
	bool ret = true;

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}
	
	return ret;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");
	int i = 0;
	while (i != 50)
	{
		SDL_DestroyTexture(textures[i]);
		++i;
	}
	
	// TODO 6: Free all textures

	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const ModuleTextures::Load(const char* path)
{
	SDL_Surface * surface = IMG_Load(path);
	if (surface == NULL)
	{
		LOG("An error has ocurred when opening the image: %s", SDL_GetError())
			return NULL;
	}
	// TODO 2: Load and image from a path (must be a png)
	// and check for errors

	textures[last_texture] = SDL_CreateTextureFromSurface(App->render->renderer, surface);
	if (textures[last_texture] == NULL)
	{
		LOG("An error has ocurred when createing the texture: %s", SDL_GetError())
			return NULL;
	}

	SDL_FreeSurface(surface);
	return textures[0];
}
bool ModuleTextures::Unload(SDL_Texture* textu)
{
	bool ret = false;

	for (uint i = 0; i < MAX_TEXTURES; ++i)
	{
		if (textu == textures[i])
		{
			SDL_DestroyTexture(textures[i]);
			textures[i] = nullptr;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleTextures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)&width, (int*)&height);
}