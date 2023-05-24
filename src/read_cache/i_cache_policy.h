#pragma once 

namespace pos {
template<typename KeyType, typename ValueType>
class ICachePolicy {
public:
    virtual ~ICachePolicy() = default;
    virtual void Put(const KeyType&, const ValueType&) = 0;
    virtual bool Get(const KeyType&, ValueType&) = 0;
    virtual bool Delete(const KeyType&, ValueType&) = 0;
    virtual size_t Size(void) const = 0;
};
} // namespace pos
