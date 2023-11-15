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

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "quadratic_formula_face.h"

static void _quadratic_formula_face_reset_state(void *context) {
    quadratic_formula_state_t *state = (quadratic_formula_state_t *) context;

    state->index = 0;
    state->a = 1;
    state->b = 0;
    state->c = 0;
    state->kees = CASE_NONE;
    state->x1 = 0;
    state->x2 = 0;
}

static void _quadratic_formula_face_update_display(void *context) {
    quadratic_formula_state_t *state = (quadratic_formula_state_t *) context;

    char buffer[11];

    switch (state->index) {
        case 0: // entering a
            sprintf(buffer, " a  %d     ", state->a);
            watch_display_string(buffer, 0);
            break;
        case 1: // entering b
            sprintf(buffer, " b  %d     ", state->b);
            watch_display_string(buffer, 0);
            break;
        case 2: // entering c
            sprintf(buffer, " c  %d     ", state->c);
            watch_display_string(buffer, 0);
            break;
        case 3: // show x1
            switch (state->kees) {
                case CASE_0:
                    // fall through
                case CASE_1:
                    sprintf(buffer, "r1 %d%.2f", state->kees, state->x1);
                    break;
                case CASE_2:
                    sprintf(buffer, "re %d%.2f", state->kees, state->x1);
                    break;
                default:
                    _quadratic_formula_face_reset_state(state);
            }
            watch_display_string(buffer, 0);
            break;
        case 4: // show x2
            switch (state->kees) {
                case CASE_0:
                    // fall through
                case CASE_1:
                    sprintf(buffer, "r2 %d%.2f", state->kees, state->x2);
                    break;
                case CASE_2:
                    sprintf(buffer, "im %d%.2f", state->kees, state->x2);
                    break;
                default:
                    _quadratic_formula_face_reset_state(state);
            }
            watch_display_string(buffer, 0);
            break;
    }
}

static void _quadratic_formula_face_calculate(void *context) {
    quadratic_formula_state_t *state = (quadratic_formula_state_t *) context;

    int a = state->a;
    int b = state->b;
    int c = state->c;

    int D = pow(b, 2) - (4 * a * c);
    if (D > 0) {
        state->kees = CASE_0;

        state->x1 = (-b + sqrt(D))/(2*a);
        state->x2 = (-b - sqrt(D))/(2*a);
    } else if (D == 0) {
        state->kees = CASE_1;

        state->x1 = -b/(2*a);
        state->x2 = -b/(2*a);
    } else if (D < 0) {
        state->kees = CASE_2;

        state->x1 = -b/(2*a);
        state->x2 = sqrt(abs(D))/(2*a);
    }       
}

void quadratic_formula_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) settings;
    (void) watch_face_index;

    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(quadratic_formula_state_t));
        memset(*context_ptr, 0, sizeof(quadratic_formula_state_t));
        // Do any one-time tasks in here; the inside of this conditional happens only at boot.
    }
    // Do any pin or peripheral setup here; this will be called whenever the watch wakes from deep sleep.
}

void quadratic_formula_face_activate(movement_settings_t *settings, void *context) {
    (void) settings;
    quadratic_formula_state_t *state = (quadratic_formula_state_t *) context;

    // Handle any tasks related to your watch face coming on screen.
    _quadratic_formula_face_reset_state(state);
}

bool quadratic_formula_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    quadratic_formula_state_t *state = (quadratic_formula_state_t *)context;

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            // fall through
        case EVENT_TICK:
            _quadratic_formula_face_update_display(state);
            break;
        case EVENT_LIGHT_BUTTON_UP:
            if (state->index >= 4) {
                _quadratic_formula_face_reset_state(state);
            } else {
                if (state->index == 2) { 
                    _quadratic_formula_face_calculate(state);
                }
                state->index++;
            }
            break;
        case EVENT_LIGHT_LONG_PRESS:
            _quadratic_formula_face_reset_state(state);
            break;
        case EVENT_ALARM_BUTTON_UP:
            switch (state->index) {
                case 0: // increment a
                    state->a++;
                    break;
                case 1: // increment b
                    state->b++;
                    break;
                case 2: // increment c
                    state->c++;
                    break;
                default:
                    break;
            }
            break;
        case EVENT_ALARM_LONG_PRESS:
            switch (state->index) {
                case 0: // negate a
                    state->a = state->a * -1;
                    break;
                case 1: // negate b
                    state->b = state->b * -1;
                    break;
                case 2: // negate c
                    state->c = state->c * -1;
                    break;
                default:
                    break;
            }
            break;
        default:
            // Movement's default loop handler will step in for any cases you don't handle above:
            // * EVENT_LIGHT_BUTTON_DOWN lights the LED
            // * EVENT_MODE_BUTTON_UP moves to the next watch face in the list
            // * EVENT_MODE_LONG_PRESS returns to the first watch face (or skips to the secondary watch face, if configured)
            // You can override any of these behaviors by adding a case for these events to this switch statement.
            return movement_default_loop_handler(event, settings);
    }

    // return true if the watch can enter standby mode. Generally speaking, you should always return true.
    // Exceptions:
    //  * If you are displaying a color using the low-level watch_set_led_color function, you should return false.
    //  * If you are sounding the buzzer using the low-level watch_set_buzzer_on function, you should return false.
    // Note that if you are driving the LED or buzzer using Movement functions like movement_illuminate_led or
    // movement_play_alarm, you can still return true. This guidance only applies to the low-level watch_ functions.
    return true;
}

void quadratic_formula_face_resign(movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;

    // handle any cleanup before your watch face goes off-screen.
}

