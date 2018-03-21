echo off
start wc.exe -c test1.txt -o output1.txt
start wc.exe -w test1.txt -o output2.txt
start wc.exe -l test1.txt -o output3.txt
start wc.exe -a file.c -o output4.txt
start wc.exe -c -w -l -a file.c -o output5.txt
start wc.exe -w -e stoplist test1.txt -o output6.txt
start wc.exe -c -s .\dir\*.txt -o output7.txt 
start wc.exe -w -s .\dir\*.txt -o output8.txt
start wc.exe -l -s .\dir\*.txt -o output9.txt
start wc.exe -a -s .\dir\*.c -o output10.txt
start wc.exe -a -c -w -l -s .\dir\*.c -o output11.txt