#include "ConversationEngine.hpp"

ConversationEngine::ConversationEngine()
{
    llama_model_params model_params = llama_model_default_params();
    model_params.n_gpu_layers = NGL;

    if (!std::filesystem::exists("../models/Phi-3-mini-4k-instruct-q4.gguf")) {
        std::cerr << "PHI 3 Model not found!\n";
        exit(1);
    }

    a_model = llama_model_load_from_file("../models/Phi-3-mini-4k-instruct-q4.gguf", model_params);

    if (!a_model) {
        throw std::runtime_error("Failed to load LLM model!");
    }

    a_vocab = llama_model_get_vocab(a_model);

    llama_context_params params = llama_context_default_params();
    params.n_ctx = N_CTX;
    params.n_batch = N_CTX;
    a_ctx = llama_init_from_model(a_model, params);

    if (!a_ctx) {
        llama_model_free(a_model);
        throw std::runtime_error("Failed to create LLM context!");
    }

    a_smpl = llama_sampler_chain_init(llama_sampler_chain_default_params());
    llama_sampler_chain_add(a_smpl, llama_sampler_init_greedy());

};

ConversationEngine::~ConversationEngine()
{
    llama_free(a_ctx);
    llama_model_free(a_model);
};

std::string ConversationEngine::askLLM(const std::string& prompt)
{
    std::string response;

    const int n_prompt = -llama_tokenize(a_vocab, prompt.c_str(), prompt.size(), nullptr, 0, true, true);

    std::vector<llama_token> prompt_tokens(n_prompt);
    llama_tokenize(a_vocab, prompt.c_str(), prompt.size(), prompt_tokens.data(), prompt_tokens.size(), true, true);

    llama_batch batch = llama_batch_get_one(prompt_tokens.data(), prompt_tokens.size());

    for (int n_pos = 0; n_pos + batch.n_tokens < n_prompt + N_PREDICT;) {
        llama_decode(a_ctx, batch);

        llama_token new_token_id = llama_sampler_sample(a_smpl, a_ctx, -1);
        if (llama_vocab_is_eog(a_vocab, new_token_id)) break;

        char buf[128];
        int n = llama_token_to_piece(a_vocab, new_token_id, buf, sizeof(buf), 0, true);

        if (buf == "\n" && n_pos != 0)
            break;

        // std::cout << std::string(buf, n);

        response.append(buf, n);

        batch = llama_batch_get_one(&new_token_id, 1);
    }

    return response;
};
