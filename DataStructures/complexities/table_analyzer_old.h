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
        std::vector<int> inserted_keys;
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
            this->keyList_.push_back(i);
		}
        ComplexityAnalyzer<TabType>::registerBeforeOperation([this](TabType& list)
            {
        		std::uniform_int_distribution<size_t> indexDist(0, list.size() - 1);
        		index_ = indexDist(rngIndex_);
        		data_ = rngData_();
            });
    }

    template<typename TabType>
    void TabAnalyzer<TabType>::growToSize(TabType& structure, size_t size) {
        const size_t toInsert = size - structure.size();
        for (size_t i = 0; i < toInsert; ++i)
        {
            int key = this->keyList_.back();
			this->keyList_.pop_back();
            structure.insert(key, rngData_());
            this->inserted_keys.push_back(key);
        }
		//std::cout << "Grow to size List size: " << this->keyList_.size() << "\n";
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
        std::uniform_int_distribution<> distrib(0, this->inserted_keys.size()- 1);
        int randomIndex = distrib(gen);
        int valueFromList = this->inserted_keys.back();
        this->inserted_keys.pop_back();
        //std::cout << "-----------------------------\n";
        //std::cout << "Total List size: " << this->keyList_.size() << "\nInser List size: " << this->inserted_keys.size() << "\nRand inde: " << randomIndex << "\nValue from list: " << valueFromList << std::endl;
        //std::cout << "-----------------------------\n";
    	structure.remove(valueFromList);
		//this->inserted_keys.erase(this->inserted_keys.begin() + randomIndex);
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