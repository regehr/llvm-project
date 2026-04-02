```
mkdir test-suite-build
cd test-suite-build
cmake -DCMAKE_C_COMPILER=$HOME/tmp/llvm-project-regehr-2/build/bin/clang -C../cmake/caches/O3.cmake -G Ninja ..
ninja
/home/regehr/tmp/llvm-project-regehr-2/build/bin/llvm-lit -v -j 1 -o results.json .
```

```
```
