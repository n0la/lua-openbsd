o = require("openbsd")

-- Test out unveil
ret, errstr = o.unveil(".", "rwx")
print("unveil:", ret, errstr)

-- Same as pledge("rpath", NULL)
ret, s = o.pledge("rpath stdio")
print("pledge:", ret, s)

-- Same as pledge("rpath stdio wpath", "rpath stdio")
ret, s = o.pledge("rpath stdio wpath", "rpath stdio")
print("pledge:", ret, s)

print(o.arc4random())

for i = 1, 10 do
   print(o.arc4random_uniform(10))
end
