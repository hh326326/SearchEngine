/**
 * @file    :thread_pool.cc
 * @brief   :
 * @author  :hh
 * @version :V1.0.0
 * @date    :2024-12-17
 */

#include "reactor/thread_pool.h"
#include <chrono>
#include <thread>
#include <unistd.h>

namespace hh {
ThreadPool::ThreadPool(size_t thread_num, size_t queue_size)
    : _task_queue(queue_size), _thread_num(thread_num),
      // _queue_size(queue_size),
      _isExit(false) {
  _threads.reserve(_thread_num);
}

ThreadPool::~ThreadPool() {
  if (!_isExit) {
    Stop();
    _isExit = true;
  }
}

//线程池开始执行的时候，其实就是工作线程已经开启
void ThreadPool::Start() {
  for (size_t idx = 0; idx < _thread_num; ++idx) {
    /* unique_ptr<Thread> up(new WorkThread(*this)); */
    //    unique_ptr<Thread> up(new Thread(std::bind(&ThreadPool::threadFunc,
    //    this))) ;
    unique_ptr<Thread> up(new Thread([this] { ThreadFunc(); }));
    _threads.push_back(std::move(up));
  }

  for (auto &th : _threads) {
    th->Start(); //创建工作线程的id，将工作线程开始运行
  }
}

//等同于在生产者消费者例子里面的生产者
void ThreadPool::AddTask(Task &&task) {
  if (task) {
    _task_queue.Push(std::move(task));
  }
}

Task ThreadPool::GetTask() { return _task_queue.Pop(); }

//线程池的退出，其实就是线程池中几个工作线程的退出，而工作线程
// WorkThread就是从Thread继承过来的，所以每个工作线程执行join
//方法就可以了
void ThreadPool::Stop() {
  //只要任务队列中有数据，线程池中的工作线程就不能退出，让其
  // sleep
  while (!_task_queue.Empty()) {
    // sleep(1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  _isExit = true;
  //将所有等在在_notEmpty上的工作线程全部唤醒
  _task_queue.Wakeup();
  for (auto &th : _threads) {
    th->Join();
  }
}

//在线程池中封装的任务，这个任务的实际执行者WorkThread
void ThreadPool::ThreadFunc() {
  //只要线程池没有退出，就可以一直获取任务，并且执行
  //相应的process
  while (!_isExit) {
    // getTask如果执行的非常慢的话，线程池的stop有可能先执行，会将
    //_isExit设置为true
    //但是如果getTask执行的过快的话，来不及将_isExit设置为true，
    //一直卡在getTask，也就是卡在Condition中的wait上面
    Task taskcb = GetTask();
    if (taskcb) {
      taskcb();
    }
  }
}

} // namespace hh