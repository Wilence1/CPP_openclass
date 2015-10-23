#include <stdexcept>
#include <string>
#include <vector>
enum class AllocErrorType {
    InvalidFree,
    NoMemory,
};

class AllocError: std::runtime_error {
private:
    AllocErrorType type;

public:
    AllocError(AllocErrorType _type, std::string message):
            runtime_error(message),
            type(_type)
    {}
    AllocErrorType getType() const { return type; }
};
class Allocator;

struct ind_elem {
    int base;
    size_t size;
};

struct point_elem {
    void *base;
    size_t size;
};

struct ptr 
{
    struct point_elem data;
    struct ptr * next;
};
class Pointer;


class Pointer {
public:
    Pointer(struct ptr *newelem);
    Pointer();
    void *get() const;
    struct ptr *tmp;
    
    
};

//array with 0 1

class Allocator {
public:
    Allocator(void *base, size_t size);
    
    Pointer alloc(size_t N);
    
    void realloc(Pointer &p, size_t N);
    
    void free(Pointer &p);
    
    void defrag();
    
    std::string dump() { return ""; }

    
    
    void *base;
    
    size_t size;

    struct ptr *list;


};