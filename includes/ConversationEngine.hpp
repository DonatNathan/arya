#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "llama.h"
#include "ggml.h"

#include "donat_lib/Globals.hpp"

#pragma once

class ConversationEngine {

    private:
        llama_model* a_model;
        llama_context* a_ctx;
        const llama_vocab* a_vocab;
        llama_sampler* a_smpl;

    public:
        ConversationEngine();
        ~ConversationEngine();
        std::string askLLM(const std::string& prompt);

};
