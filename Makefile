# this is set up to by default to make the top level and test with CppUTest
# You need to separately get and build cpputest from cpputest.org
# You must also define CPPUTEST_HOME.  See the README.txt

all: everything
clean: cleaneverything

everything: 
	make -i -f MakefileCppUTest.mk 

cleaneverything:
	make -i -f MakefileCppUTest.mk clean
