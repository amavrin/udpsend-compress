#include "app.h"

int main(void) {
	UDPSender us("localhost", 2000);
	char buf[2048];
	int n;
	while ( (n = read(0,buf,1024)) != 0 ) {
		std::cerr << "read "<< n << " bytes" << std::endl;
		us.send(buf, n);
	}
	us.send(buf,0);

	return 0;
}
