#include "client.hpp"

int main(int argc, char const **argv)
{
	int sock = socket_settings(MAIN_PORT);

    client_handler(sock);
	
	return 0;
}