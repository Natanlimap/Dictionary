//! This class implements a Dictionary.


#ifndef _DAL_H_
#define _DAL_H_

#include <iostream>   // std::cout, std::endl
#include <stdexcept>  //
#include <functional> // std::less<>()
#include <memory>     // std::unique_ptr
#include <algorithm>  // std::copy()
#include <cstring>    // std::memmove()
#include <utility>    // std::pair, std::get<>()
#include <iterator>

/// This class implements a dictionary with an UNsorted array of keys.
/*!
 * @tparam KeyType The key type.
 * @tparam DataType Tha data type to be stored in the dictionary.
 * @tparam KeyTypeLess A functor/function pointer that compares two keys for strict order <.
 */
template <typename KeyType , typename DataType, typename KeyTypeLess = std::less<KeyType> >
class DAL
{
    protected:
        //=== Alias
        /// Alias that defines a table item.
        typedef std::pair< KeyType, DataType > entry_type;
        enum entry_id_t : size_t { 
        	KEY=0, //!< The key
            DATA=1 //!< The data
        };

        static constexpr size_t SIZE=50; //!< Default array size.
        size_t m_length;          //!< Array length
        size_t m_capacity;        //!< Current array capacity.
        std::unique_ptr<entry_type []> m_array; //!< Data storage area for the dynamic array.


    public:
        //=== special members.
        /// Default constructor.
        DAL ( size_t t = SIZE ){
        	m_length = 0;
        	m_capacity = t;
        	m_array.reset( new entry_type[t] );
        }
        /// Destructor
        virtual ~DAL (){
        	m_array.release();
        }
        /// Copy constructor
        DAL ( const DAL & );
        /// Move constructor
        DAL ( DAL && );
        /// Assignment operator
        DAL & operator= ( DAL );
        /// Move assignment operator
        DAL & operator= ( DAL && );
        //=== status members
        size_t 	capacity (void) const {
        	return m_capacity;
        }
        //=== acess members
        bool search (const KeyType & key, DataType & data) const{
        	for(size_t i = 0 ; i < m_length;i++){
        		if(key == m_array[i].first){
        			data = m_array[i].second;
        			return true;
        		}
        	}
        	return false;
        }
        //=== modifier members.
        bool insert(const KeyType & _newKey, const DataType & _newInfo){
        	if(m_length = m_capacity - 1)
        		resize();

        	for(size_t i = 0 ; i < m_length ; i++){
        		if(_newKey == m_array[i].first){
        			m_array[i].second = _newInfo;
        			return false;
        		}
        	}
        	m_array[m_length] = {_newKey, _newInfo};
        	m_length++;
        	return true;
        }
        void resize(){
        	std::unique_ptr<entry_type []> temp;
        	m_capacity = 2*m_capacity;
			temp.reset( new entry_type[m_capacity]);
			for(size_t i = 0; i<m_capacity/2;i++){
				temp[i] = m_array[i];
			}
			m_array.reset(new entry_type[m_capacity]);
			for(size_t i = 0; i<m_capacity/2;i++){
				m_array[i] = temp[i];
			}
			temp.release(); 
        }
};

/// This class implements a dictionary with a sorted array of keys.
/*!
 * @tparam KeyType The key type.
 * @tparam DataType Tha data type to be stored in the dictionary.
 * @tparam KeyTypeLess A functor/function pointer that compares two keys for strict order <.
 */
// template < typename KeyType, typename DataType, typename KeyTypeLess = std::less< KeyType > >
// class DSAL : public DAL< KeyType, DataType, KeyTypeLess >
// {
//     private:
//         /// Returns true and retrive in the second parameter the index of the requested key and returns true; false, otherwise.
//         bool find_index( const KeyType &, size_t & ) const; // Metodo de search auxiliar.

//     public:
//         //=== special methods
//         /// Default constructor
//         DSAL( size_t capacity_ = DAL<KeyType, DataType, KeyTypeLess>::SIZE ) : DAL<KeyType, DataType, KeyTypeLess>( capacity_ ) { /* Empty */ };
//         /// Destructor
//         virtual ~DSAL() { // TODO /* Empty */ };
//         /// Copy constructor
//         DSAL ( const DSAL & );
//         /// Move constructor
//         DSAL ( DSAL && );
//         /// Assignment operator
//         DSAL & operator= ( DSAL );
//         /// Move assignment operator
//         DSAL & operator= ( DSAL && );

//         //=== modifiers overwritten methods.

//         //=== Acessor members
// };

// #include "dal.inl" // This is to get "implementation" from another file.
#endif
