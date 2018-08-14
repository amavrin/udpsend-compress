Sample for https://www.freelancer.com/projects/software-architecture/socket-programming-16124407/

There are 3 processes.
1) 'client' sends data at specified speed via UDP
2) 'middle' compresses data and send them to server
3) 'server' recives the data and write it to stdout.

compress_op.h: compress operations
sock_send.h: UDP operations

client.cpp: sample sender, takes data from stdin, send UDP to middle
middle.cpp: receive UDP from client, compress, send UDP to server
server.cpp: receive UDP from middle, writes to stdout

server and middle takes no argument.
client takes an optional atgument -m speed_in_bytes_per_sec

To run the sample, open 3 terminals:
1. ./server > Out.gz
2. ./middle
3. cat some_file | ./client -m 10240

After processes are finished, run "gunzip Out.gz",
theh run "diff some_file Out" and see they equal.

To run tests, cd to test directory and run "make". 
