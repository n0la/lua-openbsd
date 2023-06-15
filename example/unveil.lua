o = require("openbsd")

print("adding /tmp ...")
print(o.unveil("/tmp", "rw"))
print("adding /var/cache ...")
print(o.unveil("/var/cache", "r"))

-- Remove further calls to unveil
print("disabling further calls to unveil ...")
print(o.unveil(nil, nil))

-- Check if it worked
print("adding /etc (should fail) ...")
print(o.unveil("/etc", "rwx"))

print("testing arg check (should fail) ...")
status, err = pcall(o.unveil, false, 1.0)
print(status, err)
