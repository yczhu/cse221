# Generate files of size 4kb - 1GB
mkdir rand_files
cd rand_files
dd if=/dev/urandom of=4kb bs=4096 count=1
dd if=/dev/urandom of=8kb bs=4096 count=2
dd if=/dev/urandom of=16kb bs=4096 count=4
dd if=/dev/urandom of=32kb bs=4096 count=8
dd if=/dev/urandom of=64kb bs=4096 count=16
dd if=/dev/urandom of=128kb bs=4096 count=32
dd if=/dev/urandom of=256kb bs=4096 count=64
dd if=/dev/urandom of=512kb bs=4096 count=128
dd if=/dev/urandom of=1mb bs=4096 count=256
dd if=/dev/urandom of=2mb bs=4096 count=512
dd if=/dev/urandom of=4mb bs=4096 count=1024
dd if=/dev/urandom of=8mb bs=4096 count=2048
dd if=/dev/urandom of=16mb bs=4096 count=4096
dd if=/dev/urandom of=32mb bs=4096 count=8192
dd if=/dev/urandom of=64mb bs=4096 count=16384
dd if=/dev/urandom of=128mb bs=4096 count=32768
dd if=/dev/urandom of=256mb bs=4096 count=65536
dd if=/dev/urandom of=512mb bs=4096 count=131072
dd if=/dev/urandom of=1gb bs=4096 count=262144
