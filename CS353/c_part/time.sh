#!/bin/bash
echo "Compilation..."
`cd tp1;gcc -Wall liste.c -o liste`
`cd tp2;gcc -Wall liste.c -o liste`
echo "...Done"
echo "---------- TP1 ----------"
tail -n1 < <(time ./tp1/liste)
echo "---------- TP2 ----------"
tail -n1 < <(time ./tp2/liste)