local tcp = require "lib.tcp"

local server = tcp:new()
local clients = {}

local function broadcast(message)
	for _, client in ipairs(clients) do
		client:write({message}, function(client, status)
			if status < 0 then
				print(string.format("Server: failed to broadcast to '%s'", client))
			end
		end)
	end
end

local function disconnect(client)
	local filtered = {}
	for _, c in ipairs(clients) do
		if c ~= client then
			table.insert(filtered, c)
		end
	end
	clients = filtered
	broadcast(string.format("Client '%s' disconnected", client))
	client:close(function(client)
		print(string.format("Server: client '%s' closed the connection", client))
	end)
end

server:bind("0.0.0.0", 8000)
server:listen(function(server, status)
	if status < 0 then
		server:close(function(server)
			error("Error occured during listening")
		end)
	end

	local client = server:accept()
	table.insert(clients, client)
	client:read(function(client, nread, data)
		if nread > 0 then
			print(string.format("Server: client '%s' send a massage", client))
			broadcast(string.format("'%s': %s", client, data))
		elseif nread < 0 then
			disconnect(client)
		end
	end)
end)
