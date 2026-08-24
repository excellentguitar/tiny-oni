// ============================================================

class World
{
public:

    std::vector<Tile> cells;
    std::vector<int> surface;

    World();

    Tile& at(int x, int y);
    const Tile& at(int x, int y) const;

    bool inside(int x, int y) const;

    bool solidType(Type type) const;
    bool solid(int x, int y) const;
    bool open(int x, int y) const;

    void generate();

    void dig(int x, int y);
    void makeWater(int x, int y);
    void makeGas(int x, int y);

    void step();

    float totalWater() const;

private:

    float noise1D(float x, float scale) const;
    float noise2D(int x, int y, float scale) const;

    void addRockMass(
        int cx,
        int cy,
        int rx,
        int ry,
        Type rock
    );

    void addCopperVein(
        int cx,
        int cy,
        int length
    );

    void addOreCluster(
        Type ore,
        int cx,
        int cy,
        int size
    );

    void addCoalSeam(
        int cx,
        int cy
    );

    void generateCaves();
    void generateMagma();
    void fillSurfaceOxygen();
    void generateTrees();

    void moveWater(
        int x1,
        int y1,
        int x2,
        int y2,
        float rate
    );

    void stepWater();

    void diffuseGas(
        int x1,
        int y1,
        int x2,
        int y2
    );

    void stepGas();

    void cleanFluids();

    enum class TreeKind
    {
        Oak,
        Pine,
        Birch,
        Redwood,
        Acacia,
        Purpleheart,
        Ebony
    };

    TreeKind randomTree();

    void makeTree(
        int baseX,
        int baseY,
        int height,
        TreeKind kind
    );

    void leaf(
        int x,
        int y,
        TreeKind kind
    );

    void makeOakCrown(
        int cx,
        int cy,
        int r
    );

    void makePineCrown(
        int cx,
        int cy,
        int height
    );

    void makeBirchCrown(
        int cx,
        int cy
    );

    void makeRedwoodCrown(
        int cx,
        int cy
    );

    void makeAcaciaCrown(
        int cx,
        int cy
    );

    void makePurpleheartCrown(
        int cx,
        int cy
    );

    void makeEbonyCrown(
        int cx,
        int cy
    );

    void makeLeafBlob(
        int cx,
        int cy,
        int rx,
        int ry,
        TreeKind kind
    );
};


// ============================================================
// END FILE: core/World.h
// ============================================================
