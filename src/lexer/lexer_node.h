#pragma once

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <queue>

// --------------------- MatchResult --------------------- //
//                                                         //
// Used to store the combined state of when matching with  //
// iterators.                                              //
//                                                         //
// ------------------------------------------------------- //

struct MatchResult {
    // True if ANY navigated to nodes are marked 'end'.
    bool is_end;

    // True if NO non-episolon nodes could be navigated to.
    bool is_empty;
};

// ---------------------- LexerNode ---------------------- //
//                                                         //
// A single state in the state machine representing the    //
// sequence regex. Links to other states via valid         //
// transitions and contains some additional metadata.      //
//                                                         //
// ------------------------------------------------------- //

class LexerNode {
private:
    // Map of valid transitions from this state to another based
    // on the next provided character. Epsilon transitions (~)
    // are always valid and are always taken before other checks
    // are done. A single character may lead to multiple DIFFERENT
    // nodes.
    std::unordered_map<char, std::vector<LexerNode*>> m_transitions;

    // Indicates if the node is a valid Regex end node (a complete
    // match).
    bool m_is_end;

    // Primary for debugging purposes, the nodes unique ID.
    std::size_t m_node_number;
public:
    LexerNode(bool is_end = false);

    // Add a single transition to the LexerNode
    // 
    // value: Character which enables the transition
    // target: Pointer of Node this transition leads to
    void addTransition(char value, LexerNode* target);

    // Add multiple transitions to the LexerNode (with the same)
    // target.
    //
    // values: Set of characters which enable the transition
    // target: Pointer of Node this transition leads to
    void addTransitions(std::unordered_set<char> values, LexerNode* target);

    // Remove a single transition from the LexerNode.
    //
    // node: The Target Node to remove
    // key:  The transition character this node is found under.
    //
    // return: If the removal was successful or not (the node existed)
    bool removeTransition(LexerNode* node, char key);

    // Getter for the is_end property.
    //
    // return: is_end property.
    bool is_end();

    // A method to get all nodes after matching with the input
    // character.
    //
    // c: The Character to find all resultant states of
    //
    // return: A list of States the transition leads to.
    std::vector<LexerNode*> match(char c);

    // A recursive method for navigating all nodes AFTER the match 
    // transition has occured. This is needed as any EPSILON transitions
    // MUST be taken and we need to know their end states (result in any
    // connections / lead to END nodes). Any non-epsilon nodes found are
    // added to new_iterators (the next set of possible states).
    //
    // result: The MatchResult object which maintains the overall match
    //         state of the current Match operation.
    // new_iterators: The queue of next states as a result of the match
    //         transition taking place.
    void handle_postiterators(MatchResult & result, std::queue<LexerNode*>& new_iterators);

    // A recursive method primarily needed for the first transition of
    // each sequence. It ensures that any immediate empsilon transitions
    // taken so that we check all correct states for transitions.
    //
    // old_iterators: The list of current possible states, updated before
    //         the match occurs to ensure all possible current states are 
    //         considered
    void handle_preiterators(std::queue<LexerNode*>& old_iterators);

    // Helper methods for printing out nodes.
    void printSingular();
    void printInitial();
    void printNode( std::unordered_set<int>& visited);
};

// ---------------------- NodeBlock ---------------------- //
//                                                         //
// An abstract object for representing a set of nodes.     //
// Contains a reference to its entry and exit points and   //
// is the main object used to implement the Thompsons      //
// construction.                                           //
//                                                         //
// ------------------------------------------------------- //

class NodeBlock {
public:
    // The First node in the block, the node other node 
    // blocks connect to if they want to lead into this
    // block.
    LexerNode* entry;

    // The last node in the block, the node connected from
    // to link blocks together,
    LexerNode* exit;

    // A property to indicate this block is divergent, any
    // blocks attached to this block will be inserted within
    // with an epsilon transition (this allows the parent
    // block entry to lead to two different NodeBlocks
    // simultaneously).
    bool divergent;

    NodeBlock()
    : entry(new LexerNode())
    , exit(new LexerNode(true))
    , divergent(false){}

    NodeBlock(bool div)
    : entry(new LexerNode())
    , exit(new LexerNode(true))
    , divergent(div){}

    NodeBlock(LexerNode* _entry, LexerNode* _exit)
    : entry(_entry)
    , exit(_exit)
    , divergent(false){}
};