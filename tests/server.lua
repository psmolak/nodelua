function check(status, msg)
	if (status < 0) then
		print(msg)
		abort()
	end
end

function on_alloc(handle, suggested_size, uv_buf_t_ptr)
	-- allocation is handeled in C
end

server = uv.tcp_new()
status = uv.tcp_init(server)
check(status, "Error occured while initializing server")

status = uv.tcp_bind(server, "0.0.0.0", 8000, 0)
check(status, "Error occured while binding addres to server")

status = uv.listen(server, 128, function(request, status)
	if (status < 0) then
		print("Error status code in on_connection callback")
		return
	end

	print("Server: new connection!")

	client = uv.tcp_new()
	status = uv.tcp_init(client)
	check(status, "Error occured while initializing new client")

	uv.accept(server, client)
	
	uv.read_start(client, on_alloc, function(client, nread, data)
		if (nread < 0) then
			uv.close(client, function(handle)
				print("Server: closing client in callback")
			end)
		else
			print(nread, data)
		end
	end)
end)

check(status, "Error occured while starting listening server")
