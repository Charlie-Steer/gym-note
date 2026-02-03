typedef char Char;
typedef int8_t I8;
typedef int32_t I32;
typedef int64_t I64;
typedef uint8_t U8;
typedef uint32_t U32;
typedef uint64_t U64;
typedef float F32;
typedef double F64;
typedef char * C_String;

#define KILOBYTE 1024ULL
#define MEGABYTE (1024ULL * KILOBYTE)
#define GIGABYTE (1024ULL * MEGABYTE)

typedef struct string {
	char *buffer;
	U64 length;
	U64 capacity;
} String;

typedef struct Arena {
	U8 *buffer;
	U64 size;
	U64 position;
} Arena;

// #define SUCCESS true;
// #define FAILURE false;
// typedef bool bool_error;

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

typedef struct {
    I32 x;
    I32 y;
} Vec2;
typedef Vec2 V2;

typedef struct {
    I32 x;
    I32 y;
    I32 z;
} Vec3;
typedef Vec3 V3;

typedef struct set {
	int weight;
	int reps;
} Set;

typedef struct session {
	SDL_Time date_time;
	int set_count;
	Set *sets;
} Session;

typedef struct exercise {
	String name;
	Session *history;
} Exercise;

typedef struct exercise_group {
	int count;
	Exercise *exercises;
} Exercise_Group;

struct Window {
	I32 x;
	I32 y;
	SDL_Window *handle;
};

struct Window window = {
	.x = 360,
	.y = 640,
};

typedef struct Mouse {
	bool is_left_clicking;
	F32 x;
	F32 y;
} Mouse;

Mouse mouse = {0};