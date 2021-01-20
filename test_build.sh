cd build
cmake .. && make && rm * -rf || return
cmake ../ -DCMAKE_TOOLCHAIN_FILE=../rv64_linux_cross_toolchain_file -DCMAKE_LIBRARY_PATH="/home/bamvor/works/source2/OpenAMP/dep/sysfsutils-2.1.0/install/lib;/usr/riscv64-linux-gnu/lib;/usr/lib/gcc-cross/riscv64-linux-gnu/7/" && make && rm * -rf || return

