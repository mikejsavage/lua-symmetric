package = "symmetric"
version = "scm-1"

source = {
	url = "git://github.com/mikejsavage/lua-symmetric.git",
}

description = {
	summary = "A Lua module for symmetric cryptography",
	homepage = "http://github.com/mikejsavage/lua-symmetric",
	license = "ISC",
	maintainer = "Mike Savage",
}

dependencies = {
	"lua >= 5.1",
}

build = {
	type = "make",

	install_pass = false,

	build_variables = {
		LUA_INCDIR = "$(LUA_INCDIR)",
	},

	install = {
		lib = {
			[ "symmetric" ] = "symmetric.so",
		},
	},
}
