#!/usr/bin/env bash

source "${BASH_SOURCE%/*}"/_setup_ah.sh
echo "Testing compressing and decompressing stream with two byte ..."
echo -n "10" | ${AH} -cv 2>/dev/null | ${AH} -dv 2>/dev/null | egrep "^10$" >/dev/null
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing compressing and decompressing stream with two bytes done." \
     || echo "... Testing compressing and decompressing stream with two bytes failed with exit code ${EXITCODE}." >&2;
test ${EXITCODE} -eq 0
echo "Testing compressing and decompressing stream with two symbols ..."
echo -n "101010" | ${AH} -cv 2>/dev/null | ${AH} -dv 2>/dev/null | egrep "^101010$" >/dev/null
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing compressing and decompressing stream with two symbols done." \
     || echo "... Testing compressing and decompressing stream with two symbols failed with exit code ${EXITCODE}." >&2;
test ${EXITCODE} -eq 0
