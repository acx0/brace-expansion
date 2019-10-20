#!/usr/bin/env bash

set -e

make -j$(( $(nproc) * 2 )) test
./brace-expansion-smoke-test.sh
