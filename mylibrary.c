#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

// #ifdef LUA

// static int l_sin(lua_State *L) {
//   double d = lua_tonumber(L, 1); /* get argument */
//   lua_pushnumber(L, sin(d));     /* push result */
//   return 1;                      /* number of results */
// }

static int l_dir(lua_State *L) {
  DIR *dir;
  struct dirent *entry;
  int i;
  const char *path = luaL_checkstring(L, 1);

  /* open directory */
  dir = opendir(path);
  if (dir == NULL) {                    /* error opening the directory? */
    lua_pushnil(L);                     /* return nil and ... */
    lua_pushstring(L, strerror(errno)); /* error message */
    return 2;                           /* number of results */
  }

  /* create result table */
  lua_newtable(L);
  i = 1;
  while ((entry = readdir(dir)) != NULL) {
    lua_pushnumber(L, i++);           /* push key */
    lua_pushstring(L, entry->d_name); /* push value */
    lua_settable(L, -3);
  }

  closedir(dir);
  return 1;
}

static const struct luaL_Reg mylib[] = {
    {"dir", l_dir}, {NULL, NULL} /* sentinel */
};

int mylibrary(lua_State *L) {
  lua_pushcfunction(L, l_dir);
  lua_setglobal(L, "dir");
  // luaL_openlib(L, "mylib", mylib, 0);
  return 1;
}

// #endif
