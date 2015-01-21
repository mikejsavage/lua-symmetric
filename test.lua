#! /usr/bin/lua

local symmetric = require( "symmetric" )

local function tohex( str )
	return ( str:gsub( "(.)", function( c )
		return ( "%02x" ):format( string.byte( c ) )
	end ) )
end

local key = symmetric.key()
print( tohex( key ) )

local c = symmetric.encrypt( "hello", key )
print( tohex( c ) )

print( symmetric.decrypt( c, key ) )

print( symmetric.decrypt( "1", key ) )

print( pcall( symmetric.encrypt, nil, key ) )
print( pcall( symmetric.decrypt, nil, key ) )
