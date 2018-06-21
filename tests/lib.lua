local request = require "lib.request"
local handler = require "lib.handler"
local stream = require "lib.stream"
local timer = require "lib.timer"
local utils = require "lib.utils"
local tcp = require "lib.tcp"

print(request)
print(handler)
print(stream)
print(timer)
print(utils)
print(tcp)

local server = tcp:new()

server:bind("0.0.0.0", 8000)
server:listen(function(server, status)
	if status < 0 then
		-- error, must close the server socket & free memory
		server:close(function(server)
			server:delete()
			error("Error occured during listening")
		end)
	end

	local client = server:accept()
	client:read(function(client, nread, data)
		if nread > 0 then
			print("Got data from client")
			print(data)
		elseif nread < 0 then
			client:close(function(client)
				print(string.format("Closing client due to error on read"))
				client:delete()
			end)
		end
	end)
end)
