all: clean test

test: runner

runner: sock_send_test.h
	cxxtestgen --error-printer -o runner.cpp sock_send_test.h
	g++ -o runner runner.cpp -lz
	touch out
	./runner

clean:
	rm -f runner out
