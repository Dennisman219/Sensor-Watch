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
#include "lua_face.h"

#include "lualib.h"
#include "lauxlib.h"
//#include "lprefix.h"

#include "lua_file.h"

void lua_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) settings;
    (void) watch_face_index;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(lua_state_t));
        memset(*context_ptr, 0, sizeof(lua_state_t));
        // Do any one-time tasks in here; the inside of this conditional happens only at boot.
    }
    // Do any pin or peripheral setup here; this will be called whenever the watch wakes from deep sleep.
}

void lua_face_activate(movement_settings_t *settings, void *context) {
    (void) settings;
    lua_state_t *state = (lua_state_t *) context;

    // Handle any tasks related to your watch face coming on screen.
    state->load_ok = false;
    state->L = luaL_newstate();
    luaL_requiref(state->L, LUA_GNAME, luaopen_base, 1);
    //luaL_requiref(state->L, LUA_LOADLIBNAME, luaopen_package, 1);
    //luaL_requiref(state->L, LUA_COLIBNAME, luaopen_coroutine, 1);
    //luaL_requiref(state->L, LUA_STRLIBNAME, luaopen_string, 1);
    //luaL_requiref(state->L, LUA_MATHLIBNAME, luaopen_math, 1);
    //luaL_requiref(state->L, LUA_UTF8LIBNAME, luaopen_utf8, 1);
    luaL_requiref(state->L, LUA_SENSORWATCHLIBNAME, luaopen_sensorwatch, 1);

    watch_display_string("LU", 0);

    int ret = luaL_dostring(state->L, (char*) lua_file);
    if (ret != LUA_OK) {
        watch_display_string("ER", 0);
#ifdef __EMSCRIPTEN__
        printf("Error: %s\n", lua_tostring(state->L, -1));
        lua_pop(state->L, 1); // pop error message
#endif
    } else {
        state->load_ok = true;
        watch_display_string("OK", 0);
    }
}

bool lua_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    lua_state_t *state = (lua_state_t *) context;

    const char *lua_event;
    switch (event.event_type) {
        case EVENT_ACTIVATE:
            lua_event = "activate";
            break;
        case EVENT_TICK:
            lua_event = "tick";
            break;
        case EVENT_LIGHT_BUTTON_UP:
            lua_event = "light-button-up";
            break;
        case EVENT_ALARM_BUTTON_UP:
            lua_event = "alarm-button-up";
            break;
        case EVENT_TIMEOUT:
            lua_event = "timeout";
            break;
        case EVENT_LOW_ENERGY_UPDATE:
            lua_event = "low-energy-update";
            break;
        default:
            return movement_default_loop_handler(event, settings);
            break;
    }
    
    if (state->load_ok) {
        lua_getglobal(state->L, "sw_loop");
        lua_pushstring(state->L, lua_event);
        int result = lua_pcall(state->L, 1, 0, 0);
        if (result != 0) {
            watch_display_string("ER", 0);
#ifdef __EMSCRIPTEN__
            printf("Error: %s\n", lua_tostring(state->L, -1));
            lua_pop(state->L, 1); // pop error message
#endif
        }
    }

    return true;
}

void lua_face_resign(movement_settings_t *settings, void *context) {
    (void) settings;
    lua_state_t *state = (lua_state_t *) context;

    // handle any cleanup before your watch face goes off-screen.
    lua_close(state->L);
}