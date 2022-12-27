#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include <bsd_auth.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <config.h>


static void lo_die(lua_State *L, char const *e)
{
    lua_pushstring(L, e);
    lua_error(L);
}

int lua_pledge(lua_State *L)
{
    char const *farg = NULL;
    char const *earg = NULL;
    int ret = 0;
    int top = 0;

#ifndef HAVE_PLEDGE
    lo_die(L, "pledge: not supported");
#endif

    top = lua_gettop(L);

    luaL_argcheck(L, lua_isstring(L, 1), 1,
                  "pledge: first argument must be string");
    if (top == 2) {
        luaL_argcheck(L, lua_isstring(L, 2), 1,
                      "pledge: second argument must be string");
    } else if (top > 2) {
        lo_die(L, "pledge: too many arguments");
    }

    farg = lua_tostring(L, 1);
    earg = lua_tostring(L, 2);

    ret = pledge(farg, earg);

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

int lua_unveil(lua_State *L)
{
    char const *path = NULL, *mode = NULL;
    int ret = 0;

#ifndef HAVE_UNVEIL
    lo_die(L, "unveil: not supported");
#endif

    luaL_argcheck(L, lua_isstring(L, 1), 1,
                  "unveil: first argument must be string");
    luaL_argcheck(L, lua_isstring(L, 2), 1,
                  "unveil: second argument must be string");

    path = lua_tostring(L, 1);
    mode = lua_tostring(L, 2);

    ret = unveil(path, mode);
    lua_pushnumber(L, ret);

    if (ret == -1) {
        lua_pushstring(L, strerror(errno));
        return 2;
    }

    return 1;
}

char* checkString_or_nil(lua_State *L, int index, char* buf, size_t bufsize)
{
    if (lua_isnoneornil(L, index)) return NULL;
    strlcpy(buf, luaL_checkstring(L, index), bufsize);
    return buf;
}

int lua_userokay(lua_State *L)
{
    size_t NAMELEN = LOGIN_NAME_MAX + 1 + NAME_MAX + 1;
    size_t TYPELEN = 128;
    size_t PASSLEN = 2048;

    char usernamebuf[NAMELEN];
    char stylebuf[TYPELEN];
    char typebuf[TYPELEN];
    char passwordbuf[PASSLEN];

    char *username = checkString_or_nil(L, 1, usernamebuf, NAMELEN);
    char *style = checkString_or_nil(L, 2, stylebuf, TYPELEN);
    char *type = checkString_or_nil(L, 3, typebuf, TYPELEN);
    char *password = checkString_or_nil(L, 4, passwordbuf, PASSLEN);

    lua_pushboolean(L, auth_userokay(username, style, type, password));
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
    { "unveil", lua_unveil },
    { "auth_userokay", lua_userokay },
    { NULL, NULL },
};

int luaopen_openbsd(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, l_pledge, 0);

    return 1;
}
