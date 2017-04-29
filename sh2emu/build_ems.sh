# emcc -o btesh2.html btesh2_main.c -O3 --preload-file pl_ems -s TOTAL_MEMORY=268435456 -s SAFE_HEAP=1

emcc -o btesh2.html btesh2_main.c -O3 --preload-file pl_ems -s TOTAL_MEMORY=402653184 -s WASM=1 -s "BINARYEN_METHOD='native-wasm,asmjs'"
