#ifndef DOUBLE_COUNTER_H
#define DOUBLE_COUNTER_H

#include "values.h"

// reads language 0^n 1^m 0^n 1^m : m,n > 0
// algorithm:
//  if not 0:
//      FAIL
//
//  # copy left 0^n to end of tape
//  loop:
//      mark 0 as null
//      find_or_fail 1,0,1,null
//      write 0
//      find_left 1,0,1,null
//      move right
//      if 0:
//          goto loop
//  
//  # erase left 0^n from end of tape, verifying they are the same size
//  loop:
//      find 0
//      write null
//      find null
//      move left
//      if 1:
//          FAIL
//      write null
//      find_left null
//      move right
//      if 0:
//          goto loop
//
//  # check for remaining 0's at the end of tape
//  find null
//  move left
//  if 0:
//      FAIL
//  
//  # mark ends of tape
//  move right
//  write 0
//  move left
//  find_left null,1,null
//  write 0
//  find null
//
//  # erase 1's
//  loop:
//      find_left 0 or 1
//      if 0:
//          goto check_for_one
//      write null
//      find 0 or 1
//      if 0:
//          FAIL
//      write null
//
//  check_for_one:
//      find 0 or 1
//      if 1:
//          FAIL
//
//  SUCCESS
//      
//  find right 0^n
//  erase
// states:
//  reading-zeros:
//      overwrite-left-zero
//      scan-right-for-zero-in-zeros
//      scan-right-for-zero-in-ones
//      overwrite-right-zero
//      scan-left-for-zero

template <typename State, typename Head>
struct DoubleCounterTransition {
    using newState = Halt;
    using newHead = Head;
    using direction = Stay;
}

#define TRANSITION(fromState, fromHead, toState, toHead, direction) \
template <> \
struct DoubleCounterTransition<Int< fromState >, Int< fromHead >> { \
    using newState = Int< toState >; \
    using newHead = Int< toHead >; \
    using direction = Int< direction >; \
}

TRANSITION()

#undef TRANSITION

#endif