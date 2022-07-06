#!/bin/sh

APATH=src/overlays/actors
cd $APATH
for d in */
do
	cd $d
	OPATH=../../../../plugins/"$d"cfiles.mk
	>$OPATH
	printf "plugins/lib%s.so: " "${d%/}" >> $OPATH
	for cc in *.c
	do
		printf "plugins/%s%s " "$d" "${cc%.c}".o >> $OPATH
	done
	printf "\n\t\$(CXX) \$^ -o \$@ \$(LDFLAGS) -shared -fuse-ld=\$(LD) \$(LDDIRS)" >> $OPATH
	for cc in *.c
	do
		printf "\n\nplugins/%s%s: %s/%s%s\n\t\$(CC) -c -fPIC \$(CFLAGS) \$(CPPFLAGS) \$(OPTFLAGS) \$(INC_DIRS) $< -o \$@" "$d" "${cc%.c}".o "$APATH" "$d" "$cc" >> $OPATH
	done
	printf "\n" >> $OPATH
	cd ../
done
