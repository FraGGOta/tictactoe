#include "client.hpp"

char board[3][3];

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
        perror("connect");
        exit(2);
    }

    return sock;
}

bool check_server(int sock)
{
    int status;
    send(sock, &status, sizeof(status), 0);
    int bytes_read = recv(sock, &status, sizeof(status), 0);

    if (!bytes_read)
        return false;
    
    return true;
}

/* 
В блоке catch после подключения к запасному серверу дублируются операции из связанного блока try, 
прерванные из-за разрыва соединения с предыдущим сервером. Это нужно для того чтобы после подключения 
к новому серверу клиент продолжил выполнять операции с позиции, на которой он остановился во время разрыва, 
а не улетал в непонятное место.
Переменная start_game означает этап игры во время разрыва. 0 - игра не началась(игроки либо один из игроков находятся в соостоянии
выбора стороны); 1 - игра началась(появилась либо пустая доска, либо уже заполенная).
*/

void client_handler(int sock)
{
    char sign;

    int row = -1, col = -1;
    int start_game = -1;

    bool is_val_1 = false;
    bool is_val_2 = false;
    bool is_avl_sign = false;

    init_game_field();

    //system("clear");
   
    cout << "Choose X/O" << endl << endl;

    try
    {
        do
        {
            cin >> sign;

            sign = toupper(sign);

            send(sock, &sign, sizeof(sign), 0);

            if (!recv(sock, &is_avl_sign, sizeof(is_avl_sign), 0))
                throw -1;

            if (!is_avl_sign)
            {
                cout << endl << sign << " has already been choosen" << endl << endl;
            }
        }
        while(!is_avl_sign);
    }
    catch(int)
    {
        sleep(1);
        
        sock = socket_settings(3426);
          
        int s = 0;

        start_game = 0;

        send(sock, &start_game, sizeof(start_game), 0);

        do 
        {
            if (s)
                cin >> sign;

            ++s;

            sign = toupper(sign);

            send(sock, &sign, sizeof(sign), 0);
            recv(sock, &is_avl_sign, sizeof(is_avl_sign), 0);

            if (!is_avl_sign)
            {
                cout << endl << sign << " has already been choosen" << endl << endl;
            }
        }
        while(!is_avl_sign);
    }

    cout << "Waiting for opponent" << endl;
    
    bool begin_game = false;
   
    try
    {
        if(!recv(sock, &begin_game, sizeof(begin_game), 0))
            throw -1;
    }
    catch(int)
    {
        sleep(1);
        
        sock = socket_settings(3426);

        start_game = 0;

        send(sock, &start_game, sizeof(start_game), 0);
        send(sock, &sign, sizeof(sign), 0);

        recv(sock, &is_avl_sign, sizeof(is_avl_sign), 0);
        recv(sock, &begin_game, sizeof(begin_game), 0);
    }

    //system("clear");

    while(1) 
    {
        //system("clear");
        
        cout << sign << " field" << endl << endl;
        
        print_game_board();
        
        cout << endl;

        char winner = -1;
        
        if (!recv(sock, &winner, sizeof(winner), 0))
            throw -1;
        
        if (winner != -1)
        {
            if (winner == 0)
                cout << "Draw!" << endl;
            else
                cout << winner << " won!" << endl;
            break;
        }

        cout << "Waiting for opponent" << endl;
        
        try
        {
            if (!recv(sock, &winner, sizeof(winner), 0))
                throw -1;
            if (!recv(sock, &row, sizeof(row), 0))
                throw -1;
            if (!recv(sock, &col, sizeof(col), 0))
                throw -1;
            
            if (row != -1 && col != -1)
                board[row - 1][col - 1] = (sign == 'X') ? 'O' : 'X';

            //system("clear");
            
            cout << sign << " field" << endl << endl;
            
            print_game_board();
            
            cout << endl;

            if (winner != -1)
            {
                if (winner == 0)
                    cout << "Draw!" << endl;
                else
                    cout << winner << " won!" << endl;
                
                row = 0;
                col = 0;
                
                if (!check_server(sock))
                    throw -1;

                send(sock, &row, sizeof(row), 0);
                send(sock, &col, sizeof(col), 0);
                
                break;
            }
        }
        catch(int)
        {
            sleep(1);
           
            sock = socket_settings(3426);
           
            start_game = 1;

            send(sock, &start_game, sizeof(start_game), 0);
            send(sock, &sign, sizeof(sign), 0);
           
            recv(sock, &winner, sizeof(winner), 0);
            recv(sock, &row, sizeof(row), 0);
            recv(sock, &col, sizeof(col), 0);
    
            
            if (row != -1 && col != -1)
                board[row - 1][col - 1] = (sign == 'X') ? 'O' : 'X';

            //system("clear");
            
            cout << sign << " field" << endl << endl;
            
            print_game_board();
            
            cout << endl;

            if (winner != -1)
            {
                if (winner == 0)
                    cout << "Draw!" << endl;
                else
                    cout << winner << " won!" << endl;
                
                row = 0;
                col = 0;
                
                check_server(sock);

                send(sock, &row, sizeof(row), 0);
                send(sock, &col, sizeof(col), 0);
                
                break;

            }
        }
        
        try
        {
            do 
            {
                cin >> row >> col;

                if (!check_server(sock))
                    throw -1;
                send(sock, &row, sizeof(row), 0);
                send(sock, &col, sizeof(col), 0);
            
                if (row == 0 && col == 0)
                {
                    cout << endl << "Please pick a value between 1 and 3" << endl;
                    continue;
                }
            
                if (!recv(sock, &is_val_1, sizeof(is_val_1), 0))
                    throw -1;
                if (!recv(sock, &is_val_2, sizeof(is_val_2), 0))
                    throw -1;

                if (!is_val_1)
                    cout << endl << "Please pick a value between 1 and 3" << endl;
                
                if (!is_val_2)
                    cout << endl << "That move has already been done" << endl;
            }
            while(!is_val_1 || !is_val_2);

            board[row - 1][col - 1] = sign;
        }
        catch(int)
        {
            sleep(1);
            
            sock = socket_settings(3426);
            
            int s = 0;

            start_game = 1;

            send(sock, &start_game, sizeof(start_game), 0);
            send(sock, &sign, sizeof(sign), 0);
                        
            do
            {
                if (s)
                    cin >> row >> col;

                ++s;

                check_server(sock);
      
                send(sock, &row, sizeof(row), 0);
                send(sock, &col, sizeof(col), 0);
            
                if (row == 0 && col == 0)
                {
                    cout << endl << "Please pick a value between 1 and 3" << endl;
                    continue;
                }
            
                recv(sock, &is_val_1, sizeof(is_val_1), 0);
                recv(sock, &is_val_2, sizeof(is_val_2), 0);

                if (!is_val_1)
                    cout << endl << "Please pick a value between 1 and 3" << endl;
                
                if (!is_val_2)
                    cout << endl << "That move has already been done" << endl;
            }
            while(!is_val_1 || !is_val_2);

            board[row - 1][col - 1] = sign;
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

