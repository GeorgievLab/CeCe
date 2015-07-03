#pragma once

#include "Reaction.hpp"
#include "core/DynamicArray.hpp"

class ReactionParser
{
    const char* current;
    const char* end_of_string;
    bool validator;
    bool reversible;
    
    void check_push(String& id, DynamicArray<String>& array);
    
    DynamicArray<String> parseList();

    float parseRate(const char end_char);
    
    public:
    Reaction parseReactionCode(const String& code);
};