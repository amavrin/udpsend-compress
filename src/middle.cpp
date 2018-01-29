#include "app.h"

int main(void) {
	char inbuf[2048];
	char outbuf[2048];
	char *bptr;

	UDPSender us("localhost", 3000);
	UDPReceiver ur(2000);
	Compress c;

	int nrec = 0;
	for (;;) {
		bptr = ur.receive();
		nrec = ur.len();
		if (nrec != 0) {
			std::cerr << "received " << nrec << "bytes: " << bptr << std::endl;
			c.compr((const char*)bptr, nrec);
		} else {
			std::cerr << "received 0 bytes, finishing" << std::endl;
			c.finish();
		}
		us.send((const char*)c.cdata(), c.clen());
		std::cerr << "send " << c.clen() << "bytes" << std::endl;
	}
	us.send(inbuf,0);
	return 0;
}
