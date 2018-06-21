local request = require "lib.request"
local stream = require "lib.stream"
local utils = require "lib.utils"

local Tcp = stream:new()

function Tcp:new(ptr)
    local object = {}
    self.__index = self
    setmetatable(object, self)
    if not ptr then
        object._ptr = uv.tcp_new()
        local status = uv.tcp_init(object._ptr)
        utils.assert_status(status, "uv.tcp_init")
    else
        object._ptr = ptr
    end
    return object
end

function Tcp:delete()
    uv.tcp_delete(self._ptr)
end

function Tcp:keepalive(enable, delay)
    local status = uv.tcp_keepalive(self._ptr, enable and 1 or 0, delay)
    utils.assert_status(status, "uv.tcp_keepalive")
end

function Tcp:bind(ip, port, flags)
    assert(type(ip) == 'string', "1st argument required to be string")
    assert(type(port) == 'number', "2nd argument required to be string")
    local status = uv.tcp_bind(self._ptr, ip, port, flags or 0)
    utils.assert_status(status, "uv.tcp_bind")
end

function Tcp:connect(ip, port, callback)
    utils.assert_cb(callback, 3)
    assert(type(ip) == 'string', "1st argument rquired to be string")
    assert(type(port) == 'number', "2nd argument required to be string")
    local req = request.connect:new()
    local status = uv.tcp_connect(req._ptr, self._ptr, ip, port, function(req_, status)
        callback(self, status)
        req:delete()
    end)
    utils.assert_status(status, "uv.tcp_connect")
end

function Tcp:accept()
    local client = Tcp:new()
    local status = uv.accept(self._ptr, client._ptr)
    utils.assert_status(status, "uv.accept in Tcp:accept")
    return client
end

return Tcp
