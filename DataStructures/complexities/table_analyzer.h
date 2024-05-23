#pragma once

#include <complexities/complexity_analyzer.h>
#include <libds/adt/table.h>
#include <random>
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

    private:
        std::default_random_engine rngData_;
        std::default_random_engine rngKey_;

    };

    /**
     * @brief Analyzes complexity of an insertion at the beginning.
     */
    template<class Table>
    class TableInsertAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };

    /**
     * @brief Analyzes complexity of an erasure at the beginning.
     */
     template<class Table>
     class TableRemoveAnalyzer : public TableAnalyzer<Table>
     {
     public:
         explicit TableRemoveAnalyzer(const std::string& name);

     protected:
         void executeOperation(Table& structure) override;
     };

     /**
      * @brief Container for all table analyzers.
      */
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
            ComplexityAnalyzer<Table>::registerBeforeOperation([this](Table& table)
            {
                //std::cout << "Before operation" << std::endl;
                std::uniform_int_distribution<size_t> dist(0, (table.size() - 1) * 10);
                this->key_ = dist(this->rngKey_);
                while (table.contains(key_))
                {
                    this->key_ = dist(this->rngKey_);
                }
                this->data_ = this->rngData_();
            });
      }

      template<class Table>
      void TableAnalyzer<Table>::growToSize(Table& structure, size_t size)
      {
          //std::cout << "Growing to size: " << size << std::endl;
          size_t count = size - structure.size();
          for (size_t i = 0; i < count; ++i) {
              std::uniform_int_distribution<size_t> dist(0, (structure.size() - 1) * 10);
              int key = dist(this->rngData_);
              int data = this->rngData_();

              while (structure.contains(key))
              {
                  key = dist(this->rngData_);
              }
                keys_.push_back(key);
              structure.insert(key, data);
          }
      }

      //-----------

      template<class Table>
      TableInsertAnalyzer<Table>::TableInsertAnalyzer(const std::string& name) :
        TableAnalyzer<Table>(name)
      {
      }

      template<class Table>
      void TableInsertAnalyzer<Table>::executeOperation(Table& structure)
      {
         structure.insert(this->key_, this->data_);
         std::cout << "Inserting key: " << this->key_ << " data: " << this->data_ << std::endl;
      }

      //-----------

      template<class Table>
      TableRemoveAnalyzer<Table>::TableRemoveAnalyzer(const std::string& name) :
        TableAnalyzer<Table>(name)
      {
      }

      template<class Table>
      void TableRemoveAnalyzer<Table>::executeOperation(Table& structure)
      {
       // auto iterator = structure.begin();
       // auto key = (*iterator).key_;
        structure.remove((*structure.begin()).key_);
std::cout << "Removing key" << std::endl;
          //structure.remove(this->keys_.back());
           // std::cout << "Removing key: " << this->keys_.back() << std::endl;
            //this->keys_.pop_back();
      }

      //-----------

      inline TablesAnalyzer::TablesAnalyzer() :
        CompositeAnalyzer("Tables")
      {
          addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::SortedSequenceTable<int, int>>>("SortedSequenceTable-insert"));
          addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::SortedSequenceTable<int, int>>>("SortedSequenceTable-remove"));
          addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::BinarySearchTree<int, int>>>("BinarySearchTree-insert"));
          addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::BinarySearchTree<int, int>>>("BinarySearchTree-remove"));
          addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::Treap<int, int>>>("Treap-insert"));
          addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::Treap<int, int>>>("Treap-remove"));
      }
}