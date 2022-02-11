/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * MIT License
 *
 * Copyright (c) 2022 Valasiadis Fotios
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 
 * devector.hpp 0.0.0
 * 
 * A header-only continuous-storage double ended vector implementation for efficient insertions/removals at both the start & end of it.
 */

#ifndef DEVECTOR_RDSL_2892021
#define DEVECTOR_RDSL_2892021

#include <memory>
#include <stdexcept>
#include <iterator>

namespace rdsl{

template<class al>
using al_traits = std::allocator_traits<al>;

template<class it>
using it_traits = std::iterator_traits<it>;

template<class It>
struct is_at_least_forward{
    static constexpr bool value = false;
};

template<>
struct is_at_least_forward<std::forward_iterator_tag>{
    static constexpr bool value = true;
};

template<>
struct is_at_least_forward<std::bidirectional_iterator_tag>{
    static constexpr bool value = true;
};

template<>
struct is_at_least_forward<std::random_access_iterator_tag>{
    static constexpr bool value = true;
};

template<class It>
struct is_at_least_input{
    static constexpr bool value = is_at_least_forward<It>::value;
};

template<>
struct is_at_least_input<std::input_iterator_tag>{
    static constexpr bool value = true;
};

#if __cplusplus < 201402L
template<bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;
#else
template<bool B, class T = void>
using enable_if_t = std::enable_if_t<B, T>;
#endif

template<class It>
using is_iterator = enable_if_t<is_at_least_input<typename it_traits<It>::iterator_category>::value, int>;

struct offset_by{
    static size_t off_by(size_t free_blocks) noexcept{
        return free_blocks / 2;
    }
};

template<typename T, class Alloc = std::allocator<T>, class OffsetBy = rdsl::offset_by>
struct devector{
    using value_type = T;
    using allocator_type = Alloc;
    using offset_by_type = OffsetBy;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename al_traits<allocator_type>::pointer;
    using const_pointer = typename al_traits<allocator_type>::const_pointer;
    using size_type = typename al_traits<allocator_type>::size_type;
    using difference_type = typename al_traits<allocator_type>::difference_type;

private:
    pointer arr;
    size_type capacity_;
    pointer begin_;
    pointer end_;

    static constexpr float factor = 1.6f; // new_capacity = capacity * factor
    offset_by_type offs;
    allocator_type alloc;

    template<class it>
    using it_traits = it_traits<it>;

    class devector_iterator: public std::iterator<std::random_access_iterator_tag, value_type>{
        pointer ptr = nullptr;

    public:
        devector_iterator() = default;
        devector_iterator(const devector_iterator&) = default;
        devector_iterator& operator=(const devector_iterator&) = default;
        ~devector_iterator() = default;

        devector_iterator(pointer ptr){
            this->ptr = ptr;
        }

        bool operator==(const devector_iterator& other) const noexcept{
            return ptr == other.ptr;
        }

        bool operator!=(const devector_iterator& other) const noexcept{
            return ptr != other.ptr;
        }

        reference operator*() noexcept{
            return *ptr;
        }

        const_reference operator*() const noexcept{
            return *ptr;
        }

        pointer operator->() noexcept{
            return ptr;
        }

        const_pointer operator->() const noexcept{
            return ptr;
        }

        devector_iterator& operator++() noexcept{
            ++ptr;
            return *this;
        }

        devector_iterator operator++(int) noexcept{
            return devector_iterator(ptr++);
        }

        devector_iterator& operator--() noexcept{
            --ptr;
            return *this;
        }

        devector_iterator operator--(int) noexcept{
            return devector_iterator(ptr--);
        }

        difference_type operator+(devector_iterator it) const noexcept{
            return ptr + it.ptr;
        }

        devector_iterator operator+(size_type i) const noexcept{
            return devector_iterator(ptr + i);
        }

        friend devector_iterator operator+(size_type i, const devector_iterator& it) noexcept{
            return devector_iterator(i + it.ptr);
        }

        difference_type operator-(devector_iterator it) const noexcept{
            return ptr - it.ptr;
        }

        devector_iterator operator-(size_type i) const noexcept{
            return devector_iterator(ptr - i);
        }

        friend devector_iterator operator-(size_type i, const devector_iterator& it) noexcept{
            return devector_iterator(i - it.ptr);
        }

        bool operator<(const devector_iterator& it) const noexcept{
            return ptr < it.ptr;
        }

        bool operator>(const devector_iterator& it) const noexcept{
            return ptr > it.ptr;
        }

