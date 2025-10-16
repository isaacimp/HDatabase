#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    SDL_Texture *text_texture = NULL;
    TTF_Font *font = NULL;

    bool running = true;

    SDL_SetAppMetadata("Life in Data", "1.0", "https://isaacmp.xyz");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't init SDL: %s", SDL_GetError());
        return 1;
    }

    if (!SDL_CreateWindowAndRenderer("Life in Data", 1920, 1080, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    /* Get actual render output size and calculate DPI scale */
    int render_width, render_height;
    int window_width, window_height;
    SDL_GetRenderOutputSize(renderer, &render_width, &render_height);
    SDL_GetWindowSize(window, &window_width, &window_height);
    float dpi_scale = (float)render_width / (float)window_width;
    SDL_Log("Window: %dx%d, Render: %dx%d, DPI Scale: %.2f",
            window_width, window_height, render_width, render_height, dpi_scale);
/*
    surface = SDL_LoadBMP("sample.bmp");
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_Log("Couldn't create texture from surface: %s", SDL_GetError());
        }
        SDL_DestroySurface(surface);
    } else {
        SDL_Log("Note: sample.bmp not found, continuing without it");
    }
*/
    if (!TTF_Init()) {
        SDL_Log("Couldn't init SDL_ttf: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    /* Scale font size based on DPI (18pt base size) */
    int base_font_size = 18;
    int scaled_font_size = (int)(base_font_size * dpi_scale);
    font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", scaled_font_size);
    SDL_Log("Using font size: %d (base: %d, scale: %.2f)", scaled_font_size, base_font_size, dpi_scale);
    if (!font) {
        SDL_Log("Couldn't load font: %s", SDL_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    /* Render text to texture */
    SDL_Color black = {0, 0, 0, 255};
    const char *text = "Welcome Isaac, to your Life in Data!";
    surface = TTF_RenderText_Blended(font, text, 0, black);
    if (surface) {
        text_texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);
        if (!text_texture) {
            SDL_Log("Couldn't create text texture: %s", SDL_GetError());
        }
    }

    /*
     * initialize http client librarys for api fetchs
     * Initial data fetch and load
     */

    /* Main loop */
    while (running) {
        SDL_Event event;

        /* Handle events */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            /*
             * Mouse clicks
             * Mouse hover
             * Keyboard Shortcuts
             * Window resizing
             */
        }

        /* Update and render */
        SDL_SetRenderDrawColorFloat(renderer, 1.0f, 1.0f, 1.0f, 1.0f);  /* White background */
        SDL_RenderClear(renderer);

        if (texture) {
            SDL_RenderTexture(renderer, texture, NULL, NULL);
        }

        /* Render text at top left */
        if (text_texture) {
            float text_width, text_height;
            SDL_GetTextureSize(text_texture, &text_width, &text_height);
            SDL_FRect text_rect = {
                20,          /* 20 pixels from left */
                20,          /* 20 pixels from top */
                text_width,  /* exact texture width */
                text_height  /* exact texture height */
            };
            SDL_RenderTexture(renderer, text_texture, NULL, &text_rect);
        }

        /*
         * Refresh data
         * update animations
         * render UI
         */

        SDL_RenderPresent(renderer);
    }

    /*
     * Save current
     * close connections
     * free fonts textures etc
     */
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    if (text_texture) {
        SDL_DestroyTexture(text_texture);
    }
    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
