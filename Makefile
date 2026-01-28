NAME = gym_note.exe

CC = clang

SDL_DIR = SDL3

CFLAGS  = -I$(SDL_DIR) -I$(SDL_DIR)\SDL3
LDFLAGS = -L$(SDL_DIR) -lSDL3

SRC_DIR = src
BUILD_DIR = build

SRCS = \
	src/main.c \
	src/test1.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
JSONS = $(OBJS:.o=.json)


all: $(NAME) compile_commands.json

re: fclean all

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) -MJ $(BUILD_DIR)/$*.json

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

# AI generated command. Single line JSON.
# compile_commands.json: $(OBJS)
# 	powershell -NoProfile -Command "$$j = Get-Content -Raw $(JSONS) | %% { $$_ -replace ',\s*$$','' }; '[' + ($$j -join ',') + ']' | Set-Content $@"

# AI generated command. Multi-line JSON.
compile_commands.json: $(OBJS)
	powershell -NoProfile -Command "\
	$$files = Get-ChildItem '$(BUILD_DIR)\\*.json'; \
	$$list = @(); \
	foreach ($$f in $$files) { \
	    $$raw = Get-Content $$f -Raw; \
	    $$clean = $$raw -replace ',\s*$$',''; \
	    $$list += ($$clean | ConvertFrom-Json); \
	}; \
	ConvertTo-Json -InputObject $$list -Depth 5 | Set-Content '$@'"

clean:
	-rmdir /s /q $(BUILD_DIR)
	-del compile_commands.json

fclean: clean
	-del $(NAME)

.PHONY: all re clean fclean