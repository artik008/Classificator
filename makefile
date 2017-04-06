all: clean Author_Class clean1

Author_Class: Author_Class.o Features.o Model.o Classificate.o
	g++ -std=c++0x  build/features.o build/model.o build/classificate.o build/main.o -o bin/Author_Class -lboost_system -lboost_thread -lpthread -lfreeling

Author_Class.o: src/main.cpp src/include/classificate.h
	g++ -std=c++0x -c src/main.cpp -o build/main.o -lboost_system -lboost_thread -lpthread -lfreeling

Features.o: src/include/features.cpp src/include/features.h
	g++ -std=gnu++11 -c src/include/features.cpp -o build/features.o -lboost_system -lboost_thread -lpthread -lfreeling

Model.o: src/include/model.cpp src/include/model.h src/include/features.h
	g++ -std=gnu++11 -c src/include/model.cpp -o build/model.o -lboost_system -lboost_thread -lpthread -lfreeling

Classificate.o: src/include/classificate.cpp src/include/classificate.h
	g++ -std=gnu++11 -c src/include/classificate.cpp -o build/classificate.o -lboost_system -lboost_thread -lpthread -lfreeling


clean1: 
	rm -rf build/*.o

clean: 
	rm -rf *.o Author_Class
	
