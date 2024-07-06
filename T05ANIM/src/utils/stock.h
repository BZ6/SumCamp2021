/* FILE NAME  : stock.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 22.07.2021
 * PURPOSE    : Stock handle module.
 */

#ifndef __stock_h_
#define __stock_h_

#include <cassert>

#include "../zagl.h"

// implementation
template<typename type>
  class stock
  {
  private:
    INT RealSize, MaxSize;
    type *Array;
 
  public:
    /* Stock constructor function .
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    stock( VOID ) : Array(nullptr), RealSize(0), MaxSize(0)
    {
    } /* End of 'stock' function */

    /* Stock desctructor function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~stock( VOID )
    {
      if (Array != nullptr)
        delete[] Array;
      Array = nullptr;
      RealSize = 0;
      MaxSize = 0;
    } /* End of '' function */
 
    /* Copy stock constructor.
     * ARGUMENTS:
     *   - stock to be copied:
     *       const stock<type> &CopyData;
     * RETURNS: None.
     */
    stock( const stock<type> &CopyData ) :
      Array(new type[CopyData.RealSize]),
      RealSize(CopyData.RealSize),
      MaxSize(CopyData.RealSize)
    {
      for (INT i = 0; i < RealSize; i++)
        Array[i] = CopyData.Array[i];
    } /* End of 'stock' function */
 
    /* Operator = function. 
     * ARGUMENTS:
     *   - stock to be copied:
     *       const stock<type> &CopyData;
     * RETURNS:
     *   (stock &) copied stock.
     */
    stock & operator=( const stock<type> &CopyData )
    {
      if (Array != nullptr)
        delete[] Array;
      Array = new type[CopyData.RealSize];
      RealSize = CopyData.RealSize;
      MaxSize = CopyData.RealSize;
      for (INT i = 0; i < RealSize; i++)
        Array[i] = CopyData.Array[i];
      return *this;
    } /* End of 'operator=' function */
 
    /* Get stock size function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (INT) stock size.
     */
    INT Size( VOID ) const
    {
      return RealSize;
    } /* End of 'Size' function */

    /* Stock add function.
     * ARGUMETNS:
     *   - data to be added:
     *       const type &NewData;
     * RETURNS:
     *   (stock &) result stock.
     */
    stock & Add( const type &NewData )
    {
      if (RealSize >= MaxSize)
      {
        // increase allocated memory
        if (MaxSize == 0)
          MaxSize = 1;
        else
          MaxSize *= 2;
 
        type *new_bulk = new type[MaxSize];
        if (Array != nullptr)
        {
          // copy old data
          for (INT i = 0; i < RealSize; i++)
            new_bulk[i] = Array[i];
          // remove old data
          delete[] Array;
        }
        Array = new_bulk;
      }
      Array[RealSize++] = NewData;
      return *this;
    } /* End of 'Add' function */

    /* Operator << function.
     * ARGUMENTS:
     *   - data to be added:
     *       const type &NewData;
     * RETURNS:
     *   (stock &) result stock.
     */
    stock & operator<<( const type &NewData )
    {
      return Add(NewData);
    } /* End of 'operator<<' function */
 
    /* Operator [] function.
     * ARGUMENTS:
     *   - stock index:
     *       INT Index;
     * RETURNS:
     *   (type &) Getted Data.
     */
    type & operator[]( INT Index )
    {
      assert(Index >= 0 && Index < RealSize);
      return Array[Index];
    } /* End of 'operator[]' function */

    /* Operator [] function.
     * ARGUMENTS:
     *   - stock index:
     *       INT Index;
     * RETURNS:
     *   (type &) Getted Data.
     */
    type & operator[]( INT Index ) const
    {
      assert(Index >= 0 && Index < RealSize);
      return Array[Index];
    } /* End of 'operator[]' function */
 
    /* Clear stock function.
     * ARGUMETNS: None.
     * RETURNS: None.
     */
    VOID Clear( VOID )
    {
      RealSize = 0;
    } /* End of 'Clear' function */
 
    /* Walker function.
     * ARGUMENTS:
     *   - walker function:
     *       type2 &Walker;
     * RETURNS: None.
     */
    template<typename type2>
      VOID Walk( const type2 &Walker )
      {
        for (INT i = 0; i < RealSize; i++)
          Walker(Array[i]);
      } /* End of 'Walk' function */
  }; /* End of 'stock' class */

#endif /* __stock_h_ */

/* END OF 'stock.h' FILE */