        bool operator<=(const devector_iterator& it) const noexcept{
            return ptr <= it.ptr;
        }

        bool operator>=(const devector_iterator& it) const noexcept{
            return ptr >= it.ptr;
        }

        devector_iterator& operator+=(size_type i) noexcept{
            ptr += i;
            return *this;
        }

        devector_iterator& operator-=(size_type i) noexcept{
            ptr -= i;
            return *this;
        }

        reference operator[](size_type index) noexcept{
            return *(ptr + index);
        }

        const_reference operator[](size_type index) const noexcept{
            return *(ptr + index);
        }
    };
    
    class const_devector_iterator: public std::iterator<std::random_access_iterator_tag, value_type>{
        pointer ptr = nullptr;

    public:
        const_devector_iterator() = default;
        const_devector_iterator(const const_devector_iterator&) = default;
        const_devector_iterator& operator=(const const_devector_iterator&) = default;
        ~const_devector_iterator() = default;

        const_devector_iterator(pointer ptr){
            this->ptr = ptr;
        }

        const_devector_iterator(const devector_iterator& it){
            this->ptr = it.ptr;
        }

        const_devector_iterator& operator=(const devector_iterator& it){
            this->ptr = it.ptr;
        }

        bool operator==(const const_devector_iterator& other) const noexcept{
            return ptr == other.ptr;
        }

        bool operator!=(const const_devector_iterator& other) const noexcept{
            return !(*this == other);
        }

        const_reference operator*() const noexcept{
            return *ptr;
        }

        const_pointer operator->() const noexcept{
            return ptr;
        }

        const_devector_iterator& operator++() noexcept{
            ++ptr;
            return *this;
        }

        const_devector_iterator operator++(int) noexcept{
            return const_devector_iterator(ptr++);
        }

        const_devector_iterator& operator--() noexcept{
            --ptr;
            return *this;
        }

        const_devector_iterator operator--(int) noexcept{
            return const_devector_iterator(ptr--);
        }

        difference_type operator+(const_devector_iterator it) const noexcept{
            return ptr + it.ptr;
        }

        const_devector_iterator operator+(size_type i) const noexcept{
            return const_devector_iterator(ptr + i);
        }

        friend const_devector_iterator operator+(size_type i, const const_devector_iterator& it) noexcept{
            return const_devector_iterator(i + it.ptr);
        }

        difference_type operator-(const_devector_iterator it) const noexcept{
            return ptr - it.ptr;
        }

        const_devector_iterator operator-(size_type i) const noexcept{
            return const_devector_iterator(ptr - i);
        }

        friend const_devector_iterator operator-(size_type i, const const_devector_iterator& it) noexcept{
            return const_devector_iterator(i - it.ptr);
        }

        bool operator<(const const_devector_iterator& it) const noexcept{
            return ptr < it.ptr;
        }

        bool operator>(const const_devector_iterator& it) const noexcept{
            return ptr > it.ptr;
        }

        bool operator<=(const const_devector_iterator& it) const noexcept{
            return ptr <= it.ptr;
        }

        bool operator>=(const const_devector_iterator& it) const noexcept{
            return ptr >= it.ptr;
        }

        const_devector_iterator& operator+=(size_type i) noexcept{
            ptr += i;
            return *this;
        }

        const_devector_iterator& operator-=(size_type i) noexcept{
            ptr -= i;
            return *this;
        }

        const_reference operator[](size_type index) const noexcept{
            return *(ptr + index);
        }
    };
    
    struct buffer_guard{
        pointer begin;
        pointer end;
        allocator_type& alloc;

        buffer_guard(allocator_type& alloc, pointer begin, pointer end)
        : alloc(alloc), begin(begin), end(end) {}

        buffer_guard(allocator_type& alloc, pointer start)
        : alloc(alloc), begin(start), end(start) {}

        void release(){
            begin = end;
        }

        ~buffer_guard(){
            while(begin != end){
                al_traits<allocator_type>::destroy(alloc, begin);
                ++begin;
            }
        }
    };

    struct memory_guard{
        pointer arr;
        size_type capacity;
        allocator_type& alloc;

        memory_guard(allocator_type& alloc, size_type capacity)
        : alloc(alloc), capacity(capacity), arr(alloc.allocate(capacity)) {}

        void release(){
            arr = nullptr;
        }

        ~memory_guard(){
            if(arr){
                alloc.deallocate(arr, capacity);
            }
        }
    };

public:
    using iterator = devector_iterator;
    using const_iterator = const_devector_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    size_type capacity() const noexcept{
        return capacity_;
    }

