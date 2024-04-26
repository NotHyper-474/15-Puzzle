#pragma once

#include <unordered_map>
#include <string>

/*
This file was originally a typedef for a Boost Multi-Index Container,
But then I noticed an ordered-by-insertion map wasn't necessary
Now it's just a more convenient way to use this type
*/
typedef std::unordered_map<std::string, int> countMap;