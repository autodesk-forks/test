//  (C) Copyright 2016 Raffi Enficiaud.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
///@file
///@brief Contains the definition of the Junit log formatter (OF_JUNIT)
// ***************************************************************************

#ifndef BOOST_TEST_JUNIT_LOG_FORMATTER__
#define BOOST_TEST_JUNIT_LOG_FORMATTER__

// Boost.Test
#include <boost/test/detail/global_typedef.hpp>
#include <boost/test/unit_test_log_formatter.hpp>
#include <boost/test/tree/test_unit.hpp>


// STL
#include <cstddef> // std::size_t
#include <map>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {
namespace output {

  class junit_log_formatter;

  namespace junit_impl
  {
    struct test_unit_ : public test_results
    {
      test_unit_type tu_type; // stored because otherwise difficult to retrieve
      double time; // in sec
      size_t test_nb; // number of test cases contained in the unit
      
      bool disabled;
      std::string cdata;
      std::string failure;

      void set_skipped(bool is_skipped = true) {
        this->p_skipped.value = is_skipped;
      }

      friend class boost::unit_test::output::junit_log_formatter;
    };
  }

// ************************************************************************** //
// **************               junit_log_formatter              ************** //
// ************************************************************************** //

class junit_log_formatter : public unit_test_log_formatter {
public:

    junit_log_formatter()
    {
      this->m_log_level = log_successful_tests;
    }

    // Formatter interface
    void    log_start( std::ostream&, counter_t test_cases_amount );
    void    log_finish( std::ostream& );
    void    log_build_info( std::ostream& );

    void    test_unit_start( std::ostream&, test_unit const& tu );
    void    test_unit_finish( std::ostream&, test_unit const& tu, unsigned long elapsed );
    void    test_unit_skipped( std::ostream&, test_unit const& tu, const_string reason );

    void    log_exception_start( std::ostream&, log_checkpoint_data const&, execution_exception const& ex );
    void    log_exception_finish( std::ostream& );

    void    log_entry_start( std::ostream&, log_entry_data const&, log_entry_types let );
    using   unit_test_log_formatter::log_entry_value; // bring base class functions into overload set
    void    log_entry_value( std::ostream&, const_string value );
    void    log_entry_finish( std::ostream& );

    void    entry_context_start( std::ostream&, log_level );
    void    log_entry_context( std::ostream&, const_string );
    void    entry_context_finish( std::ostream& );

    //! Discards changes in the log level
    virtual void        set_log_level(log_level )
    {
    }


private:
    typedef std::map<test_unit_id, junit_impl::test_unit_> map_trace_t;
    map_trace_t map_tests;
    std::string m_curr_tag;
    bool m_value_closed;
};

} // namespace output
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_JUNIT_LOG_FORMATTER__
