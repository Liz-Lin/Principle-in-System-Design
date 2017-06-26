#! /bin/sh

path=(`echo $PATH | tr ":" " "`)
#$@for all the parameters passed to the script
for i in $@
do
for f in ${path[*]}
do
if [ -f $f/$i ]
then
echo $f/$i
fi
done
done