    size_type max_size() const{
        return al_traits<allocator_type>::max_size(alloc);
    }

    iterator begin() noexcept{
        return begin_;
    }

    const_iterator begin() const noexcept{
        return begin_;
    }

    iterator end() noexcept{
        return end_;
    }

    const_iterator end() const noexcept{
        return end_;
    }

    reverse_iterator rbegin() noexcept{
        return reverse_iterator(end_);
    }

    const_reverse_iterator rbegin() const noexcept{
        return const_reverse_iterator(end_);
    }

    reverse_iterator rend() noexcept{
        return reverse_iterator(begin_);
    }

    const_reverse_iterator rend() const noexcept{
        return const_reverse_iterator(begin_);
    }

    const_iterator cbegin() const noexcept{
        return begin();
    }

    const_iterator cend() const noexcept{
        return end();
    }

    const_reverse_iterator crbegin() const noexcept{
        return rbegin();
    }

    const_reverse_iterator crend() const noexcept{
        return rend();
    }

    size_type size() const noexcept{
        return end() - begin();
    }

    bool empty() const noexcept{
        return end() == begin();
    }

private:
    size_type free_front() const noexcept{ return begin_ - iterator(arr); }
    size_type free_back() const noexcept{ return arr + capacity_ - end_; }
    size_type free_total() const noexcept{ return capacity_ - size(); }

    pointer allocate_n(size_type n){
        auto ptr = alloc.allocate(n);
        capacity_ = n;
        return ptr;
    }

    void deallocate() noexcept{
        if(capacity_){
            alloc.deallocate(arr, capacity_);
            capacity_ = 0;
        }
    }

    void construct(size_type n, const_reference val){
        begin_ = end_ = arr + offs.off_by(capacity_ - n);
        while(n--){
            al_traits<allocator_type>::construct(alloc, end_, val);
            ++end_;
        }
    }

    template<class InputIterator, is_iterator<InputIterator> = 0>
    void construct(InputIterator first, InputIterator last, size_type distance){
        begin_ = end_ = arr + offs.off_by(capacity_ - distance);
        while(first != last){
            al_traits<allocator_type>::construct(alloc, end_, *first);
            ++first;
            ++end_;
        }
    }

    template<class InputIterator, is_iterator<InputIterator> = 0>
    void construct_move(InputIterator first, InputIterator last, size_type distance){
        begin_ = end_ = arr + offs.off_by(capacity_ - distance);
        while(first != last){
            al_traits<allocator_type>::construct(alloc, end_, std::move_if_noexcept(*first));
            ++first;
            ++end_;
        }
    }

    void destroy_all() noexcept{
        while(begin_ != end_){
            al_traits<allocator_type>::destroy(alloc, begin_);
            ++begin_;
        }
    }

    void steal_ownership(devector& x) noexcept{
        capacity_ = x.capacity_;
        arr = x.arr;
        begin_ = x.begin_;
        end_ = x.end_;
        
        x.arr = x.begin_ = x.end_ = nullptr;
        x.capacity_ = 0;
    }
    
    bool in_bounds(iterator it) const noexcept{
        return it >= begin_ && it < end_;
    }

    int next_capacity() const noexcept{
        return factor * capacity_ + 1;
    }
    /**
     * @brief Allocates a new memory chunk of *new_capacity* capacity and copies all elements into it, respecting the offset factor.
     * *new capacity* should be greater equal to size.
     */
    void reallocate(size_type new_capacity, size_type offset){
        memory_guard mem_guard(alloc, new_capacity);

        buffer_guard buf_guard(alloc, mem_guard.arr + offset, mem_guard.arr + offset + size());
     
        for(; begin_ != end_; ++begin_, ++buf_guard.end){
            al_traits<allocator_type>::construct(alloc, buf_guard.end, std::move_if_noexcept(*begin_));
            al_traits<allocator_type>::destroy(alloc, begin_);
        }
        deallocate();

        arr = mem_guard.arr;
        begin_ = buf_guard.begin;
        end_ = buf_guard.end;
        capacity_ = new_capacity;

        buf_guard.release();
        mem_guard.release();
    }

    void reallocate(size_type new_capacity){
        reallocate(new_capacity, offs.off_by(free_total()));
    }

