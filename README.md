# devector
Header-only **double ended vector** implementation for efficient insertions/removals at both the start  &amp; end of it at the expense of an extra pointer.

Following C++ standard with slight modifications.

Currently C++11, C++14 coming next.

**Now in Beta**

## Implementation details
**devector** keeps a dynamic array internally which it uses to store its objects. It differs from *std::vector* from the fact that *begin()* isn't necessarily pointing to the start of the array, it's instead up to the user to decide.

A simple visualisation:

![image](https://user-images.githubusercontent.com/72366635/153786421-6ba94e57-47f1-42d2-a190-d12f2d003eeb.png)

In order to control where *begin* is placed, a third template parameter named **OffsetBy** is introduced which **should** have a method with the following signature:

`size_t off_by(size_t free_blocks);`

The one and only parameter is the count of free slots inside the array at any given moment the method is called.
It should return an integer which is going to be **begin**'s offset from **array_begin**. You are free to make the method **static, const, noexcept** or whatever else you desire. The default implementation is this:

![image](https://user-images.githubusercontent.com/72366635/153787147-86a15cce-aa3a-4b00-97b8-93fb419e699d.png)

The offset is used upon reallocations, assignments of any sort, or shifting caused from operations such as *insert*, *erase* *operator=* etc...

A general rule of thumb is that whenever an operation has to move and/or copy things around, devector will automatically place them in the desired position according to the offset.


As a consequence of the above, if you make a custom OffsetBy class that always returns 0 as offset, the behavior should be exactly the same as **std::vector**.

If needed in the future **offset_by_traits** shall also be introduced.
## Which methods differ from std::vector and how

This container implements the very same methods the [standard](https://en.cppreference.com/w/cpp/container/vector) does, following exception safety rules, iterator validity and even thread-safety wherever possible. The differences are in fact:

Any operation that causes shifting, like **insert at any position besides begin & end, erase at any position besides begin & end, emplace at any position besides begin & end** will now not only invalidate the iterators following **pos**(the position iterator each of these functions take as argument), but the whole [begin,end) set except for very specific situations which you shouldn't rely upon.

New methods are introduced, here is a list:
* resize_front()
* resize_back()
* push_front()
* pop_front()
* emplace_front()
* get_offset_by()

each of which does exactly the same their x_back() pair does but for the front instead. resize() still exists and defaults to resize_back().


Every constructor or operation that previously had an optional **allocator_type& alloc** parameter now also has an optional **offset_by_type& off_by** type.


## Collaborate
You are absolutely welcome to report bugs, contribute by solving issues, or even help build C++14, C++17, and C++20 versions.

## About
Licenced under MIT Licence.
