#!/usr/bin/env bash

source "${BASH_SOURCE%/*}"/_setup_ah.sh
echo "Testing verbose mode ..."
echo "short message" | ${AH} -cv >/dev/null 2>/dev/null
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing verbose mode done." \
     || echo "... Testing verbose mode failed with exit code ${EXITCODE}." >&2;
test ${EXITCODE} -eq 0
