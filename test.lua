#!/usr/bin/lua

magic = require("magic")

m = magic.open()
assert(m:load())
print(m:errno())

-- test magic.descriptor() from stdin
if arg[1] == "--stdin" then
	-- stdin = 0
	m:setflags(magic.MIME_ENCODING)
	print(m:descriptor(0))
	os.exit(0)
else
	os.execute("lua test.lua --stdin < Makefile")
end

print(m:file("Makefile"))
m:setflags(magic.MIME_TYPE)
print(m:error())
print(m:file("Makefile"))

m:setflags(magic.NONE)
print(m:buffer("#!/bin/bash\n"))

print(magic.getpath())
