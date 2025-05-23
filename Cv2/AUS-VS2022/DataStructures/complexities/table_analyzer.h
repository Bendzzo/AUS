#pragma once
#include "complexities/complexity_analyzer.h"
#include "libds/adt/table.h"

namespace ds::utils
{

    template<class TableType>
    class TableAnalyzer : public ComplexityAnalyzer<TableType>
    {
    protected:
        explicit TableAnalyzer(const std::string& name);

    protected:
        void growToSize(TableType& structure, size_t size) override;
    private:
        std::default_random_engine rngData_;
        std::default_random_engine rngKey_;
        
    protected:
        int key;
        int data;
        std::vector<int> insertedKeys_;
    };


    template<class TableType>
    class TableInsertAnalyzer : public TableAnalyzer<TableType>
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(TableType& structure) override;
        
    };

    //////////////
    template<class TableType>
    class TableFindAnalyzer : public TableAnalyzer<TableType>
    {
    public:
        explicit TableFindAnalyzer(const std::string& name);

    protected:
        void executeOperation(TableType& structure) override;
        std::default_random_engine rngForFind_;
    };


    class TablesAnalyzer : public CompositeAnalyzer
    {
    public:
        TablesAnalyzer();
    };

    //------------------------
    template <class TableType>
    TableAnalyzer<TableType>::TableAnalyzer(const std::string& name)
        : ComplexityAnalyzer<TableType>(name), rngData_(144), rngKey_(144), key(0), data(0)
    {
        this->registerBeforeOperation([&](TableType& structure)
            {
                std::uniform_int_distribution<int> keyDist(1, 1000000);
                std::uniform_int_distribution<int> dataDist(1, 1000);

                key = keyDist(rngKey_);
                data = dataDist(rngData_);
            });
    }

    template <class TableType>
    void TableAnalyzer<TableType>::growToSize(TableType& structure, size_t size)
    {
        insertedKeys_.clear();

        while (structure.size() < size)
        {
            std::uniform_int_distribution<int> keyDist(1, 1000000);
            std::uniform_int_distribution<int> dataDist(1, 1000);

            int key = keyDist(rngKey_);
            int data = dataDist(rngData_);

            if (!structure.contains(key))
            {
                structure.insert(key, data);
                insertedKeys_.push_back(key);
            }
        }
    }

    template <class TableType>
    TableInsertAnalyzer<TableType>::TableInsertAnalyzer(const std::string& name) :
        TableAnalyzer<TableType>(name)
    {
        
    }

    template <class TableType>
    void TableInsertAnalyzer<TableType>::executeOperation(TableType& structure)
    {
        structure.insert(this->key, this->data);
    }

    template <class TableType>
    TableFindAnalyzer<TableType>::TableFindAnalyzer(const std::string& name):
        TableAnalyzer<TableType>(name),
        rngForFind_(144)
    {
        this->registerBeforeOperation([&](TableType& structure)
            {
                this->insertedKeys_.clear();

                std::uniform_int_distribution<int> keyDist(1, 1000000);
                int randomKey = keyDist(this->rngForFind_);
                this->insertedKeys_.push_back(randomKey);
            });
    }

    template <class TableType>
    void TableFindAnalyzer<TableType>::executeOperation(TableType& structure)
    {
        if (!this->insertedKeys_.empty())
        {
            std::uniform_int_distribution<size_t> indexDist(0, this->insertedKeys_.size() - 1);
            size_t randomIndex = indexDist(rngForFind_);
            int keyToFind = this->insertedKeys_[randomIndex];

            try
            {
                structure.find(keyToFind);
            }
            catch (...)
            {
            }
        }
        else
        {
            std::uniform_int_distribution<int> keyDist(1, 1000000);
            int randomKey = keyDist(rngForFind_);

            try
            {
                structure.find(randomKey);
            }
            catch (...)
            {
            }
        }
    }


    //----------

    inline TablesAnalyzer::TablesAnalyzer() :
        CompositeAnalyzer("Tables")
    {
        this->addAnalyzer(std::make_unique<TableInsertAnalyzer<adt::HashTable<int, int>>>("Hash-table-insert"));
        this->addAnalyzer(std::make_unique<TableFindAnalyzer<adt::HashTable<int, int>>>("Hash-table-remove"));
    }
}
