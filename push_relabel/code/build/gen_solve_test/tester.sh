#!/bin/bash

for i in {1...5}; do
   t = $i
   for j in {1...35}	
      r = $j
      c = 36 - $j
      max = 10*$j
      echo "Test Case: type = $t"
      echo "r = $r"
      echo "c = $c" 

done 
