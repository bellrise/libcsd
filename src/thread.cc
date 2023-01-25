#include <libcsd/thread.h>

#include <signal.h>
#include <pthread.h>

using namespace csd;

void thread::run(fptr_t fptr, void *arg)
{
	if (_is_running)
		pthread_join(id, &ret);

	pthread_create(&id, NULL, fptr, arg);
	_is_running = true;
}

thread::thread(fptr_t fptr)
{
	run(fptr, NULL);
}

thread::thread(fptr_t fptr, void *arg)
{
	run(fptr, arg);
}

thread::~thread()
{
	this->join();
}

pthread_t thread::getid() const
{
	return id;
}

int thread::join()
{
	int c = pthread_join(id, &ret);
	_is_running = false;
	return c;
}

void *thread::get_result()
{
	if (_is_running)
		this->join();

	return ret;
}

int thread::detach() const
{
	return pthread_detach(id);
}

int thread::cancel() const
{
	return pthread_cancel(id);
}

void thread::exit()
{
	pthread_exit(&id);
}

int thread::kill(int sig) const
{
	return pthread_kill(id, sig);
}

bool thread::operator==(const thread& other) const
{
	return id == other.getid();
}

void thread::operator()(fptr_t fptr, void *arg)
{
	run(fptr, arg);
}

void thread::operator()(fptr_t fptr)
{
	run(fptr, NULL);
}
