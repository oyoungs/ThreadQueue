#ifndef EASY_THREAD_PROMISE_H
#define EASY_THREAD_PROMISE_H

#include <thread/thread.h>

namespace Easy {

    class BrokenPromise: public std::exception {};
    class NoPromiseValue: public std::runtime_error {
    public:
        NoPromiseValue() :std::runtime_error("Promise Object Has No Value Prepared") {
        }
    };

    class DestroyerOfQueues
    {
    public:
        DestroyerOfQueues(const std::shared_ptr<ThreadQueue>& queue);

        void dispatch(const ThreadClosure& closure);

        ~DestroyerOfQueues();

    private:
        std::shared_ptr<ThreadQueue> queue;
    };


    template<typename T>
    using ResultBlock = std::function<T()>;
    template<typename T, typename U>
    using ResultWithArgBlock = std::function<T(const U&)>;


    template<typename ReturnType> class Promise;

    template<typename T>
    using PromiseClosure = std::function<void(Promise<T>)>;

    template<typename ReturnType>
    class Promise
    {

    public:
        template<typename T>
        using PromiseClosure = std::function<void(Promise<T>)>;


        Promise(const PromiseClosure<ReturnType>& closure) {
            queue = std::make_shared<DestroyerOfQueues>(Threading::getQueue(Threading::Serial));
            queue->dispatch([=]{
                try {
                    closure(*this);
                } catch(std::exception& e) {
                    fail(e);
                }
            });
        }

        Promise(const std::function<ReturnType()>& closure) {
            queue = std::make_shared<DestroyerOfQueues>(Threading::getQueue(Threading::Serial));
            queue->dispatch([=]{
                try {
                    set(closure());
                } catch(std::exception& e) {
                    fail(e);
                }
            });
        }

        template<typename LastType>
        Promise(Promise<LastType>& from, const ResultWithArgBlock<ReturnType, ResultBlock<LastType>>& closure) {
            queue = from.queue;
            queue->dispatch([=, &from]{
                try {
                    ResultBlock<LastType> block = [=, &from]{
                        try {
                            auto v = from.wait();
                            return v;
                        } catch(...) {
                            throw BrokenPromise();
                        }
                    };
                    auto value = closure(block);
                    set(value);
                } catch(std::exception& e) {
                    fail(e);
                }
            });
        }

        ReturnType get() {
            event.lock();
            defer ([=] { event.unlock();});

            if(error) {
                throw *error;
            }
            if(value) {
                return *value;
            }

            throw NoPromiseValue();
        }

        void set(const ReturnType& v) {
            event.lock();
            defer ([=] { event.unlock();});
            value = std::make_shared<ReturnType>(v);
            event.broadcast();
        }

        void fail(const std::exception& exception) {
            event.lock();
            defer ([=] {event.unlock();});
            error = std::make_shared<std::exception>(exception);
            event.broadcast();
        }

        template<typename NextType>
        Promise<NextType> then(const ResultWithArgBlock<NextType, ResultBlock<ReturnType>>& closure) {
            return Promise<NextType>(*this, closure);
        }

        Promise when(const std::function<void(const std::exception&)>& closure) {
            ResultWithArgBlock<ReturnType, ResultBlock<ReturnType>> block = [=](const ResultBlock<ReturnType>& value) -> ReturnType {
                try {
                    return value();
                } catch(std::exception& e) {
                    try {
                        closure(e);
                    } catch(...) {}
                    throw e;
                }
            };

            return Promise(*this, block);
        }

        ReturnType wait(double seconds = Threading::noTimeout) {
            event.lock();
            defer ([=] { event.unlock();});

            if(error) {
                throw *error;
            }

            if(value) {
                return *value;
            }

            do {
                if(error) {
                    throw *error;
                }

                if(value) {
                    return *value;
                }
            } while(event.wait(seconds));

            if(error) {
                throw *error;
            }

            if(value) {
                return *value;
            }

            throw NoPromiseValue();
        }

        std::shared_ptr<DestroyerOfQueues> queue;
    private:
        Threading::Event event;
        std::shared_ptr<ReturnType> value;
        std::shared_ptr<std::exception> error;
    };


}

#endif /*EASY_THREAD_PROMISE_H*/
