rem emcc -o btesh2.html btesh2_main.c -O3 --preload-file testkern_sh4.elf -s TOTAL_MEMORY=268435456 -s SAFE_HEAP=1

emcc -o btesh2.html btesh2_main.c -O3 --preload-file vmlinux -s TOTAL_MEMORY=268435456 -s SAFE_HEAP=1
