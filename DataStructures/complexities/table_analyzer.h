#pragma once

#include <complexities/complexity_analyzer.h>
#include <libds/adt/table.h>
#include <random>
#include <limits>
#include <vector>

namespace ds::utils
{
    /**
     * @brief Common base for table analyzers.
     */
    template<class Table>
    class TableAnalyzer : public ComplexityAnalyzer<Table>
    {
    protected:
        explicit TableAnalyzer(const std::string& name);

    protected:
        void growToSize(Table& structure, size_t size) override;
        int key_;
        int data_;
        std::vector<int> keys_;

    protected:
        std::default_random_engine rngData_;
        std::default_random_engine rngKey_;

    };

    template<class Table>
    class TableInsertAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };

     template<class Table>
     class TableRemoveAnalyzer : public TableAnalyzer<Table>
     {
     public:
         explicit TableRemoveAnalyzer(const std::string& name);

     protected:
         void executeOperation(Table& structure) override;
     };

    template<class Table>
    class TableFindAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableFindAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };

      class TablesAnalyzer : public CompositeAnalyzer
      {
      public:
          TablesAnalyzer();
      };

      //-----------

      template<class Table>
      TableAnalyzer<Table>::TableAnalyzer(const std::string& name) :
        ComplexityAnalyzer<Table>(name),
        rngData_(144),
        rngKey_(144),
        key_(0),
        data_(0)
      {
            ComplexityAnalyzer<Table>::registerAfterReplication([this](Table& table)
            {
                this->keys_.clear();
            });
      }

      template<class Table>
      void TableAnalyzer<Table>::growToSize(Table& structure, size_t size)
      {
          size_t count = size - structure.size();
          for (size_t i = 0; i < count; ++i) {
              std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
              int key = dist(this->rngData_);
              int data = this->rngData_();
              while (structure.contains(key))
              {
                  key = dist(this->rngData_);
              }
              this->keys_.push_back(key);
              structure.insert(key, data);
          }

      }

      //-----------

      template<class Table>
      TableInsertAnalyzer<Table>::TableInsertAnalyzer(const std::string& name) :
        TableAnalyzer<Table>(name)
      {
          ComplexityAnalyzer<Table>::registerBeforeOperation([this](Table& table)
            {
                std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
                this->key_ = dist(this->rngKey_);
                while (table.contains(this->key_))
                {
                    this->key_ = dist(this->rngKey_);
                }
                this->data_ = this->rngData_();
            });
      }

      template<class Table>
      void TableInsertAnalyzer<Table>::executeOperation(Table& structure)
      {
         structure.insert(this->key_, this->data_);
      }

      //-----------

      template<class Table>
      TableRemoveAnalyzer<Table>::TableRemoveAnalyzer(const std::string& name) :
        TableAnalyzer<Table>(name)
      {
          ComplexityAnalyzer<Table>::registerBeforeOperation([this](Table& table)
          {
              std::uniform_int_distribution<size_t> dist(0, (this->keys_.size() - 1));
              size_t index = dist(this->rngKey_);
              this->key_ = this->keys_[index];
              std::swap(this->keys_[index], this->keys_.back());
              this->keys_.pop_back();
          });
      }

      template<class Table>
      void TableRemoveAnalyzer<Table>::executeOperation(Table& structure)
      {
            structure.remove(this->key_);
      }

    template<class Table>
    TableFindAnalyzer<Table>::TableFindAnalyzer(const std::string& name) :
            TableAnalyzer<Table>(name)
    {
        ComplexityAnalyzer<Table>::registerBeforeOperation([this](Table& table)
        {
            std::uniform_int_distribution<size_t> dist(0, (this->keys_.size() - 1));
            size_t index = dist(this->rngKey_);
            this->key_ = this->keys_.at(index);
            });
    }

    template<class Table>
    void TableFindAnalyzer<Table>::executeOperation(Table& structure)
    {
        structure.find(this->key_);
    }

      //-----------

      inline TablesAnalyzer::TablesAnalyzer() :
        CompositeAnalyzer("Tables")
      {
          addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::SortedSequenceTable<int, int>>>("SortedSequenceTable-insert"));
          addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::SortedSequenceTable<int, int>>>("SortedSequenceTable-remove"));
          addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::SortedSequenceTable<int, int>>>("SortedSequenceTable-find"));
          addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::Treap<int, int>>>("Treap-insert"));
          addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::Treap<int, int>>>("Treap-remove"));
          addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::Treap<int, int>>>("Treap-find"));
          addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::BinarySearchTree<int, int>>>("BinarySearchTree-insert"));
          addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::BinarySearchTree<int, int>>>("BinarySearchTree-remove"));
          addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::BinarySearchTree<int, int>>>("BinarySearchTree-find"));
          addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::UnsortedExplicitSequenceTable<int,int>>>("UnsortedExplicitSequenceTable-insert"));
          addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::UnsortedExplicitSequenceTable<int,int>>>("UnsortedExplicitSequenceTable-remove"));
          addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::UnsortedExplicitSequenceTable<int,int>>>("UnsortedExplicitSequenceTable-find"));
          addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::UnsortedImplicitSequenceTable<int, int>>>("UnsortedImplicitSequenceTable-insert"));
          addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::UnsortedImplicitSequenceTable<int, int>>>("UnsortedImplicitSequenceTable-remove"));
          addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::UnsortedImplicitSequenceTable<int, int>>>("UnsortedImplicitSequenceTable-find"));
      }
}