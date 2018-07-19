//==================================================================================================
/**
  EVE - Expressive Vector Engine
  Copyright 2018 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef EVE_DETAIL_FUNCTION_SIMD_PPC_VMX_SLICE_HPP_INCLUDED
#define EVE_DETAIL_FUNCTION_SIMD_PPC_VMX_SLICE_HPP_INCLUDED

#include <eve/detail/abi.hpp>
#include <eve/detail/meta.hpp>
#include <eve/forward.hpp>
#include <type_traits>
#include <cstddef>

namespace eve { namespace detail
{
  template< typename T, typename N, typename Slice
          , typename = std::enable_if_t<(N::value>1)>
          >
  EVE_FORCEINLINE auto slice( pack<T,N,ppc_> const& a, Slice const& ) noexcept
  {
    if constexpr(Slice::value)
    {
      auto select = [](auto const& v, auto size)
      {
        auto mask = [&](auto... I)
        {
          auto offset = (sizeof(T) * size)/2;
          __vector unsigned char m ={ static_cast<std::uint8_t>(offset + I%offset)...};
          return m;
        };

        return vec_perm(v.storage(), v.storage(), apply<16>(mask));
      };

      return pack<T,typename N::split_type>(select(a,N{}));
    }
    else
    {
      return pack<T,typename N::split_type>(a.storage());
    }
  }

  template< typename T, typename N
          , typename = std::enable_if_t<(N::value>1)>
          >
  EVE_FORCEINLINE auto slice( pack<T,N,ppc_> const& a ) noexcept
  {
    std::array<pack<T,typename N::split_type>,2> that{slice(a,lower_), slice(a,upper_)};
    return that;
  }

} }

#endif