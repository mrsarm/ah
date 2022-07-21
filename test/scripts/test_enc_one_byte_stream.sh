#!/usr/bin/env bash

source "${BASH_SOURCE%/*}"/_setup_ah.sh
echo "Testing compressing and decompressing stream with one byte ..."
echo -n "s" | ${AH} -cv 2>/dev/null | ${AH} -dv 2>/dev/null | egrep "^s$" >/dev/null
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing compressing and decompressing stream with one byte done." \
     || echo "... Testing compressing and decompressing stream with one byte failed with exit code ${EXITCODE}." >&2;
test ${EXITCODE} -eq 0
echo "Testing compressing and decompressing stream with one symbol ..."
echo -n "sssssssss" | ${AH} -cv 2>/dev/null | ${AH} -dv 2>/dev/null | egrep "^sssssssss$" >/dev/null
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing compressing and decompressing stream with one symbol done." \
     || echo "... Testing compressing and decompressing stream with one symbol failed with exit code ${EXITCODE}." >&2;
test ${EXITCODE} -eq 0
