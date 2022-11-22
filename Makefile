


all:server_executable client_executable




server_executable:server.o server_lib.o
	g++ server.o server_lib.o -o server_executable

client_executable:client.o
	g++ client.o -o client_executable

server.o:server.cpp server_lib.hpp
	g++ -c server.cpp

server_lib.o:server_lib.cpp
	g++ -c server_lib.cpp

client.o:client.cpp
	g++ -c client.cpp

clean:
	rm *.o server_executable client_executable