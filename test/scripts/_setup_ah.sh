#!/usr/bin/env bash
if [ -z "${AH}" ]; then
    if [ -x "../out/ah" ]; then
        AH="../out/ah"
    elif [ -x "out/ah" ]; then
        AH="out/ah"
    else
        AH="ah"
    fi
fi
