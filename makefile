all: minitimer

minitimer: winmain.cpp
	g++ -mwindows -Wall -Wextra -pedantic winmain.cpp -luser32 -lgdi32 -o minitimer
