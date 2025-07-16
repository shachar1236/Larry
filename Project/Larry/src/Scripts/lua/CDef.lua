
return [[
typedef struct Vec1 {
    float x;
} Vec1;
typedef struct Vec2 {
    float x, y;
} Vec2;
typedef struct Vec3 {
    float x, y, z;
} Vec3;
typedef struct Vec4 {
    float x, y, z, w;
} Vec4;



    typedef unsigned long ECS_TypeHashCode;
    typedef long int64_t;

    typedef struct {
        void* value;
        ECS_TypeHashCode type;
    } ECS_Any;

    typedef void* ECS_World;
    typedef int64_t ECS_Entity;
    typedef void* ECS_AnyQueue;
    typedef void* ECS_TypeQueue;

    void ECS_Init();

    void* ECS_CreateWorld();

    void ECS_RegisterType(ECS_World, ECS_TypeHashCode type, int type_size, void(*destructor)(const void*)); // register the type if it dosent already exists

    ECS_Entity ECS_CreateEntity(ECS_World, char* name);
    bool ECS_IsEntityAlive(ECS_World, ECS_Entity);
    void ECS_KillEntity(ECS_World, ECS_Entity);

    // its the user responsability to free this memory
    char* GetEntityName(ECS_World world_, ECS_Entity);

    // gives you a queue to work with
    ECS_AnyQueue ECS_InitAnyQueue(ECS_World);
    // pushes a value to queue
    void ECS_PushToAnyQueue(ECS_AnyQueue, ECS_Any);
    // pops a value to queue
    ECS_Any ECS_PopFromAnyQueue(ECS_AnyQueue queue);
    // when you are done using the queue call this function and the world will take ownership of the state
    void ECS_DoneWithAnyQueue(ECS_World, ECS_AnyQueue queue);

    // gives you a state to work with
    ECS_TypeQueue ECS_InitTypeQueue(ECS_World);
    // pushes a value to state
    void ECS_PushToTypeQueue(ECS_TypeQueue, ECS_TypeHashCode);
    // when you are done using the state call this function and the world will take ownership of the state
    void ECS_DoneWithTypeQueue(ECS_World, ECS_TypeQueue queue);

    // insert the new types and return a pointer to all of then in the resultQueue
    // returns if succesfuly inserted
    bool ECS_InsertComponents(ECS_World, ECS_Entity, ECS_TypeQueue types, ECS_AnyQueue resultQueue);
    void ECS_SetComponents(ECS_World, ECS_Entity, ECS_TypeQueue types, ECS_AnyQueue resultQueue);
    ECS_Any ECS_GetComponent(ECS_World, ECS_Entity, ECS_TypeHashCode);
    void ECS_DeleteComponent(ECS_World, ECS_Entity, ECS_TypeHashCode);

    void* ECS_CreateSingelton(ECS_World, ECS_TypeHashCode); // returns a pointer to the singelton
    void* ECS_GetSingelton(ECS_World, ECS_TypeHashCode);

    void ECS_System(ECS_World, ECS_TypeQueue components_types, ECS_AnyQueue system_components_queue, void(*SystemFunc)(ECS_Entity, ECS_AnyQueue components, bool* stop));



enum TextureWrappingOptions {
        REPEAT = 0x2901,
        MIRRORED_REPEAT  = 0x8370,
        CLAMP_TO_EDGE = 0x812F,
        CLAMP_TO_BORDER = 0x812D
    };

    enum TextureFilterOptions {
        NEAREST = 0x2600,
        LINEAR = 0x2601
    };

    enum MipmapFilterOptions {
        NEAREST_MIPMAP_NEAREST = 0x2700,
        LINEAR_MIPMAP_NEAREST = 0x2701,
        NEAREST_MIPMAP_LINEAR = 0x2702,
        LINEAR_MIPMAP_LINEAR = 0x2703
    };

     typedef struct TextureConfig {
        bool CreateMipmap ;
        enum TextureWrappingOptions TextureWrappingS ;
        enum TextureWrappingOptions TextureWrappingT ;
        enum TextureFilterOptions TextureFilterMin ;
        enum TextureFilterOptions TextureFilterMag ;
        enum MipmapFilterOptions MipmapFilterMin ;
    } TextureConfig ;



    void* LuaLoadTexture(void* world, const char* path, TextureConfig config);


typedef struct Background {
    Vec4 color;
    void* texture;
} Background;
typedef struct Button {
    Vec2 dimentions;
    Vec4 color;
} Button;
typedef struct Quad {
    Vec3 dimentions;
    Vec4 color;
    void* texture;
} Quad;

typedef struct Transform {
    Vec3 translation;
    Vec3 scale;
    Vec3 rotation_axis;
    float rotation_size;
    Vec3 _realTranslation; // the current translation plus parent realTranslation
    int _realTranslationTimestemp;
} Transform;


]]