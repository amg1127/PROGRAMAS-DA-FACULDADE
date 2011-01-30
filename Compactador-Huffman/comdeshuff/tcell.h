class TCell {
    private:
        int _id;
        int _weight;
        char _character;
        TCell *_parent;
        TCell *_son0;
        TCell *_son1;
        bool _hasSons;
        std::string _pathToSon (TCell *);
        template <typename SWPTYPE>
            inline void swapvar (SWPTYPE &v1, SWPTYPE &v2) { SWPTYPE aux; aux=v1; v1=v2; v2=aux; }
    public:
        TCell (TCell * = NULL);
        ~TCell ();
        int id (void);
        void setId (int);
        char character (void);
        void setCharacter (char);
        int weight ();
        void setWeight (int);
        int distanceToRoot (void);
        int maxDistanceToSons (void);
        bool isRoot (void);
        bool hasSons (void);
        void killSons (void);
        void makeSons (void);
        TCell *son0 (void);
        TCell *son1 (void);
        TCell *son (int);
        TCell *parent (void);
        TCell *root (void);
        TCell *findSonByWeight (int);
        TCell *findSonById (int);
        TCell *findSonByCharacter (char);
        TCell *findSonByPath (std::string);
        TCell *self (void);
        std::string pathFromRootToMe (void);
        std::string pathToSon (TCell &);
        std::string pathToSon (TCell *);
        bool isSonOf (TCell &);
        bool isSonOf (TCell *);
        bool isParentOf (TCell &);
        bool isParentOf (TCell *);
        void swap (TCell &, bool = false);
        void swap (TCell *, bool = false);
        void copyFrom (TCell &, bool = false);
        void copyFrom (TCell *, bool = false);
        void copyTo (TCell &, bool = false);
        void copyTo (TCell *, bool = false);
        std::string dump (int = 0);
};
