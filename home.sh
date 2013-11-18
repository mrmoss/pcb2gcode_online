#!/bin/sh

if [ $# -lt 1 ]
then
   exit 1
fi

if [ -f $1 ];
then
	#remove last two lines
	cp ${1} ${1}.tmp
	sed '$ d' ${1}.tmp > ${1}
	rm -f ${1}.tmp
	cp ${1} ${1}.tmp
	sed '$ d' ${1}.tmp > ${1}
	rm -f ${1}.tmp

	#add go home to end of file
	echo "G91 G28 X0\nG91 G28 Y0\nM2" > ${1}.tmp
	cat ${1} ${1}.tmp > ${1}.tmp2
	mv ${1}.tmp2 ${1}
	rm -f ${1}.tmp ${1}.tmp2
fi