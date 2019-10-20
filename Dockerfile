FROM debian:buster-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    clang \
    googletest \
    make \
    && rm -rf /var/lib/apt/lists

WORKDIR /opt/brace-expansion
ADD *.cc *.h Makefile build.sh brace-expansion-smoke-test.sh run-all-tests.sh ./
