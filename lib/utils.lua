local function assert_cb(cb, arg)
    if type(cb) ~= "function" then
        error(string.format("%dst argument is required to be function", arg))
    end
end

local function assert_status(status, arg)
    if status < 0 then
        error(string.format("Error occured during %s", arg))
    end
end

return {
    assert_cb = assert_cb,
    assert_status = assert_status
}
