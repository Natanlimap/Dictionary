/**
 * DAL/DSAL classes implementation.
 */

#include "dal.h"

/**
 * ---------------------------------------------------------------------------
 * Common methods from DAL.
 * ---------------------------------------------------------------------------
 */

//----------------------------------------------------------------------------------------
/*! @brief Basic constructor
 *  Creates a dictionary of the requested capacity.
 *
 *  @param capacity_ The requested storage capacity of the dictionary.
 */
// template < typename KeyType, typename DataType, typename KeyTypeLess >
// DAL<KeyType,DataType,KeyTypeLess>::DAL ( size_t capacity_ )
//     : m_length{ 0 }
//     , m_capacity{ capacity_ }
//     , m_array{ new entry_type[ m_capacity ] }
// { /* empty */ }



//  DAL ( size_t tam = SIZE ){
//  	 m_capacity = tam;
//  	 m_length = 0;
//  	 m_array = new std::unique_ptr<entry_type>[m_capacity];
//  }
//  virtual ~DAL (){
//  	delete[]m_array;
//  }

//  bool insert(const KeyType & _newKey, const DataType & _newInfo){
//  	m_array[m_length].KEY = _newKey;
//  	m_array[m_length].DATA = _newInfo;
//  	m_length++;
//  }