/* Copyright Natanael Copa 2011

LGPL

*/

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <magic.h>

#define MODULE_NAME "magic"
#define MODULE_VERSION "0.1"
#define LUA_MAGIC_META "magic"

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

static int Ptodo(lua_State *L)
{
	printf("todo\n");
	return 0;
}

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

static int Pload(lua_State *L)
{
	magic_t m = Pmagic_checkarg(L, 1);
	char *filename = luaL_optstring(L, 2, NULL);
	lua_pushstring(L, magic_load(m, filename));
	return 1;
}

static const luaL_reg Pmagic_methods[] = {
	{"open",	Popen},
	{"close",	Pclose},
	{"getpath",	Ptodo},
	{"file",	Pfile},
	{"descriptor",	Ptodo},
	{"buffer",	Ptodo},
	{"error",	Perror},
	{"setflags",	Ptodo},
	{"load",	Pload},
	{"compile",	Ptodo},
	{"check",	Ptodo},
	{"list",	Ptodo},
	{"errno",	Ptodo},
	{NULL, 		NULL}
};

static const luaL_reg Pmeta_methods[] = {
	{"__gc",	Pclose},
	{NULL,		NULL},
};

LUALIB_API int luaopen_magic(lua_State *L)
{
	luaL_openlib(L, MODULE_NAME, Pmagic_methods, 0);
	lua_pushliteral(L, "version");
	lua_pushliteral(L, MODULE_VERSION);
	lua_settable(L, -3);

	init_consts(L);

	luaL_newmetatable(L, LUA_MAGIC_META);
	luaL_register(L, NULL, Pmeta_methods);
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);
	lua_rawset(L, -3);
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);
	lua_rawset(L, -3);
	lua_pop(L, 1);

	return 1;
}

