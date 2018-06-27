local fs = require "lib/filesystem"

fs.open("examples/echo-server.lua", function(file, status)
	if status < 0 then
		error("Error during opening file")
	end

	file:read(function(file, data, nread)
		if nread > 0 then
			fs.open("output.txt", function(file, status)
				if status < 0 then
					error("Error during opening output file")
				end

				file:write({data}, function(file, status)
					if status >= 0 then
						print("Succesfully written to output.txt")
						file:close(function(status)
							if status >= 0 then
								print("Closed output.txt")
							end
						end)
					end
				end)
			end)
		end
	end)
end)
