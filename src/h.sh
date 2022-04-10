#!/bin/bash 
 
# script file for BASH 
# which bash
# save this file as h.sh
# chmod +x h.sh
# ./h.sh
# checked in https://www.shellcheck.net/



printf "make color bar images (ppm files) and txt files with data using c console program\n"
gcc h.c -lm -Wall
./a.out

printf "make color profiles : convert txt files to png using gnuplot script\n"
gnuplot h.gp 


printf "convert all ppm files in this directory to png using Image Magic convert program\n"
for file in *.ppm ; do
  # b is name of file without extension
  b=$(basename "$file" .ppm)
  # convert  using ImageMagic
  convert "${b}".ppm "${b}".png
done


printf "delete all ppm and txt  files \n"
rm ./*.ppm
# rm ./*.txt



printf "info about software \n"
bash --version
make -v
gcc --version
gnuplot --version
convert -version
convert -list resource

