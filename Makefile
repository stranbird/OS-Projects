osh: 
	cd src && make

install:
	mkdir -p bin
	mv src/osh bin/osh

clean:
	cd src && rm *.o