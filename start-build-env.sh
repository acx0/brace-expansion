#!/usr/bin/env bash

set -e

tag="brace-expansion"
docker build -t "$tag" .

# SYS_PTRACE allows sanitized executables to run in container
docker run --cap-add SYS_PTRACE -it "$tag"
