function callback()
    print("Running callback!")
end

timer, status = l_uv_timer_init()
status = l_uv_timer_start(timer, callback, 0, 1000)
