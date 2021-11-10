#include "CContactListener.h"

CContactListener::CContactListener()
{
	m_Contacts = {};
}

CContactListener::~CContactListener()
{
}

void CContactListener::BeginContact(b2Contact* contact)
{
	MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
	m_Contacts.push_back(myContact);
}

void CContactListener::EndContact(b2Contact* contact)
{
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    std::vector<MyContact>::iterator pos;
    pos = std::find(m_Contacts.begin(), m_Contacts.end(), myContact);
    if (pos != m_Contacts.end())
    {
        pos = m_Contacts.erase(pos);
    }
}

void CContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void CContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}
