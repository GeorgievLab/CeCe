#pragma once

#include "Reaction.hpp"
#include "core/DynamicArray.hpp"
#include "core/Grid.hpp"

class ReactionParser
{
    const char* current;
    const char* end_of_string;
    bool validator;
    
    void check_push(String& id, DynamicArray<String>& array);
    
    DynamicArray<String> parseList();

    float parseRate();
    
    public:
    Reaction parseReactionCode(const String& code);
};