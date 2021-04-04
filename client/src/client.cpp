#include "client.hpp"

char board[3][3];

int socket_settings(char const *id, uint16_t port)
{
    struct sockaddr_in addr;
    struct hostent *hp;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
  
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    hp = gethostbyname(id);
    
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);

    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {   
        return -1;
    }

    return sock;
}

bool check_server(int sock)
{
    int status;

    send(sock, &status, sizeof(status), 0);

    int bytes_read = recv(sock, &status, sizeof(status), 0);

    if(!bytes_read)
        return false;
    
    return true;
}

void chose_sign(int *sock, char *sign)
{
    bool is_avl_sign = true;

    for(int i = 0, j = 0; ; ++i, j = 0)
    {
        system("clear");
        cout << " ~ " << "Choose your sign X or O" << " ~ " << endl << endl;

        try
        {
            if(i)
            {
                int start_game = 0;

                send(*sock, &start_game, sizeof(start_game), 0);
            }

            string msg = "";
            do
            {
                if(!is_avl_sign)
                {
                    system("clear");
                    cout << " ~ " << "Choose your sign X or O" << " ~ " << endl << endl;
                    cout << " ~ " << msg << " ~ " << endl;
                }

                if(i)
                {
                    if(j)
                    {
                        string buf;
                        cout << "> " << flush;
                        getline(cin, buf);
                        *sign = buf[0];
                    }
                    ++j;
                }
                else
                {
                    string buf;
                    cout << "> " << flush;
                    getline(cin, buf);
                    *sign = buf[0];
                }

                *sign = toupper(*sign);

                send(*sock, sign, sizeof(*sign), 0);

                if(!recv(*sock, &is_avl_sign, sizeof(is_avl_sign), 0))
                    throw -1;

                if (*sign != 'X' && *sign != 'O')
                    msg = "Invalid input! Try again";
                else
                    msg = "This sign has already been choosen";
            }
            while(!is_avl_sign);

           break;
        }
        catch(int)
        { 
            for (int i = MAIN_PORT; i <= LAST_PORT; ++i)
            {    
                *sock = socket_settings(SERV_ID, i);
                
                if(*sock != -1)
                    break;
            }
            if(*sock == -1)
            {
                perror("All servers aren't acceptable");
                exit(2);
            }
        }
    }
}

void wait_opponent_sign(int *sock, char sign)
{
    bool begin_game = false;

    for (int i = 0; ; ++i)
    {
        try
        {
            if(i)
            {
                int start_game = 0;

                bool is_avl_sign = false;
     
                send(*sock, &start_game, sizeof(start_game), 0);
                send(*sock, &sign, sizeof(sign), 0);

                if(!recv(*sock, &is_avl_sign, sizeof(is_avl_sign), 0))
                    throw -1;
            }

            if(!recv(*sock, &begin_game, sizeof(begin_game), 0))
                throw -1;

           break;
        }
        catch(int)
        { 
            for (int i = MAIN_PORT; i <= LAST_PORT; ++i)
            {             
                *sock = socket_settings(SERV_ID, i);
                
                if(*sock != -1)
                    break;
            }
            if(*sock == -1)
            {
                perror("All servers aren't acceptable");
                exit(2);
            }
        }
    }
}

