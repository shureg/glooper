#!/usr/bin/env sh
find . -regex ".*/[0-9][0-9][0-9].*\.py" -type f -printf "%f\n" | cut -c1-3 | sort | uniq | tail -n1
