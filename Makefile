rain: rain.o
	g++ -O2 rain.o -o rain
	rm -f rain.o
	echo "Successfully compiled the program. Now execute it with './rain'"

rain.o: rain.cpp
	g++ -c rain.cpp -o rain.o

clean:
	rm -f rain rain.o
