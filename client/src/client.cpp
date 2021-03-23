#include "client.hpp"

char board[3][3];

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

void client_handler(void)
{
    init_game_field();

    char sign, winner;

    int row, col;

    int movement = 0, game_over = 0;

    bool is_val_3 = false;
    bool is_val_1 = false;
    bool is_val_2 = false;
    bool is_avl_sign = true;
    bool is_end = false;

    system("clear");
   
    cout << "Choose X/O" << endl << endl;

    do
    {
        cin >> sign;

        sign = toupper(sign);

        send(sock, &sign, sizeof(sign), 0);

        recv(sock, &is_avl_sign, sizeof(is_avl_sign), 0);

        if (!is_avl_sign)
        {
            cout << endl << sign << " has already been choosen" << endl << endl;
        }
    }
    while(!is_avl_sign);

    while(1)
    { 
        recv(sock, &game_over, sizeof(game_over), 0);

        if (game_over == 1)
        {
            system("clear");

            print_game_board();

            recv(sock, &winner, sizeof(winner), 0);

            cout << endl << "Player " << winner << " won" << endl;

            break;
        }
        else if (game_over == 2)
        {
            system("clear");

            print_game_board();

            cout << endl << "Draw" << endl;

            break;
        }

       system("clear");

        cout << sign << " field" << endl << endl;

        print_game_board();
       
       do
       {
            recv(sock, &is_end, sizeof(is_end), 0);

            if (is_end)
            {
                break;
            }

            cout << endl;

            cin >> row >> col;
            
            send(sock, &row, sizeof(row), 0);
            send(sock, &col, sizeof(col), 0);
        
            recv(sock, &is_val_1, sizeof(is_val_1), 0);
            recv(sock, &is_val_2, sizeof(is_val_2), 0);
            recv(sock, &is_val_3, sizeof(is_val_3), 0);

            if (!is_val_1 || !is_val_2)
            {
               cout << endl << "Please pick a value between 1 and 3" << endl;
            }

            if (!is_val_3)
            {
                cout << endl << "That move has already been done" << endl;
            }
        }
        while(!is_val_1 || !is_val_2  || !is_val_3);

        if (is_end)
        {   
            recv(sock, &row, sizeof(row), 0);
            recv(sock, &col, sizeof(col), 0);
        
            board[row - 1][col - 1] = (sign == 'X') ? 'O' : 'X';
            
            continue;
        }

        board[row - 1][col - 1] = sign;

        movement += 2;

        if (movement <= 8)
        {
            recv(sock, &row, sizeof(row), 0);
            recv(sock, &col, sizeof(col), 0);
        
            board[row - 1][col - 1] = (sign == 'X') ? 'O' : 'X';
        }     
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

void print_game_board(void)
{
     cout << "\t";

    for(int i = 0 ; i < 3 ; ++i)
    {
        cout << i + 1 << "\t";
    }

    cout << endl;

    for(int i = 0 ; i < 3 ; ++i)
    {
        cout << i + 1 << "\t";

        fill_game_field(i);
    }
}

