#include "terrain/terrSelection.h"

Selection::Selection() :
        Vector<GridInfo>(__FILE__, __LINE__),
        mName(0),
        mUndoFlags(0),
        mHashListSize(1024)
{
    VECTOR_SET_ASSOCIATION(mHashLists);

    // clear the hash list
    mHashLists.setSize(mHashListSize);
    reset();
}

Selection::~Selection()
{
}

void Selection::reset()
{
    PROFILE_SCOPE( TerrainEditor_Selection_Reset );

    for(U32 i = 0; i < mHashListSize; i++)
        mHashLists[i] = -1;
    clear();
}

bool Selection::validate()
{
    PROFILE_SCOPE( TerrainEditor_Selection_Validate );

    // scan all the hashes and verify that the heads they point to point back to them
    U32 hashesProcessed = 0;
    for(U32 i = 0; i < mHashLists.size(); i++)
    {
        U32 entry = mHashLists[i];
        if(entry == -1)
            continue;

        GridInfo info = (*this)[entry];
        U32 hashIndex = getHashIndex(info.mGridPoint.gridPos);

        if( entry != mHashLists[hashIndex] )
        {
            AssertFatal(false, "Selection hash lists corrupted");
            return false;
        }
        hashesProcessed++;
    }

    // scan all the entries and verify that anything w/ a prev == -1 is correctly in the hash table
    U32 headsProcessed = 0;
    for(U32 i = 0; i < size(); i++)
    {
        GridInfo info = (*this)[i];
        if(info.mPrev != -1)
            continue;

        U32 hashIndex = getHashIndex(info.mGridPoint.gridPos);

        if(mHashLists[hashIndex] != i)
        {
            AssertFatal(false, "Selection list heads corrupted");
            return false;
        }
        headsProcessed++;
    }
    AssertFatal(headsProcessed == hashesProcessed, "Selection's number of hashes and number of list heads differ.");
    return true;
}

U32 Selection::getHashIndex(const Point2I & pos)
{
    PROFILE_SCOPE( TerrainEditor_Selection_GetHashIndex );

    Point2F pnt = Point2F((F32)pos.x, (F32)pos.y) + Point2F(1.3f,3.5f);
    return( (U32)(mFloor(mHashLists.size() * mFmod(pnt.len() * 0.618f, 1.0f))) );
}

S32 Selection::lookup(const Point2I & pos)
{
    PROFILE_SCOPE( TerrainEditor_Selection_Lookup );

    U32 index = getHashIndex(pos);

    S32 entry = mHashLists[index];

    while(entry != -1)
    {
        if((*this)[entry].mGridPoint.gridPos == pos)
            return(entry);

        entry = (*this)[entry].mNext;
    }

    return(-1);
}

void Selection::insert(GridInfo info)
{
    PROFILE_SCOPE( TerrainEditor_Selection_Insert );

    //validate();
    // get the index into the hash table
    U32 index = getHashIndex(info.mGridPoint.gridPos);

    // if there is an existing linked list, make it our next
    info.mNext = mHashLists[index];
    info.mPrev = -1;

    // if there is an existing linked list, make us it's prev
    U32 indexOfNewEntry = size();
    if(info.mNext != -1)
        (*this)[info.mNext].mPrev = indexOfNewEntry;

    // the hash table holds the heads of the linked lists. make us the head of this list.
    mHashLists[index] = indexOfNewEntry;

    // copy us into the vector
    push_back(info);
    //validate();
}

