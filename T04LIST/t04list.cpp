/* FILE: t04list.cpp
 * PROGRAMMER: BZ6
 * DATE: 19.07.2021
 * PURPOSE: List.
 */

 /* Debug memory allocation support */
#ifndef NDEBUG 
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h> 
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

static class __Dummy
{
public:
  /* Class constructor */
  __Dummy( void )
  {
    SetDbgMemHooks();
  } /* End of '__Dummy' constructor */
} __ooppss;
#endif /* _DEBUG */ 

#ifdef _DEBUG 
# ifdef _CRTDBG_MAP_ALLOC 
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
# endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */

#include <iostream>

/* namespace 'bz6' */
namespace bz6
{
  /* Structure list */
  template<typename Type>
    struct list
    {
    private:
      bool Empty = true;
      int Count;

      /* Structure entry */
      struct entry
      {
        Type Data;
        entry *Next;
        entry( Type NewData, entry *NewNext = nullptr ) : Data(NewData), Next(NewNext)
        {
        } /* End of constructor */
      } *Start;
    public:
      list( void ): Start(nullptr), Count(0)
      {
      } /* End of constructor */
      list( const list &List )
      {
        Clear();

        if (List.Start != nullptr)
        {
          entry *ptr = List.Start, **ptrl = &Start;
          int n = List.CountOfElements();

          *ptrl = new entry( ptr->Data );
          ptr = ptr->Next;

          for (int i = 0; i < n - 1; i++)
          {
            (*ptrl)->Next = new entry( ptr->Data );
            ptr = ptr->Next;
            ptrl = &(*ptrl)->Next;
          }
          Count = n;
          Empty = false;
        }
      } /* End of copy constructor */
      list & PushFront( Type Data )
      {
        Start = new entry(Data, Start);
        Count++;
        Empty = false;
        return *this;
      } /* End of 'PushFront' function */
      list & PushBack( Type Data )
      {
        entry **ptr = &Start;
        while (*ptr != nullptr)
          ptr = &(*ptr)->Next;
        *ptr = new entry(Data);
        Count++;
        Empty = false;
        return *this;
      } /* End of 'PushBack' function */
      ~list( void )
      {
        if (!Empty)
          while (Start != nullptr)
          {
            entry *old = Start;
            Start = Start->Next;
            delete old;
          }
      } /* End of destructor */
      void Display( void )
      {
        if (Empty)
          return;
        entry *ptr = Start;
        while (ptr != nullptr)
        {
          std::cout << ptr->Data << "\n";
          ptr = ptr->Next;
        }
      } /* End of 'Display' function */
      void Clear( void )
      {
        if (Empty)
          return;
        while (Start != nullptr)
        {
          entry *old = Start;
          Start = Start->Next;
          delete old;
        }
        Count = 0;
      } /* End of 'Clear' function */
      bool PopFront( Type *Data )
      {
        if (Empty)
          return false;
        entry *old = Start;
        Start = Start->Next;
        *Data = old->Data;
        delete old;
        Count--;
        if (Count == 0)
          Empty = true;
        return true;
      } /* End of 'PopFront' function */
      bool PopFront( void )
      {
        if (Empty)
          return false;
        entry *old = Start;
        Start = Start->Next;
        delete old;
        Count--;
        if (Count == 0)
          Empty = true;
        return true;
      } /* End of 'PopFront' function */
      bool PopBack( Type *Data )
      {
        if (Empty)
          return false;
        entry *ptr = Start;
        while (ptr->Next != nullptr)
          ptr = ptr->Next;
        *Data = ptr->Data;
        delete ptr;
        Count--;
        if (Count == 0)
          Empty = true;
        return true;
      } /* End of 'PopBack' function */
      bool PopBack( void )
      {
        if (Empty)
          return false;
        entry *ptr = Start;
        while (ptr->Next != nullptr)
          ptr = ptr->Next;
        delete ptr;
        Count--;
        if (Count == 0)
          Empty = true;
        return true;
      } /* End of 'PopBack' function */
      bool IsEmpty( void ) const
      {
        return Empty;
      } /* End of 'IsEmpty' function */
      int CountOfElements( void ) const
      {
        return Count;
      } /* End of 'CountOfElements' function */
      list & operator=( const list &List )
      {
        Clear();

        if (List.Start != nullptr)
        {
          entry *ptr = List.Start, **ptrl = &Start;
          int n = List.CountOfElements();

          *ptrl = new entry( ptr->Data );
          ptr = ptr->Next;

          for (int i = 0; i < n - 1; i++)
          {
            (*ptrl)->Next = new entry( ptr->Data );
            ptr = ptr->Next;
            ptrl = &(*ptrl)->Next;
          }
          Count = n;
          Empty = false;
        }
        return *this;
      } /* End of 'operator=' function */
    };
} /* end of 'bz6' namespace */

/* Entry point of program */
int main( void )
{
  bz6::list<double> L;
  double x;

  L.PushFront(10.47).PushBack(23.5);
  L.Display();
  bz6::list<double> l;
  l = L;
  if (L.PopFront(&x))
    std::cout << x << "\n";
  if (L.PopBack(&x))
    std::cout << x << "\n";
  L.Display();
  L.Clear();
  l.Display();
} /* End of 'main' function */

/* END OF 't04list.cpp' FILE */