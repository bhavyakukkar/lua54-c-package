mylibrary = require('mylibrary')
Person = mylibrary.Person


--[[
--   Function
--]]
a, b = 41, 33
min, max = mylibrary.minMax(a, b)
s = 'Between %d and %d, min = %d and max = %d'
print(s:format(a, b, min, max)) -- Between 41 and 33, min = 33 and max = 41


--[[
--   Userdata
--]]
local p = Person.new("John", 23)
print(Person.greet(p)) -- Hi, my name is John and I am 23 years old

Person.setAge(p, 25)
print(Person.greet(p)) -- Hi, my name is John and I am 25 years old
