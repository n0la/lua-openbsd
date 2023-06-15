OpenBSD Lua Library

Implements:

 o pledge()
 o arc4random()
 o arc4random_uniform()
 o unveil()
 o auth_userokay()

Works and has been tested on Lua 5.1, 5.2, 5.3 and 5.4.

Note that pledge() pre 6.3 takes an optional set of paths
as the second argument.

 -- OK
 o.pledge('rpath stdio')
 -- Error
 o.pledge('rpath stdio', 'stdio')

Build:

 $ mkdir build && cd build
 $ cmake .. -DMODLUA_VERSION=5.4
 $ make
 $ make install

Usage:

 #!/usr/bin/env lua51

 o = require('openbsd')
 ret, error_string = o.pledge('rpath stdio')
 if ret == -1 then
    error(error_string)
 end

Or:

 #!/usr/bin/env lua54

 o = require('openbsd')

 for i = 1, 10 do
     print(o.arc4random_uniform(20))
 end

Written by Florian Stinglmayr <fstinglmayr@gmail.com>
Copyright (c) 2016, All Rights Reserved
