### 什么是拷贝构造函数

   构造函数是一种特殊的类成员函数，是当创建一个类的对象时，它被调用来对类的数据成员进行初始化和分配内存。（构造函数的命名必须和类名完全相同）

构造函数可以被重载，可以多个，可以带参数。析构函数只有一个，不能被重载，不能带参数

**拷贝构造函数**是C++独有的，它是一种特殊的构造函数，用基于同一类的一个对象构造和初始化另一个对象。当没有重载拷贝构造函数时，通过默认拷贝构造函数来创建一个对象

**一、如果一个构造函数的第一个参数是自身类型的引用，且任何额外参数都有默认值，则此构造函数是拷贝构造函数。**

```c++
Class Foo{
    public:
    Foo();//默认构造函数
    Foo (const Foo&)；//拷贝构造函数
}；
```

拷贝构造函数的第一个参数必须是一个引用类型。

**二、拷贝构造函数调用的时机**

1）一个对象以值传递的方式传入函数体

```c++
class CExample   
{  
private:  
 int a;   
public:  
 //构造函数  
 CExample(int b)  
 {   
    a = b;  
    cout<<"creat: "<<a<<endl;  
 }    
 //拷贝构造  
 CExample(const CExample& C)  
 {  
    a = C.a;  
    cout<<"copy"<<endl;  
 }     
 //析构函数  
 ~CExample()  
 {  
    cout<< "delete: "<<a<<endl;  
 }   
 void Show ()  
 {  
    cout<<a<<endl;  
 }  
};    
//全局函数，传入的是对象  
void g_Fun(CExample C)  
{  
 cout<<"test"<<endl;  
}    
int main()  
{  
 CExample test(1);  
 //传入对象  
 g_Fun(test);   
 return 0;  
} 
```

调用g_Fun()时，会产生以下几个重要步骤：
  (1).test对象传入形参时，会先会产生一个临时变量，就叫 C 吧。
  (2).然后调用拷贝构造函数把test的值给C。 整个这两个步骤有点像：CExample C(test);
  (3).等g_Fun()执行完后, 析构掉 C 对象。

2）一个对象以值传递的方式从函数返回

```c++
class CExample   
{  
private:  
 int a;  
public:  
 //构造函数  
 CExample(int b)  
 {   
  a = b;  
 }    
 //拷贝构造  
 CExample(const CExample& C)  
 {  
  a = C.a;  
  cout<<"copy"<<endl;  
 }    
 void Show ()  
 {  
  cout<<a<<endl;  
 }  
};    
//全局函数  
CExample g_Fun()  
{  
 CExample temp(0);  
 return temp;  
}    
int main()  
{  
 g_Fun();  
 return 0;  
}  
```

当g_Fun()函数执行到return时，会产生以下几个重要步骤：
  (1). 先会产生一个临时变量，就叫XXXX吧。
  (2). 然后调用拷贝构造函数把temp的值给XXXX。整个这两个步骤有点像：CExample XXXX(temp);
  (3). 在函数执行到最后先析构temp局部变量。
  (4). 等g_Fun()执行完后再析构掉XXXX对象

3）一个对象需要通过另一个对象进行初始化

```c++
CExample A(100);  
CExample B = A;   
CExample B(A);  
```

后两句都会调用拷贝构造函数。

**三、深拷贝和浅拷贝**

浅拷贝：如果复制的对象中引用了一个外部内容（例如分配在堆上的数据），那么在复制这个对象的时候，让新旧两个对象指向同一个外部内容，就是浅拷贝。（指针虽然复制了，但所指向的空间内容并没有复制，而是由两个对象共用，两个对象不独立，删除空间存在）

深拷贝：如果在复制这个对象的时候为新对象制作了外部对象的独立复制，就是深拷贝。



```c++
class Rect  
{  
public:  
    Rect()      // 构造函数，p指向堆中分配的一空间  
    {  
        p = new int(100);  
    }  
    ~Rect()     // 析构函数，释放动态分配的空间  
    {  
        if(p != NULL)  
        {  
            delete p;  
        }  
    }  
private:  
    int width;  
    int height;  
    int *p;     // 一指针成员  
};  
  
int main()  
{  
    Rect rect1;  
    Rect rect2(rect1);   // 复制对象  
    return 0;  
}  
```

在这段代码运行结束之前，会出现一个运行错误。原因就在于在进行对象复制时，对于动态分配的内容没有进行正确的操作。我们来分析一下：

在运行定义rect1对象后，由于在构造函数中有一个动态分配的语句，因此执行后的内存情况大致如下：

![1522070867587](C:\Users\Xu\AppData\Local\Temp\1522070867587.png)

在使用rect1复制rect2时，由于执行的是浅拷贝，只是将成员的值进行赋值，这时 rect1.p = rect2.p，也即这两个指针指向了堆里的同一个空间，如下图所示：

![1522070905425](C:\Users\Xu\AppData\Local\Temp\1522070905425.png)

当然，这不是我们所期望的结果，在销毁对象时，两个对象的析构函数将对同一个内存空间释放两次，这就是错误出现的原因。我们需要的不是两个p有相同的值，而是两个p指向的空间有相同的值，解决办法就是使用“深拷贝”。



在“深拷贝”的情况下，对于对象中动态成员，就不能仅仅简单地赋值了，而应该重新动态分配空间，如上面的例子就应该按照如下的方式进行处理：

```c++
class Rect  
{  
public:  
    Rect()      // 构造函数，p指向堆中分配的一空间  
    {  
        p = new int(100);  
    }  
    Rect(const Rect& r)  
    {  
        width = r.width;  
        height = r.height;  
        p = new int;    // 为新对象重新动态分配空间  
        *p = *(r.p);  
    }  
    ~Rect()     // 析构函数，释放动态分配的空间  
    {  
        if(p != NULL)  
        {  
            delete p;  
        }  
    }  
private:  
    int width;  
    int height;  
    int *p;     // 一指针成员  
};  
```

此时，在完成对象的复制后，内存的一个大致情况如下：

![1522070976237](C:\Users\Xu\AppData\Local\Temp\1522070976237.png)

此时rect1的p和rect2的p各自指向一段内存空间，但它们指向的空间具有相同的内容，这就是所谓的“深拷贝”。



**四、防止默认拷贝发生**

通过对对象复制的分析，我们发现对象的复制大多在进行“值传递”时发生，这里有一个小技巧可以防止按值传递——**声明一个私有拷贝构造函数**。甚至不必去定义这个拷贝构造函数，这样因为拷贝构造函数是私有的，如果用户试图按值传递或函数返回该类对象，将得到一个编译错误，从而可以避免按值传递或返回对象。

```c++
// 防止按值传递  
class CExample   
{  
private:  
    int a;  
  
public:  
    //构造函数  
    CExample(int b)  
    {   
        a = b;  
        cout<<"creat: "<<a<<endl;  
    }  
  
private:  
    //拷贝构造，只是声明  
    CExample(const CExample& C);  
  
public:  
    ~CExample()  
    {  
        cout<< "delete: "<<a<<endl;  
    }  
  
    void Show ()  
    {  
        cout<<a<<endl;  
    }  
};  
  
//全局函数  
void g_Fun(CExample C)  
{  
    cout<<"test"<<endl;  
}  
  
int main()  
{  
    CExample test(1);  
    //g_Fun(test); 按值传递将出错  
      
    return 0;  
}   
```