    template<int step, int dec, class Pred>
    void move_elements(iterator& dest, iterator& src, Pred pred){
        while(!empty() && !in_bounds(dest) && pred()){
            al_traits<allocator_type>::construct(alloc, dest, std::move(src[dec]));
            al_traits<allocator_type>::destroy(alloc, src + dec);
            src = src + step;
            dest = dest + step;
        }
    }

    iterator shift(iterator new_begin, iterator new_end, const_iterator pos, size_type n){
        iterator free_space;

        buffer_guard front_guard(alloc, new_begin);
        move_elements<1, 0>(front_guard.end, begin_, [this, pos]{ return begin_ != pos; });

        buffer_guard front_guard1(alloc, front_guard.end + n);
        if(begin_ == pos){
            free_space = front_guard.end;
            move_elements<1, 0>(front_guard1.end, begin_, []{ return true; });
        }else{
            front_guard1.release();
        }

        buffer_guard back_guard(alloc, new_end);
        move_elements<-1, -1>(back_guard.begin, end_, [this, pos]{ return end_ != pos; });

        buffer_guard back_guard1(alloc, back_guard.begin - n);
        if(end_ == pos){
            free_space = back_guard.begin - n + 1;
            move_elements<-1, -1>(back_guard1.begin, end_, []{ return true; });
        }else{
            back_guard1.release();
        }

        front_guard.release();
        front_guard1.release();
        back_guard.release();
        back_guard1.release();

        return free_space;
    }

    size_type capacity_to_fit(size_type n) const noexcept{
        float temp_capacity = capacity_ ? capacity_ : 1;
        do{
            temp_capacity = factor * temp_capacity;
        }while(temp_capacity < n);
        return static_cast<size_type>(temp_capacity);
    }

    template<class... Value>
    iterator insert_impl(const_iterator position, size_type n, Value&&... val){
        iterator pos; // position of first newly-created element

        if(n <= free_total()){
            if(position == begin_){
                buffer_guard front_guard(begin_ - n);
                while(n--){
                    al_traits<allocator_type>::construct(alloc, front_guard.end, std::forward<Value>(val)...);
                    ++front_guard.end;
                }
                begin_ = front_guard.begin;
                front_guard.release();
            }else if(position == end_){
                while(n--){
                    al_traits<allocator_type>::construct(alloc, end_, std::forward<Value>(val)...);
                    ++end_;
                }
            }else{
                const iterator new_begin = arr + offs.off_by(free_total() - n);
                const iterator new_end = new_begin + n + size();

                const iterator free_space = shift(new_begin, new_end, position, n);

                buffer_guard front_guard(alloc, new_begin, free_space);
                buffer_guard back_guard(alloc, free_space + n, new_end);

                while(n--){
                    al_traits<allocator_type>::construct(alloc, front_guard.end, std::forward<Value>(val)...);
                    ++front_guard.end;
                }

                front_guard.release();
                back_guard.release();

                pos = free_space;
            }
        }else{
            const size_type new_size = size() + n;

            memory_guard mem_guard(alloc, capacity_to_fit(new_size));
            
            const size_type front_space = offs.off_by(mem_guard.capacity - new_size);

            buffer_guard buf_guard(alloc, mem_guard.arr + front_space);

            auto it = begin();
            for(; it != position; ++it){
                al_traits<allocator_type>::construct(alloc, buf_guard.end, std::move_if_noexcept(*it));
                ++buf_guard.end;
            }

            pos = buf_guard.end;
            while(n--){
                al_traits<allocator_type>::construct(alloc, buf_guard.end, std::forward<Value>(val)...);
                ++buf_guard.end;
            }
            for(; it != end(); ++it){
                al_traits<allocator_type>::construct(alloc, std::move_if_noexcept(*it));
            }

            destroy_all();
            deallocate();

            arr = mem_guard.arr;
            capacity_ = mem_guard.capacity;

            begin_ = buf_guard.begin;
            end_ = buf_guard.end;

            buf_guard.release();
            mem_guard.release();
        }

        return pos;
    }

public:
    
    explicit devector(const allocator_type& allocator = allocator_type(), const offset_by_type& offset_by = offset_by_type())
    :alloc(allocator), offs(offset_by)
    {
        begin_ = end_ = arr = nullptr;
        capacity_ = 0;
    }

    explicit devector(const offset_by_type& offset_by)
    :devector(allocator_type(), offset_by)
    {}
    
    devector(
        size_type n,
        const_reference val,
        const allocator_type& allocator = allocator_type(),
        const offset_by_type& offset_by = offset_by_type()
    )
    :alloc(allocator), offs(offset_by)
    {
        arr = allocate_n(n);
        try{
            construct(n, val);
        }catch(...){
            destroy_all();
            deallocate();
            throw;
        }
    }

