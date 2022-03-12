#!/usr/bin/env bash

source "${BASH_SOURCE%/*}"/_setup_ah.sh
echo "Testing compressing and decompressing stream ..."
echo -n "short message" | ${AH} -cv 2>/dev/null | ${AH} -dv 2>/dev/null | grep "short message" >/dev/null
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing compressing and decompressing stream done." \
     || echo "... Testing compressing and decompressing stream failed with exit code ${EXITCODE}." >&2;
test ${EXITCODE} -eq 0
