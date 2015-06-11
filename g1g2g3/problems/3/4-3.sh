#!/bin/bash
./sin 10000 3528 44100 > sin.raw
./read_data2 sin.raw > sin.txt
