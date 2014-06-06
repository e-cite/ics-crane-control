g++ ../src/*.cpp ../src/*/*.cpp -o ../bin/ics-crane-control_release
g++ ../src/*.cpp ../src/*/*.cpp -pthread -std=c++0x -D DEBUG -o ../bin/ics-crane-control_printf
g++ ../src/*.cpp ../src/*/*.cpp -pthread -std=c++0x -lncurses -D NCURSES -o ../bin/ics-crane-control_ncurses

