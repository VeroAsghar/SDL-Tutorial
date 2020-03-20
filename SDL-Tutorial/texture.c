//
//  texture.c
//  SDL-Tutorial
//
//  Created by Vero on 18.03.20.
//  Copyright © 2020 Asgard Inc. All rights reserved.
//

#include "texture.h"

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;


Texture* Texture_New() {
    Texture* self = (Texture*) malloc(sizeof(Texture));
    Texture_Init(self);
    return self;
}

void Texture_Init(Texture* self) {
    self->texture = NULL;
    self->width = 0;
    self->height = 0;
}

void Texture_Destroy(Texture* self) {
    if (self != NULL) {
        Texture_DeleteMembers(self);
        free(self);
    }
}

void Texture_DeleteMembers(Texture* self) {
    if (self->texture != NULL) {
        SDL_DestroyTexture(self->texture);
        self->texture = NULL;
        self->width = 0;
        self->height = 0;
    }
}
    
bool Texture_LoadFromFile(Texture* self, const char* path) {
    
    // Get rid of preexisting texture
    Texture_DeleteMembers(self);
    
    // The final texture
    SDL_Texture* new_texture = NULL;
    
    // Load image at specific path
    SDL_Surface* loaded_surface = IMG_Load(path);
    if (loaded_surface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
        
        // Color key image
        SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0x00, 0xFF, 0xFF));
        
        // Create new texture from surface pixels
        new_texture = SDL_CreateTextureFromSurface(gRenderer, loaded_surface);
        if (new_texture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        } else {
            
            // Get image dimensions
            self->width = loaded_surface->w;
            self->height = loaded_surface->h;
        }
        
        // Get rid of old loaded surface
        SDL_FreeSurface(loaded_surface);
    }
    
    // Return success
    self->texture = new_texture;
    return self->texture != NULL;
}

bool Texture_LoadFromRenderedText(Texture* self, const char* texture_text, SDL_Color text_color) {
    
    // Get rid of preexisting texture
    Texture_DeleteMembers(self);
    
    // Render text surface
    SDL_Surface* text_surface = TTF_RenderText_Solid(gFont, texture_text, text_color);
    if (text_surface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        
        // Create texture from surface pixels
        self->texture = SDL_CreateTextureFromSurface(gRenderer, text_surface);
        if (self->texture == NULL) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        } else {
            
            // Get image dimensions
            self->width = text_surface->w;
            self->height = text_surface->h;
        }
        
        // Get rid of old surface
        SDL_FreeSurface(text_surface);
    }
    
    return self->texture != NULL;
}

void Texture_SetColor(Texture* self, Uint8 red, Uint8 green, Uint8 blue) {
    // Modulate texture
    SDL_SetTextureColorMod(self->texture, red, green, blue);
}

void Texture_SetBlendMode(Texture* self, SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(self->texture, blending);
}

void Texture_SetAlpha(Texture* self, Uint8 alpha) {
    SDL_SetTextureAlphaMod(self->texture, alpha);
}

void Texture_Render(Texture* self, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    
    // Set rendering space and render to screen
    SDL_Rect render_quad = {x, y, self->width, self->height};
    
    // Set clip rendering dimensions
    if (clip != NULL) {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }
    
    SDL_RenderCopyEx(gRenderer, self->texture, clip, &render_quad, angle, center, flip);
}

int Texture_GetWidth(Texture* self) {
    return self->width;
}

int Texture_GetHeight(Texture* self) {
    return self->height;
}
    
