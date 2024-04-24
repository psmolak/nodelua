nodelua
=======
 
  **nodelua** is an attempt to bring the idea of asynchronous programming
  based on event loop like **nodejs** into the realm Lua programming lnagugae.

  For more details please refer to documentation inside `docs/`.

Compilation & running
============================

You need the following dependencies in order to compile the code:
 * liblua5.3-0
 * liblua5.3-dev
 * libuv1
 * libuv1-dev

then in the main directory run `make`.

Usage
=====

`nodelua [<file>]`
If no `<file>` is provided, the program reads from its stdin.

You can find more usage examples inside `tests/` directory.

Example
==================

```lua
local tcp = require "lib.tcp"

server = tcp:new()
server:bind("0.0.0.0", 8000)

server:listen(function(server, status)
   -- error, must close the server socket & free memory
   if status < 0 then
      server:close(function(server)
         error("Error occured during listening")
      end)
   end

   -- otherwise accept new client
   local client = server:accept()
   client:read(function(client, nread, data)
      if nread > 0 then
         print(string.format("Client '%s' said: %s", client, data))
      elseif nread < 0 then
         client:close(function(client)
            print(string.format("Client '%s' closed the connection", client))
         end)
      end
   end)
end)
```

