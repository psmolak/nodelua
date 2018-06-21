local handler = require "lib.handler"
local request = require "lib.request"
local utils = require "lib.utils"

local Stream = handler:new()
local BACKLOG = 128

function Stream:new()
    local class = {}
    self.__index = self
    setmetatable(class, self)
    return class
end

function Stream:shutdown(callback)
    utils.assert_cb(callback, 1)
    local req = request.Shutdown:new()
    local status = uv.shutdown(req._ptr, self._ptr, function(_, status) 
        callback(self, status)
        req:delete()
    end)
    utils.assert_status(status, "uv.shutdown")
    return self
end

function Stream:listen(callback)
    utils.assert_cb(callback, 1)
    local status = uv.listen(self._ptr, BACKLOG, function(sever, status)
        callback(self, status)
    end)
    utils.assert_status(status, "uv.listen")
    return self
end

function Stream:accept()
    -- Subclass specific function
    error("This method is implemented in subclasses and must not be called")
end

function Stream:read(callback)
    utils.assert_cb(callback, 1)
    local alloc = function() end
    local status = uv.read_start(self._ptr, alloc, function(stream, nread, data)
        -- The callback is responsible for stopping closing the stream when
        -- an error happens by calling uv_read_stop() or uv_close().
        callback(self, nread, data)
    end)
    utils.assert_status(status, "uv.read_stop")
    return self
end

function Stream:stop()
    local status = uv.read_stop(self._ptr)
    utils.assert_status(status, "uv.read_stop")
    return self
end

function Stream:write(data, callback)
    utils.assert_cb(callback, 2)
    assert(type(data) == "table", "1st argument required to be table of data (strings)")
    for i = 1, #data do
        assert(type(data[i]) == "string", string.format("%d argument of data table must be string", i))
    end

    local req = request.write:new()
    local status = uv.write(req._ptr, self._ptr, data, #data, function(req_, status)
        callback(self, status)
        req:delete()
    end)
    utils.assert_status(status, "uv.write")
    return self
end

function Stream:readable()
    return uv.is_readable(self._ptr) == 1
end

function Stream:writable()
    return uv.is_writable(self._ptr) == 1
end

return Stream
