#!/usr/bin/lua

magic = require("magic")

m = magic.open()

assert(m:load())

print(m:errno())
print(m:file("Makefile"))

m:setflags(magic.MIME_TYPE)
print(m:error())

print(m:file("Makefile"))

