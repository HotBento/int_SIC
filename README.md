# int_SIC
本项目为浙江大学控制科学与工程学院毕业论文（设计）项目“动态环境下的影响力最大化算法研究”的附属代码。



### 1 编译准备

#### 1.1 操作系统

实验中使用的操作系统为Ubuntu20.04，其他Linux操作系统也可能能够完成编译，但目前未经过测试。

#### 1.2 编程语言及标准

主体部分使用C++20标准编写，数据处理及可视化使用MATLAB2020完成。请确保编译器支持C++20标准，推荐使用`g++-10`作为编译器。

#### 1.3 数据集

[sx-askubuntu](http://snap.stanford.edu/data/sx-askubuntu.html)、[sx-stackoverflow](http://snap.stanford.edu/data/sx-stackoverflow.html)、[sx-superuser](http://snap.stanford.edu/data/sx-superuser.html)、[higgs-twitter](http://snap.stanford.edu/data/higgs-twitter.html)

下载数据集后将其放在`{WorkspaceDirection}/database`文件夹中。其中，`{WorkspaceDirection}`为项目根目录。



### 2 编译

以`g++10`为例进行编译。

创建目录`{WorkspaceDirection}/output`，然后进行编译：

```shell
g++-10 {WorkspaceDirection}/src/sic_main.cpp {WorkspaceDirection}/int_SIC/src/sic.cpp {WorkspaceDirection}/src/sievestream_recurse.cpp -lpthread -std=c++20 -o {WorkspaceDirection}/output/sic_recurse.out
g++-10 {WorkspaceDirection}/src/sic_main.cpp {WorkspaceDirection}/int_SIC/src/sic.cpp {WorkspaceDirection}/src/sievestream_loop.cpp -lpthread -std=c++20 -o {WorkspaceDirection}/output/sic_loop.out
g++-10 {WorkspaceDirection}/src/sic_main.cpp {WorkspaceDirection}/int_SIC/src/sic.cpp {WorkspaceDirection}/src/sievestream_v0.cpp -lpthread -std=c++20 -o {WorkspaceDirection}/output/sic_v0.out
g++-10 {WorkspaceDirection}/src/sic_main.cpp {WorkspaceDirection}/int_SIC/src/sic.cpp {WorkspaceDirection}/src/sievestream_loop_noseedprune.cpp -lpthread -std=c++20 -o {WorkspaceDirection}/output/sic_loop_noseedprune.out
```

编译后在`{WorkspaceDirection}/output`中得到四个可执行文件。



### 3 运行
四个可执行文件的格式均相同，以下以`sic_loop.out`为例。

```shell
{WorkspaceDirection}/output/sic_loop.out -u 500000 -beta 0.2 -k 50 -n 25000 -l 2500 -dbplace {WorkspaceDirection}/database/higgs-activity_time.txt -logplace {WorkspaceDirection}/log/loop
```

`-u`、`-beta`、`-k`、`-n`、`-l`参数意义及设定值请参照论文内容。

`-dbplace`是数据集的路径；

`-logplace`是结果的存储路径。



### 4 数据处理及可视化

数据处理及可视化使用MATLAB2020完成。直接运行`{WorkspaceDirection}/log/expriment_plot.m`中的代码可以使用我们预先存储的结果得到论文中的图片。如果需要更改需要处理及可视化的结果文件请更改`{WorkspaceDirection}/log`中对应`.m`文件中的对应路径。