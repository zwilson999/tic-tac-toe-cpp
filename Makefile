build:
	g++ -o ./bin/tic_tac_toe ./src/main.cpp -Wall
run:
	g++ -o ./bin/tic_tac_toe ./src/main.cpp -Wall && ./bin/tic_tac_toe
check:
	valgrind --leak-check=full \
		 --show-leak-kinds=all \
		 --track-origins=yes \
		 --verbose \
		 ./bin/tic_tac_toe
