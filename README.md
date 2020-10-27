# myantileech
## aMule 反吸血库
基于 https://github.com/persmule/amule-dlp.antiLeech 的修改版本，未测试
使用 clang-format 格式化源代码
编译完成的后，直接将 libantiLeech.so 扔到 $HOME/.aMule，然后在 amule 里重新加载
## 编译与安装命令
### 准备
C++ 编译器至少支持 -std=c++11
### 编译
```sh
# 清除环境
$ make clean
```
```sh
# 编译
$ make
```
```sh
# 安装（安装到 $HOME/.aMule 目录下)
$ make install
```
### 其他编译选项
```sh
# 启用 DLP_ASSERT
$ CXXFLAGS='-DDLP_USE_ASSERT' make
```
```sh
# 启用DLP_CDBG（打印测试信息到 stderr）
$ CXXFLAGS='-DDLP_USE_CDBG' make
```
