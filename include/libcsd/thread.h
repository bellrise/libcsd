#pragma once

#include <libcsd/map.h>

using pthread_t = unsigned long;
using fptr_t = void *(*) (void *);

namespace csd
{

struct thread {
	/* inherited */
	thread() = default;
	thread(fptr_t fptr);
	thread(fptr_t fptr, void *arg);
	pthread_t getid() const;
	int join();
	void *get_result();
	int detach() const;
	int cancel() const;
	void exit();
	int kill(int sig) const;
	bool operator==(const thread& other) const;
	void operator()(fptr_t fptr, void *arg);
	void operator()(fptr_t fptr);

    private:
	void *ret;
	bool _is_running;
	pthread_t id;
	void run(fptr_t fptr, void *arg);
};

} // namespace csd
