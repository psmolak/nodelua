local tcp = require "lib.tcp"

local server = tcp:new()

server:bind("0.0.0.0", 8000)
server:listen(function(server, status)
	if status < 0 then
		-- error, must close the server socket & free memory
		server:close(function(server)
			error("Error occured during listening")
		end)
	end

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
