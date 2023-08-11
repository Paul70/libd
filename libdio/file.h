#ifndef FILE_H
#define FILE_H
#include <string_view>

namespace DIO {
namespace FILE {

auto load(std::string_view path);

} // namespace FILE
} // namespace DIO

#endif // FILE_H
