#include <iostream>
#include <limits>
#include <random>
#include <algorithm>
#include <string>
#include <vector>

struct Coordinates 
{
        int row;
        int col;
};

class TicTacToe {
        private:
                std::vector<std::vector<char>> board;
                std::vector<Coordinates> available; // flexible vec member to represent available spaces on the board. this is 1-indexed
                char player_id;
                char computer_id;
                int turn_count;
                bool resume;
                
                bool
                validate_coordinate_pair(int row, int col)
                {
                        return this->board[row-1][col-1] == '_' ? true : false; 
                }
                
                bool
                plot(struct Coordinates coords, char symbol)
                {
                        if (!this->available.empty())
                        {
                                if (this->validate_coordinate_pair(coords.row, coords.col))
                                {
                                        // remove elements from available spaces that have the coordinates of the point we are about to plot
                                        // see erase-remove idiom online for more
                                        this->available.erase(
                                                std::remove_if(
                                                        this->available.begin(), 
                                                        this->available.end(), 
                                                        [&](const Coordinates& c) { return c.row == coords.row && c.col == coords.col; }
                                                )
                                        );

                                        // set the board space to the appropriate mark
                                        this->board[coords.row-1][coords.col-1] = symbol; 
                                        return true;
                                }
                                return false;
                        }
                        else
                        {
                                std::cout << "No more possible moves! Cat's game!\n";
                                return false;
                        }
                }
                
                bool
                check(char symbol)
                {
                        // row 1
                        if (this->board[0][0] == symbol && this->board[0][1] == this->board[0][0] && this->board[0][2] == this->board[0][0])
                                return true;
                        // row 2
                        else if (this->board[1][0] == symbol && this->board[1][1] == this->board[1][0] && this->board[1][2] == this->board[1][0])
                                return true;
                        // row 3
                        else if (this->board[2][0] == symbol && this->board[2][1] == this->board[2][0] && this->board[2][2] == this->board[2][0])
                                return true;
                        // col 1
                        else if (this->board[0][0] == symbol && this->board[1][0] == this->board[0][0] && this->board[2][0] == this->board[0][0])
                                return true;
                        // col 2
                        else if (this->board[0][1] == symbol && this->board[1][1] == this->board[0][1] && this->board[2][1] == this->board[0][1])
                                return true;
                        // col 3
                        else if (this->board[0][2] == symbol && this->board[1][2] == this->board[0][2] && this->board[2][2] == this->board[0][2])
                                return true;
                        // left diagonal  
                        else if (this->board[0][0] == symbol && this->board[1][1] == this->board[0][0] && this->board[2][2] == this->board[0][0])
                                return true;
                        // right diagonal 
                        else if (this->board[0][2] == symbol && this->board[1][1] == this->board[0][2] && this->board[2][0] == this->board[0][2])
                                return true;
                        else
                                return false;
                }

                void
                print_available()
                {
                        std::cout << "available spaces" << std::endl;
                        for (const auto &c: this->available)
                        {
                                std::cout << "Row: " << c.row << " Col: " << c.col << std::endl;
                        }
                }

                std::vector<Coordinates>::iterator 
                choose_random_available()
                {
                        // init random choice by randomly choosing value from our
                        // available spaces vector
                        static std::random_device rd;
                        static std::mt19937 gen(rd());

                        // create iterators for our available indices to choose from
                        std::vector<Coordinates>::iterator start = this->available.begin();

                        // create uniform distribution based on size of our available vector
                        std::uniform_int_distribution<> distrib(0, std::distance(start, this->available.end())-1);
                        std::advance(start, distrib(gen)); // advance the start iterator by a random amount
                        return start;
                }

                void
                restart()
                {
                        while (1)
                        {
                                std::string play_again;
                                std::cout << "Would you like to play again? (Y/n)\n";
                                std::cout << "=>";
                                std::cin >> play_again;

                                // convert to lowercase
                                std::transform(play_again.begin(), play_again.end(), play_again.begin(), ::tolower);

                                if (play_again == "y")
                                {
                                        // reset state and start anew
                                        *this = TicTacToe();
                                        this->start();
                                        break;
                                }
                                else if (play_again == "n")
                                {
                                        std::cout << "Goodbye!\n";
                                        this->resume = false;
                                        break;
                                }
                                else
                                {
                                        std::cout << "Sorry that isn't an option\n";
                                        std::cout << "Select 'Y' (yes) to play again or 'n' (no) to exit.\n";
                                }
                        }
                }

