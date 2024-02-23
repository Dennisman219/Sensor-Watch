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

static int sensorwatch_display_string(lua_State *L) {
    char* rtrn = (char *) lua_tostring(L, -1);
    watch_display_string(rtrn, 0);
    return 0;
}

static const luaL_Reg sensorwatchlib[] = {
    {"display_string", sensorwatch_display_string},
    {NULL, NULL}
};

LUAMOD_API int luaopen_sensorwatch(lua_State *L) {
    luaL_newlib(L, sensorwatchlib);
    return 1;
}