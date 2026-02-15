#!/usr/bin/sh
xxd -i -n lua_file ${1} $(dirname ${1})/lua_file.h