        public:
                TicTacToe() 
                {
                        this->board = {
                                {'_', '_', '_'},
                                {'_', '_', '_'},
                                {'_', '_', '_'}
                        };
                        this->turn_count = 0;
                        this->resume = true;

                        // all spaces start out as available
                        for (int i = 0; i < 3; ++i)
                        {
                                for (int j = 0; j < 3; ++j)
                                {
                                        // this->available[i * 3 + j] = (Coordinates){.row=i, .col=j};
                                        this->available.push_back( (Coordinates) {.row=i+1, .col=j+1} );
                                }
                        }
                }

                // ~TicTacToe() {}

                void
                start()
                {
                        std::cout << "Hello! Welcome to a game of tic-tac-toe. Would you like to be 'X's or 'O's?\n"; 
                        std::cout << "To exit at any time, press Ctrl+c :)\n";
                        std::cout << "Please enter your choice: \n"; 
                        std::cout << "[1]: O's\n"; 
                        std::cout << "[2]: X's\n"; 
                        char player_choice;
                        while (1) 
                        {
                                std::cout << "=>";
                                std::cin >> player_choice;
                                if (player_choice == '1')
                                {
                                        this->player_id = 'O';
                                        this->computer_id = 'X';
                                        break;
                                }
                                else if (player_choice == '2')
                                {
                                        this->player_id = 'X';
                                        this->computer_id = 'O';
                                        break;
                                }
                                else
                                {
                                        std::cout << "You chose option: " << player_choice << std::endl;
                                        std::cout << "Oops! that isn't 1 or 2. Please choose one of them again. \n";

                                        // cleanup cin
                                        std::cin.clear();
                                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                }
                        }
                }

                void
                print_board() 
                {
                        std::cout << "=================\n"; 
                        std::cout << "|  Tic-tac-Toe  |\n"; 
                        std::cout << "=================\n"; 
                        for (int i = 0; i < 3; i++) 
                        {
                                std::cout << "|   ";
                                for (int j = 0; j < 3; j++) 
                                {
                                        std::cout << this->board[i][j] << "  "; 
                                }
                                std::cout << "   |\n";
                        }
                        std::cout << "=================\n";  
                }

                void
                generate_computer_choice()
                {
                        // computer player will randomly choose a space that is available to plot
                        std::cout << "Now it's the computer's turn!\n";

                        // get random values for row/col for the computer's turn
                        // these random values will be selected via the object's 'available' vector
                        if (!this->available.empty())
                        {
                                struct Coordinates c = *this->choose_random_available();
                                std::cout << "The computer chose: " << "Row: " << c.row << " Column: " << c.col << std::endl;
                                if (this->plot(c, this->computer_id))
                                {
                                        // assess victory
                                        if (++this->turn_count >= 5 && this->check(this->computer_id))
                                        {
                                                this->print_board();
                                                std::cout << "The computer won! Better luck next time!\n";
                                                this->restart();
                                        }
                                }
                                else
                                {
                                        this->generate_computer_choice();
                                }
                        }
                        else
                        {
                                std::cout << "No more possible moves! Cat's game!\n";
                                this->restart();
                        }
                }

                void
                generate_player_choice()
                {
                        int row;
                        int col;

                        std::cout << "Please enter the coordinates of where you want to make your mark: \n";
                        std::cout << "Choose a row (1, 2, 3 are valid rows): \n";
                        while (1)
                        {
                                std::cout << "=>";
                                std::cin >> row; 
                                if (row >= 1 && row <= 3 && !std::cin.eof()) break;
                                std::cout << "You chose row: " << row << std::endl;
                                std::cout << "That isn't a valid choice." << std::endl; 
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }

                        while (1)
                        {
                                std::cout << "Choose a column (1, 2, 3 are valid columns): " << std::endl;
                                std::cout << "=>";
                                std::cin >> col;
                                if (col >= 1 && col <= 3 && !std::cin.eof()) break;
                                std::cout << "You chose column: " << col << std::endl;
                                std::cout << "That isn't a valid choice." << std::endl; 
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }

                        if (this->plot( (Coordinates) {.row=row, .col=col}, this->player_id) )
                        {
                                // assess if player is winner
                                if (++this->turn_count >= 5 && this->check(this->player_id))
                                {
                                        this->print_board();
                                        std::cout << "You won! Nice job!\n";
                                        this->restart();
                                }
                        }
                        else
                        {
                                std::cout << "That row/column place is already occupied. Please choose another space to plot.\n";
                                this->generate_player_choice();
                        }
                }

                bool
                game_over()
                {
                        return !this->resume;
                }
};

int
main(void) 
{
        TicTacToe ttt;
        ttt.start();
        while (!ttt.game_over())
        {
                ttt.generate_player_choice();
                ttt.generate_computer_choice();
                ttt.print_board();
        }
        return 0;
}       
