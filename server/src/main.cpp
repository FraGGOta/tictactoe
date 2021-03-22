#include "tictaktoe.hpp"

extern char board[3][3];

extern char player;
extern char winner;

extern int movement;
extern int listener;

int main()
{	
	socket_settings();

	int sock;

	pthread_t pid;

	while(1)
    {
        sock = accept(listener, NULL, NULL);
       
        if(sock < 0)
        {
            perror("accept");
            exit(1);
        }

        if(pthread_create(&pid, NULL, thread_handler, (void *) &sock) < 0)
    	{
        	perror("could not create thread");

        	exit(2);
    	}

        pthread_detach(pid);
    
    }	

    close(sock);

	return 0;
}
