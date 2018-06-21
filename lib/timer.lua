local handler = require "lib.handler"
local utils = require "lib.utils"

local Timer = handler:new()

function Timer:new(ptr)
    local object = {}
    self.__index = self
    setmetatable(object, self)
    if not ptr then
        object._ptr = uv.timer_new()
        local status = uv.timer_init(object._ptr)
        utils.assert_status(status, "uv.timer_init")
    else
        object._ptr = ptr
    end
    return object
end

function Timer:delete()
    uv.timer_delete(self._ptr)
end

function Timer:start(rep, callback, timeout)
    assert(type(rep) == "number", "1s argument required to be number")
    utils.assert_cb(callback, 2)
    local status = uv.timer_start(self._ptr, function()
        callback(self)
    end, timeout or 0, rep)
    utils.assert_status(status, "uv.timer_start")
end

function Timer:stop()
    local status = uv.timer_stop(self._ptr)
    utils.assert_status(status, "uv.timer_stop")
end

function Timer:again()
    local status = uv.timer_again(self._ptr) 
    utils.assert_status(status, "uv.timer_again")
end

function Timer:set_delay(delay)
    assert(type(delay) == "number", "1st argument required to be integer")
    uv.timer_set_repeat(self._ptr, delay)
end

function Timer:get_delay()
    return uv.timer_get_repeat(self._ptr)
end

return Timer
