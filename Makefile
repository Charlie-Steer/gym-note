.DEFAULT_GOAL := all

PROGRAM = bin/gym_note.exe

CC = clang

CFLAGS = -Ilibs/include -g -gcodeview
LDFLAGS = -Llibs/lib -lSDL3 -lSDL3_ttf -Wl,-debug

# SRCS = $(wildcard src/*.c)
SRCS = $(wildcard src/*.c src/*.cpp src/*.h src/*.hpp)

UNITY_NAME = UNITY_BUILD.c
UNITY_FILE = src/$(UNITY_NAME)
UNITY_OBJECT = $(UNITY_FILE:src/%.c=build/%.o)

$(UNITY_OBJECT): $(UNITY_FILE) $(SRCS)
	if not exist build mkdir build
	$(CC) -c $< -o $@ $(CFLAGS)

$(PROGRAM): $(UNITY_OBJECT)
	$(CC) $(UNITY_OBJECT) -o $(PROGRAM) $(LDFLAGS)

all: $(PROGRAM) compile_commands.json

re: fclean all

run: all
	bin/gym_note.exe

rerun: re
	bin/gym_note.exe

# AI generated command.
compile_commands.json: $(UNITY_FILE)
	powershell -NoProfile -Command "$$files = Get-ChildItem src -Filter *.c; \
	$$entries = @(); \
	$$dir = (Get-Location).Path.Replace('\','/'); \
	foreach ($$f in $$files) { \
	    $$path = $$f.FullName.Replace('\','/'); \
	    if ($$f.Name -eq '$(UNITY_NAME)') { \
	        $$args = @('clang','-Ilibs/include',$$path); \
	    } else { \
	        $$args = @('clang','-Ilibs/include','-include','$(UNITY_NAME)',$$path); \
	    } \
	    $$entries += [pscustomobject]@{ directory=$$dir; file=$$path; arguments=$$args }; \
	}; \
	$$entries | ConvertTo-Json -Depth 3 | Set-Content compile_commands.json"

# compile_commands.json: $(UNITY_FILE)
# 	@echo Generating $@...
# 	@powershell -NoProfile -Command "$$files = Get-ChildItem src -Filter *.c; \
# 	$$entries = @(); \
# 	$$dir = (Get-Location).Path.Replace('\','/'); \
# 	$$unityPath = \"$$dir/$(UNITY_FILE)\"; \
# 	foreach ($$f in $$files) { \
# 	    $$path = $$f.FullName.Replace('\','/'); \
# 	    if ($$f.Name -eq '$(UNITY_NAME)') { \
# 	        $$args = @('clang','-Ilibs/include',$$path); \
# 	    } else { \
# 	        $$args = @('clang','-Ilibs/include','-include',$$unityPath,$$path); \
# 	    } \
# 	    $$entries += [pscustomobject]@{ directory=$$dir; file=$$path; arguments=$$args }; \
# 	}; \
# 	$$entries | ConvertTo-Json -Depth 3 | Set-Content compile_commands.json"

clean:
	-rmdir /s /q build
	-del compile_commands.json

fclean: clean
	-del bin\gym_note.exe

.PHONY: all re clean fclean run rerun