nodelua
=======
 
  **nodelua** jest próbą przeniesienia ideii asynchronicznego interpretera
  opertego o pętle zdarzeń - **nodejs** - do świata Lua.  
  Dokladna dokumentacja znajduje sie w katalogu `docs/`.

Kompilacja oraz uruchomienie
============================

Do skompilowania potrzebne są następujące bilbioteki (Debian):
 * liblua5.3-0
 * liblua5.3-dev
 * libuv1
 * libuv1-dev

Nastepnie w głównym katalogu projektu należy skompilować program
poleceniem `make`.  
Program `nodelua` przyjmuje jako pierwszy argument nazwę pliku z kodem
Lua lub czyta ze standardowego wejścia.  
Przykładowe programy znajdują się w katalogu `tests/`.

Przykładowy program
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

Założenia projektu
===============

   * moduł umożliwiający **komunikację z gniazdami TCP**  
   * moduł odpowiedzialny za **komunikację z plikami**  
   * moduł pozwalający na **cykliczne uruchamanie funkcji w zadanych interwałach czasowych**  
   * obiektowy wrapper na surowy moduł C  
   * prosty asynchroniczny server www demonstrujący możliwości bilbioteki  

