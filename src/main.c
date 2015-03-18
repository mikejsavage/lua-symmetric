/*
 * Copyright (c) 2015, Michael Savage <mike@mikejsavage.co.uk>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <string.h>

#include <sodium.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#if LUA_VERSION_NUM < 502
	#define luaL_newlib( L, l ) ( lua_newtable( L ), luaL_register( L, NULL, l ) )
#endif

#define stringify_helper( x ) #x
#define stringify( x ) stringify_helper( x )

static int luasymmetric_encrypt( lua_State * const L ) {
	size_t message_len;
	size_t key_len;
	const char * const message = luaL_checklstring( L, 1, &message_len );
	const char * const key = luaL_checklstring( L, 2, &key_len );

	if( key_len != crypto_secretbox_KEYBYTES ) {
		lua_pushliteral( L, "key should be " stringify( crypto_secretbox_KEYBYTES ) " bytes" );
		return lua_error( L );
	}

	const size_t ciphertext_len = message_len + crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES;

	/* overflow */
	if( ciphertext_len < message_len ) {
		lua_pushliteral( L, "message too long" );
		return lua_error( L );
	}

	char * const ciphertext = malloc( ciphertext_len );

	if( ciphertext == NULL ) {
		lua_pushliteral( L, "out of memory" );
		return lua_error( L );
	}

	/* prepend iv */
	arc4random_buf( ciphertext, crypto_secretbox_NONCEBYTES );

	/*
	 * the only failure case is the ciphertext not fitting in memory
	 * but we already tested for that
	 */
	( void ) crypto_secretbox_easy( ciphertext + crypto_secretbox_NONCEBYTES,
		message, message_len, ciphertext, key );

	lua_pushlstring( L, ciphertext, ciphertext_len );

	explicit_bzero( ciphertext, ciphertext_len );
	free( ciphertext );

	return 1;
}

static int luasymmetric_decrypt( lua_State * const L ) {
	size_t ciphertext_len;
	size_t key_len;
	const char * const ciphertext = luaL_checklstring( L, 1, &ciphertext_len );
	const char * const key = luaL_checklstring( L, 2, &key_len );

	if( key_len != crypto_secretbox_KEYBYTES ) {
		lua_pushliteral( L, "key should be " stringify( crypto_secretbox_KEYBYTES ) " bytes" );
		return lua_error( L );
	}

	if( ciphertext_len < crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES ) {
		lua_pushnil( L );
		return 1;
	}

	const size_t message_len = ciphertext_len - crypto_secretbox_NONCEBYTES - crypto_secretbox_MACBYTES;
	char * const message = malloc( message_len );

	if( message == NULL ) {
		lua_pushliteral( L, "out of memory" );
		return lua_error( L );
	}

	const int ok = crypto_secretbox_open_easy( message,
		ciphertext + crypto_secretbox_NONCEBYTES, ciphertext_len - crypto_secretbox_NONCEBYTES,
		ciphertext, key );

	if( ok == 0 ) {
		lua_pushlstring( L, message, message_len );
	}
	else {
		lua_pushnil( L );
	}

	explicit_bzero( message, message_len );
	free( message );

	return 1;
}

static int luasymmetric_key( lua_State * const L ) {
	char key[ crypto_secretbox_KEYBYTES ];
	arc4random_buf( key, sizeof( key ) );

	lua_pushlstring( L, key, sizeof( key ) );
	explicit_bzero( key, sizeof( key ) );

	return 1;
}

static const struct luaL_Reg luasymmetric_lib[] = {
	{ "encrypt", luasymmetric_encrypt },
	{ "decrypt", luasymmetric_decrypt },
	{ "key", luasymmetric_key },
	{ NULL, NULL },
};

LUALIB_API int luaopen_symmetric( lua_State * const L ) {
	luaL_newlib( L, luasymmetric_lib );

	return 1;
}
