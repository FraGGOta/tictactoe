#include <signal.h>
#include "tictaktoe.hpp"

int main(int argc, char const **argv)
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, NULL);
    
    int listener, sock, sock_1;

    vector<int> clients;
    vector<int> opt_servs;

    sockets socks;

    pthread_t pid;

    switch(argc)
    {
        case 1:
            listener = server_socket_settings(SERV_ID, MAIN_PORT);

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

        	while(1)
            {
                sock = accept(listener, NULL, NULL);
               
                if(sock < 0)
                {
                    perror("accept");
                    exit(1);
                }

                clients.push_back(sock);
                
                socks.clients = &clients;
                socks.opt_servs = &opt_servs;

                if(pthread_create(&pid, NULL, main_server_handler, (void *)&socks) < 0)
            	{
                	perror("could not create thread");

                	exit(2);
            	}

                pthread_detach(pid);
            }

        default:
            string param = argv[1];

            if (param == "--opt_serv_1")
            {
                listener = server_socket_settings(SERV_ID, MAIN_PORT + 1);
                
                sock = client_socket_settings(SERV_ID, MAIN_PORT);
          
                while(opt_servs.size() != 1)
                {
                   sock_1 = accept(listener, NULL, NULL);
                   
                    if(sock_1 < 0)
                    {
                        perror("accept");
                        exit(1);
                    }

                    opt_servs.push_back(sock_1);
                }

                if(sock != -1)
                    listen_current_server(sock);

                while(1)
                {
                    sock = accept(listener, NULL, NULL);
                   
                    if(sock < 0)
                    {
                        perror("accept");
                        exit(1);
                    }

                    clients.push_back(sock);

                    socks.clients = &clients;
                    socks.opt_servs = &opt_servs;
            
                    if(pthread_create(&pid, NULL, opt_server_handler, (void *)&socks) < 0)
                    {
                        perror("could not create thread");

                        exit(2);
                    }

                    pthread_detach(pid);
                }
            }
            else if (param == "--opt_serv_2")
            {
                listener = server_socket_settings(SERV_ID, MAIN_PORT + 2);
                
                sock = client_socket_settings(SERV_ID, MAIN_PORT);
                sock_1 = client_socket_settings(SERV_ID, MAIN_PORT + 1);
                
                if (sock != -1)
                    listen_current_server(sock);          
                
                if (sock_1 != -1)
                    listen_current_server(sock_1);

                while(1)
                {
                    sock = accept(listener, NULL, NULL);
                   
                    if(sock < 0)
                    {
                        perror("accept");
                        exit(1);
                    }

                    clients.push_back(sock);
                      
                    socks.clients = &clients;
                    socks.opt_servs = nullptr;

                    if(pthread_create(&pid, NULL, opt_server_handler, (void *)&socks) < 0)
                    {
                        perror("could not create thread");

                        exit(2);
                    }

                    pthread_detach(pid);
                }
            }
    }	

	return 0;
}
