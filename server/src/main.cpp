#include "tictaktoe.hpp"

int main(int argc, char const **argv)
{	
    int listener, sock;

    vector<int> clients;
    vector<int> opt_servs;

    sockets socks;

    pthread_t pid;

    switch(argc)
    {
        case 1:
            listener = main_server_socket_settings();

        	while(1)
            {
                while(opt_servs.size() != 2)
                {
                    sock = accept(listener, NULL, NULL);
                   
                    if(sock < 0)
                    {
                        perror("accept");
                        exit(1);
                    }

                    opt_servs.push_back(sock);
                }

                sock = accept(listener, NULL, NULL);
               
                if(sock < 0)
                {
                    perror("accept");
                    exit(1);
                }

                clients.push_back(sock);
                
                socks.clients = &clients;
                socks.opt_servs = &opt_servs;

                if(pthread_create(&pid, NULL, main_server_handler, (void *) &socks) < 0)
            	{
                	perror("could not create thread");

                	exit(2);
            	}

                pthread_detach(pid);
            }

        default:
            sock = opt_server_socket_settings(argv[1], atoi(argv[2]));
            opt_server_handler(sock);
    }	

	return 0;
}
