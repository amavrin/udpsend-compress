#include "app.h"

int main(void) {
	char inbuf[2048];
	char outbuf[2048];
	char *bptr;

	UDPSender us("localhost", 3000);
	UDPReceiver ur(2000);
	Compress c(1024);

	int nrec = 0;
	do {
		bptr = ur.receive();
		nrec = ur.len();
			std::cerr << "received " << nrec << "bytes: " << bptr << std::endl;
			c.compr((const char*)bptr, nrec);
		if (c.clen() > 0) {
			us.send((const char*)c.cdata(), c.clen());
			std::cerr << "send " << c.clen() << "bytes" << std::endl;
		}
	} while (nrec != 0);
	us.send(inbuf,0);
	return 0;
}
