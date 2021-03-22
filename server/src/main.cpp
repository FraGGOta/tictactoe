#include "tictaktoe.hpp"

extern int listener;

int main()
{	
	socket_settings();

    pthread_t pid;

	int sock;

    vector<int> clients;

	while(1)
    {
        sock = accept(listener, NULL, NULL);
       
        if(sock < 0)
        {
            perror("accept");
            exit(1);
        }

        clients.push_back(sock);

        if(pthread_create(&pid, NULL, thread_handler, (void *) &clients) < 0)
    	{
        	perror("could not create thread");

        	exit(2);
    	}

        pthread_detach(pid);
    
    }	

    close(sock);

	return 0;
}
