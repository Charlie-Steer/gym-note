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