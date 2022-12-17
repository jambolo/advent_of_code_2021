#if !defined(PATHFINDER_H_INCLUDED)
#define PATHFINDER_H_INCLUDED

#pragma once

#include <vector>

//! General A* Pathfinder.
class PathFinder
{
public:

    class Node;
    class Edge;

    using NodeList = std::vector<Node *>;   //!< A list of nodes.
    using EdgeList = std::vector<Edge *>;   //!< A list of edges.
    using Path     = NodeList;              //!< A path.

    //! Pathfinding parameters.
    struct Policy
    {
        int maxNodes;   //!< Maximum number of nodes to be used in the search (or <= 0 for unlimited)
    };

    // Constructor
    PathFinder(NodeList * domain = nullptr, Policy const & policy = Policy{ -1 });

    //! Finds the shortest path. Returns an empty path if not path was found.
    Path findPath(Node * start, Node * goal);

private:

    // Reset the status of all nodes in the domain to clear old values
    void resetNodes();

    // Constructs the path
    Path constructPath(Node * from, Node * to);

#if defined(_DEBUG)
    void validateNode(Node * node);
#endif

    NodeList * domain_;
    Policy policy_;
};

//! Pathfinder node.
class PathFinder::Node
{
public:

    Node() = default;

    //! Returns the estimated cost from this node to the goal. The value must not be greater the actual cost.
    //
    //! @note    This value is assumed to be constant.
    virtual float h(Node const * goal) const = 0;

    //! Updates pathfinding values for the node.
    void update(float cost, Node * pred);

    //! Anticipates being added to the open queue.
    void open(float cost, Node * pPredecessor, Node const * goal);

    //! Returns true if the node is open.
    bool isOpen() const { return status_ == Status::OPEN; }

    //! Anticipates being closed.
    void close();

    //! Returns true if the node is closed.
    bool isClosed() const { return status_ == Status::CLOSED; }

    //! Resets the node's status to not visited.
    void reset();

    EdgeList edges;   //!< List of edges leaving the node

    // For use by the pathfinder

    float f = 0.0f;                 //!< Estimated cost of total path through this node
    float g = 0.0f;                 //!< Cost of path to this node from the start
    Node * predecessor = nullptr;   //!< Previous node in the path (assuming the path goes through this node)

private:

    enum class Status
    {
        NOT_VISITED,
        OPEN,
        CLOSED
    };

    float cachedH_ = 0.0f;                  // Cached value of h()
    Status status_ = Status::NOT_VISITED;   // Node status
};

//! Pathfinder edge.
class PathFinder::Edge
{
public:

    float cost;             //!< Cost of traversing the edge.
    Node * to = nullptr;    //!< Link to the edge's destination node.
};

#endif // !defined(PATHFINDER_H_INCLUDED)
