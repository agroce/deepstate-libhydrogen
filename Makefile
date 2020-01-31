FILES=testhydrogen testhydrogen_san testhydrogen_LF testhydrogen_AFL

all: $(FILES)

clean:
	rm -rf $(FILES)

testhydrogen: TestHydrogen.cpp
	clang++ -o testhydrogen TestHydrogen.cpp -ldeepstate -lhydrogen

testhydrogen_san: TestHydrogen.cpp
	clang++ -o testhydrogen_san TestHydrogen.cpp -ldeepstate -lhydrogen_san -fsanitize=address,undefined

testhydrogen_LF: TestHydrogen.cpp
	clang++ -o testhydrogen_LF TestHydrogen.cpp -ldeepstate_LF -lhydrogen_LF -fsanitize=fuzzer

testhydrogen_AFL: TestHydrogen.cpp
	afl-clang++ -o testhydrogen_AFL TestHydrogen.cpp -ldeepstate_AFL -lhydrogen_AFL
