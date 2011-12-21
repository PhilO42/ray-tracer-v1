#!/bin/sh

for filename in *.txt
do 
  awk < $filename '{$3=0}{print $1" " $2" 0"}'> ${filename}2
  mv ${filename}2 $filename
done;
