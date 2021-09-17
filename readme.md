# git 相关配置及操作
## linux安装git
```
sudo apt install git
```
## 设置用户名和邮箱
```
git config --global user.name "your name"
git config --global user.email "your email"
```
## 查看用户名和邮箱
```
git config user.name
git config user.email
```
## 创建版本库
```
mkdir hello
cd hello
git init
```
## 创建文件并提交
```
echo This is my first git project ! > readme.txt
git status	//查看状态
git add readme.txt //将文件添加到仓库
git status 
git commit -m "create readme.txt" //提交到仓库
```
## 修改文件并提交
```
echo This is my first git project and I change the file > readme.txt
git status //查看状态
git diff //查看修改内容
git add . //添加所有修改
git status 
git commit -m "change readme.txt"
```
## 版本回退
```
git log //查看日志
git reset --hard HEAD ^ //回退一个版本
git reset --hard 
```

