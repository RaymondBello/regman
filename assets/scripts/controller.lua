-- define a string variable
str = 'Regman';

-- run a loop from 1 to length of str
for i = 1, #str do
   -- get substring of 1 chracter
   local c = str:sub(i,i)
   -- print char
   print(c)
end