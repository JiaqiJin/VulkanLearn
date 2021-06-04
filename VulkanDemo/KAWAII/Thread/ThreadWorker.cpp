#include "ThreadWorker.hpp"

ThreadWorker::ThreadWorker()
{

}

void ThreadWorker::AppendJob(ThreadJob job)
{
	std::unique_lock<std::mutex> lock(m_queueMutex);
	m_jobQueue.push(job);
	m_condition.notify_one();
}

void ThreadWorker::Loop()
{
	while (true)
	{
		ThreadJob job;
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			m_condition.wait(lock, [this] { return !m_jobQueue.empty() || m_isDestroying; });

			if (m_isDestroying)
			{
				break;
			}

			job = m_jobQueue.front();
			m_jobQueue.pop();
			m_isWorking = true;
		}

		// Excute the job
		job.job();
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			m_isWorking = false;
			m_condition.notify_one();
		}
	}
}

ThreadWorker::~ThreadWorker()
{

}