    devector(size_type n, const_reference val, const offset_by_type& offset_by)
    :devector(n, val, allocator_type(), offset_by)
    {}

    explicit devector(size_type n)
    :devector(n, value_type())
    {}

    template<class InputIterator, is_iterator<InputIterator> = 0>
    devector(
        InputIterator first,
        InputIterator last,
        size_type distance,
        const allocator_type& allocator = allocator_type(),
        const offset_by_type& offset_by = offset_by_type()
    )
    :alloc(allocator), offs(offset_by)
    {
       arr = allocate_n(distance);
       try{
           construct(first, last, distance);
       }catch(...){
           destroy_all();
           deallocate();
           throw;
       }
    }

    template<class InputIterator, is_iterator<InputIterator> = 0>
    devector(
        InputIterator first,
        InputIterator last,
        size_type distance,
        const offset_by_type& offset_by
    )
    :devector(first, last, distance, allocator_type(), offset_by)
    {}

    template<class InputIterator, is_iterator<InputIterator> = 0>
    devector(
        InputIterator first,
        InputIterator last,
        const allocator_type& allocator = allocator_type(),
        const offset_by_type& offset_by = offset_by_type()
    )
    :devector(allocator, offset_by)
    {
        if(is_at_least_forward<typename it_traits<InputIterator>::iterator_category>::value){
            const size_type distance = std::distance(first, last);
            arr = allocate_n(distance);
            try{
                construct(first, last, distance);
            }catch(...){
                destroy_all();
                deallocate();
                throw;
            }
        }else{
            while(first != last){
                push_back(*first);
                ++first;
            }
        }
    }

    template<class InputIterator, is_iterator<InputIterator> = 0>
    devector(
        InputIterator first,
        InputIterator last,
        const offset_by_type& offset_by
    )
    :devector(first, last, allocator_type(), offset_by)
    {}

    devector(const devector& x, const allocator_type& allocator, const offset_by_type& offset_by)
    :devector(x.begin(), x.end(), x.size(), allocator, offset_by)
    {}

    devector(const devector& x, const allocator_type& allocator)
    :devector(x.begin(), x.end(), x.size(), allocator, x.offs)
    {}

    devector(const devector& x, const offset_by_type& offset_by)
    :devector(x.begin(), x.end(), x.size(), x.alloc, offset_by)
    {}

    devector(const devector& x)
    :devector(x, al_traits<allocator_type>::select_on_container_copy_construction(x.alloc), x.offs)
    {}

    devector(devector&& x) noexcept
    :devector(std::move(x), x.alloc, x.offs)
    {}

    devector(devector&& x, const allocator_type& allocator, const offset_by_type& offset_by) noexcept
    : offs(offset_by)
    {
        if(allocator == x.alloc){
            alloc = x.alloc;
            steal_ownership(x);
        }else{
            alloc = allocator;

            arr = allocate_n(x.size());
            construct_move(x.begin(), x.end(), x.size());
        }
    }

    devector(devector&& x, const allocator_type& allocator) noexcept 
    :devector(std::move(x), allocator, x.offs)
    {}

    devector(devector&& x, const offset_by_type& offset_by) noexcept
    :devector(std::move(x), x.alloc, offset_by)
    {}

    devector(
        std::initializer_list<value_type> il,
        const allocator_type& allocator = allocator_type(),
        const offset_by_type& offset_by = offset_by_type()
    )
    :devector(il.begin(), il.end(), il.size(), allocator, offset_by)
    {}

    devector(std::initializer_list<value_type> il, const offset_by_type& offset_by)
    :devector(il.begin(), il.end(), il.size(), allocator_type(), offset_by)
    {}

    ~devector(){
        destroy_all();
        deallocate();
    }

    template<class InputIterator, is_iterator<InputIterator> = 0>
    void assign (InputIterator first, InputIterator last, size_type distance){
        destroy_all();
        if(capacity_ < distance){
            reallocate(capacity_to_fit(distance));
        }
        construct(first, last, distance);
    }

    template<class InputIterator, is_iterator<InputIterator> = 0>
    void assign (InputIterator first, InputIterator last){
        if(is_at_least_forward<InputIterator>::value){
            assign(first, last, std::distance(first,last));
        }else{
            destroy_all();
            begin_ = end_ = arr + offs.off_by(free_total());
            while(first != last){
                push_back(*first);
                ++first;
            }
        }
    }

