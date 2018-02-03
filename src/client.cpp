#include "app.h"

#include <sysexits.h>

void usage(char *pn) {
	std::cerr << "usage: " << pn << " [-m speed ]\n";
}

int main(int argc, char **argv) {

	int bps = 0;
	int c;

	while ( (c = getopt(argc, argv, "m:")) != -1)
	    switch (c) {
		case 'm':
			bps = atoi(optarg);
			break;
		default:
			usage(argv[0]);
			exit(EX_USAGE);
	}

	UDPSender us("localhost", 2000);
	us.setspeedlim(bps);

	char buf[2048];
	int n;
	while ( (n = read(0,buf,1024)) != 0 ) {
		std::cerr << "client read "<< n << " bytes" << std::endl;
		us.send(buf, n);
	}
	us.send(buf,0);

	return 0;
}
