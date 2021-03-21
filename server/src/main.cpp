#include "tictaktoe.hpp"

extern char board[3][3];

extern char player;
extern char winner;

extern int movement;

extern int listener, sock;

int main()
{	
	socket_settings();

	int row;
	int col;

	bool is_avl = false;
    bool is_val_1 = false;
    bool is_val_2 = false;
	
	int game_over = 0;

	while(1)
    {
        sock = accept(listener, NULL, NULL);

        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }

    	while(1)
		{
			if(game_over_validate()) 
			{
				if (movement == 9)
				{
					game_over = 2;

					send(sock, &game_over, sizeof(game_over), 0);
				}
				else
				{
					game_over = 1;

					send(sock, &game_over, sizeof(game_over), 0);
				}

				init_game_field();

				is_avl = false;
				is_val_1 = false;
				is_val_2 = false;

				game_over = 0;
				movement = 0;

				break;

			}
			else
			{
				send(sock, &game_over, sizeof(game_over), 0);
			}

			do
			{
				recv(sock, &row, sizeof(row), 0);
				recv(sock, &col, sizeof(col), 0);
			
				is_val_1 = border_validate(row); 
				is_val_2 = border_validate(col); 
				is_avl = avalible_cell_validate(row - 1, col - 1);

				send(sock, &is_val_1, sizeof(is_val_1), 0);
            	send(sock, &is_val_2, sizeof(is_val_2), 0);
           		send(sock, &is_avl, sizeof(is_avl), 0);
			}
			while(!is_val_1 || !is_val_2  || !is_avl);

			board[row - 1][col - 1] = player;

			player = player == 'X' ? 'O' : 'X';
		
			++movement;
		}

        close(sock);
    }	

	return 0;
}
