/*
Copyright (c) 2021 Natanael Copa <ncopa@alpinelinux.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <magic.h>

#define MODULE_NAME "magic"
#ifndef MODULE_VERSION
#define MODULE_VERSION "unknown"
#endif
#define LUA_MAGIC_META "magic"

#if LUA_VERSION_NUM < 502
#  define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
#  define luaL_setfuncs(L,l,n) (assert(n==0), luaL_register(L,NULL,l))
#else
static int luaL_typerror (lua_State *L, int narg, const char *tname)
{
	const char *msg = lua_pushfstring(L, "%s expected, got %s",
					  tname, luaL_typename(L, narg));
	return luaL_argerror(L, narg, msg);
}
#endif

struct magic_constmap {
	const char *name;
	int value;
} constmap[] = {
	{ "NONE", MAGIC_NONE },
	{ "DEBUG", MAGIC_DEBUG },
	{ "SYMLINK", MAGIC_SYMLINK },
	{ "COMPRESS", MAGIC_COMPRESS },
	{ "DEVICES", MAGIC_DEVICES },
	{ "MIME_TYPE", MAGIC_MIME_TYPE },
	{ "CONTINUE", MAGIC_CONTINUE },
	{ "CHECK", MAGIC_CHECK },
	{ "PRESERVE_ATIME", MAGIC_PRESERVE_ATIME },
	{ "RAW", MAGIC_RAW },
	{ "ERROR", MAGIC_ERROR },
	{ "MIME_ENCODING", MAGIC_MIME_ENCODING },
	{ "APPLE", MAGIC_APPLE },
	{ "NO_CHECK_COMPRESS", MAGIC_NO_CHECK_COMPRESS },
	{ "NO_CHECK_TAR", MAGIC_NO_CHECK_TAR },
	{ "NO_CHECK_SOFT", MAGIC_NO_CHECK_SOFT },
	{ "NO_CHECK_APPTYPE", MAGIC_NO_CHECK_APPTYPE },
	{ "NO_CHECK_ELF", MAGIC_NO_CHECK_ELF },
	{ "NO_CHECK_TEXT", MAGIC_NO_CHECK_TEXT },
	{ "NO_CHECK_CDF", MAGIC_NO_CHECK_CDF },
	{ "NO_CHECK_TOKENS", MAGIC_NO_CHECK_TOKENS },
	{ "NO_CHECK_ENCODING", MAGIC_NO_CHECK_ENCODING },
	{ "NO_CHECK_BUILTIN", MAGIC_NO_CHECK_BUILTIN },
	{ "NO_CHECK_TEXT", MAGIC_NO_CHECK_TEXT },
	{ "NO_CHECK_FORTRAN", MAGIC_NO_CHECK_FORTRAN },
	{ "NO_CHECK_TROFF", MAGIC_NO_CHECK_TROFF },
	{ NULL, 0},
};

static void init_consts(lua_State *L)
{
	int i;
	for (i = 0; constmap[i].name != NULL; i++) {
		lua_pushstring(L, constmap[i].name);
		lua_pushnumber(L, constmap[i].value);
		lua_settable(L, -3);
	}
}

#if 0
static int Ptodo(lua_State *L)
{
	printf("todo\n");
	return 0;
}
#endif

static int Popen(lua_State *L)
{
	magic_t *h;
	int flags = luaL_optinteger(L, 1, MAGIC_NONE);

	h = lua_newuserdata(L, sizeof(magic_t));
	luaL_getmetatable(L, LUA_MAGIC_META);
	lua_setmetatable(L, -2);
	*h = magic_open(flags);
	if (*h == NULL)
		luaL_error(L, "magic_open failed");
	return 1;
}

static magic_t Pmagic_checkarg(lua_State *L, int index)
{
	magic_t *m;
	luaL_checktype(L, index, LUA_TUSERDATA);
	m = (magic_t *) luaL_checkudata(L, index, LUA_MAGIC_META);
	if (m == NULL)
		luaL_typerror(L, index, LUA_MAGIC_META);
	return *m;
}

static int Pclose(lua_State *L)
{
	magic_t m = Pmagic_checkarg(L, 1);
	if (m)
		magic_close(m);
	m = NULL;
	return 0;
}

static int Pfile(lua_State *L)
{
	magic_t m = Pmagic_checkarg(L, 1);
	const char *filename = luaL_checkstring(L, 2);

	lua_pushstring(L, magic_file(m, filename));
	return 1;
}

static int Perror(lua_State *L)
{
	magic_t m = Pmagic_checkarg(L, 1);
	lua_pushstring(L, magic_error(m));
	return 1;
}

static int Perrno(lua_State *L)
{
	magic_t m = Pmagic_checkarg(L, 1);
	int err = magic_errno(m);
	lua_pushinteger(L, err);
	lua_pushstring(L, strerror(err));
	return 2;
}

static int Pload(lua_State *L)
{
	magic_t m = Pmagic_checkarg(L, 1);
	const char *filename = luaL_optstring(L, 2, NULL);
	if (magic_load(m, filename)==0) {
		lua_pushboolean(L,1);
		return 1;
	} else {
		lua_pushnil(L);
		lua_pushstring(L,  magic_error(m));
		return 2;
	}
}

static int Psetflags(lua_State *L)
{
	magic_t m = Pmagic_checkarg(L, 1);
	int flags = luaL_checknumber(L, 2);
	if (magic_setflags(m, flags)) {
		lua_pushboolean(L,1);
		return 1;
	} else {
		lua_pushnil(L);
		lua_pushstring(L, magic_error(m));
		return 2;
	}
}

static int Pdescriptor(lua_State *L)
{
	magic_t m = Pmagic_checkarg(L, 1);
	int fd = luaL_checknumber(L, 2);
	lua_pushstring(L, magic_descriptor(m, fd));
	return 1;
}

static int Pbuffer(lua_State *L)
{
	size_t size;
	magic_t m = Pmagic_checkarg(L, 1);
	const char *buf = luaL_checklstring(L, 2, &size);
	lua_pushstring(L, magic_buffer(m, buf, size));
	return 1;
}

static int Pgetpath(lua_State *L)
{
	// from file.h in 'file' sources
	#define FILE_LOAD 0
	const char *magicfile = luaL_optstring(L, 1, NULL);
	lua_pushstring(L, magic_getpath(magicfile, FILE_LOAD));
	return 1;
	#undef FILE_LOAD
}

static const luaL_Reg Pmagic_methods[] = {
	{"open",	Popen},
	{"close",	Pclose},
	{"getpath",	Pgetpath},
	{"file",	Pfile},
	{"descriptor",	Pdescriptor},
	{"buffer",	Pbuffer},
	{"error",	Perror},
	{"setflags",	Psetflags},
	{"load",	Pload},
/*	{"compile",	Ptodo},
	{"check",	Ptodo},
	{"list",	Ptodo},
*/
	{"errno",	Perrno},
	{NULL,		NULL}
};

static const luaL_Reg Pmeta_methods[] = {
	{"__gc",	Pclose},
	{NULL,		NULL},
};

LUALIB_API int luaopen_magic(lua_State *L)
{
	luaL_newlib(L, Pmagic_methods);
	lua_pushliteral(L, "version");
	lua_pushliteral(L, MODULE_VERSION);
	lua_settable(L, -3);

	init_consts(L);

	luaL_newmetatable(L, LUA_MAGIC_META);
	luaL_setfuncs(L, Pmeta_methods, 0);
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);
	lua_rawset(L, -3);
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	return 1;
}

