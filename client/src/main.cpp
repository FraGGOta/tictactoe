#include "client.hpp"

int main(int argc, char const **argv)
{
	int sock = socket_settings(3425);

    client_handler(sock);
	
	return 0;
}