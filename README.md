lua-file-magic
==============

Lua binding to libmagic.

Example usage:

```
magic = require("magic")

cookie = magic.open()
assert(cookie:load())


cookie:setflags(magic.MIME_TYPE)
print("mime type of "..arg[1].." is "..cookie:file(arg[1]))
```

See [libmagic manpage][] for more details.


[libmagic manpage]: http://linux.die.net/man/3/libmagic
