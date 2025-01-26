#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdlib.h>


/*
 *
 * Interacting with C functions
 *
 */
int min(int a, int b) {
  if (a > b)
    return a;
  else
    return b;
}

int max(int a, int b) {
  if (a < b)
    return a;
  else
    return b;
}

// Lua function that returns both the min and max since Lua functions can return multipler values
static int lua_minMax(lua_State *L) {
  // Get the numbers passed to the function call that should be sitting in the
  // stack Since Lua is dynamically-typed, the value in the stack might not be
  // an integer, so we need to check. If the user passed the wrong type, this
  // function will raise an error
  int a = luaL_checkinteger(L, 1);
  // The 2nd argument resides at the 2nd position on the stack
  int b = luaL_checkinteger(L, 2);

  int _min = min(a, b);
  int _max = max(a, b);

  // Push the return values to the Lua stack from where they will be popped into the Lua state and
  // assigned into any variables accepting the return values
  // Since they'll be popped from the stack, we push them in reverse-order
  
  // Second return value
  lua_pushinteger(L, _max);
  // First return value
  lua_pushinteger(L, _min);

  // Return with the number of values our function returns which is 2
  return 2;
}
// go to line 141 to see how we register this to be usable from the library


/*
 *
 * Interacting with C state using the Lua Userdata type
 *
 */
typedef struct Person {
  const char *name;
  int age;
} Person;

// C constructor for Person
void Person_construct(Person *p, const char *name, int age) {
  if (p == NULL) {
    exit(1);
  }
  p->name = name;
  p->age = age;
}

// Lua interface to C constructor for Person
static int lua_Person_construct(lua_State *L) {
  // Pop the first argument which will be the name
  const char *name = luaL_checkstring(L, 1);
  // Pop the first argument which will be the age
  int age = luaL_checkinteger(L, 2);

  // Register and get the generic (void*) address that the Userdata for our
  // object will reside in `lua_newuserdata` will also handle pushing this into
  // the stack
  Person *p = (Person *)lua_newuserdata(L, sizeof(Person));

  // construct the object
  Person_construct(p, name, age);

  // Returning the single value, the user-datum
  return 1;
}

// C Immutable method for Person
int Person_greet(const Person *p, char *write) {
  return sprintf(write, "Hi, my name is %s and I am %d years old", p->name,
                 p->age);
}

// Lua interface to C Immutable method for Person
static int lua_Person_greet(lua_State *L) {
  char buffer[100];
  Person *p = lua_touserdata(L, 1);
  Person_greet(p, buffer);

  lua_pushstring(L, buffer);
  // Returning the string returned by the `Person_greet` method
  return 1;
}

// C Mutable method for Person
void Person_setAge(Person *p, int newAge) {
  p->age = newAge;
  return;
}

// Lua interface to C Mutable method for Person
static int lua_Person_setAge(lua_State *L) {
  Person *p = lua_touserdata(L, 1);
  int newAge = lua_tointeger(L, 2);
  Person_setAge(p, newAge);

  // Since `Person_setAge` returns nothing
  return 0;
}

// initialize our Person factory by bundling the constructor & methods into a
// single Lua table
static const struct luaL_Reg lua_Person_lib[] = {
    {"new", lua_Person_construct},
    {"greet", lua_Person_greet},
    {"setAge", lua_Person_setAge},
    {NULL, NULL},
};
// Function that will create the table and push it onto the stack
static int lua_Person_table(lua_State *L) {
  lua_newtable(L);
  luaL_setfuncs(L, lua_Person_lib, 0);
  return 1;
}


/*
 *
 * The Entry-point of our library
 *
 */

// The `luaopen_` prefix of this function is important.
// When you `require('myLibraryNameFooBar')` in Lua, it looks for the symbol
// `luaopen_myLibraryNameFooBar` in the file `myLibraryNameFooBar.so`
int luaopen_mylibrary(lua_State *L) {
  lua_newtable(L);
  // Add the entry for `addTwonums` to our library table
  {
    lua_pushstring(L, "minMax");
    lua_pushcfunction(L, lua_minMax);
    lua_settable(L, -3);
  }
  // Add the entry for the `Person` factory to our library table
  {
    lua_pushstring(L, "Person");
    lua_Person_table(L);
    lua_settable(L, -3);
  }
  // Returning the single table for our library
  return 1;
}
