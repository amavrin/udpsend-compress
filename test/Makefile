all: clean test

test: runner

runner: sock_send_test.h
	cxxtestgen --error-printer -o runner.cpp sock_send_test.h
	g++ -o runner runner.cpp -lz
	./runner

clean:
	rm -f runner out
	echo -n > out
	echo -n > large_out.gz
	rm -f large_out
