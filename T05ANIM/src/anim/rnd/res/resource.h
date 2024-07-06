/* FILE NAME   : resource.h
 * PROGRAMMER  : Bogdan Zakharkin.
 * LAST UPDATE : 24.07.2021.
 * PURPOSE     : Animation project.
 *               Animation system.
 *               Render system.
 *               Resources.
 *               Resource decloration.
 */

#ifndef __resource_h_
#define __resource_h_

#include <map>
#include <string>

#include "../../../zagl.h"

namespace zagl
{
  template<typename entry_type, typename index_type = INT>
  class resource_manager
  {
  protected:
    struct entry_ref : public entry_type
    {
      index_type EntryRef;

      entry_ref( const index_type &RefVal = {} ) : EntryRef(RefVal)
      {
      }

      entry_ref( const entry_type &EntryVal, const index_type &RefVal = {} ) : 
        entry_type(EntryVal), EntryRef(RefVal)
      {
      }
    };

    INT TotalRefCnt = 0;
    std::map<index_type, entry_type> Stock;

    entry_type * add( const entry_type &Entry )
    {
      if constexpr (std::is_convertible_v<index_type, INT>)
      {
        entry_ref ref(Entry, TotalRefCnt);
        return &(Stock[TotalRefCnt++] = ref);
      }
      else
      {
        entry_ref ref(Entry, Entry.Name);
        return &(Stock[Entry.Name] = ref);
      }
    }
    resource_manager & Update( VOID )
    {
      for (auto entry : Stock)
        entry.second.Update();
      //Stock.clear();
      return *this;
    }
    resource_manager & Init( VOID )
    {
      for (auto entry : Stock)
        entry.second.Load();
      //Stock.clear();
      return *this;
    }
    ~resource_manager ( VOID )
    {
      for (auto &entry : Stock)
        entry.second.Free();
    }
  public:
    entry_type * Find( const index_type &Name )
    {
      if (Stock.find(Name) == Stock.end())
        return nullptr;
      return &Stock[Name];
    }
    resource_manager & Delete( entry_type *Entry )
    {
      if (Entry == nullptr)
        return *this;
      Entry->Free();
      auto find_entry = Stock.find(reinterpret_cast<entry_ref *>(Entry)->ref);
    }
  };
}

#endif /* __resource_h_ */

/* END OF 'resource.h' FILE */