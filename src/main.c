// #define SDL_MAIN_HANDLED


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

	bool window_should_close = false;
	while (!window_should_close) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				window_should_close = true;
			} else if (e.type == SDL_EVENT_KEY_DOWN) {
				if (e.key.scancode == SDL_SCANCODE_ESCAPE) {
					window_should_close = true;
				}
			}
		}
		double now = (double)SDL_GetTicks() / 1000.0;
		
		// SDL_SetRenderDrawColorFloat(renderer, 0, 0, 1, 1);
		float brightness = .2;
		SDL_SetRenderDrawColorFloat(renderer, 1 * brightness, 1 * brightness, 1 * brightness, 1);
		SDL_RenderClear(renderer);

		// SDL_text

		SDL_RenderPresent(renderer);

		// Frame End.
		arena_clear(&frame_arena);
	}

	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window.handle);
    SDL_Quit();
	return 0;
}