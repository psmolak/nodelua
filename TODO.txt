* Remove lua_State* field from every l_*_d struct and instead store it
  inside uv_loop_t->data.
