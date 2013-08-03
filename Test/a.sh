#/bin/bash

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
do
newfile="com$i""_neu.bmp"
newneural="com$i.bin"
newJPEG="com$i""_JPEG.bmp"
echo Neural Network Compression Time
time ./neu.out c com$i.png > faaltu.txt
echo Neural Network Decompression Time 
time ./neu.out d $newneural > faaltu.txt
echo com$i.png $newfile n > tempFile.txt
echo Neural Network Performance 
./noise < tempFile.txt
echo JPEG Compression Time
echo c com$i.png temp > tempJPEG
time ./JPEG < tempJPEG >opt
echo d temp $newJPEG > tempJPG
echo JPEG Decompression Time 
time ./JPEG < tempJPG >opt
echo com$i.png $newJPEG n > tempFile.txt
echo JPEG Performance
./noise < tempFile.txt
echo File Sizes in kilobytes
echo Original File Size
echo $(($(stat -c%s $newfile)/1024))
echo Neural Network Size
echo $(($(stat -c%s $newneural)/1024))
echo JPEG Size
temp="temp"
echo $(($(stat -c%s $temp)/1024))
echo 
echo
done
