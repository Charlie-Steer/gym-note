/*
typedef struct UI_Element {
	F32 x, y, w, h;

	bool is_hovered, is_pressed;
	bool is_active; // Text fields.

	String label;
	SDL_Color color;

	void (*action)(void);
} UI_Element;

// TODO: Base it on boxes with an FRect to manage clicking areas.
void draw_exercises_menu(Exercise_Group *exercise_group, TTF_TextEngine *text_engine, TTF_Font *font) {
	I32 font_height = TTF_GetFontHeight(font);
	I32 line_spacing = 0;
	I32 x_offset = 5;
	I32 y_offset = 5;
	for (I32 exercise_idx = 0; exercise_idx < exercise_group->count; exercise_idx++) {
		Exercise *exercise = &exercise_group->exercises[exercise_idx];
		TTF_Text *text = TTF_CreateText(text_engine, font, exercise->name.buffer, exercise->name.length);
		TTF_DrawRendererText(text, x_offset, y_offset);
		y_offset += font_height + line_spacing;
	}
}
*/

#include "SDL3/SDL_log.h"
typedef enum UI_Element_Type {
	UI_AREA,
	UI_BUTTON,
} UI_Element_Type;

typedef enum UI_Layout_Direction {
	UI_HORIZONTAL,
	UI_VERTICAL,
} UI_Layout_Direction;

typedef struct UI_Grid_Config {
	I32 max_elements_per_line;
	bool set_manual_secondary_axis_element_length;
	I32 secondary_axis_element_length;
	// I32 max_elements_vertical;
} UI_Grid_Config;

// typedef struct UI_Area {
//     I32 x, y, w, h;
//     UI_Element *elements;
//     enum UI_Layout_Direction layout_direction;
//     UI_Grid_Config *grid_config;
// } UI_Area;

typedef struct UI_Element {
	UI_Element_Type element_type;
	F32 x, y, w, h;
	SDL_FColor background_color;
	SDL_FColor text_color;
	void (*action)(void *);
	struct UI_Element *elements;
	I32 child_count;
	UI_Layout_Direction layout_direction;
	UI_Grid_Config *grid_config;
} UI_Element;

void UI_configure_area(UI_Element *area, I32 x, I32 y, I32 w, I32 h, UI_Layout_Direction layout_direction, UI_Grid_Config *grid_config) {
	area->element_type = UI_AREA;
	area->x = x;
	area->y = y;
	area->w = w;
	area->h = h;
	area->layout_direction = layout_direction;
	area->grid_config = grid_config;
}

void UI_configure_button(UI_Element *button, void (*action)(void *), SDL_FColor *background_color, SDL_FColor *text_color) {
	button->element_type = UI_BUTTON;
	button->action = action;
	if (background_color == NULL) {
		button->background_color = (SDL_FColor){0, 0, 0, 1};
	} else {
		button->background_color = *background_color;
	}
	if (text_color == NULL) {
		button->text_color = (SDL_FColor){1, 1, 1, 1};
	} else {
		button->text_color = *text_color;
	}
}

void UI_append_child_element(Arena *arena, UI_Element *parent, UI_Element *child) {
	parent->child_count += 1;
	UI_Element *elements = arena_allocate(arena, parent->child_count * sizeof(UI_Element));
	I32 element_idx = 0;
	for (; element_idx < parent->child_count - 1; element_idx++) {
		assert (parent->elements != NULL);
		elements[element_idx] = parent->elements[element_idx];
	}
	elements[element_idx] = *child;
}

void UI_create_children(Arena *arena, UI_Element *parent, I32 count) {
	parent->child_count = count;
	parent->elements = arena_allocate(arena, count * sizeof(UI_Element));
}

void UI_change_color(void *ui_element) {
	((UI_Element *)ui_element)->background_color = (SDL_FColor){0, 1, 1, 1};
}

