// Use SDLs main callback system
// https://wiki.libsdl.org/SDL3/README/main-functions
#define SDL_MAIN_USE_CALLBACKS

// Include SDL
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h> // Only include SDL_main.h in one file

// Application needs a window, renderer, and result status.
struct Application
{
	SDL_Window *window{nullptr};
	SDL_Renderer *renderer{nullptr};
	SDL_AppResult status{SDL_APP_CONTINUE};
};

// Runs once at startup
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
	// Initialize SDL
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Failed to initialize SDL with error: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// Create Window with Renderer
	SDL_Window *window;
	SDL_Renderer *renderer;

	if (!SDL_CreateWindowAndRenderer("Minimal SDL3 CMake Project", 1280, 720, 0, &window, &renderer))
	{
		SDL_Log("Failed to initialize SDL Window or Renderer with error: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// Set up application data
	*appstate = new Application{
		.window = window,
		.renderer = renderer};

	SDL_Log("Application started.");

	return SDL_APP_CONTINUE;
}

// Runs when a new event (keyboard/mouse input, controllers, etc.)
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
	Application *app = (Application *)appstate;

	// Window "X" event shuts down the app
	if (event->type == SDL_EVENT_QUIT)
	{
		app->status = SDL_APP_SUCCESS;
	}

	return SDL_APP_CONTINUE;
}

// Runs once per frame while application is running
SDL_AppResult SDL_AppIterate(void *appstate)
{
	Application *app = (Application *)appstate;

	float elapsedTime = SDL_GetTicks() / 1000.0f; // In seconds

	// Some colors that change per frame
	float red = SDL_sin(elapsedTime);
	float green = SDL_sin(elapsedTime + SDL_PI_D * 2 / 3);
	float blue = SDL_sin(elapsedTime + SDL_PI_D * 4 / 3);

	// Set the renderer drawing color
	SDL_SetRenderDrawColorFloat(app->renderer, red, green, blue, SDL_ALPHA_OPAQUE);

	// Clear the window to draw
	SDL_RenderClear(app->renderer);

	// Draw renderer to screen
	SDL_RenderPresent(app->renderer);

	// Delay in milliseconds to set FPS to ~60.
	SDL_Delay(16);

	return app->status;
}

// Exit the application and cleanup SDL.
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
	Application *app = (Application *)appstate;

	if (app)
	{
		SDL_DestroyRenderer(app->renderer);
		SDL_DestroyWindow(app->window);

		delete app;
	}

	SDL_Log("Application closed.");

	SDL_Quit();
}
