/*
 * Author: Tatu Ylonen <ylo@cs.hut.fi>
 * Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
 *                    All rights reserved
 * Functions for manipulating fifo buffers (that can grow if needed).
 *
 * As far as I am concerned, the code I have written for this software
 * can be used freely for any purpose.  Any derived versions of this
 * software must be clearly marked as such, and if the derived work is
 * incompatible with the protocol description in the RFC file, it must be
 * called by a name other than "ssh" or "Secure Shell".
 */

#include "includes.h"
RCSID("$OpenBSD: buffer.c,v 1.21.2.1 2005/03/10 17:15:04 brad Exp $");

#include "xmalloc.h"
#include "buffer.h"
#include "log.h"

/* Initializes the buffer structure. */

void
buffer_init(Buffer *buffer)
{
	const u_int len = 4096;

	buffer->alloc = 0;
	buffer->buf = xmalloc(len);
	buffer->alloc = len;
	buffer->offset = 0;
	buffer->end = 0;
}

/* Frees any memory used for the buffer. */

void
buffer_free(Buffer *buffer)
{
	if (buffer->alloc > 0) {
		memset(buffer->buf, 0, buffer->alloc);
		buffer->alloc = 0;
		xfree(buffer->buf);
	}
}

/*
 * Clears any data from the buffer, making it empty.  This does not actually
 * zero the memory.
 */

void
buffer_clear(Buffer *buffer)
{
	buffer->offset = 0;
	buffer->end = 0;
}

/* Appends data to the buffer, expanding it if necessary. */

void
buffer_append(Buffer *buffer, const void *data, u_int len)
{
	void *p;
	p = buffer_append_space(buffer, len);
	memcpy(p, data, len);
}

/*
 * Appends space to the buffer, expanding the buffer if necessary. This does
 * not actually copy the data into the buffer, but instead returns a pointer
 * to the allocated region.
 */

void *
buffer_append_space(Buffer *buffer, u_int len)
{
	u_int newlen;
	void *p;

	if (len > 0x100000)
		fatal("buffer_append_space: len %u not supported", len);

	/* If the buffer is empty, start using it from the beginning. */
	if (buffer->offset == buffer->end) {
		buffer->offset = 0;
		buffer->end = 0;
	}
restart:
	/* If there is enough space to store all data, store it now. */
	if (buffer->end + len < buffer->alloc) {
		p = buffer->buf + buffer->end;
		buffer->end += len;
		return p;
	}
	/*
	 * If the buffer is quite empty, but all data is at the end, move the
	 * data to the beginning and retry.
	 */
	if (buffer->offset > buffer->alloc / 2) {
		memmove(buffer->buf, buffer->buf + buffer->offset,
			buffer->end - buffer->offset);
		buffer->end -= buffer->offset;
		buffer->offset = 0;
		goto restart;
	}
	/* Increase the size of the buffer and retry. */

	newlen = buffer->alloc + len + 32768;
	if (newlen > 0xa00000)
		fatal("buffer_append_space: alloc %u not supported",
		    newlen);
	buffer->buf = xrealloc(buffer->buf, newlen);
	buffer->alloc = newlen;
	goto restart;
	/* NOTREACHED */
}

/* Returns the number of bytes of data in the buffer. */

u_int
buffer_len(Buffer *buffer)
{
	return buffer->end - buffer->offset;
}

/* Gets data from the beginning of the buffer. */

int
buffer_get_ret(Buffer *buffer, void *buf, u_int len)
{
	if (len > buffer->end - buffer->offset) {
		error("buffer_get_ret: trying to get more bytes %d than in buffer %d",
		    len, buffer->end - buffer->offset);
		return (-1);
	}
	memcpy(buf, buffer->buf + buffer->offset, len);
	buffer->offset += len;
	return (0);
}

void
buffer_get(Buffer *buffer, void *buf, u_int len)
{
	if (buffer_get_ret(buffer, buf, len) == -1)
		fatal("buffer_get: buffer error");
}

/* Consumes the given number of bytes from the beginning of the buffer. */

int
buffer_consume_ret(Buffer *buffer, u_int bytes)
{
	if (bytes > buffer->end - buffer->offset) {
		error("buffer_consume_ret: trying to get more bytes than in buffer");
		return (-1);
	}
	buffer->offset += bytes;
	return (0);
}

void
buffer_consume(Buffer *buffer, u_int bytes)
{
	if (buffer_consume_ret(buffer, bytes) == -1)
		fatal("buffer_consume: buffer error");
}

/* Consumes the given number of bytes from the end of the buffer. */

int
buffer_consume_end_ret(Buffer *buffer, u_int bytes)
{
	if (bytes > buffer->end - buffer->offset)
		return (-1);
	buffer->end -= bytes;
	return (0);
}

void
buffer_consume_end(Buffer *buffer, u_int bytes)
{
	if (buffer_consume_end_ret(buffer, bytes) == -1)
		fatal("buffer_consume_end: trying to get more bytes than in buffer");
}

/* Returns a pointer to the first used byte in the buffer. */

void *
buffer_ptr(Buffer *buffer)
{
	return buffer->buf + buffer->offset;
}

/* Dumps the contents of the buffer to stderr. */

void
buffer_dump(Buffer *buffer)
{
	u_int i;
	u_char *ucp = buffer->buf;

	for (i = buffer->offset; i < buffer->end; i++) {
		fprintf(stderr, "%02x", ucp[i]);
		if ((i-buffer->offset)%16==15)
			fprintf(stderr, "\r\n");
		else if ((i-buffer->offset)%2==1)
			fprintf(stderr, " ");
	}
	fprintf(stderr, "\r\n");
}
