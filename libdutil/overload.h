#ifndef DUTIL_OVERLOAD_H
#define DUTIL_OVERLOAD_H

namespace DUTIL {

/*! \brief Overload patter for std::variant visitors
 *
 * In the overload pattern, we inherit from several lambdas and then expose
 * their operator() for std::visit. That way, you write overloads “in place”.
 *
 * In short, define
 */

template <typename... Ts>
struct Overload : Ts...
{
  // struct Overload inherits the call operator from its base type T.
  using Ts::operator()...;
};

/*! \brief Call Operatro () for Overload type.
 *
 * In the overload patte
 */

template <typename... Ts>
Overload(Ts...) -> Overload<Ts...>;

}  // namespace DUTIL
#endif  // DUTIL_OVERLOAD_H
