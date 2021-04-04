#include "client.hpp"

int main(int argc, char const **argv)
{
	int sock = socket_settings(SERV_ID, MAIN_PORT);

    client_handler(sock);

    close(sock);
	
	return 0;
}