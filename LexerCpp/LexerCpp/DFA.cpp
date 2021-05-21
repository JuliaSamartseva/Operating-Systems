#include "DFA.h"

template<typename T>
DFA<T>::DFA(int initial, bool isfinal) {
	this->m_initial = this->m_state = initial;
	last_state = 0;
	add_state(initial, isfinal);
	add_state(-1, false); // invalid dead state
}

template<typename T>
DFA<T>::~DFA(void) {}

template<typename T>
void DFA<T>::add_state(int s, bool isfinal) {
	m_states.insert(s);
	if (isfinal) m_final_states.insert(s);
	last_state++;
}

template<typename T>
void DFA<T>::add_transition(int src, T input, int dest) {
	m_transitions.insert(pair<pair<int, T>, int>(pair<int, T>(src, input), dest));
}

template<typename T>
void DFA<T>::add_any_transition(int src, int dest) {
	any_transitions[src] = dest;
}

template<typename T>
void DFA<T>::reset() {
	m_state = m_initial;
}

template<typename T>
int DFA<T>::input(T inp) {
	// cases where the input matters (or if there are exclusions to 'any')
	auto tr = make_pair(m_state, inp);
	if (m_transitions.count(tr) > 0) {
		auto it = m_transitions.find(tr);
		return m_state = it->second;
	} 

	// no matter what is the input, current state will be destination
	if (any_transitions.find(m_state) != any_transitions.end()) {
		return m_state = any_transitions[m_state];
	}

	return m_state = -1;
}

template<typename T>
bool DFA<T>::is_accepting()
{
	return m_final_states.count(m_state) != 0;
}

template<typename T>
bool DFA<T>::is_accepting(T inp)
{
	auto tr = make_pair(m_state, inp);
	if (m_transitions.count(tr) > 0) {
		auto it = m_transitions.find(tr);
		return m_final_states.count(it->second) != 0;
	}

	// if the state is in any_transitions
	if (any_transitions.find(m_state) != any_transitions.end()) {
		return true;
	}

	else return false;
}

template<typename T>
int DFA<T>::state()
{
	return m_state;
}


template class DFA<char>;