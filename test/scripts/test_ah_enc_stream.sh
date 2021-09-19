#!/usr/bin/env bash

source "${BASH_SOURCE%/*}"/_setup_ah.sh
echo "Testing compressing and decompressing stream ..."
echo -n "short message" | ${AH} -c | ${AH} -d | grep "short message" >/dev/null
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing compressing and decompressing stream done." \
     || echo "... Testing compressing and decompressing stream failed with exit code ${EXITCODE}." >&2;
test ${EXITCODE} -eq 0
