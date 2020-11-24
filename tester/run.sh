#!/bin/bash
cat inputs | stdbuf -o 0 ../Program | tee outputs