    void assign(size_type n, const_reference val){
        destroy_all();
        if(capacity_ < n){
            reallocate(capacity_to_fit(n));
        }
        construct(n, val);
    }

    void assign(std::initializer_list<value_type> il){
        destroy_all();
        if(capacity_ < il.size()){
            reallocate(capacity_to_fit(il.size()));
        }
        construct(il.begin(), il.end(), il.size());
    }

    devector& operator=(const devector& x){
        if(this == &x){
            return *this;
        }

        offs = x.offs;

        destroy_all();

        if(al_traits<allocator_type>::propagate_on_container_copy_assignment::value && alloc != x.alloc){
            auto new_capacity = capacity_to_fit(x.size());
            
            deallocate();

            alloc = x.alloc;
            arr = allocate_n(new_capacity);
        }else{
            if(capacity_ < x.size()){
                reallocate(capacity_to_fit(x.size()));
            }
        }

        construct(x.begin(), x.end(), x.size());

        return *this;
    }

    devector& operator=(devector&& x){
        if(this == &x){
            return *this;
        }

        offs = x.offs;

        destroy_all();

        if(!al_traits<allocator_type>::propagate_on_container_move_assignment::value){
            if(alloc != x.alloc){
                reallocate(capacity_to_fit(x.size()));
                construct_move(x.begin(), x.end(), x.size());
            }else{
                deallocate();
                steal_ownership(x);
            } 
        }else{
            deallocate();
            steal_ownership(x);

            alloc = x.alloc;
        }
        
        return *this;
    }

    devector& operator=(std::initializer_list<value_type> il){
        destroy_all();

        if(capacity_ < il.size()){
            reallocate(capacity_to_fit(il.size()));
        }

        construct(il.begin(), il.end(), il.size());
        return *this;
    }

    void resize_front(size_type n, const_reference val = value_type()){
        while(n < size()){
            pop_front();
        }
        if(n > capacity_){
            reallocate(capacity_to_fit(n), free_back());
        }
        while(n > size()){
            push_front(val);
        }
    }

    void resize_back(size_type n, const_reference val = value_type()){
        while(n < size()){
            pop_back();
        }
        if(n > capacity_){
            reallocate(capacity_to_fit(n), free_front());
        }
        while(n > size()){
            push_back(val);
        }
    }

    void resize(size_type n, const_reference val = value_type()){
        resize_back(n, val);
    }

    
    void reserve(size_type n){
        if(n > capacity_){
            reallocate(n);
        }
    }

    void shrink_to_fit(){
        reallocate(size());
    }

    reference operator[](size_type index){
        return begin_[index];
    }

    const_reference operator[](size_type index) const{
        return begin_[index];
    }

    reference at(size_type index){
        if(in_bounds(begin_ + index)){
            return begin_[index];
        }else{
            throw std::out_of_range("index " + std::to_string(index) + " out of range for array of size " + std::to_string(size()));
        }
    }

    const_reference at(size_type index) const{
        if(in_bounds(begin_ + index)){
            return begin_[index];
        }else{
            throw std::out_of_range("index " + std::to_string(index) + " out of range for array of size " + std::to_string(size()));
        }
    }

    reference front(){
        return *begin_;
    }

    const_reference front() const{
        return *begin_;
    }

    reference back(){
        return *(end_ - 1);
    }

    const_reference back() const{
        return *(end_ - 1);
    }

    pointer data() noexcept{
        return arr;
    }

    const_pointer data() const noexcept{
        return arr;
    }


    void push_back(const_reference val){
        if(!free_back()){
            const auto new_capacity = next_capacity();
            reallocate(new_capacity, new_capacity - size() - offs.off_by(new_capacity - size()) + 1);
        }

        al_traits<allocator_type>::construct(alloc, end_, val);
        ++end_;
    }

    void push_back(value_type&& val){
        if(!free_back()){
            const auto new_capacity = next_capacity();
            reallocate(new_capacity, new_capacity - size() - offs.off_by(new_capacity - size()) + 1);
        }

        al_traits<allocator_type>::construct(alloc, end_, std::move(val));
        ++end_;
    }

    void push_front(const_reference val){
        if(!free_front()){
            const auto new_capacity = next_capacity();
            reallocate(new_capacity, offs.off_by(new_capacity - size()) + 1);
        }

        al_traits<allocator_type>::construct(alloc, begin_ - 1, val);
        --begin_;
    }

