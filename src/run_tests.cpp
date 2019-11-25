/**
 * @file driver_dictionary.cpp
 * @brief File for DAL and DSAL classes tests .
 */

#include <iostream>   // cout, endl
#include <cassert>    // assert()
#include <random>     // random_device, mt19937
#include <iterator>   // std::begin(), std::end()


#include "../include/test_manager.h"
#include "../include/dal.h"

/**
 * @brief      Class for my key comparator.
 */
class MyKeyComparator {
    public:
        bool operator ()( const int & lhs, const int & rhs ) const {
            return lhs < rhs ;
        }
};


int main ( void )
{
    // Creates a test manager for the DAL class.
    TestManager tm{ "DAL <int, string> Suite" };

    {
        //=== Test #1
        std::string test_id{"DefaultCtro"};
        REGISTER( tm, test_id, "Testing the default constructor." );
        // // Testing default constructor.
        DAL<int , std::string > dic;
        EXPECT_EQUAL( tm, test_id,   dic.capacity(), 50);
    }
    {
        //=== Test #2
        auto test_id{ "CtroSize" };
        REGISTER( tm, test_id, "Testing the constructor that receives a size." );
        DAL<int, std::string> dic2(100);
        EXPECT_EQUAL( tm, test_id,   dic2.capacity(), 100 );
    }

    {
        // Testing insert.
        DAL<int, std::string, MyKeyComparator> dict(3);
        auto test_id{ "InsertRegular" };
        REGISTER( tm, test_id, "Testing the regular insert." );
        // testing the regular insert
        auto result1 =  dict.insert( 1, "AAA" ) ;
        auto result2 =  dict.insert( 2, "BBB" ) ;
        EXPECT_TRUE ( tm, test_id, result1 and result2 );
    }

    {
        auto test_id{ "InsertDuplicate" };
        REGISTER( tm, test_id, "Testing the insert failure when we insert a duplicate key." );
        DAL<int, std::string, MyKeyComparator> dict(3);
        auto result1 =  dict.insert( 1, "AAA" ) ;
        auto result2 =  dict.insert( 2, "BBB" ) ;
        EXPECT_TRUE( tm, test_id, result1 and result2 );
        // Testing the insert failure for duplicate keys.
        EXPECT_FALSE( tm, test_id, dict.insert( 2, "CCC" ) );
    }
     {
        auto test_id{ "InsertDuplicate" };
        REGISTER( tm, test_id, "Testing the insert failure when we insert a duplicate key." );
        DAL<int, std::string, MyKeyComparator> dict(3);
        auto result1 =  dict.insert( 1, "AAA" ) ;
        auto result2 =  dict.insert( 2, "BBB" ) ;
        DAL<int, std::string, MyKeyComparator> copyed(dict);
     
    }

    {
        // Testing the insert for overflow check.
        auto test_id{ "InsertOverflow" };
        REGISTER( tm, test_id, "Testing the insert indication of overflow when we exceed the dictionary limit." );
        DAL<int, std::string, MyKeyComparator> dict(3);
        auto result1 =  dict.insert( 1, "AAA" ) ;
        auto result2 =  dict.insert( 2, "BBB" ) ;
        auto result8 = ( dict.insert( 3, "CCC" ) );
        EXPECT_TRUE( tm, test_id, result1 and result2 and result8 );
        // Trying to insert an extra item.
        // The dictionary may return false if it's a static version,
        // or true, if it's a dynamic version.
        auto old_size = dict.capacity();
        auto result4 = dict.insert( 4, "DDD" );
        if ( result4 == true ) // Dynamic version.
            EXPECT_TRUE( tm, test_id,   dict.capacity() > old_size );
        else
            EXPECT_FALSE( tm, test_id,   result4  ); // static dictionary
    }

    {
        // Testing search.
        DAL<int, std::string> dict;
        std::string result;

        // The Data
        struct Entry
        {
            int key;
            std::string data;
        } table[] =
        {
            { 1, "AAA" },
            { 2, "BBB" },
            { 3, "CCC" },
            { 4, "DDD" },
            { 5, "EEE" }
        };

        auto test_id{ "SearchingBasic" };
        REGISTER( tm, test_id, "Testing basic searching.");
        bool passed{ false };
        // insert all itens first.
        for ( const auto & e : table )
        {
            passed = dict.insert( e.key, e.data ) ;
            if ( not passed ) break;
        }
        EXPECT_TRUE( tm, test_id,  passed );

        // retrieve itens
        passed = false;
        for ( const auto & e : table )
        {
            passed = ( dict.search( e.key, result ) );
            if ( not passed ) break;
            passed = ( result == e.data );
            if ( not passed ) break;
        }
        EXPECT_TRUE( tm, test_id,  passed );

        {
            auto test_id{ "SearchingInexisting" };
            REGISTER( tm, test_id, "Trying to retrieve an inexisting element from the dictionary.");
            // serching for non-existing key.
            EXPECT_FALSE( tm, test_id,    ( dict.search( 6, result ) ) );
        }

    }
    {
        std::string result;
        auto test_id{ "SearchingEmpty" };
        REGISTER( tm, test_id, "Searching in an empty dictionary.");
        // seaching in an empty dictionary.
        DAL<int, std::string> dict;
        EXPECT_FALSE( tm, test_id, ( dict.search( 1, result)  ) ) ;
    }

    {
        // Testing remove.
        DAL<int, std::string> dict;
        std::string result;

        // The Data
        struct Entry
        {
            int key;
            std::string data;
        };

        Entry table[] =
        {
            { 1, "AAA" },
            { 2, "BBB" },
            { 3, "CCC" },
            { 4, "DDD" },
            { 5, "EEE" }
        };


        auto test_id{ "RemoveBasic" };
        REGISTER( tm, test_id, "Testing basic removing of data.");
        // insert all itens first.

        for ( const auto & e : table )
        {
            EXPECT_TRUE( tm, test_id, dict.insert( e.key, e.data ) ) ;
        }
        // Trying to remove a non-existing element.
        EXPECT_FALSE ( tm, test_id,   dict.remove( 6 , result ) );

        // remove itens
        for ( const auto & e : table )
        {

            EXPECT_TRUE( tm, test_id,   dict.remove( e.key, result ) );
            // EXPECT_EQUAL( tm, test_id, result , e.data );

        }
        EXPECT_TRUE( tm, test_id,   dict.empty() );

        // seaching in an empty dictionary.
        DAL<int, std::string> dict2;
        EXPECT_FALSE( tm, test_id, dict2.remove( 1, result) );


    }

    {
        // Testing min/max.
        DAL<int, std::string> dict;
        std::string result;

        // The Data
        struct Entry
        {
            int key;
            std::string data;
        };

        Entry table[] =
        {
            { 1, "AAA" },
            { 2, "BBB" },
            { 3, "CCC" },
            { 4, "DDD" },
            { 5, "EEE" }
        };

        // Shuffle table
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle( std::begin( table ), std::end( table ), g);

        auto test_id{ "MinMaxBasic" };
        REGISTER( tm, test_id, "Testing basic use of min/max.");
        // insert all itens first.
        for ( const auto & e : table )
        {
            EXPECT_TRUE( tm, test_id, dict.insert( e.key, e.data ) ) ;
        }

        EXPECT_EQUAL( tm, test_id,   dict.min(), 1 );
        EXPECT_EQUAL( tm, test_id,   dict.max(), 5 );

        // Trying to get min/max of a empty dictionary.
        DAL<int, std::string> dict2;
        auto worked( false );
        try {
            result = dict2.min();
        }
        catch ( std::out_of_range & e )
        {
            worked = true;
        }
        EXPECT_TRUE( tm, test_id, worked );
        worked =  false;
        try {
            result = dict2.max();
        }
        catch ( std::out_of_range & e )
        {
            worked = true;
        }
        EXPECT_TRUE( tm, test_id, worked );
    }

    {
        // Testing successor/predecessor
        DAL<int, std::string> dict;
        std::string result;

        // The Data
        struct Entry
        {
            int key;
            std::string data;
        };

        Entry table[] =
        {
            { 1, "AAA" },
            { 2, "BBB" },
            { 3, "CCC" },
            { 4, "DDD" },
            { 5, "EEE" }
        };

        // Shuffle table
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle( std::begin( table ), std::end( table ), g);

        auto test_id{ "SucePredBasic" };
        REGISTER( tm, test_id, "Testing basic use of successor/predecessor.");
        // insert all itens first.
        for ( const auto & e : table )
        {
            EXPECT_TRUE( tm, test_id, dict.insert( e.key, e.data ) ) ;
        }

        auto key { dict.min() };
        int next_key{0};
        int i{1};
        while( dict.successor( key, next_key ) )
        {
            EXPECT_EQUAL( tm, test_id, key, i++ );
            // go to the next key.
            key = next_key;
        }
        EXPECT_EQUAL( tm, test_id, key, i );

        key = dict.max();
        next_key = 0;
        i = 5;
        while( dict.predecessor( key, next_key ) )
        {
            EXPECT_EQUAL( tm, test_id, key, i-- );
            // go to the next key.
            key = next_key;
        }
        EXPECT_EQUAL( tm, test_id, key, i );
    }

    // Creates a test manager for the DSAL class.
    TestManager tm2{ "DSAL<int, string> Suite" };

    {
        //=== Test #1
        std::string test_id{"DefaultCtro"};
        REGISTER( tm2, test_id, "Testing the default constructor." );
        // Testing default constructor.
        DSAL<int, std::string > dic;
        EXPECT_EQUAL( tm2, test_id,   dic.capacity(), 50 );
    }

    {
        //=== Test #2
        auto test_id{ "CtroSize" };
        REGISTER( tm2, test_id, "Testing the constructor that receives a size." );
        DSAL<int, std::string> dic2(100);
        EXPECT_EQUAL( tm2, test_id,   dic2.capacity(), 100 );
    }

    {
        // Testing insert.
        DSAL<int, std::string, MyKeyComparator> dict(3);
        auto test_id{ "InsertRegular" };
        REGISTER( tm2, test_id, "Testing the regular insert." );
        // testing the regular insert
        auto result1 =  dict.insert( 1, "AAA" ) ;
        auto result2 =  dict.insert( 2, "BBB" ) ;
        EXPECT_TRUE ( tm2, test_id, result1 and result2 );
    }
    
    {
        auto test_id{ "InsertDuplicate" };
        REGISTER( tm2, test_id, "Testing the insert failure when we insert a duplicate key." );
        DSAL<int, std::string, MyKeyComparator> dict(3);
        auto result1 =  dict.insert( 1, "AAA" ) ;
        auto result2 =  dict.insert( 2, "BBB" ) ;
        EXPECT_TRUE( tm2, test_id, result1 and result2 );
        // Testing the insert failure for duplicate keys.
        EXPECT_FALSE( tm2, test_id, dict.insert( 2, "CCC" ) );
    }

    {
        // Testing the insert for overflow check.
        auto test_id{ "InsertOverflow" };
        REGISTER( tm2, test_id, "Testing the insert indication of overflow when we exceed the dictionary limit." );
        DSAL<int, std::string, MyKeyComparator> dict(3);
        auto result1 =  dict.insert( 1, "AAA" ) ;
        auto result2 =  dict.insert( 2, "BBB" ) ;
        auto result8 = ( dict.insert( 3, "CCC" ) );
        EXPECT_TRUE( tm2, test_id, result1 and result2 and result8 );
        // Trying to insert an extra item.
        // The dictionary may return false if it's a static version,
        // or true, if it's a dynamic version.
        auto old_size = dict.capacity();
        auto result4 = dict.insert( 4, "DDD" );
        if ( result4 == true ) // Dynamic version.
            EXPECT_TRUE( tm2, test_id,   dict.capacity() > old_size );
        else
            EXPECT_FALSE( tm2, test_id,   result4  ); // static dictionary
    }

    // {
    //     // Testing search.
    //     DSAL<int, std::string> dict;
    //     std::string result;

    //     // The Data
    //     struct Entry
    //     {
    //         int key;
    //         std::string data;
    //     } table[] =
    //     {
    //         { 1, "AAA" },
    //         { 2, "BBB" },
    //         { 3, "CCC" },
    //         { 4, "DDD" },
    //         { 5, "EEE" }
    //     };

    //     auto test_id{ "SearchingBasic" };
    //     REGISTER( tm2, test_id, "Testing basic searching.");
    //     bool passed{ false };
    //     // insert all itens first.
    //     for ( const auto & e : table )
    //     {
    //         passed = dict.insert( e.key, e.data ) ;
    //         if ( not passed ) break;
    //     }
    //     EXPECT_TRUE( tm2, test_id,  passed );

    //     // retrieve itens
    //     passed = false;
    //     for ( const auto & e : table )
    //     {
    //         passed = ( dict.search( e.key, result ) );
    //         if ( not passed ) break;
    //         passed = ( result == e.data );
    //         if ( not passed ) break;
    //     }
    //     EXPECT_TRUE( tm2, test_id,  passed );

    //     {
    //         auto test_id{ "SearchingInexisting" };
    //         REGISTER( tm2, test_id, "Trying to retrieve an inexisting element from the dictionary.");
    //         // serching for non-existing key.
    //         EXPECT_FALSE( tm2, test_id,    ( dict.search( 6, result ) ) );
    //     }
    // }
    // {
    //     std::string result;
    //     auto test_id{ "SearchingEmpty" };
    //     REGISTER( tm2, test_id, "Searching in an empty dictionary.");
    //     // seaching in an empty dictionary.
    //     DSAL<int, std::string> dict;
    //     EXPECT_FALSE( tm2, test_id, ( dict.search( 1, result)  ) ) ;
    // }

    // {
    //     // Testing remove.
    //     DSAL<int, std::string> dict;
    //     std::string result;

    //     // The Data
    //     struct Entry
    //     {
    //         int key;
    //         std::string data;
    //     };

    //     Entry table[] =
    //     {
    //         { 1, "AAA" },
    //         { 2, "BBB" },
    //         { 3, "CCC" },
    //         { 4, "DDD" },
    //         { 5, "EEE" }
    //     };


    //     auto test_id{ "RemoveBasic" };
    //     REGISTER( tm2, test_id, "Testing basic removing of data.");
    //     // insert all itens first.
    //     for ( const auto & e : table )
    //     {
    //         EXPECT_TRUE( tm2, test_id, dict.insert( e.key, e.data ) ) ;
    //     }

    //     // Trying to remove a non-existing element.
    //     EXPECT_FALSE ( tm2, test_id,   dict.remove( 6 , result ) );

    //     // remove itens
    //     for ( const auto & e : table )
    //     {
    //         EXPECT_TRUE( tm2, test_id,   dict.remove( e.key, result ) );
    //         EXPECT_EQUAL( tm2, test_id, result , e.data );
    //     }

    //     EXPECT_TRUE( tm2, test_id,   dict.empty() );

    //     // seaching in an empty dictionary.
    //     DSAL<int, std::string> dict2;
    //     EXPECT_FALSE( tm2, test_id, dict2.remove( 1, result) );
    // }

    // {
    //     // Testing min/max.
    //     DSAL<int, std::string> dict;
    //     std::string result;

    //     // The Data
    //     struct Entry
    //     {
    //         int key;
    //         std::string data;
    //     };

    //     Entry table[] =
    //     {
    //         { 1, "AAA" },
    //         { 2, "BBB" },
    //         { 3, "CCC" },
    //         { 4, "DDD" },
    //         { 5, "EEE" }
    //     };

    //     // Shuffle table
    //     std::random_device rd;
    //     std::mt19937 g(rd());
    //     std::shuffle( std::begin( table ), std::end( table ), g);

    //     auto test_id{ "MinMaxBasic" };
    //     REGISTER( tm2, test_id, "Testing basic use of min/max.");
    //     // insert all itens first.
    //     for ( const auto & e : table )
    //     {
    //         EXPECT_TRUE( tm2, test_id, dict.insert( e.key, e.data ) ) ;
    //     }

    //     EXPECT_EQUAL( tm2, test_id,   dict.min(), 1 );
    //     EXPECT_EQUAL( tm2, test_id,   dict.max(), 5 );

    //     // Trying to get min/max of a empty dictionary.
    //     DSAL<int, std::string> dict2;
    //     auto worked( false );
    //     try {
    //         result = dict2.min();
    //     }
    //     catch ( std::out_of_range & e )
    //     {
    //         worked = true;
    //     }
    //     EXPECT_TRUE( tm2, test_id, worked );
    //     worked =  false;
    //     try {
    //         result = dict2.max();
    //     }
    //     catch ( std::out_of_range & e )
    //     {
    //         worked = true;
    //     }
    //     EXPECT_TRUE( tm2, test_id, worked );
    // }

    // {
    //     // Testing successor/predecessor
    //     DSAL<int, std::string> dict;
    //     std::string result;

    //     // The Data
    //     struct Entry
    //     {
    //         int key;
    //         std::string data;
    //     };

    //     Entry table[] =
    //     {
    //         { 1, "AAA" },
    //         { 2, "BBB" },
    //         { 3, "CCC" },
    //         { 4, "DDD" },
    //         { 5, "EEE" }
    //     };

    //     // Shuffle table
    //     std::random_device rd;
    //     std::mt19937 g(rd());
    //     std::shuffle( std::begin( table ), std::end( table ), g);

    //     auto test_id{ "SucePredBasic" };
    //     REGISTER( tm2, test_id, "Testing basic use of successor/predecessor.");
    //     // insert all itens first.
    //     for ( const auto & e : table )
    //     {
    //         EXPECT_TRUE( tm2, test_id, dict.insert( e.key, e.data ) ) ;
    //     }

    //     auto key { dict.min() };
    //     int next_key{0};
    //     int i{1};
    //     while( dict.successor( key, next_key ) )
    //     {
    //         EXPECT_EQUAL( tm2, test_id, key, i++ );
    //         // go to the next key.
    //         key = next_key;
    //     }
    //     EXPECT_EQUAL( tm2, test_id, key, i );

    //     key = dict.max();
    //     next_key = 0;
    //     i = 5;
    //     while( dict.predecessor( key, next_key ) )
    //     {
    //         EXPECT_EQUAL( tm2, test_id, key, i-- );
    //         // go to the next key.
    //         key = next_key;
    //     }
    //     EXPECT_EQUAL( tm2, test_id, key, i );
    // }

    tm.summary();
    std::cout << std::endl;
    tm2.summary();
    return EXIT_SUCCESS;
}
