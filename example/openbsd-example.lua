o = require("openbsd")

-- Same as pledge("rpath", NULL)
ret, s = o.pledge("rpath stdio")
print(ret, s)

-- Same as pledge("rpath", { NULL })
ret, s = o.pledge("rpath stdio", {})
print(ret, s)

-- Same as pledge("rpath stdio wpath", "rpath stdio")
ret, s = o.pledge("rpath stdio wpath", "rpath stdio")

print(ret, s)

print(o.arc4random())

for i = 1, 10 do
   print(o.arc4random_uniform(10))
end
