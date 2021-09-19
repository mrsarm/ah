#!/usr/bin/env bash

AH="out/ah"
echo "Testing wrong arguments ..."
${AH} -Nop >/dev/null 2>/dev/null
EXITCODE=$?
test ${EXITCODE} -eq 3 && echo "... Testing wrong arguments done." \
     || echo "... Testing wrong arguments failed with exit code ${EXITCODE}." >&2;
test ${EXITCODE} -eq 3
