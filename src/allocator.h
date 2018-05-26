#include <type_traits>
#include <memory>

template <typename T, size_t Size = 10>
class Allocator
{
    private:
    std::unique_ptr<unsigned char []> data;
    std::size_t offset;  

    public:    
    template <typename U, size_t> friend class Allocator;

    using value_type = T;
    using pointer = T *;

    ~Allocator()
    {
        //std::cout << "Allocator dtor: " << this << std::endl;
    };

    template <class U> 
        struct rebind {
            using other =  Allocator<U, Size>; 
        };

    Allocator()
        : data( std::make_unique<unsigned char[]>(sizeof(value_type) * Size) )
        , offset(0)
    {
        //std::cout << "Allocator default ctor: " << this << std::endl;
    }

    Allocator(const Allocator& rhs)
        : data(nullptr)
        , offset(0)
    {
        //std::cout << "Allocator copy ctor: " << this << std::endl;
    }

    template <typename U>
    Allocator(const Allocator<U>& rhs)
        : offset(rhs.offset)
        , data{ nullptr }
    { 

    }

    pointer allocate(std::size_t n)
    {
        if (data == nullptr)
        {
            data = std::move(std::make_unique<unsigned char[]>(sizeof(value_type) * Size));
        }

        if (offset + sizeof(value_type) * n > Size * sizeof(value_type))
        {
            throw std::bad_alloc();
        }

        void * result = data.get() + offset;
        offset += sizeof(value_type) * n;

        return static_cast<pointer>(result);
    }

    void deallocate(pointer /*p*/, std::size_t /*n*/)
    {
        //std::cout << "Allocator: " << this << ". deallocate." << std::endl;
    }

    template <typename ... Args >
    void construct(T* p, Args&& ... args) 
    {
        new(p) T(std::forward <Args>(args) ... );
    }
};
