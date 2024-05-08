riscv-gnu-toolchain is the source code of riscv-gnu-toolchain.
riscv-toolchain is the compiled file of riscv-gnu-toolchain


Progress:
1. Clone the riscv-gnu-toolchain.
```bash
git clone --recursive https://gitee.com/mirrors/riscv-gnu-toolchain.git
```

2. Install some tools
```bash
sudo apt-get install autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build
```

3. write the following path in bashrc
```
export RISCV="/root/riscv-toolchain"
export PATH=$PATH:$RISCV/bin
```
it means the riscv-gnu-toolchain will be installed to /root/riscv-toolchain. Then `source ~/.bashrc`.

4. cd riscv-gnu-toolchain folder run 
```
cd riscv-gnu-toolchain/
./configure --prefix=$RISCV
make linux -j32
```

### Riscv cross compile and qemu env
Progress:
1. 安装qemu虚拟化工具，在终端下执了如下命令:
```bash
sudo apt-get install qemu-user-static
```
Test if the qemu-riscv64-static is installed successfully:
```bash
qemu-riscv64-static -version
```
The follow info shows up:
```bash
root@bbdb2ab67207:~# qemu-riscv64-static -version
qemu-riscv64 version 6.2.0 (Debian 1:6.2+dfsg-2ubuntu6.6)
Copyright (c) 2003-2021 Fabrice Bellard and the QEMU Project developers
```

2. 下载ubuntu_riscv根文件系统发行版
```bash
apt install wget
```
The file system can be found on `http://cdimage.ubuntu.com/ubuntu-base/`:
```bash
wget https://cdimage.ubuntu.com/ubuntu-base/releases/20.04/release/ubuntu-base-20.04.2-base-riscv64.tar.gz
mkdir ubuntu_riscv64
tar -xvf ubuntu-base-20.04.2-base-riscv64.tar.gz -C ubuntu_riscv64/
```

3. Copy `qemu-riscv64-static` to ubuntu file system
```bash
sudo cp /usr/bin/qemu-riscv64-static ubuntu_riscv64/usr/bin/
```

4. Copy the host's DNS configuration file to the risvc ubuntu file system
```bash
sudo cp /etc/resolv.conf  ubuntu-ubuntu_riscv64/etc/resolv.conf
```

5. Creat bash file for mount and unmount the riscv ubuntu file system for convenience
```bash
vi ch-mount.sh
```

Write in the following script
```bash
#!/bin/bash

function mnt() {
    echo "MOUNTING"
    sudo mount -t proc /proc ${2}proc
    sudo mount -t sysfs /sys ${2}sys
    sudo mount -o bind /dev ${2}dev

    sudo chroot ${2}
}

function umnt() {
    echo "UNMOUNTING"
    sudo umount ${2}proc
    sudo umount ${2}sys
    sudo umount ${2}dev
}

if [ "$1" == "-m" ] && [ -n "$2" ] ;
then
    mnt $1 $2
elif [ "$1" == "-u" ] && [ -n "$2" ];
then
    umnt $1 $2
else
    echo ""
    echo "Either 1'st, 2'nd or both parameters were missing"
    echo ""
    echo "1'st parameter can be one of these: -m(mount) OR -u(umount)"
    echo "2'nd parameter is the full path of rootfs directory(with trailing '/')"
    echo ""
    echo "For example: ch-mount -m /media/sdcard/"
    echo ""
    echo 1st parameter : ${1}
    echo 2nd parameter : ${2}
fi
```

Give the bash script the authority:
```bash
sudo chmod +x ch-mount.sh
```

The run the script to mount:
```bash
sudo ./ch-mount.sh -m ubuntu_riscv64/
```

The un mount:
```bash
sudo ./ch-mount.sh -u ubuntu_riscv64/
```

You will meet errors:
```bash
chroot: failed to run command '/bin/bash': Exec format error
```
Unmount!!! if you meet error!!! 

You should enable binfmts
```bash
apt-get install binfmt-support
update-binfmts --display
```

The `update-binfmts --display` show that the arch is disable, enable them:
```bash
update-binfmts --enable
```

Check again use `update-binfmts --display`, you will find that they are enables.

Then you can run `sudo ./ch-mount.sh -m ubuntu_riscv64/` again to mount again(UNMOUNT FIRST IF YOU MEET ERROR!!!!!USE THE UNMOUNT -u script)

The run `pwd` or `uname -a`, you can find you are in the RISCV65 system:
```bash
$ uname -a
Linux a5568792fe82 5.15.0-50-generic #56~20.04.1-Ubuntu SMP Tue Sep 27 15:51:29 UTC 2022 riscv64 riscv64 riscv64 GNU/Linux
```

Run `exit` if you want to turn back to host, and DONT FORGET TO UNMOUNT!!


Other modules may be required to install, install lmod:
```bash
apt-get install kmod
```
