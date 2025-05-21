高效率的 $CP$ 模板

代码风格易懂，接口优秀。

对拍只需要传入三个函数即可。

例如：
```cpp
auto _data = [&](auto cout) {

};

auto solve1 = [&](auto cin, auto cout) {

};

auto solve2 = [&](auto cin, auto cout) {

};
compare(_data, solve1, solve2, 1000);
```

普通线段树只需要传入抽象群 $\text{info, op, e}$ 即可

懒线段树需要多 $\text{id, Composition, Mapping, Function}$

树状数组传入一个类型即可，可以重载加法为聚合操作即可实现其他操作的树状数组

$Dijkstra$ 需要如果传入抽象类型 $\text{Group}$, 类型要重载比较运算符

$EratosthenesSieve$ 高度优化，在小于 $10^8$ 下的效率比欧拉筛快近 $3$ 倍

先这样吧，以后再改