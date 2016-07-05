# A minimalistic "container hypervisor" using linux-namespaces

## Usage

```
$ sudo c-microvisor /path/to/your/root

#with flags:
$ sudo c-microvisor -[0PHImunc?] /path/to/your/root
```

## Flags

* `-0` No namespace at all
* `-P` No PID namespace
* `-H` No UTS namespace (Hostname, NIS domain name)
* `-I` No IPC namespace
* `-m` Own mount namespace
* `-u` Own user namespace
* `-n` Own net namespace
* `-c` Own cgroups namespace
* `-?` Show help

## Help output
```
Usage:
	c-microvisor `path'
	c-microvisor `flags' `path'
Flags:
	-0	No namespace at all
	-P	No PID namespace
	-H	No UTS namespace (Hostname, NIS domain name)
	-I	No IPC namespace
	-m	Own mount namespace
	-u	Own user namespace
	-n	Own net namespace
	-c	Own cgroups namespace
	-?	Print this help
```
