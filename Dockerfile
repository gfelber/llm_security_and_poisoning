FROM klee/klee:3.0@sha256:8f76ff8a820e255b5b6503eb84dfb2e9e48dae52f46b6d82d1e67e492f63f2d2 AS builder

RUN mkdir /home/klee/build
WORKDIR /home/klee/build/
COPY ./template/simple_oob_read/ ./
COPY ./template/simple_oob_read/tests/bad.c ./test.c
COPY ./Makefile ./

RUN make

FROM scratch AS export
COPY --from=builder /home/klee/build/out /
