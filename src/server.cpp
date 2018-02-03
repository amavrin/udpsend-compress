#include "app.h"

int main(void) {
	UDPReceiver ur(3000);
	int nrec = 0;
	do {
		char *bptr = ur.receive();
		nrec = ur.len();
		std::cerr << "server received " << nrec << "bytes" << std::endl;
		write(STDOUT_FILENO,bptr,nrec);
	} while (nrec != 0);
		
	return 0;
}
