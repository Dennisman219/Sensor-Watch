/*
** $Id: lsensorwatchlib.c $   
** Lua bindings for the sensorwatch library
** (c) Dennisman219, 2024
*/

#define lsensorwatchlib_c
#define LUA_LIB

#include "movement.h"

//#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

static int sensorwatch_clear_display(lua_State *L) {
    watch_display_string("          ", 0);
    return 0;
}

static int sensorwatch_display_string(lua_State *L) {
    int position = 4;
    if (!lua_isstring(L, 1)) {
        #ifdef __EMSCRIPTEN__
        printf("Error: display_string: 1st argument is not of type string\n");
        #endif
        //return 1;
    }
    if (lua_isnumber(L, 2)) {
        position = lua_tointeger(L, 2);
    }
    const char* text = lua_tostring(L, 1);
    watch_display_string(text, position);
    return 0;
}

static const luaL_Reg sensorwatchlib[] = {
    {"display_string", sensorwatch_display_string},
    {"clear_display", sensorwatch_clear_display},
    {NULL, NULL}
};

LUAMOD_API int luaopen_sensorwatch(lua_State *L) {
    luaL_newlib(L, sensorwatchlib);
    return 1;
}