bool Selection::remove(const GridInfo &info)
{
    PROFILE_SCOPE( TerrainEditor_Selection_Remove );

    if(size() < 1)
        return false;

    //AssertFatal( validate(), "Selection hashLists corrupted before Selection.remove()");

    U32 hashIndex = getHashIndex(info.mGridPoint.gridPos);
    S32 listHead = mHashLists[hashIndex];
    //AssertFatal(listHead < size(), "A Selection's hash table is corrupt.");

    if(listHead == -1)
        return(false);

    const S32 victimEntry = lookup(info.mGridPoint.gridPos);
    if( victimEntry == -1 )
        return(false);

    const GridInfo victim = (*this)[victimEntry];
    const S32 vicPrev = victim.mPrev;
    const S32 vicNext = victim.mNext;

    // remove us from the linked list, if there is one.
    if(vicPrev != -1)
        (*this)[vicPrev].mNext = vicNext;
    if(vicNext != -1)
        (*this)[vicNext].mPrev = vicPrev;

    // if we were the head of the list, make our next the new head in the hash table.
    if(vicPrev == -1)
        mHashLists[hashIndex] = vicNext;

    // if we're not the last element in the vector, copy the last element to our position.
    if(victimEntry != size() - 1)
    {
        // copy last into victim, and re-cache next & prev
        const GridInfo lastEntry = last();
        const S32 lastPrev = lastEntry.mPrev;
        const S32 lastNext = lastEntry.mNext;
        (*this)[victimEntry] = lastEntry;

        // update the new element's next and prev, to reestablish it in it's linked list.
        if(lastPrev != -1)
            (*this)[lastPrev].mNext = victimEntry;
        if(lastNext != -1)
            (*this)[lastNext].mPrev = victimEntry;

        // if it was the head of it's list, update the hash table with its new position.
        if(lastPrev == -1)
        {
            const U32 lastHash = getHashIndex(lastEntry.mGridPoint.gridPos);
            AssertFatal(mHashLists[lastHash] == size() - 1, "Selection hashLists corrupted during Selection.remove() (oldmsg)");
            mHashLists[lastHash] = victimEntry;
        }
    }

    // decrement the vector, we're done here
    pop_back();
    //AssertFatal( validate(), "Selection hashLists corrupted after Selection.remove()");
    return true;
}

bool Selection::add(const GridInfo &info)
{
    PROFILE_SCOPE( TerrainEditor_Selection_Add );

    S32 index = lookup(info.mGridPoint.gridPos);
    if(index != -1)
        return(false);

    insert(info);
    return(true);
}

bool Selection::getInfo(Point2I pos, GridInfo & info)
{
    PROFILE_SCOPE( TerrainEditor_Selection_GetInfo );

    S32 index = lookup(pos);
    if(index == -1)
        return(false);

    info = (*this)[index];
    return(true);
}

bool Selection::setInfo(GridInfo & info)
{
    PROFILE_SCOPE( TerrainEditor_Selection_SetInfo );

    S32 index = lookup(info.mGridPoint.gridPos);
    if(index == -1)
        return(false);

    S32 next = (*this)[index].mNext;
    S32 prev = (*this)[index].mPrev;

    (*this)[index] = info;
    (*this)[index].mNext = next;
    (*this)[index].mPrev = prev;

    return(true);
}

F32 Selection::getAvgHeight()
{
    PROFILE_SCOPE( TerrainEditor_Selection_GetAvgHeight );

    if(!size())
        return(0);

    F32 avg = 0.f;
    for(U32 i = 0; i < size(); i++)
        avg += (*this)[i].mHeight;

    return(avg / size());
}

F32 Selection::getMinHeight()
{
    PROFILE_SCOPE( TerrainEditor_Selection_GetMinHeight );

    if(!size())
        return(0);

    F32 minHeight = (*this)[0].mHeight;
    for(U32 i = 1; i < size(); i++)
        minHeight = getMin(minHeight, (*this)[i].mHeight);

    return minHeight;
}

F32 Selection::getMaxHeight()
{
    PROFILE_SCOPE( TerrainEditor_Selection_GetMaxHeight );

    if(!size())
        return(0);

    F32 maxHeight = (*this)[0].mHeight;
    for(U32 i = 1; i < size(); i++)
        maxHeight = getMax(maxHeight, (*this)[i].mHeight);

    return maxHeight;
}
