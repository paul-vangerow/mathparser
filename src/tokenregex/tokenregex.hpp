#include <iostream>
#include <vector>
#include <unordered_set>
#include <stack>

struct Modifier {
    // State Machine Tree Splits (Nonlinear numeric progressions)
    bool diverging;
    // Node can be skipped (Previous node inherits modifiers and links)
    bool optional;
    // Node can be repeated (Node inherits modifiers and links of previous node)
    bool recursive;
    // Node before which the modifiers apply (Containing links to the node).
    std::size_t begin;
    // Node itself.
    std::size_t end;
    // Diverge Merger
    std::size_t merge_end;

    Modifier(bool _diverging, 
             bool _optional, 
             bool _recursive, 
             std::size_t _begin, 
             std::size_t _end,
             std::size_t _merge_end = 0) 
    : diverging(_diverging)
    , optional(_optional)
    , recursive(_recursive)
    , begin(_begin)
    , end(_end)
    , merge_end(_merge_end)
    {}
};

template <typename T> 
struct SequenceConstructor {
    std::vector<std::unordered_set<T>> tokens;
    std::vector<Modifier> modifiers;
};

struct DivergeStackItem {
    std::size_t & start_val;
    std::size_t & end_val;
    std::size_t context_stack_height;

    DivergeStackItem( std::size_t & begin_ref, std::size_t & end_ref, std::size_t height ) 
    : start_val(begin_ref)
    , end_val(end_ref)
    , context_stack_height(height) 
    {}
};

static void operator<<(std::ostream& stream, SequenceConstructor<char>& val){
    stream << "Tokens:\n";
    for (auto v : val.tokens){
        stream << "[";
        for (auto i : v) stream << i << " ";
        stream << "]\n";
    }
    stream << "Modifiers:\n";
    for (auto m : val.modifiers){
        stream << "Diverging-{" << m.diverging 
               <<"} Optional-{"<< m.optional 
               <<"} Recursive-{"<< m.recursive 
               <<"} Begin-{"<< m.begin 
               <<"} End-{"<< m.end
               <<"} Merge-{" << m.merge_end << "}\n";
    }
}

// Build a set between two points
static std::unordered_set<char> make_char_set( int begin, int end, std::string match_string ){
    std::unordered_set<char> out_set;

    for (int i = begin; i <= end; i++){
        // Range of characters or not.
        if ( i == end || match_string[i+1] != '-' ) out_set.insert( match_string[i] );
        else {
            for (char j = match_string[i]; j <= match_string[i+2]; j++) out_set.insert(j);
            i += 2;
        }
    }
    return out_set;
}

// For building a sequence out of a character array
static SequenceConstructor<char> convertBasicTokenSequence(std::string match_string){
    SequenceConstructor<char> out;

    // *** States *** //
    std::size_t ctxt_begin = 0;
    std::size_t ctxt_end = 1;

    std::stack<std::size_t> ctxt_stack;
    std::stack<DivergeStackItem> merge_stack;

    for (int i = 0; i < match_string.size(); i++){
        char c = match_string[i];
        switch ( c ) {
            case '(': { // Context Begin
                ctxt_stack.push(ctxt_begin);
            } break;
            case ')': { // Context End
                ctxt_begin = ctxt_stack.top();
                if ( merge_stack.top().context_stack_height == ctxt_stack.size() ){
                    auto & v = merge_stack.top();
                    v.start_val = ctxt_begin;
                    v.end_val = ctxt_end;
                    merge_stack.pop();
                }
                ctxt_stack.pop();
            } break;
            case '[': { // Construct a Set
                for (int j = i+1; j < match_string.size(); j++){
                    if ( match_string[j] != ']' ) continue;
                    out.tokens.emplace_back( make_char_set( i+1, j-1, match_string ) );
                    i = j;
                    break;
                }
            } break;
            case '+': { // One or More
                out.modifiers.emplace_back( false, false, true, ctxt_begin, ctxt_end );
            } break;
            case '?': { // Zero or One
                out.modifiers.emplace_back( false, true, false, ctxt_begin, ctxt_end );
            } break;
            case '*': { // Zero or More
                out.modifiers.emplace_back( false, true, true, ctxt_begin, ctxt_end );
            } break;
            case '|': { // Boolean Or
                out.modifiers.emplace_back( true, false, false, ctxt_begin, ctxt_begin, ctxt_begin );
                merge_stack.push( DivergeStackItem( out.modifiers.back().begin, out.modifiers.back().end, ctxt_stack.size()) );
            } break;
            case '\\': { // Escape Special
                c = match_string[++i];
            }
            default: { // Just Normal Characters
                out.tokens.emplace_back( std::unordered_set<char>{ c } );
                ctxt_begin = ctxt_end;
                ctxt_end++;
            } break;
        }
    }

    return out;
}