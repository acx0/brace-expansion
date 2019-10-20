#!/usr/bin/env bash

set -e

make -j$(( $(nproc) * 2 )) brace-expansion-bin
