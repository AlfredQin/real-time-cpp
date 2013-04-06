///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2013.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef _UTIL_TIME_2010_04_10_H_
  #define _UTIL_TIME_2010_04_10_H_

  #include <limits>
  #include <mcal_cpu.h>
  #include <mcal_gpt.h>

  namespace util
  {
    class timer_base
    {
    protected:
      timer_base() { }

      static mcal::gpt::value_type my_now();
    };

    template<typename unsigned_tick>
    class timer : public timer_base
    {
    public:

      #if defined(_MSC_VER) && (_MSC_VER <= 1600)
      #else
        static_assert(false == std::numeric_limits<unsigned_tick>::is_signed,
                      "error: the type of unsigned_tick must be unsigned!");
      #endif

      typedef unsigned_tick tick_type;

      template<typename other_tick_type> static tick_type microseconds(const other_tick_type& value_microseconds) { return value_microseconds; }
      template<typename other_tick_type> static tick_type milliseconds(const other_tick_type& value_milliseconds) { return static_cast<tick_type>(1000UL) * microseconds(value_milliseconds); }
      template<typename other_tick_type> static tick_type seconds     (const other_tick_type& value_seconds     ) { return static_cast<tick_type>(1000UL) * milliseconds(value_seconds     ); }
      template<typename other_tick_type> static tick_type minutes     (const other_tick_type& value_minutes     ) { return static_cast<tick_type>(  60UL) * seconds     (value_minutes     ); }
      template<typename other_tick_type> static tick_type hours       (const other_tick_type& value_hours       ) { return static_cast<tick_type>(  60UL) * minutes     (value_hours       ); }
      template<typename other_tick_type> static tick_type days        (const other_tick_type& value_days        ) { return static_cast<tick_type>(  24UL) * hours       (value_days        ); }

      timer() : my_tick(tick_type(my_now())) { }

      explicit timer(const tick_type& tick_value) : my_tick(tick_type(my_now()) + tick_value) { }

      timer(const timer& other_timer) : my_tick(other_timer.my_tick) { }

      timer& operator=(const timer& other_timer)
      {
        my_tick = other_timer.my_tick;
        return *this;
      }

      void start_interval(const tick_type& tick_value)
      {
        my_tick += tick_value;
      }

      void start_relative(const tick_type& tick_value)
      {
        my_tick = static_cast<tick_type>(tick_type(my_now()) + tick_value);
      }

      bool timeout() const
      {
        const tick_type timer_mask = static_cast<tick_type>((1ULL << (std::numeric_limits<tick_type>::digits - 1)) - 1ULL);
        const tick_type delta      = static_cast<tick_type>(tick_type(my_now()) - my_tick);
        return (delta <= timer_mask);
      }

      static void blocking_delay(const tick_type& delay)
      {
        const timer t_delay(delay);

        while(false == t_delay.timeout())
        {
          mcal::cpu::nop();
        }
      }

    private:
      tick_type my_tick;
    };
  }

#endif // _UTIL_TIME_2010_04_10_H_