int wait_opponent_move(int *sock, char sign)
{
    char winner = -1;

    int row = -1, col = -1;

    for (int i = 0; ; ++i)
    {
        try
        {
            if(i)
            {
                int start_game = 1;

                send(*sock, &start_game, sizeof(start_game), 0);
                send(*sock, &sign, sizeof(sign), 0);
            }

            if(!recv(*sock, &winner, sizeof(winner), 0))
                throw -1;
            if(!recv(*sock, &row, sizeof(row), 0))
                throw -1;
            if(!recv(*sock, &col, sizeof(col), 0))
                throw -1;
            
            if(row != -1 && col != -1)
                board[row - 1][col - 1] = (sign == 'X') ? 'O' : 'X';

            system("clear");
            
            cout << "        ~ " << "You are " << sign << " player" << " ~ " << endl << endl;
            print_game_board();

            if(winner != -1)
            {
                if(winner == 0)
                    cout << " ~ " << "Draw!" << " ~ " << endl;
                else
                    cout << " ~ " << winner << " won!" << " ~ " << endl;
                
                row = 0;
                col = 0;
                
                if(!check_server(*sock))
                    throw -1;

                send(*sock, &row, sizeof(row), 0);
                send(*sock, &col, sizeof(col), 0);
                
                return 1;
            }

            break;
        }
        catch(int)
        { 
            for (int i = MAIN_PORT; i <= LAST_PORT; ++i)
            {               
                *sock = socket_settings(SERV_ID, i);
                
                if(*sock != -1)
                    break;
            }
            if(*sock == -1)
            {
                perror("All servers aren't acceptable");
                exit(2);
            }
        }
    }

    return 0;
}

void make_move(int *sock, char sign)
{
    int row = -1, col = -1;

    bool is_val_1 = false;
    bool is_val_2 = false;
   
    for (int i = 0, j = 0; ; ++i, j = 0)
    {
        try
        {
            if(i)
            {
                int start_game = 1;

                send(*sock, &start_game, sizeof(start_game), 0);
                send(*sock, &sign, sizeof(sign), 0);
            }

            string msg = "Enter row and column number";
            do 
            {
                system("clear");
                cout << "        ~ " << "You are " << sign << " player" << " ~ " << endl << endl;
                print_game_board();
                cout << endl << " ~ " << msg << " ~ " << endl;

                if(i)
                {
                    if(j)
                    {
                        string buf;
                        cout << "> " << flush;
                        getline(cin, buf);
                        row = buf[0] - '0';
                        cout << endl << "> " << flush;
                        getline(cin, buf);
                        col = buf[0] - '0';
                    }
                    ++j;
                }
                else
                {
                    string buf;
                    cout << "> " << flush;
                    getline(cin, buf);
                    row = buf[0] - '0';
                    cout << endl << "> " << flush;
                    getline(cin, buf);
                    col = buf[0] - '0';
                }
           
                if(!check_server(*sock))
                    throw -1;

                send(*sock, &row, sizeof(row), 0);
                send(*sock, &col, sizeof(col), 0);
            
                if(row == 0 && col == 0)
                {
                    msg = "Please pick a value between 1 and 3";
                    continue;
                }
            
                if(!recv(*sock, &is_val_1, sizeof(is_val_1), 0))
                    throw -1;
                if(!recv(*sock, &is_val_2, sizeof(is_val_2), 0))
                    throw -1;

                if(!is_val_1)
                    msg = "Please pick a value between 1 and 3";
                
                if(!is_val_2)
                    msg = "This move has already been done";
            }
            while(!is_val_1 || !is_val_2);

            board[row - 1][col - 1] = sign;

            break;
        }
        catch(int)
        { 
            for (int i = MAIN_PORT; i <= LAST_PORT; ++i)
            {            
                *sock = socket_settings(SERV_ID, i);
                
                if(*sock != -1)
                    break;
            }
            if(*sock == -1)
            {
                perror("All servers aren't acceptable");
                exit(2);
            }
        }
    }
}

