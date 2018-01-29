#include <iostream>
#include <stdio.h>

#include <string.h>
#include <assert.h>
#include <zlib.h>

const int B_SIZ = 2048;

class Compress {
	int level;
	z_stream strm;
	unsigned char in[B_SIZ];
	unsigned char out[B_SIZ];
	int cbytes;
	int chunk;

public:
	Compress(int l) {
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		strm.next_in = in;
		int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY);

		if (ret != Z_OK && strm.msg != NULL) std::cerr << strm.msg << std::endl;
		if (ret == Z_MEM_ERROR) {
			error("deflateInit2 memory error", 1);
		}
		if (ret == Z_STREAM_ERROR) {
			error("deflateInit2 parameter invalid", 1);
		}
		if (ret == Z_VERSION_ERROR) {
			error("deflateInit2 version error", 1);
		}

		cbytes = 0;
		chunk = l;
	}

	void compr(const char *data, int n) {
		strm.avail_in = n;
		strm.next_in = in;
		strm.avail_out = chunk;
		strm.next_out = out;
		int ret;
		if (n != 0) {
			bcopy(data, in, n);
			ret = deflate(&strm, Z_PARTIAL_FLUSH);
		} else {
			ret = deflate(&strm, Z_FINISH);
		}
		if (ret == Z_STREAM_ERROR) error("error in deflate", 1);
		cbytes = chunk - strm.avail_out;
	}

	int clen() {
		return cbytes;
	}

	unsigned char *cdata() {
		return out;
	}
};
