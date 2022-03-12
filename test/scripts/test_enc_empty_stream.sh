#!/usr/bin/env bash

source "${BASH_SOURCE%/*}"/_setup_ah.sh
echo "Testing compressing and decompressing empty stream ..."
echo -n "" | ${AH} -cv 2>/dev/null | ${AH} -dv 2>/dev/null | wc --bytes | grep "0" >/dev/null
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing compressing and decompressing empty stream done." \
     || echo "... Testing compressing and decompressing empty stream failed with exit code ${EXITCODE}." >&2;
test ${EXITCODE} -eq 0
