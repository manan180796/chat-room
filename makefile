all: client server

client: client.cpp Channel.cpp Channel.hpp 
	g++ client.cpp Channel.cpp -o client -pthread

server: server.cpp Channel.cpp Channel.hpp 
	g++ server.cpp Channel.cpp -o server -pthread