#include "ContinuousRecorder.hpp"

ContinuousRecorder::ContinuousRecorder(std::vector<int16_t>& sharedBuffer, std::mutex& mtx) : buffer(sharedBuffer), bufferMutex(mtx)
{

};

bool ContinuousRecorder::onStart()
{
    std::cout << "Recording started\n";
    return true;
};

bool ContinuousRecorder::onProcessSamples(const int16_t* samples, std::size_t count)
{
    std::lock_guard<std::mutex> lock(bufferMutex);
    buffer.insert(buffer.end(), samples, samples + count);
    return true;
};

void ContinuousRecorder::onStop()
{
    std::cout << "Recording stopped\n";
};
