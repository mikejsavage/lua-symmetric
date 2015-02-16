A Lua wrapper for libsodium's secretbox. In simpler terms, a Lua library
for symmetric encryption.


Requirements
------------

[libsodium]: http://doc.libsodium.org/

lua >= 5.1, [libsodium][libsodium]


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

Security concerns
-----------------

Generated keys and ciphertexts will use the full range of ASCII values.
They should be handled with care - displaying them as-is can introduce
subtle flaws. For example, keys and ciphertexts can contain quotes,
which makes them unsafe to insert into SQL queries<sup>1</sup>. If in
doubt, base64/hex encode them. I may change my mind and encode by
default in future (major) releases.

[sql]: http://dc406.com/home/393-sql-injection-with-raw-md5-hashes.html

<sup>1</sup>: [SQL injection with raw MD5 hashes][sql]

Additionally, Lua will keep plaintext messages and encryption keys
around in memory as part of its string interning mechanism. As far as
I'm aware, there's nothing I can do about this.
