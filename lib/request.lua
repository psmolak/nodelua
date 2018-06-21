local Request = {
    _ptr = nil
}

function Request:new()
    local class = {}
    self.__index = self
    setmetatable(class, self)
    return class
end

function Request:cancel()
    local status = uv.req_cancel(self._ptr)
    if (status < 0) then
        error("Fatal error occured during req_cancel (uv_cancel)")
    end
end

local Write = Request:new()

function Write:new(ptr)
    local object = {}
    self.__index = self
    setmetatable(object, self)
    object._ptr = ptr or uv.write_req_new()
    return object
end

function Write:delete()
    uv.write_req_delete(self._ptr)
end

local Shutdown = Request:new()

function Shutdown:new(ptr)
    local object = {}
    self.__index = self
    setmetatable(object, self)
    object._ptr = ptr or uv.shutdown_req_new()
    return object
end

function Shutdown:delete()
    uv.shutdown_req_delete(self._ptr)
end

local Connect = Request:new()

function Connect:new(ptr)
    local object = {}
    self.__index = self
    setmetatable(object, self)
    self._ptr = ptr or uv.connect_req_new()
    return object
end

function Connect:delete()
    uv.connect_req_delete(self._ptr)
end

return {
    Write = Write,
    Shutdown = Shutdown,
    Connect = Connect
}

