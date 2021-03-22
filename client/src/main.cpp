#include "client.hpp"

extern char board[3][3];

extern char player;
extern char winner;

extern int move;
extern int sock;

int main(int argc, char const **argv)
{
	assert(argc == 3);

	socket_settings(argv[1], atoi(argv[2]));

    init_game_field();

    int row, col;

    bool is_val_3 = false;
    bool is_val_1 = false;
    bool is_val_2 = false;
   
   int game_over = 0;

	while(1)
    { 
        recv(sock, &game_over, sizeof(game_over), 0);

        if (game_over == 1)
        {
           system("clear");

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

            cout << endl << "Player " << winner << " win" << endl;

            return 0;
        }
        else if (game_over == 2)
        {
            system("clear");

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

            cout << endl << "Draw" << endl;

            return 0;
        }

        winner = player == 'X' ? 'X' : 'O';

       system("clear");

        cout << "It's " << player << "'s turn" << endl << endl;

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

       do
       {
            cout << endl;

            cin >> row >> col;
            
            send(sock, &row, sizeof(row), 0);
            send(sock, &col, sizeof(col), 0);
        
            recv(sock, &is_val_1, sizeof(is_val_1), 0);
            recv(sock, &is_val_2, sizeof(is_val_2), 0);
            recv(sock, &is_val_3, sizeof(is_val_3), 0);

            if (!is_val_1 || !is_val_2)
            {
               cout << "Please pick a value between 1 and 3" << endl;
            }

            if (!is_val_3)
            {
                cout << "That move has already been done" << endl;
            }
        }
        while(!is_val_1 || !is_val_2  || !is_val_3);

        board[row - 1][col - 1] = player;

        recv(sock, &row, sizeof(row), 0);
        recv(sock, &col, sizeof(col), 0);

        board[row - 1][col - 1] = player;

        player = player == 'X' ? 'O' : 'X';
    }
	
	return 0;
}