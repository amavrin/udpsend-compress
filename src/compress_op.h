#include <iostream>
#include <stdio.h>

#include <string.h>
#include <assert.h>
#include <zlib.h>

const int IBUFSIZ = 2048;
const int OBUFSIZ = 2048;
const int ZLIB_PREFIX_SZ = 2;
const int ZLIB_SUFFIX_SZ = 4;

const int GZIP_PREFIX_SZ = 10;
const int GZIP_SUFFIX_SZ = 8;
const char gzipheader[] = {
    0x1f, 0x8b      // gzip magic number
    , 8             // compress method "defalte"
    , 1             // text data
    , 0, 0, 0, 0    // timestamp is not set
    , 2             // maximum compression flag
    , 255           // unknown OS
};



class Compress {
	int level;
	z_stream strm;
	unsigned char in[IBUFSIZ];
	unsigned char out[OBUFSIZ];
	int cbytes;
	int headerpatch;
	int tailpatch;
	uint32_t crc;
	uint32_t totalb;

public:
	Compress() {
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
		if (ret != Z_OK) error("error in deflateInit", 1);
		cbytes = 0;
		headerpatch = 1;
		tailpatch = 0;
		crc = 0;
		totalb = 0;
	}

	void compr(const char *data, int n) {
		unsigned char *outptr = out;
		// https://habrahabr.ru/post/133176/
		if (headerpatch) {
			outptr += GZIP_PREFIX_SZ - ZLIB_PREFIX_SZ;
		}

		bcopy(data, in, n);
		strm.avail_in = n;
		strm.next_in = in;
		strm.avail_out = OBUFSIZ;
		strm.next_out = outptr;
		int ret = deflate(&strm, Z_NO_FLUSH);
		cbytes = OBUFSIZ - strm.avail_out;
		if (ret == Z_STREAM_ERROR) error("error in deflate", 1);

		if (headerpatch) {
			bcopy(gzipheader,out,GZIP_PREFIX_SZ);
			cbytes += GZIP_PREFIX_SZ - ZLIB_PREFIX_SZ;
			headerpatch = 0;
		}

		crc = crc32(crc,in,n);
		totalb += n;
	}

	void finish() {
		strm.avail_in = 0;
		strm.next_in = in;
		strm.avail_out = OBUFSIZ;
		strm.next_out = out;
		int ret = deflate(&strm, Z_FINISH);
		cbytes = OBUFSIZ - strm.avail_out - ZLIB_SUFFIX_SZ;

		bcopy(&crc, out+cbytes, sizeof(crc));
		cbytes += sizeof(crc);
		bcopy(&totalb, out+cbytes, sizeof(totalb));
		cbytes += sizeof(totalb);
		
	}

	int clen() {
		return cbytes;
	}

	unsigned char *cdata() {
		return out;
	}
};
