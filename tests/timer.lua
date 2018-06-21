timer = uv.timer_new()
uv.timer_init(timer)

uv.timer_start(timer, function()
	print("Hello form callback!")
end, 0, 1000)
