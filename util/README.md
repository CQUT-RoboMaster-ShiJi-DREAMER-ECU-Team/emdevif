# emdevif_util

实用功能。

## BitInt

提供对任意位宽(<=64)整型的支持，使用方法与普通的整型类似。

示例：

```C++
import emdevif.util.bitInt;

int main()
{
    using namespace emdevif;
    
    BitInt<12> a;   // 12 位有符号整型
    UBitInt<20> b;  // 20 位无符号整型
    
    std::cout << "a + b = " << (a + b) << std::endl;  // 可以当作普通的整型使用
    return 0;
}
```

## InitOnce

提供限制变量只能进行一次写入的功能。

示例：

```C++
#include <iostream>
#include <string>
#include <cassert>

import emdevif.util.initOnce;

int main()
{
    using namespace emdevif;

    InitOnce<int> value;  // 未初始化

    value = 42;   // 第一次写入成功
    value = 100;  // 第二次写入将会被忽略

    std::cout << "Value: " << static_cast<int>(value) << std::endl;  // 输出 42
    std::cout << "Value: " << value() << std::endl;  // 也可以通过函数调用的方式获取值

    ////////////////////////////////

    InitOnce<float> fvalue(3.14f);  // 通过构造函数初始化
    fvalue = 2.71f;  // 已经被初始化了，写入将会被忽略

    std::cout << "FValue: " << fvalue() << std::endl;  // 输出 3.14

    ////////////////////////////////

    InitOnce<std::string> str_value;
    auto ec = str_value.init("Hello");  // 使用 init 方法初始化，能够获取错误码
    assert(ec == ErrorCode::Success);  // 初始化成功
    ec = str_value.init("World");  // 再次初始化将会失败
    assert(ec == ErrorCode::AlreadyExists);  // 初始化失败

    InitOnce<std::string> str_value2 = std::move(str_value);  // 支持移动语义
    std::cout << "str_value2: " << str_value2() << std::endl;  // 输出 Hello

    return 0;
}
```
