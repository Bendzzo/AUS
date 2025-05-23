//#pragma once
//
//#include <complexities/complexity_analyzer.h>
//#include <libds/adt/table.h>
//#include <random>
//#include <vector>
//#include <algorithm>
//
//namespace ds::utils
//{
//    template<class TableType>
//    class HashTableAnalyzer : public ComplexityAnalyzer<TableType>
//    {
//    protected:
//        explicit HashTableAnalyzer(const std::string& name);
//
//    protected:
//        void growToSize(TableType& structure, size_t size) override;
//
//        int getRandomKey() const;
//        int getRandomData() const;
//
//    private:
//        std::default_random_engine rngData_;
//        std::default_random_engine rngKey_;
//        std::vector<int> availableKeys_;
//        mutable size_t keyIndex_;
//        int key_;
//        int data_;
//    };
//
//    template<class TableType>
//    class HashTableInsertAnalyzer : public HashTableAnalyzer<TableType>
//    {
//    public:
//        explicit HashTableInsertAnalyzer(const std::string& name);
//
//    protected:
//        void executeOperation(TableType& structure) override;
//    };
//
//    template<class TableType>
//    class HashTableFindAnalyzer : public HashTableAnalyzer<TableType>
//    {
//    public:
//        explicit HashTableFindAnalyzer(const std::string& name);
//
//    protected:
//        void executeOperation(TableType& structure) override;
//    };
//
//    class HashTablesAnalyzer : public CompositeAnalyzer
//    {
//    public:
//        HashTablesAnalyzer();
//    };
//
//    //----------
//
//    template<class TableType>
//    HashTableAnalyzer<TableType>::HashTableAnalyzer(const std::string& name) :
//        ComplexityAnalyzer<TableType>(name),
//        rngData_(144),
//        rngKey_(144),
//        keyIndex_(0),
//        key_(0),
//        data_(0)
//    {
//        for (int i = 0; i < 1000000; ++i)
//        {
//            availableKeys_.push_back(i);
//        }
//        std::shuffle(availableKeys_.begin(), availableKeys_.end(), rngKey_);
//
//        ComplexityAnalyzer<TableType>::registerBeforeOperation([this](TableType& table)
//            {
//                if (keyIndex_ < availableKeys_.size())
//                {
//                    key_ = availableKeys_[keyIndex_];
//                    keyIndex_++;
//                }
//                else
//                {
//                    key_ = rngKey_();
//                }
//                data_ = rngData_();
//            });
//    }
//
//    template<class TableType>
//    void HashTableAnalyzer<TableType>::growToSize(TableType& structure, size_t size)
//    {
//        const size_t toInsert = size - structure.size();
//        for (size_t i = 0; i < toInsert; ++i)
//        {
//            int key, data;
//            if (keyIndex_ < availableKeys_.size())
//            {
//                key = availableKeys_[keyIndex_];
//                keyIndex_++;
//            }
//            else
//            {
//                do
//                {
//                    key = rngKey_();
//                } while (structure.contains(key));
//            }
//            data = rngData_();
//            structure.insert(key, data);
//        }
//    }
//
//    template<class TableType>
//    int HashTableAnalyzer<TableType>::getRandomKey() const
//    {
//        return key_;
//    }
//
//    template<class TableType>
//    int HashTableAnalyzer<TableType>::getRandomData() const
//    {
//        return data_;
//    }
//
//    //----------
//
//    template<class TableType>
//    HashTableInsertAnalyzer<TableType>::HashTableInsertAnalyzer(const std::string& name) :
//        HashTableAnalyzer<TableType>(name)
//    {
//    }
//
//    template<class TableType>
//    void HashTableInsertAnalyzer<TableType>::executeOperation(TableType& structure)
//    {
//        int key = this->getRandomKey();
//        int data = this->getRandomData();
//        structure.insert(key, data);
//    }
//
//    //----------
//
//    template<class TableType>
//    HashTableFindAnalyzer<TableType>::HashTableFindAnalyzer(const std::string& name) :
//        HashTableAnalyzer<TableType>(name)
//    {
//    }
//
//    template<class TableType>
//    void HashTableFindAnalyzer<TableType>::executeOperation(TableType& structure)
//    {
//        if (!structure.isEmpty())
//        {
//            auto it = structure.begin();
//            std::advance(it, this->getRandomKey() % structure.size());
//            int keyToFind = (*it).key_;
//            structure.find(keyToFind);
//        }
//    }
//
//    //----------
//
//    inline HashTablesAnalyzer::HashTablesAnalyzer() :
//        CompositeAnalyzer("HashTables")
//    {
//        this->addAnalyzer(std::make_unique<HashTableInsertAnalyzer<ds::adt::HashTable<int, int>>>("hashtable-insert"));
//        this->addAnalyzer(std::make_unique<HashTableFindAnalyzer<ds::adt::HashTable<int, int>>>("hashtable-find"));
//    }
//}
