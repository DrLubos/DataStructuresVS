# pragma once

#include <complexities/complexity_analyzer.h>
#include <libds/adt/queue.h>
#include <random>
#include <limits>
#include <limits.h>


namespace ds::utils {
    template<class Queue>
    class QueueAnalyzer : public ComplexityAnalyzer<Queue> {
    protected:
        explicit QueueAnalyzer(const std::string &name);
    protected:
        void growToSize(Queue &structure, size_t size) override;
        std::default_random_engine rngData_;
        int data_;
		int maximalPriority = 50000;
    };

    template<class Queue>
    class QueueInsertAnalyzer : public QueueAnalyzer<Queue> {
    public:
        explicit QueueInsertAnalyzer(const std::string &name);

    protected:
        void executeOperation(Queue &structure) override;
    };

    template<class Queue>
    class QueueRemoveAnalyzer : public QueueAnalyzer<Queue> {
    public:
        explicit QueueRemoveAnalyzer(const std::string &name);

    protected:
        void executeOperation(Queue &structure) override;
    };

    class QueuesAnalyzer : public CompositeAnalyzer {
    public:
        QueuesAnalyzer();
    };

    //-------------

    template<class Queue>
    QueueAnalyzer<Queue>::QueueAnalyzer(const std::string &name)
            : ComplexityAnalyzer<Queue>(name),
              rngData_(144),
              data_(0) {
    }

    template<class Queue>
    void QueueAnalyzer<Queue>::growToSize(Queue &structure, size_t size) {
        const size_t toInsert = size - structure.size();
        for (size_t i = 0; i < toInsert; ++i) {
            std::uniform_int_distribution<int> dist(0, this->maximalPriority);
			int randomInt = dist(this->rngData_);
            structure.push(randomInt, randomInt);
        }
    }


    //-------------

    template<class Queue>
    QueueInsertAnalyzer<Queue>::QueueInsertAnalyzer(const std::string &name)
            : QueueAnalyzer<Queue>(name) {
        ComplexityAnalyzer<Queue>::registerBeforeOperation([&](Queue& queue) {
            std::uniform_int_distribution<int> dist(0, this->maximalPriority);
            this->data_ = dist(this->rngData_);
            });
    }

    template<class Queue>
    void QueueInsertAnalyzer<Queue>::executeOperation(Queue &structure) {
        structure.push(this->data_, this->data_);
    }

    //-------------

    template<class Queue>
    QueueRemoveAnalyzer<Queue>::QueueRemoveAnalyzer(const std::string &name)
            : QueueAnalyzer<Queue>(name) {
    }

    template<class Queue>
    void QueueRemoveAnalyzer<Queue>::executeOperation(Queue &structure) {
        structure.pop();
    }

    //-------------

class TwoListAnalyzer : public QueueAnalyzer<ds::adt::TwoLists<int, int>> {
            TwoListAnalyzer(std::string& name);
    ds::adt::TwoLists<int,int> createPrototype() override {
        return ds::adt::TwoLists<int, int>(10000);
    }
        };

    inline QueuesAnalyzer::QueuesAnalyzer() :
            CompositeAnalyzer("Queues")
    {
        this->addAnalyzer(std::make_unique<QueueInsertAnalyzer<ds::adt::TwoLists<int, int>>>("Two-lists-insert"));
        this->addAnalyzer(std::make_unique<QueueRemoveAnalyzer<ds::adt::TwoLists<int, int>>>("Two-lists-remove"));

    }
}