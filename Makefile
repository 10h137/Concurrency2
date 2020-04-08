all:
	make clean
	g++ main.cpp -o run

clean:
	rm run || true