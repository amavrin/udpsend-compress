#include "../src/sock_send.h"
#include "../src/compress_op.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


class SockTest : public CxxTest::TestSuite {
public:
	void testCanSendUDP() {
		UDPSender us("localhost", 2000);
		UDPReceiver ur(2000);
		char *buf = strdup("abcdef0123456789");

		us.send(buf, strlen(buf));
		char *buf2 = ur.receive();

		int nrec = ur.len();

		TS_ASSERT_EQUALS(strncmp(buf,buf2,nrec), 0);
		TS_ASSERT_EQUALS(strlen(buf), nrec);
	}

	void testCanCompressData() {

		const char *data = "0123456789aaaaaaaaaaaaaaaaaaa.";
		const char *data1= "0123456789";
		const char *data2=           "aaaaaaaaaaaaaaaaaaa.";
		char buf[1024];
		char buf2[1024];

                int d = open("out",O_TRUNC|O_WRONLY);

		Compress c;

		c.compr(data1, strlen(data1));
                int n = write(d,c.cdata(),c.clen());

		c.compr(data2, strlen(data2));
                n = write(d,c.cdata(),c.clen());

		c.finish();
                n = write(d,c.cdata(),c.clen());

                close(d);

		FILE *f;
		if ( (f = popen("/bin/zcat out", "r")) == NULL) {
			std::cerr << "error in popen: " << strerror(errno) << std::endl;
		}
		int nread;
		while ( (n = fread(buf,1,1,f)) != 0) {
			bcopy(buf, buf2 + nread, n);
			nread += n;
			// std::cerr << "fread returns " << nread << " " << feof(f) << " " << ferror(f) << std::endl;
		}

                TS_ASSERT_EQUALS(nread, strlen(data));
                TS_ASSERT_EQUALS(strncmp(data,buf2,nread), 0);
	}
};
