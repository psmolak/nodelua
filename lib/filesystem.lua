local utils = require "lib.utils"

local File = {
	_fd = nil
}

local function open(path, callback)
	assert(type(path) == 'string', '1st argument required to be string')
	utils.assert_cb(callback, 2)
	local req = uv.fs_req_new()
	local status = uv.fs_open(req, path, 0, 0, function(request, fd)
		local file = File:new()
		file.path = path
		file._fd = fd
		callback(file, fd)
		uv.fs_req_cleanup(req)
		uv.fs_req_delete(req)
	end)
	utils.assert_status(status, "uv.fs_open")
end

function File:new()
	local object = {}
	self.__index = self
	setmetatable(object, self)
	return object
end

function File:read(callback)
	utils.assert_cb(callback, 1)
	local req = uv.fs_req_new()
	local status = uv.fs_read(req, self._fd, {}, 1, -1, function(request, data, nread)
		callback(self, data, nread)
		if nread == 0 then
			uv.fs_req_cleanup(req)
			uv.fs_req_delete(req)
		end
	end)
	utils.assert_status(status, "uv.fs_read")
end

function File:write(data, callback)
	utils.assert_cb(callback, 2)
	local req = uv.fs_req_new()
	local status = uv.fs_write(req, self._fd, data, #data, -1, function(request, status)
		callback(self, status)
		uv.fs_req_cleanup(req)
		uv.fs_req_delete(req)
	end)
	utils.assert_status(status, "uv.fs_write")
end

function File:close(callback)
	utils.assert_cb(callback, 1)
	local req = uv.fs_req_new()
	local status = uv.fs_close(req, self._fd, function(request, status)
		callback(status)
		uv.fs_req_cleanup(req)
		uv.fs_req_delete(req)
	end)
	utils.assert_status(status, "uv.fs_close")
end

return {
	open = open
}
