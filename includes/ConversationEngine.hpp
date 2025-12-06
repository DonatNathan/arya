#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "llama.h"
#include "ggml.h"

#pragma once

class ConversationEngine {

    private:
        llama_model* a_model;
        llama_context* a_ctx;

    public:
        ConversationEngine();
        ~ConversationEngine();
        std::string askLLM(const std::string& prompt);

};
