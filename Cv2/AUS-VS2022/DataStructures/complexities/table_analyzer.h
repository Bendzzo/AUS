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
        int key_;
        int data_;
        std::vector<int> insertedKeys_;
    };

    /////////////
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

    /////////////////
    
    template <class TableType>
    TableAnalyzer<TableType>::TableAnalyzer(const std::string& name)
        : ComplexityAnalyzer<TableType>(name), rngData_(144), rngKey_(144), key_(0), data_(0)
    {
        this->registerBeforeOperation([&](TableType& structure)
            {
                std::uniform_int_distribution<int> keyDist(1, 1000000);
                std::uniform_int_distribution<int> dataDist(1, 1000);

                key_ = keyDist(rngKey_);
                data_ = dataDist(rngData_);
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

            try
            {
                structure.insert(key, data);
                insertedKeys_.push_back(key);
            }
            catch (const std::logic_error&)
            {
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
        try
        {
            structure.insert(this->key_, this->data_);
        }
        catch (const std::logic_error&)
        {
        }
    }

    template <class TableType>
    TableFindAnalyzer<TableType>::TableFindAnalyzer(const std::string& name):
        TableAnalyzer<TableType>(name),
        rngForFind_(144)
    {
        this->registerBeforeOperation([&](TableType& structure)
            {
                std::uniform_int_distribution<int> choiceDist(1, 4);

                if (choiceDist(rngForFind_) < 4 && !this->insertedKeys_.empty())
                {
                    std::uniform_int_distribution<size_t> indexDist(0, this->insertedKeys_.size() - 1);
                    size_t randomIndex = indexDist(rngForFind_);
                    this->key_ = this->insertedKeys_[randomIndex];
                }
                else
                {
                    std::uniform_int_distribution<int> keyDist(1, 1000000);
                    this->key_ = keyDist(rngForFind_);
                }
            });
    }

    template <class TableType>
    void TableFindAnalyzer<TableType>::executeOperation(TableType& structure)
    {
        try
        {
            structure.find(this->key_);
        }
    	catch (...)
        {
	        
        }
    }


    //----------

    inline TablesAnalyzer::TablesAnalyzer() :
        CompositeAnalyzer("Tables")
    {
        this->addAnalyzer(std::make_unique<TableInsertAnalyzer<adt::HashTable<int, int>>>("Hash-table-insert"));
        this->addAnalyzer(std::make_unique<TableFindAnalyzer<adt::HashTable<int, int>>>("Hash-table-find"));
    }
}
