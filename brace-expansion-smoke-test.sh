#!/usr/bin/env bash

# Just a sanity check for the expected behaviour of the binary given valid and invalid inputs.
# More extensive unit-tests are located in the *_test.cc files and can be run with `make test`.

set -e

assert_eq() {
    local expected="$1"
    local actual="$2"
    if [[ $expected != $actual ]]; then
        echo >&2 "FAIL: expected: '$expected', got '$actual'"
        exit 1
    fi
}

bin=brace-expansion-bin
make -sj$(( $(nproc) * 2 )) $bin
echo -n "smoke test: "

# command substitution strips trailing newline so we append 'x' to preserve it, then remove 'x'
output="$(echo 'A{b,{c,d}}{E}' | ./$bin; echo x)"
output="${output%x}"
assert_eq $'AbE AcE AdE\n' "$output"

output="$(echo 'A{b,{c,d}}{E' | ./$bin; echo x)"
output="${output%x}"
assert_eq '' "$output"
echo "OK"
