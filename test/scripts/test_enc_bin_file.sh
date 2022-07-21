#!/usr/bin/env bash

source "${BASH_SOURCE%/*}"/_setup_ah.sh
FILE="${BASH_SOURCE%/*}/../../out/ah"       # Lets compress itself
echo "Testing compressing binary file ..."
${AH} "${FILE}"
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing compressing binary file done." \
     || echo "... Testing compressing binary file failed with exit code ${EXITCODE}." >&2;
echo "Testing decompressing binary file ..."
mv "${FILE}.ah" "${FILE}.tmp.ah"
${AH} -d "${FILE}.tmp.ah"
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing decompressing binary file done." \
     || echo "... Testing decompressing binary file failed with exit code ${EXITCODE}." >&2;
echo "Testing output ..."
diff "${FILE}" "${FILE}.tmp" > /dev/null
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing output done." \
     || echo "... Testing output failed." >&2;
rm "${FILE}.tmp.ah" "${FILE}.tmp"   # comment this line to skip output deletion
test ${EXITCODE} -eq 0
