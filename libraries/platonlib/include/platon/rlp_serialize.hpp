#pragma once

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <vector>
#include "platon/RLP.h"
#include "platon/common.h"
#include "platon/rlp_extend.hpp"

#define PLATON_REFLECT_MEMBER_NUMBER(r, OP, elem) items_number++;

#define PLATON_REFLECT_MEMBER_OP_INPUT(r, OP, elem) OP t.elem

#define PLATON_REFLECT_MEMBER_OP_OUTPUT(r, OP, elem) \
  OP(rlp[vect_index], t.elem);                       \
  vect_index++;

/**
 *  Defines serialization and deserialization for a class
 *
 *  @brief Defines serialization and deserialization for a class
 *
 *  @param TYPE - the class to have its serialization and deserialization
 * defined
 *  @param MEMBERS - a sequence of member names.  (field1)(field2)(field3)
 */
#define PLATON_SERIALIZE(TYPE, MEMBERS)                                     \
  friend RLPStream& operator<<(RLPStream& rlp, const TYPE& t) {             \
    size_t items_number = 0;                                                \
    BOOST_PP_SEQ_FOR_EACH(PLATON_REFLECT_MEMBER_NUMBER, <<, MEMBERS)        \
    return rlp.appendList(items_number)                                     \
        BOOST_PP_SEQ_FOR_EACH(PLATON_REFLECT_MEMBER_OP_INPUT, <<, MEMBERS); \
  }                                                                         \
  friend void fetch(const RLP& rlp, TYPE& t) {                              \
    size_t vect_index = 0;                                                  \
    BOOST_PP_SEQ_FOR_EACH(PLATON_REFLECT_MEMBER_OP_OUTPUT, fetch, MEMBERS)  \
  }

/**
 *  Defines serialization and deserialization for a class which inherits from
 * other classes that have their serialization and deserialization defined
 *
 *  @brief Defines serialization and deserialization for a class which inherits
 * from other classes that have their serialization and deserialization defined
 *
 *  @param TYPE - the class to have its serialization and deserialization
 * defined
 *  @param BASE - a sequence of base class names (basea)(baseb)(basec)
 *  @param MEMBERS - a sequence of member names.  (field1)(field2)(field3)
 */
#define PLATON_SERIALIZE_DERIVED(TYPE, BASE, MEMBERS)                       \
  friend RLPStream& operator<<(RLPStream& rlp, const TYPE& t) {             \
    size_t items_number = 1;                                                \
    BOOST_PP_SEQ_FOR_EACH(PLATON_REFLECT_MEMBER_NUMBER, <<, MEMBERS)        \
    rlp.appendList(items_number) << static_cast<const BASE&>(t);            \
    return rlp BOOST_PP_SEQ_FOR_EACH(PLATON_REFLECT_MEMBER_OP_INPUT, <<,    \
                                     MEMBERS);                              \
  }                                                                         \
  friend void fetch(const RLP& rlp, TYPE& t) {                              \
    size_t vect_index = 0;                                                  \
    fetch(rlp[vect_index], static_cast<BASE&>(t));                          \
    vect_index++;                                                           \
    BOOST_PP_SEQ_FOR_EACH(PLATON_REFLECT_MEMBER_OP_OUTPUT, fetch, MEMBERS); \
  }
