#include <iostream>
#include <limits>
#include <random>
#include <algorithm>

// TODO:
// 1. make sure user or computer cannot choose spots that are already chosen. make sure it retries if choice is already made
// 2. add options for user to quit and restart game from any position where input is expected from stdin

struct Coordinates 
{
        int row;
        int col;
};

class TicTacToe {
        private:
                int **board;
                int player_id;
                int computer_id;
                std::vector<Coordinates> available; // flexible array member to represent available spaces on the board. this is 1-indexed
                
                bool validate_coordinate_pair(int row, int col)
                {
                        // if coordinate is 0 then it is unused and available to be plotted at
                        return this->board[row-1][col-1] == 0 ? true : false; 
                }
                
                bool plot(struct Coordinates coords, int symbol)
                {
                        if (this->validate_coordinate_pair(coords.row, coords.col))
                        {
                                this->board[coords.row-1][coords.col-1] = symbol; 

                                // remove elements from available spaces that have the coordinates of the point we are about to plot
                                // see erase-remove idiom online for more
                                this->available.erase(
                                        std::remove_if(
                                                this->available.begin(), 
                                                this->available.end(), 
                                                [&](const Coordinates& c) { return c.row == coords.row && c.col == coords.col; }
                                        )
                                );
                                return true;
                        }
                        return false;
                }
                
                bool check()
                {
                        return false;
                }
        public:
                TicTacToe(int player, int computer) 
                {
                        this->board = new int*[3]; // array of 3 pointers to ints
                        for (int i = 0; i < 3; ++i)
                        {
                                this->board[i] = new int[3];
                        }
                        this->player_id = player;
                        this->computer_id = computer;
                        
                        // all spaces start out as available
                        for (int i = 0; i < 3; ++i)
                        {
                                for (int j = 0; j < 3; ++j)
                                {
                                        // this->available[i * 3 + j] = (Coordinates){.row=i, .col=j};
                                        this->available.push_back( (Coordinates) {.row=i+1, .col=j+1} );
                                }
                        }

                        // show the user the initial row state 
                        this->print_board();
                }

                ~TicTacToe()
                {
                        for (int i = 0; i < 3; ++i)
                        {
                                delete[] this->board[i];
                        }
                        delete[] this->board;
                }

                void print_board() 
                {
                        std::cout << "===============\n"; 
                        std::cout << "| Tic-tac-Toe |\n"; 
                        std::cout << "===============\n"; 
                        for (int i = 0; i < 3; i++) 
                        {
                                std::cout << "|  ";
                                for (int j = 0; j < 3; j++) 
                                {
                                        std::cout << this->board[i][j] << "  "; 
                                }
                                std::cout << "  |\n";
                        }
                        std::cout << "===============\n";  
                }

                // void print_available()
                // {
                //         for (const auto &a: this->available)
                //         {
                //                 std::cout << "Row " << a.row << " Col: " << a.col << std::endl;
                //         }
                // }
                
                std::vector<Coordinates>::iterator choose_random_available()
                {
                        // init random choice by randomly choosing value from our
                        // available spaces vector
                        static std::random_device rd;
                        static std::mt19937 gen(rd());

                        // create iterators for our available indices to choose from
                        std::vector<Coordinates>::iterator start = this->available.begin();
                        std::vector<Coordinates>::iterator last = this->available.end();

                        // create uniform distribution based on size of our available vector
                        std::uniform_int_distribution<> distrib(0, std::distance(start, last)-1);
                        std::advance(start, distrib(gen)); // advance the iterator by a random amount
                        return start;
                }

                void generate_computer_choice()
                {
                        // get random values for row/col for the computer's turn
                        // these random values will be selected via the object's 'available' vector
                        struct Coordinates c = *this->choose_random_available();
                        std::cout << "The computer chose: \n";
                        std::cout << "Row: " << c.row << std::endl << "Column: " << c.col << std::endl;
                        if (this->plot(c, this->computer_id))
                                return;
                        else
                                this->generate_computer_choice();
                }

                void generate_player_choice()
                {
                        int row;
                        int col;

                        std::cout << "Please enter the coordinates of where you want to make your mark: \n";
                        std::cout << "Choose a row (1, 2, 3 are valid rows): \n";
                        while (1)
                        {
                                std::cout << "=>";
                                std::cin >> row; 
                                if (row >= 1 && row <= 3 && !std::cin.eof())
                                        break;
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
                                if (col >= 1 && col <= 3 && !std::cin.eof())
                                        break;
                                std::cout << "You chose column: " << col << std::endl;
                                std::cout << "That isn't a valid choice." << std::endl; 
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                        this->plot( (Coordinates) {.row=row, .col=col}, this->player_id);
                }
};

int start()
{
        std::cout << "Hello! Welcome to a game of tic-tac-toe. Would you like to be 'X's or 'O's?\n"; 
        std::cout << "To exit at any time, type Ctrl+c :)\n";
        std::cout << "Please enter your choice: \n"; 
        std::cout << "[1]: O's\n"; 
        std::cout << "[2]: X's\n"; 
        std::string player_choice;
        while (1) 
        {
                std::cout << "=>";
                std::cin >> player_choice;
                if ((player_choice == "1" || player_choice == "2") && !std::cin.eof()) 
                        break;
                std::cout << "You chose option: " << player_choice << std::endl;
                std::cout << "Oops! that isn't 1 or 2. Please choose one of them again. \n";

                // cleanup cin
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return std::stoi(player_choice);
}

int main(void) 
{

        // prompt the user for their symbol choice
        // computer will be assigned the symbol that the user did not choose
        int player_id = start();
        int computer_id = player_id == 1 ? 2 : 1 ;

        // init our Tic-tac Toe board
        TicTacToe ttt(player_id, computer_id);
        while (1)
        {
                // generate computer choice based on stdin
                ttt.generate_player_choice();

                // computer player will randomly choose a space that is available to plot
                std::cout << "Now it's the computer's turn!\n";
                ttt.generate_computer_choice();

                // show updated board state
                ttt.print_board();
        }
        return 0;
}       
