#ifndef CTABLE_H
#define CTABLE_H

#include <vector>
#include <memory>
#include <stdexcept>

#include "Helpers.h"
#include "CCell.h"

/**
 * @class CTable
 * @brief Class representing table.
 */
class CTable
{
  public:
    CTable ( size_t init_size );

    /**
     * @brief Edit cell on given position and insert given properties and checks cycle references.
     * @param[in] pos Coordinates of edited cell in a table.
     * @param[in] props Cell content to be assigned to cell.
    */
    void Edit ( TPosition pos, TCellProperties props );

    /**
     * @brief Deletes content of cells in given range.
     * @param[in] range Range of cells to be deleted.
    */
    void Delete ( TRange range ) const;

    /**
     * @brief Obtain order in which cells should be evaluated through TopSort algorithm and evaluates them.
    */
    void Evaluate ( void ) const;

    /**
     * @brief Gets cell on given position.
     * @param[in] pos Coordinates of wanted cell in a table.
     * @return Pointer to wanted cell.
    */
    std::shared_ptr< CCell > At ( TPosition pos ) const;

    /**
     * @brief Getter of size of table.
     * @return Current size of table.
    */
    size_t Size ( void ) const;

    /**
     * @brief Ensures new_size x new_size array of cells would be created and existing cells untouched.
     * @param[in] new_size New proportions of table.
    */
    void Expand ( size_t new_size );

  private:
    /**
     * @brief Detect cycle reference.
     * @param[in] cell Coordinates of starting point for cycle detection.
     * @return Whether cycle detected.
    */
    bool DetectCycle ( TPosition cell ) const;

    /**
     * @brief DFS algorithm implemented on 2D array of nodes representing cells and their dependencies.
     * @param[in] cell Coordinates of currently proccessed cell.
     * @param[in,out] nodes Node array for DFS algorithm.
     * @param[in] eval Indicates whether to evaluate cells corresponding to a node during its closure.
     * @return Whether cycle detected.
    */
    bool DFS_scan ( TPosition cell, std::vector< std::vector< TNode > > & nodes, bool eval ) const;

    /**
     * @brief Helper method constructing temporary nodes for DFS algorithm.
     * @return 2D array of nodes for DFS algorithm.
    */
    std::vector< std::vector< TNode > > ConstructNodes ( void ) const;

    /** 2D array of cells. */
    std::vector< std::vector< std::shared_ptr< CCell > > > m_Arr;
};

#endif // CTABLE_H