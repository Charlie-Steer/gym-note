// #include "SDL_events.h"
// #define SDL_MAIN_HANDLED
#include <SDL.h>
// #include "SDL_init.h"
#include <stdio.h>
// #include "SDL.h"
// #include "SDL_video.h"
#include <assert.h>

typedef int8_t I8;
typedef int32_t I32;
typedef int64_t I64;
typedef uint8_t U8;
typedef uint32_t U32;
typedef uint64_t U64;
typedef float_t F32;
typedef double_t F64;
typedef char * C_String;

// #define SUCCESS true;
// #define FAILURE false;
// typedef bool bool_error;
#define KILOBYTE 1024ULL
#define MEGABYTE (1024ULL * KILOBYTE)
#define GIGABYTE (1024ULL * MEGABYTE)

typedef struct string {
	U8 *buffer;
	U64 length;
	U64 capacity;
} String;

typedef struct Arena {
	U8 *buffer;
	U64 size;
	U64 position;
} Arena;

typedef enum bool_error {
	BAD = 0,
	GOOD = 1,
} Bool_Error;

typedef enum error {
	ERROR_NONE = 0,
	ERROR_BAD_ALLOCATION,
	ERROR_NULL_POINTER,
	ERROR_INVALID_ARGUMENT_VALUE,
	ERROR_ARENA_LIMIT,
	ERROR_GENERIC,
} Error;

void log_error(Error error) {
	char *error_message;
	switch (error) {
		case ERROR_BAD_ALLOCATION:
			error_message = "Failed to allocate memory.";
			break;
		case ERROR_NULL_POINTER:
			error_message = "Unexpected NULL pointer.";
			break;
		default:
			error_message = "Generic error.";
			break;
	}
	fprintf(stderr, "ERROR: %s\n", error_message);
}

void log_error_and_exit(Error error) {
	log_error(error);
	exit(EXIT_FAILURE);
}

void log_error_message(char *message) {
	fprintf(stderr, "ERROR: %s\n", message);
}

void log_error_message_and_exit(char *message) {
	log_error_message(message);
	exit(EXIT_FAILURE);
}

void log_debug(char *message) {
	#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s\n", message);
	#endif
}

void log_message(char *message) {
	fprintf(stdout, "LOG: %s\n", message);
}

void log_string(String message) {
	printf("%s\n", message.buffer);
}

// typedef struct Allocator {
// 	void *(*allocate)(void *allocator_state, u64 size);
// 	void *allocator_state;
// } Allocator;

// NOTE: Not memory aligned.
// bool_error arena_init(Arena *arena, u64 size) {
// 	u8 *buffer = malloc(size);
// 	if (buffer == NULL) {
// 		return BAD;
// 	}
// 	arena->buffer = buffer;
// 	arena->size = size;
// 	arena->position = 0;
// 	memset(arena->buffer, 0, size);
// 	return GOOD;
// }

void arena_init(Arena *arena, U64 size) {
	U8 *buffer = malloc(size);
	if (buffer == NULL) {
		log_error_and_exit(ERROR_BAD_ALLOCATION);
	}
	arena->buffer = buffer;
	arena->size = size;
	arena->position = 0;
	memset(arena->buffer, 0, size);
}

void arena_clear(Arena *arena) {
	arena->position = 0;
}

void *arena_allocate(Arena *arena, U64 block_size) {
	U64 new_arena_position = arena->position + block_size;
	if (block_size > arena->size - arena->position) {
		// NOTE: Maybe a non-terminating solution can be implemented.
		log_error_and_exit(ERROR_ARENA_LIMIT);
	}
	void *new_allocation_address = arena->buffer + arena->position;
	arena->position += block_size;
	return new_allocation_address;
}

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

int main(void) {
	log_message("A");
	fflush(stdout);
	SDL_Log("Hello?");
	bool ok = true;
	log_debug("A");
	ok = SDL_Init(SDL_INIT_VIDEO);
	if (!ok) {
		log_error_message_and_exit("SDL_Init.");
	}
	log_debug("B");
	log_message("B");

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer("Gym Note", 680, 400, 0, &window, &renderer);

	bool window_should_close = false;
	while (!window_should_close) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				window_should_close = true;
			}
		}
	}

    // Close and destroy the window
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
	
	Arena arena;
	arena_init(&arena, 1 * GIGABYTE);
	log_debug("C");


	String str = create_string("Hello world!", &arena);
	log_string(str);
	return 0;
}