void client_handler(int sock)
{
    char sign;

    init_game_field();

    chose_sign(&sock, &sign);

    cout << " ~ " << "Waiting for opponent..." << " ~ " << endl;
   
    wait_opponent_sign(&sock, sign);

    system("clear");

    while(1) 
    {
        system("clear");
        
        cout << "        ~ " << "You are " << sign << " player" << " ~ " << endl << endl;
        
        print_game_board();

        char winner = -1;
        
        recv(sock, &winner, sizeof(winner), 0);
        
        if(winner != -1)
        {
            if(winner == 0)
                cout << " ~ " << "Draw!" << " ~ " << endl;
            else
                cout << " ~ " << winner << " won!" << " ~ " << endl;
            break;
        }

        cout << " ~ " << "Waiting for opponent..." << " ~ " << endl;

        if(wait_opponent_move(&sock, sign) == 1)
            break;

        make_move(&sock, sign);
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

void print_sign_part(int i, int j, int row)
{
    switch (board[i][j])
    {
    case 'X':
        switch (row)
        {
        case 0:
            cout << " \\  / ";
            break;
        case 1:
            cout << "  \\/  ";
            break;
        case 2:
            cout << "  /\\  ";
            break;
        case 3:
            cout << " /  \\ ";
            break;
        default:
            break;
        }
        break;
    case 'O':
        switch (row)
        {
        case 0:
            cout << "  --  ";
            break;
        case 1:
            cout << " |  | ";
            break;
        case 2:
            cout << " |  | ";
            break;
        case 3:
            cout << "  --  ";
            break;
        default:
            break;
        }
        break;
    default:
        cout << "      ";
        break;
    }
}

void print_game_board(void)
{

    cout << "        ";
    print_sign_part(0, 0, 0);
    cout << "{";
    print_sign_part(0, 1, 0);
    cout << "}";
    print_sign_part(0, 2, 0);
    cout << endl;

    cout << "        ";
    print_sign_part(0, 0, 1);
    cout << "}";
    print_sign_part(0, 1, 1);
    cout << "{";
    print_sign_part(0, 2, 1);
    cout << endl;

    cout << "        ";
    print_sign_part(0, 0, 2);
    cout << "{";
    print_sign_part(0, 1, 2);
    cout << "}";
    print_sign_part(0, 2, 2);
    cout << endl;

    cout << "        ";
    print_sign_part(0, 0, 3);
    cout << "}";
    print_sign_part(0, 1, 3);
    cout << "{";
    print_sign_part(0, 2, 3);
    cout << endl;


    cout << "        ~-~~-~+~-~~-~+~-~~-~" << endl;

    cout << "        ";
    print_sign_part(1, 0, 0);
    cout << "{";
    print_sign_part(1, 1, 0);
    cout << "}";
    print_sign_part(1, 2, 0);
    cout << endl;

    cout << "        ";
    print_sign_part(1, 0, 1);
    cout << "}";
    print_sign_part(1, 1, 1);
    cout << "{";
    print_sign_part(1, 2, 1);
    cout << endl;

    cout << "        ";
    print_sign_part(1, 0, 2);
    cout << "{";
    print_sign_part(1, 1, 2);
    cout << "}";
    print_sign_part(1, 2, 2);
    cout << endl;

    cout << "        ";
    print_sign_part(1, 0, 3);
    cout << "}";
    print_sign_part(1, 1, 3);
    cout << "{";
    print_sign_part(1, 2, 3);
    cout << endl;

    cout << "        ~-~~-~+~-~~-~+~-~~-~" << endl;

    cout << "        ";
    print_sign_part(2, 0, 0);
    cout << "{";
    print_sign_part(2, 1, 0);
    cout << "}";
    print_sign_part(2, 2, 0);
    cout << endl;

    cout << "        ";
    print_sign_part(2, 0, 1);
    cout << "}";
    print_sign_part(2, 1, 1);
    cout << "{";
    print_sign_part(2, 2, 1);
    cout << endl;

    cout << "        ";
    print_sign_part(2, 0, 2);
    cout << "{";
    print_sign_part(2, 1, 2);
    cout << "}";
    print_sign_part(2, 2, 2);
    cout << endl;

    cout << "        ";
    print_sign_part(2, 0, 3);
    cout << "}";
    print_sign_part(2, 1, 3);
    cout << "{";
    print_sign_part(2, 2, 3);
    cout << endl << endl;

}
