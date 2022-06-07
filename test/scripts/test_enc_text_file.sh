#!/usr/bin/env bash

source "${BASH_SOURCE%/*}"/_setup_ah.sh
FILE="${BASH_SOURCE%/*}/../../COPYING"
echo "Testing compressing text file ..."
${AH} "${FILE}"
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing compressing text file done." \
     || echo "... Testing compressing text file failed with exit code ${EXITCODE}." >&2;
echo "Testing decompressing text file ..."
mv "${FILE}.ah" "${FILE}.tmp.ah"
${AH} -d "${FILE}.tmp.ah"
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing decompressing text file done." \
     || echo "... Testing decompressing text file failed with exit code ${EXITCODE}." >&2;
echo "Testing output ..."
diff "${FILE}" "${FILE}.tmp" > /dev/null
EXITCODE=$?
test ${EXITCODE} -eq 0 && echo "... Testing output done." \
     || echo "... Testing output failed." >&2;
rm "${FILE}.tmp.ah" "${FILE}.tmp"   # comment this line to skip output deletion
test ${EXITCODE} -eq 0
