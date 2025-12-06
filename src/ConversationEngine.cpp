#include "ConversationEngine.hpp"

ConversationEngine::ConversationEngine()
{
    llama_model_params model_params = llama_model_default_params();

    if (!std::filesystem::exists("../models/Phi-3-mini-4k-instruct-q4.gguf")) {
        std::cerr << "PHI 3 Model not found!\n";
        exit(1);
    }

    a_model = llama_model_load_from_file("../models/Phi-3-mini-4k-instruct-q4.gguf", model_params);

    if (!a_model) {
        throw std::runtime_error("Failed to load LLM model!");
    }

    llama_context_params params = llama_context_default_params();
    a_ctx = llama_init_from_model(a_model, params);

    if (!a_ctx) {
        llama_model_free(a_model);
        throw std::runtime_error("Failed to create LLM context!");
    }
};

ConversationEngine::~ConversationEngine()
{
    llama_free(a_ctx);
    llama_model_free(a_model);
};

std::string ConversationEngine::askLLM(const std::string& prompt)
{
    // std::vector<llama_token> tokens;
    // llama_tokenize(a_ctx, prompt.c_str(), tokens);

    // llama_eval_internal(a_ctx, tokens.data(), tokens.size(), 0, 0);

    // std::string response;
    // llama_token id;
    // while ((id = llama_sample_token(ctx, 1.0, 40)) != 0) {
    //     char c = static_cast<char>(id);
    //     response += c;
    // }
    // return response;
    return "";
};
