KLEE_FLAGS = --libc=uclibc --only-output-states-covering-new --posix-runtime --max-time=8

all: bugs corruption

# get binaries
corruption: out/corruption.linked
	klee ${KLEE_FLAGS} --output-dir=out/corruption ./out/corruption.linked

bugs: out/bugs.linked
	klee ${KLEE_FLAGS} --output-dir=out/bugs ./out/bugs.linked

out/corruption.linked: out/corruption.bc out/test.bc
	llvm-link out/test.bc out/corruption.bc -o out/corruption.linked

out/bugs.linked: out/bugs.bc out/oracle.bc out/test.bc
	llvm-link out/oracle.bc out/bugs.bc out/test.bc -o out/bugs.linked

out/bugs.bc: bugs.c out
	clang bugs.c -emit-llvm -c -g -o out/bugs.bc

out/corruption.bc: corruption.c out
		clang corruption.c -emit-llvm -c -g -o out/corruption.bc

out/oracle.bc: oracle.c out
	clang oracle.c -emit-llvm -c -g -o out/oracle.bc

out/test.bc: test.c out
	clang test.c -emit-llvm -c -g -o out/test.bc

out:
	mkdir out

docker: clean
	docker build --ulimit='stack=-1:-1' --target export -t klee-benchmark --output out .

clean:
	rm -rf out || true
