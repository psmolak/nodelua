local Handler = {}

function Handler:new()
	local class = {}	
	self.__index = self
	setmetatable(class, self)
	return class
end

function Handler:active()
	return uv.is_active(self._ptr) > 0
end

function Handler:closing()
	return uv.is_closing(self._ptr) > 0
end

function Handler:close(callback)
	if type(callback) ~= "function" then
		error("1st argument required to be function")
	end
	uv.close(self._ptr, function(handle)
        callback(self)
    end)
end

function Handler:reference()
	uv.ref(self._ptr)
end

function Handler:unreference()
	uv.unfer(self._ptr)
end

function Handler:referenced()
	return uv.has_ref(self._ptr) > 0
end

return Handler
