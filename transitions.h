#ifndef TRANSITIONS_H
#define TRANSITIONS_H

template <typename TxOutput> using new_head_t = typename TxOutput::newHead;
template <typename TxOutput> using new_state_t = typename TxOutput::state;
template <typename TxOutput> using direction_t = typename TxOutput::direction;

#endif