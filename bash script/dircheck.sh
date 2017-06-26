#! /bin/sh
for i in $@
do

num_subdir=0
num_file=0
size=0

#$i value of i ${i} if i>9
if [ -d $i ]
then
echo $i
num_subdir=`find $i -type d | wc -l`
num_subdir=$(($num_subdir -1))
num_files=`find $i -type f |wc -l`
size=`du -sm $i`
fi
echo Sub directories: $num_subdir
echo Files: $num_files
find . -size 0k
echo Size: $size
echo

done
