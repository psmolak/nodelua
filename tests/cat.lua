request = uv.fs_req_new()
status = uv.fs_open(request, "tests/server.lua", 0, 0, function(request, fd)
    -- on open callback
    if (fd < 0) then
        print("File descriptor number less than zero!")
    else
        uv.fs_read(request, fd, {}, 1, -1, function(request, data, nread)
            if (nread == 0) then
                printf("End of file")
                uv.fs_req_cleanup(request)
                uv.fs_req_delete(request)
            elseif (nread > 0) then
                print(data)
            else
                printf("Error while reading file")
            end
        end)
    end
end)
