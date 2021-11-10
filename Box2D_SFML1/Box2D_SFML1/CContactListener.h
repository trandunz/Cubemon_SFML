#pragma once
#include "NumptyBehavior.h"

struct MyContact 
{
    b2Fixture* fixtureA;
    b2Fixture* fixtureB;
    bool operator==(const MyContact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};

class CContactListener : NumptyBehavior, public b2ContactListener
{
public:
    CContactListener();
    virtual ~CContactListener();

    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

    std::vector<MyContact> m_Contacts;
};

