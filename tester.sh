#!/bin/bash
clear
echo "
-----------------------------------------------------------------
Compile and run
-----------------------------------------------------------------"
echo "rm ./txt/*.huf"
rm ./txt/*.huf
echo "rm ./txt/*.dehuf"
rm ./txt/*.dehuf
echo "gcc -o huf huf.c functions.c -std=c99"
gcc -o huf huf.c functions.c -std=c99
echo "gcc -o dehuf dehuf.c functions.c -std=c99"
gcc -o dehuf dehuf.c functions.c -std=c99

echo "huf ./txt/all.txt ./txt/all.txt.huf > /dev/null"
./huf ./txt/all.txt ./txt/all.txt.huf > /dev/null
echo "huf ./txt/shakespearcomplete.txt ./txt/shakespearcomplete.txt.huf > /dev/null"
./huf ./txt/shakespearcomplete.txt ./txt/shakespearcomplete.txt.huf > /dev/null

echo "dehuf ./txt/all.txt.huf ./txt/all.txt.dehuf > /dev/null"
./dehuf ./txt/all.txt.huf ./txt/all.txt.dehuf > /dev/null
echo "dehuf ./txt/shakespearcomplete.txt.huf ./txt/shakespearcomplete.txt.dehuf  > /dev/null"
./dehuf ./txt/shakespearcomplete.txt.huf ./txt/shakespearcomplete.txt.dehuf  > /dev/null

echo "
-----------------------------------------------------------------
Diffs:
-----------------------------------------------------------------"

echo "diff ./txt/all.txt ./txt/all.txt.dehuf renvoie:"
diff ./txt/all.txt ./txt/all.txt.dehuf

echo "diff ./txt/shakespearcomplete.txt ./txt/shakespearcomplete.txt.dehuf renvoie:"
diff ./txt/shakespearcomplete.txt ./txt/shakespearcomplete.txt.dehuf