void UI_draw_grid(SDL_Renderer *renderer, UI_Element *parent) {
	I32 position_in_line = 0;
	F32 child_width = 0;
	F32 child_height = 0;
	UI_Grid_Config *grid_config = parent->grid_config;
	SDL_Log("Draw Grid.\n");
	// WARNING: Missing base cases here!
	// WARNING: Missing base cases here!
	// WARNING: Missing base cases here!
	// WARNING: Missing base cases here!
	// WARNING: Missing base cases here!
	// WARNING: Missing base cases here!
	if (parent->layout_direction == UI_HORIZONTAL) {
		child_width = parent->w / grid_config->max_elements_per_line;
	} else {
		child_height = parent->w / grid_config->max_elements_per_line;
	}
	for (I32 child_idx = 0; child_idx < parent->child_count; child_idx++) {
		SDL_Log("loop start\n");
		SDL_FRect rect = {0};
		if (parent->layout_direction == UI_HORIZONTAL) {
			rect.w = child_width;
			rect.x = parent->x + (child_width * (parent->child_count % grid_config->max_elements_per_line));
			// int line_count = grid_config->max_elements_per_line / parent->child_count;
			I32 current_element_line_index = I32_division(grid_config->max_elements_per_line, child_idx);
			if (grid_config->set_manual_secondary_axis_element_length) {
				rect.h = grid_config->secondary_axis_element_length;
			} else {
				rect.h = parent->h / I32_division(grid_config->max_elements_per_line, parent->child_count);
			}
			rect.y = parent->y + (rect.h * current_element_line_index);
		} else {
			// TODO
		}

		SDL_Log("before mouse\n");
		// WARNING: Duplicate.
		UI_Element *child = &parent->elements[child_idx];
		if (mouse.is_left_clicking && mouse.x > rect.x && mouse.x < rect.x + rect.w && mouse.y > rect.y && mouse.y < rect.y + rect.h) {
			if (child->action != NULL) {
				child->action(child); // NOTE: Such a hardcoded argument is probably not acceptable. I can provide to UI_Element a field for a pointer to a struct with self, parent and whatever else I need depending on the function.
			}
		}
		SDL_Log("before render\n");
		FRect_print(&rect, "Button rect");
		SDL_FColor color = {child->background_color.r, child->background_color.g, child->background_color.b, child->background_color.a};
		SDL_SetRenderDrawColorFloat(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &rect);
	}
}

// TODO: Make it handle buttons as parent and infinitely nested elements.
void UI_draw_hierarchy(SDL_Renderer *renderer, UI_Element *parent) {
	SDL_Log("Enter\n");
	if (parent->element_type == UI_AREA) {
		int position_in_line = 0;
		if (parent->grid_config) {
			UI_draw_grid(renderer, parent);
		} else {
			for (int child_idx = 0; child_idx < parent->child_count; child_idx++) {
				UI_Element *child = &parent->elements[child_idx];
				SDL_FRect rect;
				if (parent->layout_direction == UI_HORIZONTAL) {
					SDL_Log("HORIZONTAL Case\n");
					rect.w = parent->w / parent->child_count;
					rect.h = parent->h;
					rect.x = parent->x + (rect.w * child_idx);
					rect.y = parent->y;
				}
				else if (parent->layout_direction == UI_VERTICAL) {
					SDL_Log("VERTICAL Case\n");
					rect.w = parent->w;
					rect.h = parent->h / parent->child_count;
					rect.x = parent->x;
					rect.y = parent->y + (rect.h * child_idx);
				}

				// WARNING: Click trigger and rendering independent from grid case.
				if (mouse.is_left_clicking && mouse.x > rect.x && mouse.x < rect.x + rect.w && mouse.y > rect.y && mouse.y < rect.y + rect.h) {
					if (child->action != NULL) {
						child->action(child); // NOTE: Such a hardcoded argument is probably not acceptable. I can provide to UI_Element a field for a pointer to a struct with self, parent and whatever else I need depending on the function.
					}
				}
				SDL_FColor color = {child->background_color.r, child->background_color.g, child->background_color.b, child->background_color.a};
				SDL_SetRenderDrawColorFloat(renderer, color.r, color.g, color.b, color.a);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}