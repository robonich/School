#!/bin/bash
sox doremi.wav -t raw -b 16 -c 1 -e s -r 44100 - | ./downsample 44 | play -t raw -b 16 -c 1 -e s -r 1000 -
