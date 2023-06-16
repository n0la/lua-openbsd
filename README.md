# OpenBSD Lua Library

Implements:

* pledge()
* arc4random()
* arc4random_uniform()
* unveil()
* auth_userokay()

Works and has been tested on Lua 5.1, 5.2, 5.3 and 5.4.

Note that pledge() pre 6.3 takes an optional set of paths as the second
argument.

```lua
-- OK
o.pledge('rpath stdio')
-- Error
o.pledge('rpath stdio', 'stdio')
```

## Build

Don't forget to set the `MODLUA_VERSION` variable to the Lua version you are
using.

```
$ mkdir build && cd build
$ cmake .. -DMODLUA_VERSION=5.4
$ make
$ make install
```

## Usage

```lua
#!/usr/bin/env lua51

o = require('openbsd')
ret, error_string = o.pledge('rpath stdio')
if ret == -1 then
   error(error_string)
end
```

Or:

```
#!/usr/bin/env lua54

o = require('openbsd')

for i = 1, 10 do
    print(o.arc4random_uniform(20))
end
```

## Author

Written by Florian Stinglmayr <fstinglmayr@gmail.com>
Copyright (c) 2016, All Rights Reserved
