#!/usr/bin/env bash

source "${BASH_SOURCE%/*}"/_setup_ah.sh
echo "Testing help ..."
${AH} -h >/dev/null
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing help done." \
     || echo "... Testing help failed with exit code ${EXITCODE}." >&2;
test ${EXITCODE} -eq 0
