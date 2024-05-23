# pragma once

#include <complexities/complexity_analyzer.h>
#include <libds/adt/queue.h>
#include <random>


namespace ds::utils {
    /**
     * @brief Common base for queue analyzers.
     */
    template<class Queue>
    class QueueAnalyzer : public ComplexityAnalyzer<Queue> {
    protected:
        explicit QueueAnalyzer(const std::string &name);

    protected:
        void growToSize(Queue &structure, size_t size) override;

        size_t getRandomIndex() const;

        int getRandomData() const;

    private:
        std::default_random_engine rngData_;
        std::default_random_engine rngIndex_;
        size_t index_;
        int data_;
    };

    /**
     * @brief Analyzes complexity of an insertion at the beginning.
     */
    template<class Queue>
    class QueueInsertAnalyzer : public QueueAnalyzer<Queue> {
    public:
        explicit QueueInsertAnalyzer(const std::string &name);

    protected:
        void executeOperation(Queue &structure) override;
    };

    /**
     * @brief Analyzes complexity of an erasure at the beginning.
     */
    template<class Queue>
    class QueueRemoveAnalyzer : public QueueAnalyzer<Queue> {
    public:
        explicit QueueRemoveAnalyzer(const std::string &name);

    protected:
        void executeOperation(Queue &structure) override;
    };

    /**
     * @brief Container for all queue analyzers.
     */
    class QueuesAnalyzer : public CompositeAnalyzer {
    public:
        QueuesAnalyzer();
    };

    //-------------

    template<class Queue>
    QueueAnalyzer<Queue>::QueueAnalyzer(const std::string &name)
            : ComplexityAnalyzer<Queue>(name),
              rngData_(144),
              rngIndex_(144),
              index_(0),
              data_(0) {
        ComplexityAnalyzer<Queue>::registerBeforeOperation([this](Queue &queue) {
            std::uniform_int_distribution<size_t> indexDist(0, queue.size() - 1);
            index_ = indexDist(rngIndex_);
            data_ = getRandomData();
            // nejaka funkcia na velkost napr queue.adjust
        });
    }

    template<class Queue>
    void QueueAnalyzer<Queue>::growToSize(Queue &structure, size_t size) {
        const size_t toInsert = size - structure.size();
        for (size_t i = 0; i < toInsert; ++i) {
            structure.push(rngData_(), rngData_());
        }
    }

    template<class Queue>
    size_t QueueAnalyzer<Queue>::getRandomIndex() const {
        return index_;
    }

    template<class Queue>
    int QueueAnalyzer<Queue>::getRandomData() const {
        return data_;
    }

    //-------------

    template<class Queue>
    QueueInsertAnalyzer<Queue>::QueueInsertAnalyzer(const std::string &name)
            : QueueAnalyzer<Queue>(name) {
    }

    template<class Queue>
    void QueueInsertAnalyzer<Queue>::executeOperation(Queue &structure) {
        auto data = this->getRandomData();
        structure.push(data, data);
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
        //this->addAnalyzer(std::make_unique<TwoListAnalyzer>());
    }
}