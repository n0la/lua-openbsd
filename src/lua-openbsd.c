#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <lua-pledge/config.h>

static void lo_die(lua_State *L, char const *e)
{
    lua_pushstring(L, e);
    lua_error(L);
}

static void free_strings(char **str, size_t n)
{
    size_t i = 0;

    if (str) {
        for (; i < n; i++) {
            free(str[i]);
        }
    }
    free(str);
}

int lua_pledge(lua_State *L)
{
    char const *farg = NULL;
    char **dirs = NULL;
    int ret = 0;
    size_t idx = 0;
    int top = 0;

#ifndef HAVE_PLEDGE
    lo_die(L, "pledge: not supported");
#endif

    top = lua_gettop(L);

    luaL_argcheck(L, lua_isstring(L, 1), 1,
                  "pledge: first argument must be string");
    if (top == 2) {
        luaL_argcheck(L, lua_istable(L, 2), 1,
                      "pledge: second argument must be table");
    } else if (top > 2) {
        lo_die(L, "pledge: too many arguments");
    }

    farg = lua_tostring(L, 1);

    /* This is the code if whitelists are enabled again.
     */
#ifdef HAVE_PLEDGE_WHITELIST
    if (!lua_isnoneornil(L, 2)) {
        size_t sz = 1;
        dirs = calloc(2, sizeof(char*));
        if (dirs == NULL) {
            lo_die(L, "pledge: calloc");
        }

        lua_pushnil(L);
        while (lua_next(L, 2)) {
            if (lua_isstring(L, -1)) {
                char const *str = lua_tostring(L, -1);

                if (idx == (sz-1)) {
                    char **tmp = NULL;

                    tmp = reallocarray(dirs, sz+10, sizeof(char*));
                    if (tmp == NULL) {
                        free_strings(dirs, idx+1);
                        lo_die(L, "pledge: calloc()");
                    }
                    dirs = tmp;
                    sz = sz + 10;
                }

                dirs[idx] = strdup(str);
                if (dirs[idx] == NULL) {
                    free_strings(dirs, idx+1);
                    lo_die(L, "pledge: strdup");
                }
                ++idx;
            }
            lua_pop(L, 1);
        }
    }
#endif

    ret = pledge(farg, (char const **)dirs);
    free_strings(dirs, idx+1);

    lua_pushnumber(L, ret);
    if (ret == -1) {
        lua_pushstring(L, strerror(errno));
        return 2;
    }

    return 1;
}

int lua_arc4random(lua_State *L)
{
#ifndef HAVE_ARC4RANDOM
    lo_die(L, "arc4random: not supported");
#endif

    lua_pushnumber(L, arc4random());
    return 1;
}

int lua_arc4random_uniform(lua_State *L)
{
    uint32_t up = 0;

#ifndef HAVE_ARC4RANDOM_UNIFORM
    lo_die(L, "arc4random_uniform: not supported");
#endif

    luaL_argcheck(L, lua_isnumber(L, 1), 1,
                  "pledge: first argument must be string");

    up = lua_tonumber(L, 1);
    lua_pushnumber(L, arc4random_uniform(up));

    return 1;
}

#if !defined LUA_VERSION_NUM || LUA_VERSION_NUM==501
/* Adapted from Lua 5.2.0
 */
static void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup) {
    luaL_checkstack(L, nup+1, "too many upvalues");
    for (; l->name != NULL; l++) {  /* fill the table with given functions */
        int i;
        lua_pushstring(L, l->name);
        for (i = 0; i < nup; i++) {  /* copy upvalues to the top */
            lua_pushvalue(L, -(nup+1));
        }
        lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
        lua_settable(L, -(nup + 3));
    }
    lua_pop(L, nup);  /* remove upvalues */
}
#endif

static const struct luaL_Reg l_pledge[] = {
    { "pledge", lua_pledge },
    { "arc4random", lua_arc4random },
    { "arc4random_uniform", lua_arc4random_uniform },
    { NULL, NULL },
};

int luaopen_openbsd(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, l_pledge, 0);

    return 1;
}
