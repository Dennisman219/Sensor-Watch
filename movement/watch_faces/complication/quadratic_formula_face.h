/*
 * MIT License
 *
 * Copyright (c) 2023 Dennisman219
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef QUADRATIC_FORMULA_FACE_H_
#define QUADRATIC_FORMULA_FACE_H_

#include "movement.h"

/*
 * A watch face to find the (real of imaginary) roots of a quadratic formula
 * Currently only supports integer constants
 * 
 * Press alarm button to increment constant by 1. Long press alarm button to negate constant.
 * Press light button to move to next constant. Once all constants are entered the roots are calculated and the first root is shown. Press the light button again to see the second root.
 * Long press the light button to reset the constants.
 */

typedef enum {
    CASE_NONE = 0,
    CASE_0,
    CASE_1,
    CASE_2,
} quadratic_formula_face_case_t;

typedef struct {
    uint8_t index;
    int a;
    int b;
    int c;
    quadratic_formula_face_case_t kees; // case is reserved keyword
    float x1;
    float x2;
} quadratic_formula_state_t;

// static void _quadratic_formula_face_reset_state(void *context);
// static void _quadratic_formula_face_update_display(void *context);
// static void _quadratic_formula_face_calculate(void *context);

void quadratic_formula_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr);
void quadratic_formula_face_activate(movement_settings_t *settings, void *context);
bool quadratic_formula_face_loop(movement_event_t event, movement_settings_t *settings, void *context);
void quadratic_formula_face_resign(movement_settings_t *settings, void *context);

#define quadratic_formula_face ((const watch_face_t){ \
    quadratic_formula_face_setup, \
    quadratic_formula_face_activate, \
    quadratic_formula_face_loop, \
    quadratic_formula_face_resign, \
    NULL, \
})

#endif // QUADRATIC_FORMULA_FACE_H_

