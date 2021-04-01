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
            listener = main_server_socket_settings(3425);

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
                sock = opt_server_socket_settings(3425);
                
                listen_main_server_by_opt_server(sock);

                listener = main_server_socket_settings(3426);

                while(1)
                {
                    while(opt_servs.size() != 1)
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
                sock = opt_server_socket_settings(3425);
                listen_main_server_by_opt_server(sock);

                sock = opt_server_socket_settings(3426);
                
                if (sock != -1)
                    listen_main_server_by_opt_server(sock);

                listener = main_server_socket_settings(3427);

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
