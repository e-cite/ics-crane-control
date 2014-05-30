g++ *.cpp ./*/*.cpp -o ics-crane-control_release
g++ *.cpp ./*/*.cpp -pthread -std=c++0x -D DEBUG -o ics-crane-control_printf
g++ *.cpp ./*/*.cpp -pthread -std=c++0x -lncurses -D NCURSES -o ics-crane-control_ncurses

