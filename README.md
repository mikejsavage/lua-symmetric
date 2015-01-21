A Lua wrapper for libsodium's secretbox. In simpler terms, a Lua library
for symmetric encryption.


Requirements
------------

lua >= 5.1


Copying
-------

Many of the files in this repository have been taken from OpenBSD's
tree. You should consult individual file headers for specific licensing
information. More broadly, everything here is compatible with the [ISC
license][ISC].

[ISC]: http://en.wikipedia.org/wiki/ISC_license


Installation
------------

	$ luarocks install bcrypt


Usage
-----

	local symmetric = require( "symmetric" )
	
	-- securely generate a random key
	local key = symmetric.key()

	local message = "hello"
	local ciphertext = symmetric.encrypt( message, key )
	assert( symmetric.decrypt( ciphertext, key ) == message )
