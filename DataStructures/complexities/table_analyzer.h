#pragma once

#include <complexities//complexity_analyzer.h>
#include <libds/adt/table.h>
#include <libds/adt/list.h>
#include <limits>
//#include <complexities/list_analyzer.h>

namespace ds::utils
{
    /**
	* @brief Common base for list analyzers.
	*/
    template<class TabType>
    class TabAnalyzer : public ComplexityAnalyzer<TabType>
	{
    protected:
        explicit TabAnalyzer(const std::string &name);

    protected:
        void growToSize(TabType &structure, size_t size) override;

        size_t getRandomIndex() const;
        int getRandomData() const;

    protected:
        std::default_random_engine rngData_;
        std::default_random_engine rngIndex_;
        size_t index_;
        int data_;
		std::vector<int> keyList_;
    };
    /**
	* @brief Analyzes complexity of an insertion at the beginning.
	*/
    template<class TabType>
    class TabInsertAnalyzer : public TabAnalyzer<TabType>
	{
    public:
        explicit TabInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(TabType& structure) override;
    };

    /**
	* @brief Analyzes complexity of an erasure at the beginning.
	*/
    template<class TabType>
    class TabRemoveAnalyzer : public TabAnalyzer<TabType>
	{
    public:
        explicit TabRemoveAnalyzer(const std::string &name);

    protected:
        void executeOperation(TabType &structure) override;
    };

    /**
	* @brief Container for all list analyzers.
	*/
    class TabsAnalyzer : public CompositeAnalyzer
    {
    public:
        TabsAnalyzer();
    };

	//----------

    template<class TabType>
    TabAnalyzer<TabType>::TabAnalyzer(const std::string& name) :
            ComplexityAnalyzer<TabType>(name),
            rngData_(144),
            rngIndex_(144),
            index_(0),
            data_(0)
    {
        this->keyList_.reserve(11000000);
		for (int i = 1; i < 11000000; i++)
		{
			//std::cout << "Inserting " << i << "\n";
            this->keyList_.push_back(i);
		}
        ComplexityAnalyzer<TabType>::registerBeforeOperation([this](TabType& list)
            {
        		std::uniform_int_distribution<size_t> indexDist(0, list.size() - 1);
        		index_ = indexDist(rngIndex_);
        		data_ = rngData_();
            });
		//std::cout << "List size: " << this->keyList_.size() << "\n";
    }

    template<typename TabType>
    void TabAnalyzer<TabType>::growToSize(TabType& structure, size_t size) {
        //for (int i = 1; i <= size; i++) {
        //    this->keyList_.push_back(i);
        //}
        //std::shuffle(this->keyList_.begin(), this->keyList_.end(), std::mt19937(std::random_device()()));
        const size_t toInsert = size - structure.size();
        for (size_t i = 0; i < toInsert; ++i)
        {
            //key = rngData_();
            int key = this->keyList_.back();
			this->keyList_.pop_back();
            structure.insert(key, rngData_());
            //keyList_.push_back(key);
            //std::cout << "Key: " << key << " Value: " << value << "\n";
        }
        //this->keyList_.clear();
        //for (int i = 1; i <= size; i++) {
         //   this->keyList_.push_back(i*structure.size());
        //}
		std::cout << "Grow to size List size: " << this->keyList_.size() << "\n";
    }

    template<class List>
    size_t TabAnalyzer<List>::getRandomIndex() const
    {
        return index_;
    }

    template<class List>
    int TabAnalyzer<List>::getRandomData() const
    {
        return data_;
    }

    template <class List>
    TabInsertAnalyzer<List>::TabInsertAnalyzer(const std::string& name) :
            TabAnalyzer<List>(name)
    {
    }

    template <class List>
    void TabInsertAnalyzer<List>::executeOperation(List& structure)
    {
        auto data = this->getRandomData();
		auto key = this->keyList_.back();
		//std::cout << "Key: " << key << " Value: " << data << "\n" << "List size: " << this->keyList_.size() << "\nStructure size: " << structure.size() << "\n";
        structure.insert(key, data);
        this->keyList_.pop_back();
    }

    template <class List>
    TabRemoveAnalyzer<List>::TabRemoveAnalyzer(const std::string& name) :
            TabAnalyzer<List>(name)
    {
    }

    template <class List>
    void TabRemoveAnalyzer<List>::executeOperation(List& structure)
    {
        if (structure.isEmpty()) {
            return;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, this->keyList_.size()- 1);
        int randomIndex = distrib(gen);
        std::cout << "List size: " << this->keyList_.size() << "\nRand inde: " << randomIndex << std::endl;
        int valueFromList = this->keyList_[randomIndex];
//        if (structure.contains(valueFromList)) {
//            structure.remove(valueFromList);
//        } else {
//            std::cout << "Neni v tabulce\n";
//        }
        structure.remove(valueFromList);
        //this->keyList_.remove(randomIndex);
		this->keyList_.erase(this->keyList_.begin() + randomIndex);
        std::cout << "List size: " << this->keyList_.size() << "\n";
    }

    //----------

    inline TabsAnalyzer::TabsAnalyzer() :
            CompositeAnalyzer("Tabs")
    {
        this->addAnalyzer(std::make_unique<TabInsertAnalyzer<ds::adt::SortedSTab<int, int>>>("tab-insert"));
        this->addAnalyzer(std::make_unique<TabRemoveAnalyzer<ds::adt::SortedSequenceTable<int, int>>>("tab-remove"));
    }

}




// Uprava
/*
namespace ds::utils {
    template<typename TabType>
    class TabAnalyzer : public ComplexityAnalyzer<TabType> {
    protected:
        explicit TabAnalyzer(const std::string& name);

    protected:
        void growToSize(TabType& structure, size_t size) override;
    };

    template<typename TabType>
    class SortedTabInsertAnalyzer : public TabAnalyzer<TabType> {
    protected:
        explicit SortedTabInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(TabType& structure) override;
    };

    template<typename TabType>
    class SortedTabRemoveAnalyzer : public TabAnalyzer<TabType> {
    protected:
        explicit SortedTabRemoveAnalyzer(const std::string& name);

    protected:
        void executeOperation(TabType& structure) override;
    };
}
 */





// Z hodiny
/*
namespace ds::utils {
    template<typename TabType>
    class TabAnalyzer : public ComplexityAnalyzer<TabType> {
    protected:
        explicit ListAnalyzer(const std::string& name);
    protected:
        void growToSize(TabType& structure, size_t size) override;
};
    template<typename TabType>
    class SortedTabInsertAnalyzer : public TabAnalyzer<TabType>{
    protected:
        explicit ListAnalyzer(const std::string& name);
    protected:
        void executeOperation(TabType& structure) override;
    };
    template<typename TabType>
class SortedTabRemoveAnalyzer : public TabAnalyzer<TabType>
        protected:
            explicit ListAnalyzer(const std::string& name);
        protected:
            void executeOperation(TabType& structure) override;
};


// ----
class BVSTabAnalyzer : public ComplexityAnalyzer<BVS<int, int>> {
protected:
explicit ListAnalyzer(const std::string& name);
protected:
void growToSize(SortedSequenceTable<int, int>& structure, size_t size) override;
};
class BVSTabInsertAnalyzer : public SortedTabAnalyzer {
protected:
    explicit ListAnalyzer(const std::string& name);
protected:
    void executeOperation(BVS<int, int>& structure) override;
};
class BVSTabRemoveAnalyzer : public SortedTabAnalyzer {
protected:
    explicit ListAnalyzer(const std::string& name);
protected:
    void executeOperation(BVS<int, int>& structure) override;
};
}*/