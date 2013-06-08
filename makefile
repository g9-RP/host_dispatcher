# Khanh Cao Quoc Nguyen 
# 311253865

hostd: hostd.c utility.h pcb.h mab.h rsrc.h sigtrap.c

	gcc -g hostd.c -o hostd -Wall 
	gcc -g sigtrap.c -o process -Wall