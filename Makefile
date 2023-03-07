start:
	g++ main.cpp -I/opt/homebrew/Cellar/sfml/2.5.1_2/include -I/opt/Users/jackhartwig/Desktop/sfml/Planet.h -o prog -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system 
	./prog
run:
	./prog