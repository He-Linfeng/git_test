# 编译Linux内核

## 下载源码

    内核下载网址：https://www.kernel.org

## 配置环境

    sudo apt update
    sudo apt upgrade
    sudo apt install libncurses,flex,bison,libssl-dev,libelf-dev

## 解压内核

    tar -zxf linux-4.14.6.tar.xz
    cd linux-4.14.6

## 配置内核

    #xxx为自己名字，中间参数表述输出路径，可以选择不要,则保存在源码一起
    make O=/home/xxx/build/kernel oldconfig
    
    #上面表示配置沿用以前版本的，下面为界面配置，可能要最大化终端   
    make O=/home/xxx/build/kernel menuconfig 

    #如果实在不会配置的话，就用默认配置，直接保存，得到.config文件
    #如果生成的.config中有“debian/canonical-certs.pem“，删掉引号内的东西”“

## 编译内核

    #编译内核，-j4表示并发编程
    make O=/home/xxx/build/kernel -j4

## 安装模块

    sudo make O=/home/xxx/build/kernel modules_install 

# 安装内核

    sudo make O=/home/xxx/build/kernel install 

# 重启

    #输入下面命令可以查看当前内核版本，重启之后应该变为我们编译的内核
    uname -r 