cd /home/user

git clone https://github.com/jedisct1/libhydrogen.git

cd libhydrogen
mkdir build
cd build
CC=afl-clang cmake ..
make
sudo make install
sudo cp libhydrogen.a /usr/local/lib/libhydrogen_AFL.a

cd ..
rm -rf build
mkdir build
cd build
CC=clang CFLAGS=-fsanitize=fuzzer-no-link cmake ..
make
sudo make install
sudo cp libhydrogen.a /usr/local/lib/libhydrogen_LF.a

cd ..
rm -rf build
mkdir build
cd build
CC=clang CFLAGS=-fsanitize=address,undefined cmake ..
make
sudo make install
sudo cp libhydrogen.a /usr/local/lib/libhydrogen_san.a

cd ..
rm -rf build
mkdir build
cd build
CC=clang cmake ..
make
sudo make install
