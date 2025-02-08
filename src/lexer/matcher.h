#include <vector>
#include <lexer/lexer_sequence.h>

class Matcher {
    struct MatchLayer {
        Matcher * parent;
        std::vector<LexerSequence> active_sequences;
        std::vector<std::string> current_tokens;
        std::unique_ptr<MatchLayer> next;

        MatchLayer(Matcher * __parent__, std::vector<LexerSequence> sequences, std::vector<std::string>&& tokens);

        int match_token(char c);
    };

    std::vector<LexerSequence>& original;
    std::unique_ptr<MatchLayer> top;

    std::vector<std::string> out_tokens;
public:
    std::unique_ptr<MatchLayer> construct_new_layer(std::vector<std::string> current_tokens, std::string matched_token = "");

    Matcher(std::vector<LexerSequence>& __original__);

    void match_token(char c);

    std::vector<std::string> get_tokens();
};