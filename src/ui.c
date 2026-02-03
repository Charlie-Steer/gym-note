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

typedef enum UI_Element_Type {
    UI_AREA,
    UI_BUTTON,
} UI_Element_Type;

typedef enum UI_Layout_Direction {
    UI_HORIZONTAL,
    UI_VERTICAL,
} UI_Layout_Direction;

typedef struct UI_Grid_Config {
    I32 max_elements_horizontal;
    I32 max_elements_vertical;
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
    I32 elements_count;
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
    parent->elements_count += 1;
    UI_Element *elements = arena_allocate(arena, parent->elements_count * sizeof(UI_Element));
    I32 element_idx = 0;
    for (; element_idx < parent->elements_count - 1; element_idx++) {
        assert (parent->elements != NULL);
        elements[element_idx] = parent->elements[element_idx];
    }
    elements[element_idx] = *child;
}

void UI_create_children(Arena *arena, UI_Element *parent, I32 count) {
    parent->elements_count = count;
    parent->elements = arena_allocate(arena, count * sizeof(UI_Element));
}

void UI_change_color(void *ui_element) {
    ((UI_Element *)ui_element)->background_color = (SDL_FColor){0, 1, 1, 1};
}

// TODO: Make it handle buttons as parent and infinitely nested elements.
void UI_draw_hierarchy(SDL_Renderer *renderer, UI_Element *parent) {
    SDL_Log("Enter\n");
    if (parent->element_type == UI_AREA) {
        for (int child_idx = 0; child_idx < parent->elements_count; child_idx++) {
            UI_Element *child = &parent->elements[child_idx];
            SDL_FRect rect;
            if (parent->grid_config == NULL) {
                if (parent->layout_direction == UI_HORIZONTAL) {
                    SDL_Log("HORIZONTAL Case\n");
                    rect.w = parent->w / parent->elements_count;
                    rect.h = parent->h;
                    rect.x = parent->x + (rect.w * child_idx);
                    rect.y = parent->y;
                }
                else if (parent->layout_direction == UI_VERTICAL) {
                    SDL_Log("VERTICAL Case\n");
                    rect.w = parent->w;
                    rect.h = parent->h / parent->elements_count;
                    rect.x = parent->x;
                    rect.y = parent->y + (rect.h * child_idx);
                }
            } else {
                // TODO.
            }
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