#include "SpeechRecognizer.hpp"

SpeechRecognizer::SpeechRecognizer(std::vector<int16_t>& sharedBuffer, std::mutex& mtx, std::string& lastTranscript, ContinuousRecorder& recorder) : a_audioBuffer(sharedBuffer), a_audioMutex(mtx), a_lastTranscript(lastTranscript), a_recorder(recorder)
{
    std::thread whisperThread(&SpeechRecognizer::whisperLoop, this);
    whisperThread.detach();
};

void SpeechRecognizer::whisperLoop()
{
    if (!std::filesystem::exists("../external/whisper.cpp/models/ggml-base.en.bin")) {
        std::cerr << "Model not found!\n";
        exit(1);
    }

    whisper_context* ctx = whisper_init_from_file_with_params("../external/whisper.cpp/models/ggml-base.en.bin", {});

    while (true) {

        if (!a_recorder.readyToTranscribe)
            continue;

        a_recorder.readyToTranscribe = false;

        std::vector<int16_t> pcm;
        {
            std::lock_guard<std::mutex> lock(a_audioMutex);
            pcm = a_audioBuffer;
            a_audioBuffer.clear();
        }

        if (pcm.empty())
            continue;

        std::vector<float> mono16k = resampleTo16k(pcm);

        whisper_full_params params = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
        params.print_realtime = false;
        params.print_progress = false;
        params.print_timestamps = false;
        params.language = "en";
        params.detect_language = false;
        params.no_timestamps = true;
        params.no_context = true;
        params.single_segment = true;

        if (whisper_full(ctx, params, mono16k.data(), mono16k.size()) == 0) {
            const char* txt = whisper_full_get_segment_text(ctx, 0);
            
            if (txt && strlen(txt) > 0) {
                std::cout << "[TRANSCRIPT] " << txt << "\n";
                a_lastTranscript = txt;
            }
        }
    }
};

std::vector<float> SpeechRecognizer::resampleTo16k(const std::vector<int16_t>& pcm44100) {
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
};
