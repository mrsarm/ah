#!/usr/bin/env bash

source "${BASH_SOURCE%/*}"/_setup_ah.sh
echo "Testing wrong input file ..."
echo "invalid_compressed_input" | ${AH} -dc >/dev/null 2>/dev/null
EXITCODE=$?
test ${EXITCODE} -eq 7 && echo "... Testing wrong input file done." \
     || echo "... Testing wrong input file failed with exit code ${EXITCODE}." >&2;
test ${EXITCODE} -eq 7
