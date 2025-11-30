#include "Arya.hpp"

Arya::Arya()
    :
    m_rec(m_audioBuffer, m_audioMutex)
{
    if (!sf::SoundBufferRecorder::isAvailable()) {
        std::cerr << "No audio input available on your device" << std::endl;
        exit(1);
    }

    if (!m_rec.start(44100)) {
        std::cerr << "Failed to start Arya's continuous recorder" << std::endl;
        exit(1);
    }

    std::thread whisperThread(&Arya::WhisperLoop, this);
    whisperThread.detach();
    
    while (true) {
        Update();
    }
};

Arya::~Arya()
{
    m_rec.stop();
};

void Arya::CheckEvents()
{

};

void Arya::Update()
{
    if (!lastTranscript.empty()) {
        lastTranscript.clear();
    }
};

void Arya::WhisperLoop()
{
    if (!std::filesystem::exists("../external/whisper.cpp/models/ggml-base.en.bin")) {
        std::cerr << "Model not found!\n";
        exit(1);
    }
    whisper_context* ctx = whisper_init_from_file_with_params("../external/whisper.cpp/models/ggml-base.en.bin", {});

    while (true) {

        if (!m_rec.readyToTranscribe) {
            continue;
        }

        m_rec.readyToTranscribe = false;

        // --- Get audio chunk ---
        std::vector<int16_t> pcm;
        {
            std::lock_guard<std::mutex> lock(m_audioMutex);
            pcm = m_audioBuffer;
            m_audioBuffer.clear();
        }

        if (pcm.empty()) continue;

        // --- Resample ---
        std::vector<float> mono16k = resampleTo16k(pcm);

        // --- Whisper params ---
        whisper_full_params params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
        params.print_realtime   = false;
        params.print_progress   = false;
        params.print_timestamps = false;

        // --- Run ASR ---
        if (whisper_full(ctx, params, mono16k.data(), mono16k.size()) == 0) {
            const char* txt = whisper_full_get_segment_text(ctx, 0);
            
            if (txt && strlen(txt) > 0) {
                std::cout << "[TRANSCRIPT] " << txt << "\n";
                lastTranscript = txt;
            }
        }
    }
};

std::vector<float> Arya::resampleTo16k(const std::vector<int16_t>& pcm44100) {
    double ratio = 16000.0 / 44100.0;
    size_t outSize = pcm44100.size() * ratio;

    std::vector<float> out(outSize);

    for (size_t i = 0; i < outSize; i++) {
        double srcIndex = i / ratio;
        size_t i0 = (size_t)srcIndex;
        size_t i1 = std::min(i0 + 1, pcm44100.size() - 1);
        double frac = srcIndex - i0;

        float sample = (1 - frac) * pcm44100[i0] + frac * pcm44100[i1];
        out[i] = sample / 32768.f;
    }
    return out;
}
