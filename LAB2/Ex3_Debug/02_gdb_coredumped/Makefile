all:
	gcc -g -o test src_coredumped.c
setup:
	ulimit -c unlimited	
	
	echo "1" | sudo tee /proc/sys/kernel/core_uses_pid 
	cat /proc/sys/kernel/core_uses_pid

	echo "core" | sudo tee /proc/sys/kernel/core_pattern
	cat /proc/sys/kernel/core_pattern
clean:
	rm -rf test 
	rm -rf core*