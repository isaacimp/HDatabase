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

    SDL_SetAppMetadata("Health Dashboard", "1.0", "https://isaacmp.xyz");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't init SDL: %s", SDL_GetError());
        return 1;
    }

    if (!SDL_CreateWindowAndRenderer("Health Dashboard", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

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

    if (!TTF_Init()) {
        SDL_Log("Couldn't init SDL_ttf: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 48);
    if (!font) {
        SDL_Log("Couldn't load font: %s", SDL_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    /* Render text to texture */
    SDL_Color white = {255, 255, 255, 255};
    const char *text = "Health Dashboard";
    surface = TTF_RenderText_Solid(font, text, 0, white);
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
        const double now = ((double)SDL_GetTicks()) / 1000.0;
        const float red = (float) (0.5 + 0.5 * SDL_sin(now));
        const float green = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
        const float blue = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
        SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);

        SDL_RenderClear(renderer);

        if (texture) {
            SDL_RenderTexture(renderer, texture, NULL, NULL);
        }

        /* Render text centered at top */
        if (text_texture) {
            float text_width, text_height;
            SDL_GetTextureSize(text_texture, &text_width, &text_height);
            SDL_FRect text_rect = {
                (640 - text_width) / 2,  /* center horizontally */
                20,                       /* 20 pixels from top */
                text_width,
                text_height
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
