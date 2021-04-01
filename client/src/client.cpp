#include "client.hpp"

char board[3][3];

int row = -1, col = -1;

int socket_settings(uint16_t port)
{
    struct sockaddr_in addr;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
  
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

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

int reconnect_from_choosing_sign_stage(int i, int *sock, char *sign)
{
    int s = 0;

    bool is_avl_sign = false;

    try
    {
        if(i)
        {
            int start_game = 0;

            send(*sock, &start_game, sizeof(start_game), 0);
        }

        do
        {
            if(i)
            {
                if(s)
                    cin >> *sign;
                 ++s;
            }
            else
                 cin >> *sign;

            *sign = toupper(*sign);

            send(*sock, sign, sizeof(*sign), 0);

            if(!recv(*sock, &is_avl_sign, sizeof(is_avl_sign), 0))
                throw -1;

            if(!is_avl_sign)
            {
                cout << endl << *sign << " has already been choosen" << endl << endl;
            }
        }
        while(!is_avl_sign);

        return 0;
    }
    catch(int)
    { 
        for (int i = 1; ;++i)
        {
            sleep(1);
            
            *sock = socket_settings(MAIN_PORT + i);
            
            if(*sock != -1)
                break;
        }

        return -1;
    }
}

int reconnect_from_waiting_choosing_sign_stage(int i, int *sock, char *sign)
{
    bool begin_game = false;

    try
    {
        if(i)
        {
            int start_game = 0;

            bool is_avl_sign = false;
            
            send(*sock, &start_game, sizeof(start_game), 0);
            send(*sock, sign, sizeof(*sign), 0);

            if(!recv(*sock, &is_avl_sign, sizeof(is_avl_sign), 0))
                throw -1;
        }

        if(!recv(*sock, &begin_game, sizeof(begin_game), 0))
            throw -1;

        return 0;
    }
    catch(int)
    {
        for (int i = 1; ;++i)
        {
            sleep(1);
            
            *sock = socket_settings(MAIN_PORT + i);
            
            if(*sock != -1)
                break;
        }

        return - 1;
    }
}

/*int reconnect_from_waiting_making_movement_stage(int i, int *sock, char *sign)
{
    char winner = -1;

    try
    {
        if(i)
        {
            int start_game = 1;

            send(*sock, &start_game, sizeof(start_game), 0);
            send(*sock, sign, sizeof(sign), 0);
        }

        if(!recv(*sock, &winner, sizeof(winner), 0))
            throw -1;
        if(!recv(*sock, &row, sizeof(row), 0))
            throw -1;
        if(!recv(*sock, &col, sizeof(col), 0))
            throw -1;
    
        if(row != -1 && col != -1)
            board[row - 1][col - 1] = (*sign == 'X') ? 'O' : 'X';

        //system("clear");
        
        cout << *sign << " field" << endl << endl;
        
        print_game_board();
        
        cout << endl;

        if(winner != -1)
        {
            if(winner == 0)
                cout << "Draw!" << endl;
            else
                cout << winner << " won!" << endl;
            
            row = 0;
            col = 0;
            
            if(!check_server(*sock))
                throw -1;

            send(*sock, &row, sizeof(row), 0);
            send(*sock, &col, sizeof(col), 0);
            
            return 1;
        }

        return 0;
    }
    catch(int)
    {
        for (int i = 1; ;++i)
        {
            sleep(1);
            
            *sock = socket_settings(MAIN_PORT + i);
            
            if(*sock != -1)
                break;
        }

        return -1; 
    }
}

int reconnect_from_making_movement_stage(int i, int *sock, char *sign)
{
    int s = 0;

    bool is_val_1 = false;
    bool is_val_2 = false;
   
    try
    {
        if(i)
        {
            int start_game = 1;

            send(*sock, &start_game, sizeof(start_game), 0);
            send(*sock, sign, sizeof(sign), 0);
        }

        do 
        {
            if(i)
            {
                if(s)
                    cin >> row >> col;
                ++s;
            }
            else
                cin >> row >> col;
          
            if(!check_server(*sock))
                throw -1;
          
            send(*sock, &row, sizeof(row), 0);
            send(*sock, &col, sizeof(col), 0);
        
            if(row == 0 && col == 0)
            {
                cout << endl << "Please pick a value between 1 and 3" << endl;
                    continue;
            }
        
            if(!recv(*sock, &is_val_1, sizeof(is_val_1), 0))
                throw -1;
            if(!recv(*sock, &is_val_2, sizeof(is_val_2), 0))
                throw -1;

            if(!is_val_1)
                cout << endl << "Please pick a value between 1 and 3" << endl;
            
            if(!is_val_2)
                cout << endl << "That move has already been done" << endl;
        }
        while(!is_val_1 || !is_val_2);

        board[row - 1][col - 1] = *sign;

        return 0;
    }
    catch(int)
    {
        for (int i = 1; ;++i)
        {
            sleep(1);
        
            *sock = socket_settings(MAIN_PORT + i);
        
            if(*sock != -1)
                break;
        }

        return - 1;
    }
}*/

void client_handler(int sock)
{
    char sign;

    int start_game = -1;

    bool is_val_1 = false;
    bool is_val_2 = false;

    init_game_field();

    //system("clear");
   
    cout << "Choose X/O" << endl << endl;

    for (int i = 0; ; ++i)
    {
        if(!reconnect_from_choosing_sign_stage(i, &sock, &sign))
            break;
    }
     
    cout << "Waiting for opponent" << endl;
   
    for (int i = 0; ; ++i)
    {
        if(!reconnect_from_waiting_choosing_sign_stage(i, &sock, &sign))
            break;
    }

    //system("clear");

    while(1) 
    {
        //system("clear");
        
        cout << sign << " field" << endl << endl;
        
        print_game_board();
        
        cout << endl;

        char winner = -1;
        
        recv(sock, &winner, sizeof(winner), 0);
        
        if(winner != -1)
        {
            if(winner == 0)
                cout << "Draw!" << endl;
            else
                cout << winner << " won!" << endl;
            return;
        }

        cout << "Waiting for opponent" << endl;
        
        for (int i = 0; ; ++i)
        {
           /* int status = reconnect_from_waiting_making_movement_stage(i, &sock, &sign);
               
            if(!status)
                break;
            else if(status == 1)
                return;*/
            try
            {
                if(i)
                {
                    start_game = 1;

                    send(sock, &start_game, sizeof(start_game), 0);
                    send(sock, &sign, sizeof(sign), 0);
                }

                if(!recv(sock, &winner, sizeof(winner), 0))
                    throw -1;
                if(!recv(sock, &row, sizeof(row), 0))
                    throw -1;
                if(!recv(sock, &col, sizeof(col), 0))
                    throw -1;
                
                if(row != -1 && col != -1)
                    board[row - 1][col - 1] = (sign == 'X') ? 'O' : 'X';

                //system("clear");
                
                cout << sign << " field" << endl << endl;
                
                print_game_board();
                
                cout << endl;

                if(winner != -1)
                {
                    if(winner == 0)
                        cout << "Draw!" << endl;
                    else
                        cout << winner << " won!" << endl;
                    
                    row = 0;
                    col = 0;
                    
                    if(!check_server(sock))
                        throw -1;

                    send(sock, &row, sizeof(row), 0);
                    send(sock, &col, sizeof(col), 0);
                    
                    return;
                }

                break;
            }
            catch(int)
            {
                for (int i = 1; ;++i)
                {
                    sleep(1);
                    
                    sock = socket_settings(MAIN_PORT + i);
                    
                    if(sock != -1)
                        break;
                }

                continue; 
     
            }
        }
        
        for (int i = 0, s = 0; ; ++i, s = 0)
        {
         /*   if(!reconnect_from_making_movement_stage(i, &sock, &sign))
                break;*/

            try
            {
                if(i)
                {
                    start_game = 1;

                    send(sock, &start_game, sizeof(start_game), 0);
                    send(sock, &sign, sizeof(sign), 0);
                }

                do 
                {
                    if(i)
                    {
                        if(s)
                            cin >> row >> col;
                        ++s;
                    }
                    else
                        cin >> row >> col;
               
                    if(!check_server(sock))
                        throw -1;

                    send(sock, &row, sizeof(row), 0);
                    send(sock, &col, sizeof(col), 0);
                
                    if(row == 0 && col == 0)
                    {
                        cout << endl << "Please pick a value between 1 and 3" << endl;
                        continue;
                    }
                
                    if(!recv(sock, &is_val_1, sizeof(is_val_1), 0))
                        throw -1;
                    if(!recv(sock, &is_val_2, sizeof(is_val_2), 0))
                        throw -1;

                    if(!is_val_1)
                        cout << endl << "Please pick a value between 1 and 3" << endl;
                    
                    if(!is_val_2)
                        cout << endl << "That move has already been done" << endl;
                }
                while(!is_val_1 || !is_val_2);

                board[row - 1][col - 1] = sign;

                break;
            }
            catch(int)
            {
                for (int i = 1; ;++i)
                {
                    sleep(1);
                
                    sock = socket_settings(MAIN_PORT + i);
                
                    if(sock != -1)
                        break;
                }

                continue;
            }
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

