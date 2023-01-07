o = require("openbsd")

-- lua userokay.lua <user> <style>
print(o.auth_userokay(arg[1], arg[2], "", io.read()))
