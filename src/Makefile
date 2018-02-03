all: client server middle

clean:
	rm -f client server middle

client: client.cpp compress_op.h sock_send.h
	g++ -o client client.cpp

server: server.cpp compress_op.h sock_send.h
	g++ -o server server.cpp

middle: middle.cpp compress_op.h sock_send.h
	g++ -o middle middle.cpp -lz
