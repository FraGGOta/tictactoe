#include "client.hpp"

char board[3][3];

char player = 'X';
char winner;

int sock;

void socket_settings(char const *serv_id, uint16_t serv_port)
{
    struct sockaddr_in addr;
    struct hostent *hp;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
  
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(serv_port); 
    
    hp = gethostbyname(serv_id);
    
    bcopy( hp->h_addr, &addr.sin_addr, hp->h_length);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
}

void init_game_field()
{
    for(int i = 0 ; i < 3 ; ++i)
    {
        for(int j = 0 ; j < 3 ; ++j)
        {
            board[i][j] = ' ';
        }
    }
}

void fill_game_field(int i)
{
    for(int j = 0 ; j < 3 ; ++j)
    {
        cout << board[i][j] << "\t";
    }
    cout << endl;
}

