#ifndef _TEST_MANAGER_
#define _TEST_MANAGER_

/*!
 * @file test_manager.h
 * @brief Definition of the TestManager class.
 * @date November 15th, 2019
 * @author Selan R. dos Santos
 */

#include <iostream>   // cout, endl
#include <iomanip>    // setw
#include <algorithm>  // shuffle, copy

#include <unordered_map>
#include <vector>


/// Implements a simple test manager.
class TestManager {
    private:
        /// Defines a single entry in our database.
        struct Entry {
            std::string m_desc; //!< The teste description.
            size_t m_seq; //!< Test sequence number, to help us print the test in the order they've been registred.
            bool m_result; //!< The test result.
            int m_line; //!< The test line number.
            /// Default Ctro
            Entry( std::string d="no_name", size_t s = 0, bool r=true, int l=0 )
                : m_desc{ d }, m_seq{ s }, m_result{ r }, m_line{ l }
            { /* empty */ }
        };
        /// Record the tests results. The key os the test name, and the data is a `Entry`.
        std::unordered_map< std::string, Entry > tests_record;
        /// The test suite name.
        std::string test_suite_name;
        /// Number of tests registred.
        size_t n_tests;

    private:
        /// Prints out the overall result of a single test.
        void print_test_result( const std::string &test_name, const Entry &entry ) const
        {
            std::cout << "[ " << "\e[1;34mRUN\e[0m" << "      ] " << test_name << "-> " << entry.m_desc << std::endl;
            if ( entry.m_result ) std::cout << "[       " << "\e[1;32mOK\e[0m" << " ]\n";
            else std::cout << "[     "  << "\e[1;31mFAIL\e[0m" << " ] at line " << entry.m_line << ".\n";
        }

        //=== Public interface.
    public:
        /// Default constructor that may take the test suite name.
        explicit TestManager( const std::string suite_name="Default" )
            : test_suite_name{ suite_name }, n_tests{0}
        { /* empty */ }

        /// Registers a test with this suite
        inline void record ( const std::string &key_name, const std::string& msg )
        {
            // Store the entry in the data base.
            tests_record[key_name] = Entry{ msg, n_tests++ };
        }

        /// Updates the test result.
        void result( const std::string &key, bool value, int line );

        /// Shows the test suite results.
        void summary(void) const;
};

//=== MACRO definitions.
#define RESULT(tm, key, res) tm.result( key, res, __LINE__ )
#define REGISTER(tm, key, msg) tm.record( key, msg )
#define EXPECT_TRUE( tm, key, value ) tm.result( key, value==true, __LINE__ )
#define EXPECT_FALSE( tm, key, value ) tm.result( key, value==false, __LINE__ )
#define EXPECT_EQUAL( tm, key, value1, value2 ) tm.result( key, value1==value2, __LINE__ )

#endif
