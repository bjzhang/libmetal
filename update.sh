#cd build
git fetch local && git checkout -f local/Linux-rtt-openamp || (echo "git fail"; return)
cmake -DCMAKE_C_FLAGS="-ggdb -latomic"  -DWITH_EXAMPLES=ON ../
make VERBOSE=1 DESTDIR=$PWD install