    void push_front(value_type&& val){
        if(!free_front()){
            const auto new_capacity = next_capacity();
            reallocate(new_capacity, offs.off_by(new_capacity - size()) + 1);
        }

        al_traits<allocator_type>::construct(alloc, begin_ - 1, std::move(val));
        --begin_;
    }

    void pop_back() noexcept{
        al_traits<allocator_type>::destroy(alloc, end_ - 1);
        --end_;
    }

    void pop_front() noexcept{
        al_traits<allocator_type>::destroy(alloc, begin_);
        ++begin_;
    }

    iterator insert(const_iterator position, size_type n, const_reference val){
        return insert_impl(position, n, val);
    }

    iterator insert(const_iterator position, const_reference val){
        return insert_impl(position, 1, val);
    }

    iterator insert(const_iterator position, value_type&& val){
        return insert_impl(position, 1, std::move(val));
    }

    template<class InputIterator, is_iterator<InputIterator> = 0>
    iterator insert(const_iterator position, InputIterator first, InputIterator last, size_type n){
        iterator pos; // position of first newly-created element

        if(n <= free_total()){
            if(position == begin_){
                buffer_guard front_guard(begin_ - n);
                while(first != last){
                    al_traits<allocator_type>::construct(alloc, front_guard.end, *first);
                    ++front_guard.end;
                    ++first;
                }
                begin_ = front_guard.begin;
                front_guard.release();
            }else if(position == end_){
                while(first != last){
                    al_traits<allocator_type>::construct(alloc, end_, *first);
                    ++end_;
                    ++first;
                }
            }else{
                const iterator new_begin = arr + offs.off_by(free_total() - n);
                const iterator new_end = new_begin + n + size();

                const iterator free_space = shift(new_begin, new_end, position, n);

                buffer_guard front_guard(alloc, new_begin, free_space);
                buffer_guard back_guard(alloc, free_space + n, new_end);

                while(first != last){
                    al_traits<allocator_type>::construct(alloc, front_guard.end, *first);
                    ++front_guard.end;
                    ++first;
                }

                front_guard.release();
                back_guard.release();

                pos = free_space;
            }
        }else{
            const size_type new_size = size() + n;

            memory_guard mem_guard(alloc, capacity_to_fit(new_size));
            
            const size_type front_space = offs.off_by(mem_guard.capacity - new_size);

            buffer_guard buf_guard(alloc, mem_guard.arr + front_space);

            auto it = begin();
            for(; it != position; ++it){
                al_traits<allocator_type>::construct(alloc, buf_guard.end, std::move_if_noexcept(*it));
                ++buf_guard.end;
            }

            pos = buf_guard.end;
            while(first != last){
                al_traits<allocator_type>::construct(alloc, buf_guard.end, *first);
                ++buf_guard.end;
                ++first;
            }
            for(; it != end(); ++it){
                al_traits<allocator_type>::construct(alloc, std::move_if_noexcept(*it));
            }

            destroy_all();
            deallocate();

            arr = mem_guard.arr;
            capacity_ = mem_guard.capacity;

            begin_ = buf_guard.begin;
            end_ = buf_guard.end;

            buf_guard.release();
            mem_guard.release();
        }

        return pos;
    }

    template<class InputIterator, is_iterator<InputIterator> = 0>
    iterator insert(const_iterator position, InputIterator first, InputIterator last){
        if(is_at_least_forward<InputIterator>::value){
            return insert(position, first, last, std::distance(first, last));
        }else{
            iterator pos = position - 1;
            while(first != last){ // Seriously if you care about perfomance don't go for this case.
                pos = insert(pos + 1, *first);
                ++first;
            }
            return pos;
        }
    }

    iterator insert(const_iterator position, std::initializer_list<value_type> il){
        return insert(position, il.begin(), il.end(), il.size());
    }

