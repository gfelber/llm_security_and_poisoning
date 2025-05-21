KLEE_FLAGS = --libc=uclibc --only-output-states-covering-new  --posix-runtime

all: bugs

bugs: out/bugs.linked
	klee ${KLEE_FLAGS} --output-dir=out/bugs ./out/bugs.linked

out/bugs.linked: out/bugs.bc out/oracle.bc out/test.bc
	llvm-link out/oracle.bc out/bugs.bc out/test.bc -o out/bugs.linked

out/bugs.bc: bugs.c out
	clang bugs.c -emit-llvm -c -g -o out/bugs.bc

out/oracle.bc: oracle.c out
	clang oracle.c -emit-llvm -c -g -o out/oracle.bc

out/test.bc: test.c out
	clang test.c -emit-llvm -c -g -o out/test.bc

out:
	mkdir out

docker: clean
	docker build --quiet --ulimit='stack=-1:-1' --target export -t klee-benchmark --output out .

docker-verbose: clean
	docker build --ulimit='stack=-1:-1' --target export -t klee-benchmark --output out .

clean:
	rm -rf out || true
