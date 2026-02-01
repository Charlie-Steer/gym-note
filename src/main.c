// #define SDL_MAIN_HANDLED

#include "SDL3/SDL_events.h"
U64 get_cstring_length(char *text) {
	int string_index = 0;
	for (; text[string_index] != '\0'; string_index++);
	return string_index;
}

U64 get_string_length(String *string) {
	assert(string);
	return string->length;
}

String create_string(char *text, Arena *arena) {
	assert(arena);
	String string = {0};
	string.length = strlen(text);
	string.capacity = string.length + 1;
	string.buffer = arena_allocate(arena, string.length + 1);
	memcpy(string.buffer, text, string.length + 1);
	return string;
}

struct Window {
	I32 x;
	I32 y;
	SDL_Window *handle;
};

struct Window window = {
	.x = 360,
	.y = 640,
};

typedef struct User {
	bool is_left_clicking;
} User;

User user = {0};

int main(void) {
	bool ok = true;
	ok = SDL_Init(SDL_INIT_VIDEO);
	if (!ok) {
		log_error_message_and_exit("SDL_Init.");
	}

	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer("Gym Note", window.x, window.y, 0, &window.handle, &renderer);
	
	Arena program_arena;
	arena_init(&program_arena, 1 * GIGABYTE);

	String str = create_string("Hello world!", &program_arena);
	log_string(str);

	Arena frame_arena;
	arena_init(&frame_arena, 1 * GIGABYTE);

	if (!TTF_Init()) {
		SDL_Log("Could not initialize SDL_ttf: %s", SDL_GetError());
		return -1;
	}

	// Create the renderer engine
	TTF_TextEngine *text_engine = TTF_CreateRendererTextEngine(renderer);
	if (!text_engine) {
		SDL_Log("Engine failure: %s", SDL_GetError());
	}

	// Open your font as usual
	TTF_Font *font = TTF_OpenFont("assets/GoogleSans-Regular.ttf", 24.0f);
	if (!font) {
		SDL_Log("Couldn't open text: %s", SDL_GetError());
	}

	// Create the text object
	// The '0' indicates the string is null-terminated
	TTF_Text *text = TTF_CreateText(text_engine, font, "Hello SDL3!", 0);

	// You can set the color directly on the object
	TTF_SetTextColor(text, 255, 255, 255, 255); // White

	String s1 = create_string("Lat pulldown", &program_arena);
	String s2 = create_string("Bayesian cable curl", &program_arena);
	String s3 = create_string("Flat Bench Press", &program_arena);

	char *entries[] = {
		"Lat pulldown",
		"Bayesian cable curl",
		"Flat Bench Press",
	};

	TTF_Text *text_entries[3];
	for (int i = 0; i < 3; i++) {
		text_entries[i] = TTF_CreateText(text_engine, font, entries[i], 0);
	}

	bool window_should_close = false;
	int frame_idx = 0;
	while (!window_should_close) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				window_should_close = true;
			} else if (e.type == SDL_EVENT_KEY_DOWN) {
				if (e.key.scancode == SDL_SCANCODE_ESCAPE) {
					window_should_close = true;
				}
			} else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					user.is_left_clicking = true;
				}
			} else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				if (e.button.button == SDL_BUTTON_LEFT) {
					user.is_left_clicking = false;
				}
			}
		}
		double now = (double)SDL_GetTicks() / 1000.0;
		
		// SDL_SetRenderDrawColorFloat(renderer, 0, 0, 1, 1);
		float brightness = .2;
		SDL_SetRenderDrawColorFloat(renderer, 1 * brightness, 1 * brightness, 1 * brightness, 1);
		SDL_RenderClear(renderer);

		// SDL_TTF
		// Simple one-line draw command
		// TTF_DrawRendererText(text, 100.0f, 100.0f);
		// SDL_FRect rect = {100, 100, 100, 50};
		// SDL_SetRenderDrawColorFloat(renderer, 1, 0, 0, 1);
		// SDL_RenderFillRect(renderer, &rect);
		
		// int font_height = TTF_GetFontHeight(font);
		// for (int i = 0; i < 3; i++) {
		// 	TTF_DrawRendererText(text_entries[i], 10.0f, font_height*i);
		// }

		Exercise exercises[] = {
			(Exercise){
				.name = "Exercise 1",
			},
			(Exercise){
				.name = "Exercise 2",
			},
			(Exercise){
				.name = "Exercise 3",
			},
		};
		
		Exercise_Group exercise_group = {
			.exercises = exercises,
			.count = 3,
		};

		// draw_exercises_menu(&exercise_group, text_engine, font);
		// SDL_Log("frame: %d\n", frame_idx);
		// frame_idx++;

		// UI
		UI_Element hierarchy;
		// UI_configure_area(&hierarchy, 5, 5, 100, 500, UI_VERTICAL, NULL);
		UI_configure_area(&hierarchy, 5, 5, 500, 100, UI_HORIZONTAL, NULL);
		UI_Element buttons[5];
		for (int i = 0; i < 5; i++) {
			SDL_FColor color;
			if (i % 2) {
				color = (SDL_FColor){1, 0, 0, 1};
			} else {
				color = (SDL_FColor){0, 0, 1, 1};
			}
			UI_configure_button(&buttons[i], NULL, &color, NULL);
		}
		hierarchy.elements = buttons;
		hierarchy.elements_count = 5;
		UI_draw_hierarchy(renderer, &hierarchy);

		SDL_RenderPresent(renderer);

		// Frame End.
		arena_clear(&frame_arena);
	}

	TTF_DestroyText(text);
	TTF_DestroyRendererTextEngine(text_engine);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window.handle);
    SDL_Quit();
	return 0;
}