#pragma once

#include <vector> // std::vector
#include <cstdint> // uintptr_t

namespace tinyvmt
{
  class c_vmt
  {
  private:
    std::vector< uintptr_t > m_new;
    uintptr_t *m_table, *m_original;
    bool m_copied;

  public:
    c_vmt ( )
        : m_table ( nullptr ), m_original ( nullptr ), m_copied ( false ) { };

    void create ( uintptr_t table ) noexcept
    {
      if ( !table )
        return;

      this->m_new.clear ( );
      this->m_copied = false;

      this->m_table = reinterpret_cast< uintptr_t * > ( table );
      this->m_original = *reinterpret_cast< uintptr_t ** > ( this->m_table );

      // get approx. vfunc count
      int vfunc_count = 0;

      while ( this->m_original[ vfunc_count ] )
        vfunc_count++;

      for ( int i = -1; i < vfunc_count; ++i )
        this->m_new.push_back ( this->m_original[ i ] );
    }

    template < typename t = uintptr_t >
    [[nodiscard]] t get_function ( int index ) noexcept
    {
      return ( ( t ) ( this->m_new.at ( index + 1 ) ) );
    }

    template < typename t = uintptr_t >
    [[nodiscard]] t get_old_function ( int index ) noexcept
    {
      return ( ( t ) ( this->m_original[ index ] ) );
    }

    void unhook ( int index ) noexcept
    {
      this->m_new.at ( index + 1 ) = this->m_original[ index ];
    }

    void hook ( int index, uintptr_t new_func ) noexcept
    {
      if ( !this->m_copied )
      {
        auto data = this->m_new.data ( );
        *this->m_table = uintptr_t ( &data[ 1 ] ); // this is the only patch applied to memory.

        this->m_copied = true;
      }

      this->m_new.at ( index + 1 ) = new_func;
    }

    void hook ( int index, void *new_func ) noexcept
    {
      hook ( index, reinterpret_cast< uintptr_t > ( new_func ) );
    }

    void restore ( ) const noexcept
    {
      *this->m_table = uintptr_t ( this->m_original );
    }
  };
} // namespace tinyvmt