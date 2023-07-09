#ifndef DUTIL_STATICPOINTERCAST_H
#define DUTIL_STATICPOINTERCAST_H
#include <memory>

namespace DUTIL {

template<typename TO, typename FROM>
std::unique_ptr<TO> static_unique_ptr_cast(std::unique_ptr<FROM> &&from)
{
    return std::unique_ptr<TO>(static_cast<TO *>(from.release()));
}
} // namespace DUTIL
#endif // DUTIL_STATICPOINTERCAST_H
