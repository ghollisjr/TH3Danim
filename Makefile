all : libTH3Danim.so test
libTH3Danim.so : TH3Danim.o TH3Danim.cc
	g++ -c -shared `root-config --cflags` TH3Danim.cc -o libTH3Danim.so
TH3Danim.o : TH3Danim.cc
	g++ -c `root-config --cflags` TH3Danim.cc -o TH3Danim.o
test : test.cc TH3Danim.o
	g++ `root-config --cflags --libs` test.cc -o test
clean:
	rm -f libTH3Danim.so TH3Danim.o test
