# allocator
- address
- allocate
- deallocate
- max_size
- construct
- destroy

## allocate

```c++
_NODISCARD _DECLSPEC_ALLOCATOR _Ty* allocate(_CRT_GUARDOVERFLOW const size_t _Count) {
    return static_cast<_Ty*>(_Allocate<_New_alignof<_Ty>>(_Get_size_of_n<sizeof(_Ty)>(_Count)));
}

template <size_t _Align, class _Traits = _Default_allocate_traits,
    enable_if_t<(!_HAS_ALIGNED_NEW || _Align <= __STDCPP_DEFAULT_NEW_ALIGNMENT__), int> = 0>
_DECLSPEC_ALLOCATOR void* _Allocate(const size_t _Bytes) {
    // allocate _Bytes when !_HAS_ALIGNED_NEW || _Align <= __STDCPP_DEFAULT_NEW_ALIGNMENT__
#if defined(_M_IX86) || defined(_M_X64)
    if (_Bytes >= _Big_allocation_threshold) { // boost the alignment of big allocations to help autovectorization
        return _Allocate_manually_vector_aligned<_Traits>(_Bytes);
    }
#endif // defined(_M_IX86) || defined(_M_X64)

    if (_Bytes != 0) {
        return _Traits::_Allocate(_Bytes);
    }

    return nullptr;
}

template <size_t _Align, class _Traits = _Default_allocate_traits,
    enable_if_t<(_Align > __STDCPP_DEFAULT_NEW_ALIGNMENT__), int> = 0>
_DECLSPEC_ALLOCATOR void* _Allocate(const size_t _Bytes) {
    // allocate _Bytes when __cpp_aligned_new && _Align > __STDCPP_DEFAULT_NEW_ALIGNMENT__
    if (_Bytes == 0) {
        return nullptr;
    }

    size_t _Passed_align = _Align;
#if defined(_M_IX86) || defined(_M_X64)
    if (_Bytes >= _Big_allocation_threshold) {
        // boost the alignment of big allocations to help autovectorization
        _Passed_align = _Max_value(_Align, _Big_allocation_alignment);
    }
#endif // defined(_M_IX86) || defined(_M_X64)

    return _Traits::_Allocate_aligned(_Bytes, _Passed_align);
}


// STRUCT _Default_allocate_traits
struct _Default_allocate_traits {
    _DECLSPEC_ALLOCATOR static void* _Allocate(const size_t _Bytes) {
        return ::operator new(_Bytes);
    }

#ifdef __cpp_aligned_new
    _DECLSPEC_ALLOCATOR static void* _Allocate_aligned(const size_t _Bytes, const size_t _Align) {
        return ::operator new (_Bytes, align_val_t{_Align});
    }
#endif // __cpp_aligned_new
};
```
