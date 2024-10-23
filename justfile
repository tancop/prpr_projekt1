run:
    clang -O1 132983_projekt_vx.c
    ./a.out

run-msan:
    clang -g -fsanitize=memory -fsanitize-memory-track-origins -O1 132983_projekt_vx.c
    ./a.out

run-asan:
    clang -g -fsanitize=address -O1 132983_projekt_vx.c
    ./a.out
