#!/bin/bash

letter=$1

./output.sh ${letter} | diff - test-cases/test-${letter}.out.txt

