#!/bin/bash
x=0
while [ $x -ne 10000 ]
do
  ./syscall
  x=$(( $x+1 ))
done
