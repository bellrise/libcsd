#include <libcsd/thread.h>
#include <pthread.h>
#include <signal.h>

using namespace csd;

void thread::run(fptr_t fptr, void *arg)
{
	if (m_busy)
		pthread_join(m_id, &m_ret);

	pthread_create(&m_id, NULL, fptr, arg);
	m_busy = true;
}

thread::thread(fptr_t fptr)
{
	run(fptr, NULL);
}

thread::thread(fptr_t fptr, void *arg)
{
	run(fptr, arg);
}

thread::thread(thread&& moved_thread)
    : m_id(moved_thread.m_id)
    , m_ret(moved_thread.m_ret)
    , m_busy(moved_thread.m_busy)
{
	moved_thread.m_id = 0;
	moved_thread.m_ret = NULL;
	moved_thread.m_busy = false;
}

thread::~thread()
{
	pthread_join(m_id, &m_ret);
}

pthread_t thread::getid() const
{
	return m_id;
}

int thread::join()
{
	int c = pthread_join(m_id, &m_ret);
	m_busy = false;
	return c;
}

void *thread::result()
{
	if (m_busy)
		this->join();

	return m_ret;
}

int thread::detach() const
{
	return pthread_detach(m_id);
}

int thread::cancel() const
{
	return pthread_cancel(m_id);
}

void thread::exit()
{
	pthread_exit(&m_id);
}

int thread::kill(int sig) const
{
	return pthread_kill(m_id, sig);
}

bool thread::operator==(const thread& other) const
{
	return m_id == other.getid();
}

void thread::operator()(fptr_t fptr, void *arg)
{
	run(fptr, arg);
}

void thread::operator()(fptr_t fptr)
{
	run(fptr, NULL);
}
