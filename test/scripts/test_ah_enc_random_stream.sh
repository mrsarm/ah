#!/usr/bin/env bash

AH="out/ah"
echo "Testing compressing and decompressing random stream ..."
RANDOM_MSG="${RANDOM}${RANDOM}${RANDOM}${RANDOM}"
echo -n ${RANDOM_MSG} | ${AH} -c | ${AH} -d | grep ${RANDOM_MSG} >/dev/null
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing compressing and decompressing random stream done." \
     || echo "... Testing compressing and decompressing random stream failed with exit code ${EXITCODE}. " \
             "Random number used was '${RANDOM_MSG}'." >&2;
test ${EXITCODE} -eq 0
