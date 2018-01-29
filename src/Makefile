all: client server middle

clean:
	rm -f client server middle

client: client.cpp
	g++ -o client client.cpp -lz

server: server.cpp
	g++ -o server server.cpp -lz

middle: middle.cpp
	g++ -o middle middle.cpp -lz