    iterator erase(const_iterator first, const_iterator last){
        const size_type n = last - first;

        const iterator new_begin = arr + offs.off_by(free_total() - n);
        const iterator new_end = new_begin + size() - n;

        iterator pos;

        buffer_guard front_guard(alloc, new_begin);

        while(!in_bounds(front_guard.end) && begin_ != first){
            al_traits<allocator_type>::construct(alloc, front_guard.end, std::move(*begin_));
            al_traits<allocator_type>::destroy(alloc, begin_);
            ++begin_;
            ++front_guard.end;
        }

        if(begin_ == first){
            pos = front_guard.end;

            while(begin_ != last){
                al_traits<allocator_type>::destroy(alloc, begin_);
                ++begin_;
            }

            while(!empty() && !in_bounds(front_guard.end)){
                al_traits<allocator_type>::construct(alloc, front_guard.end, std::move(*begin_));
                al_traits<allocator_type>::destroy(alloc, begin_);
                ++begin_;
                ++front_guard.end;
            }
        }

        buffer_guard back_guard(alloc, new_end);

        while(!empty() && !in_bounds(back_guard.begin) && end_ != last){
            al_traits<allocator_type>::construct(alloc, back_guard.begin, std::move(end_[-1]));
            al_traits<allocator_type>::destroy(alloc, end_ - 1);
            --end_;
            --back_guard.begin;
        }

        if(end_ == last){
            pos = back_guard.begin;

            while(end_ >= first){
                al_traits<allocator_type>::destroy(alloc, end_[-1]);
                --end_;
            }

            while(!empty() && !in_bounds(back_guard.begin)){
                al_traits<allocator_type>::construct(alloc, back_guard.begin, std::move(end_[-1]));
                al_traits<allocator_type>::destroy(alloc, end_ - 1);
                --end_;
                --back_guard.begin;
            }
        }

        begin_ = new_begin;
        end_ = new_end;

        front_guard.release();
        back_guard.release();

        return pos;
    }

    iterator erase(const_iterator position){
        return erase(position, position + 1);
    }

    void swap(devector& x){
        std::swap(arr, x.arr);
        std::swap(begin_, x.begin_);
        std::swap(end_, x.end_);
        std::swap(capacity_, x.capacity_);
        std::swap(offs, x.offs);
        if(al_traits<allocator_type>::propagate_on_container_swap){
            std::swap(alloc, x.alloc);
        }
    }

    void clear() noexcept{
        destroy_all();
    }

    template<class... Args>
    iterator emplace(const_iterator position, Args&&... args){
        return insert_impl(position, 1, std::forward<Args>(args)...);
    }

    template<class... Args>
    iterator emplace_back(Args&&... args){
        return insert_impl(end_, 1, std::forward<Args>(args)...);
    }

    template<class... Args>
    iterator emplace_front(Args&&... args){
        return insert_impl(begin_, 1, std::forward<Args>(args)...);
    }

    allocator_type get_allocator() const noexcept{
        return alloc;
    }

    offset_by_type getOffsetBy() const noexcept{
        return offs;
    }
};

template<class T, class Alloc, class OffsetByA, class OffsetByB>
bool operator== (const devector<T, Alloc, OffsetByA>& lhs, const devector<T, Alloc, OffsetByB>& rhs){
    if(lhs.size == rhs){
        for(auto it0 = lhs.begin(), it1 = rhs.begin(); it0 != lhs.end(); ++it0, ++it1){
            if(*it0 != *it1){
                return false;
            }
        }
        return true;
    }
    return false;
}

template<class T, class Alloc, class OffsetByA, class OffsetByB>
bool operator!= (const devector<T, Alloc, OffsetByA>& lhs, const devector<T, Alloc, OffsetByB>& rhs){
    return !(lhs == rhs);
}

template<class T, class Alloc, class OffsetByA, class OffsetByB>
bool operator< (const devector<T, Alloc, OffsetByA>& lhs, const devector<T, Alloc, OffsetByB>& rhs){
    auto it0 = lhs.begin();
    auto it1 = rhs.begin();

    while(it0 != lhs.end() && it1 != rhs.end()){
        if(*it0 < *it1){
            return true;
        }else if(*it0 > *it1){
            return false;
        }
    }

    if(it0 == lhs.end()){
        return true;
    }

    return false;
}

template<class T, class Alloc, class OffsetByA, class OffsetByB>
bool operator<= (const devector<T, Alloc, OffsetByA>& lhs, const devector<T, Alloc, OffsetByB>& rhs){
    return !(rhs < lhs);
}

template<class T, class Alloc, class OffsetByA, class OffsetByB>
bool operator> (const devector<T, Alloc, OffsetByA>& lhs, const devector<T, Alloc, OffsetByB>& rhs){
    return rhs < lhs;
}

template<class T, class Alloc, class OffsetByA, class OffsetByB>
bool operator>= (const devector<T, Alloc, OffsetByA>& lhs, const devector<T, Alloc, OffsetByB>& rhs){
    return !(lhs < rhs);
}

template<class T, class Alloc, class OffsetByA, class OffsetByB>
void swap(devector<T, Alloc, OffsetByA>& x, devector<T, Alloc, OffsetByB> y){
    x.swap(y);
}
}

#endif
