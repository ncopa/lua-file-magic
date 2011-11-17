/* Copyright Natanael Copa 2011

LGPL

*/

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <magic.h>

#define MODULE_NAME "magic"
#define MODULE_VERSION "0.1"
#define MAGIC_META "magic"

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

static const luaL_reg Pmagic_methods[] = {
	{"open",	Ptodo},
	{"close",	Ptodo},
	{"getpath",	Ptodo},
	{"file",	Ptodo},
	{"descriptor",	Ptodo},
	{"buffer",	Ptodo},
	{"error",	Ptodo},
	{"setflags",	Ptodo},
	{"load",	Ptodo},
	{"compile",	Ptodo},
	{"check",	Ptodo},
	{"list",	Ptodo},
	{"errno",	Ptodo},
	{NULL, 		NULL}
};

static const luaL_reg Pmeta_methods[] = {
	{"__gc",	Ptodo},
	{NULL,		NULL},
};

LUALIB_API int luaopen_magic(lua_State *L)
{
	luaL_openlib(L, MODULE_NAME, Pmagic_methods, 0);
	lua_pushliteral(L, "version");
	lua_pushliteral(L, MODULE_VERSION);
	lua_settable(L, -3);

	init_consts(L);


}

