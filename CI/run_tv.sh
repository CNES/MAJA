#!/bin/bash

if [ $# -lt 1 ]; then
  echo Usage: $0  tv_content.txt
  exit 1
fi

echo Run TV

rm -rf logs
rm -rf err_logs
mkdir logs
mkdir err_logs

OLDIFS=$IFS
IFS=$'\n'
for cmd in $(cat $1 | grep 'Test command'); do
  TEST_ID=$(echo "$cmd" | cut -d ':' -f 1)
  TEST_NAME=$(grep "Test #$TEST_ID:" $1 | cut -d ':' -f 2)
  echo "Run #$TEST_ID $TEST_NAME"
  echo $cmd | cut -d ' ' -f 4- > logs/${TEST_ID}_${TEST_NAME}.log
  bash -c "$(echo $cmd | cut -d ' ' -f 4-)" >> logs/${TEST_ID}_${TEST_NAME}.log 2>&1
  RET_CODE=$?
  if [ $RET_CODE -ne 0 ]; then cp logs/${TEST_ID}_${TEST_NAME}.log err_logs ; fi
done
IFS=$OLDIFS

NUM_TEST_FAILED=$(ls err_logs | wc -l)
if [ $NUM_TEST_FAILED -ne 0 ]; then
echo $NUM_TEST_FAILED tests failed, check:
ls err_logs
exit 1
fi

echo All tests passed
