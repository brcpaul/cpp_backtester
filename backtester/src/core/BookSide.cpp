#include "../../includes/core/BookSide.h"

template<typename Compare>
BookSide<Compare>::BookSide() {}

template<typename Compare>
BookSide<Compare>::~BookSide() {}

template class BookSide<std::greater<double> >;
template class BookSide<std::less<double> >;