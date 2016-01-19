o = require("openbsd")

-- Same as pledge("rpath", NULL)
ret, s = o.pledge("rpath stdio")
print(ret, s)

-- Same as pledge("rpath", { NULL })
ret, s = o.pledge("rpath stdio", {})
print(ret, s)

-- Same as pledge("rpath", { "/var", "/home", ..., NULL })
ret, s = o.pledge("rpath stdio", {"/var", "/home", "/test", "/more",
                                  "/asdf", "/test", "/stuff", "/meh",
                                  "/alestorm", "/tbdm", "/opeth",
                                  "/atthegates"})
print(ret, s)

print(o.arc4random())

for i = 1, 10 do
   print(o.arc4random_uniform(10))
end
