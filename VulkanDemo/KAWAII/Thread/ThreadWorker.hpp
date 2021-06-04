#pragma once
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>

typedef std::function<void()> ThreadJobFunc;

class ThreadWorker
{
public:
	struct ThreadJob
	{
		ThreadJobFunc job;
		// TODO
	};
public:
	ThreadWorker();
	~ThreadWorker();

	void AppendJob(ThreadJob job);
	void Loop();
private:
	std::thread m_worker;
	std::mutex m_queueMutex;
	std::condition_variable m_condition;
	std::queue<ThreadJob> m_jobQueue;

	bool m_isWorking = false;
	bool m_isDestroying = false;
};