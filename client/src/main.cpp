#include "client.hpp"

int main(int argc, char const **argv)
{
	assert(argc == 3);

	int sock = socket_settings(argv[1], atoi(argv[2]));

    client_handler(sock);
	
	return 0;
}