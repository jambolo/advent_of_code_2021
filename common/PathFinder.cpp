#include "PathFinder.h"

#include <algorithm>
#include <cassert>

namespace
{

bool nodePrioritizer(PathFinder::Node const * a, PathFinder::Node const* b)
{
    // Min heap
    return a->f() > b->f();
}

}

//! @param  domain  Path nodes (if nullptr, the nodes must be explicitly reset before calling findPath)
//! @param  policy  Configuration options

PathFinder::PathFinder(NodeList * domain/* = nullptr*/, Policy const & policy/*= Policy{ -1 }*/)
    : domain_(domain)
    , policy_(policy)
{
}

//! @param    start     Start node
//! @param    end       End node
//! @param    path      Resulting path
//!
//! @returns    the path, empty if there is no path

PathFinder::Path PathFinder::findPath(Node * start, Node * end)
{
#if defined(_DEBUG)
    validateNode(start);
    validateNode(end);
#endif

    std::vector<Node *> openNodes;
    if (policy_.maxNodes > 0)
        openNodes.reserve(policy_.maxNodes);

    // Reset the status of all nodes

    resetNodes();

    // Add the start node to the open queue.

    start->open(0.f, nullptr, end);
    openNodes.push_back(start);

    // Until the open queue is empty or a path is found...

    while (!openNodes.empty())
    {
        // Get the lowest cost node as the next one to check (and close it)

        Node * pNode = openNodes.front();
        pNode->close();
        pop_heap(openNodes.begin(), openNodes.end(), nodePrioritizer);
        openNodes.pop_back();

        // If this is the goal, then we are done

        if (pNode == end)
            return constructPath(start, end);

        // Go to each neighbor and set/update its cost and make sure it is in the open queue (unless it is closed)

        EdgeList const & edges = pNode->edges;
        for (auto const & edge : edges)
        {
            Node * pNeighbor = edge->to;

#if defined(_DEBUG)
            validateNode(pNeighbor);
#endif

            // If the node is closed, then its minimum cost has been determined and there is no reason to revisit
            // it.

            if (pNeighbor->isClosed())
                continue;

            // Compute the cost to the neighbor through this node
            float cost = pNode->g() + edge->cost;

            // If the neighbor is not in the open queue, then add it

            if (!pNeighbor->isOpen())
            {
                pNeighbor->open(cost, pNode, end);

                // If the open queue is full then remove the last entry to make room. The last entry is not
                // necessarily the highest cost node, but it is guaranteed to be in the highest 50%. The
                // non-determinism is the price to pay for a fixed-size queue.

                if (policy_.maxNodes > 0 && policy_.maxNodes <= (int)openNodes.size())
                {
                    openNodes.back()->close();
                    openNodes.pop_back();
                }

                assert(policy_.maxNodes <= 0 || openNodes.size() < openNodes.capacity());
                openNodes.push_back(pNeighbor);
                push_heap(openNodes.begin(), openNodes.end(), nodePrioritizer);
            }

            // Otherwise, perhaps this is a lower-cost path to it. If so, update it to reflect the new path.

            else if (cost < pNeighbor->g())
            {
                pNeighbor->update(cost, pNode);
                // Ouch. This could be expensive. If we knew where in the heap this value is we could push_heap instead.
                make_heap(openNodes.begin(), openNodes.end(), nodePrioritizer);
            }
        }
    }

    return Path();
}

void PathFinder::resetNodes()
{
    if (domain_)
    {
        for (auto const & node : *domain_)
        {
            node->reset();
        }
    }
}

PathFinder::Path PathFinder::constructPath(Node * from, Node * to)
{
    Path path;

    // The nodes are linked from end to start, so the vector is filled in that order, and then the order of the
    // elements is the vector is reversed.

    // Add nodes to the path (end-to-start)
    for (Node * pNode = to; pNode != nullptr; pNode = pNode->predecessor())
    {
        path.push_back(pNode);
    }

    assert(path.back() == from);

    // Reverse to start-to-end
    std::reverse(path.begin(), path.end());
    return path;
}

#if defined(_DEBUG)

void PathFinder::validateNode(Node * node)
{
    // Assert that the node is in the domain (if one is provided)
    assert(domain_ == nullptr || find(domain_->begin(), domain_->end(), node) != domain_->end());
}

#endif // defined( _DEBUG )

void PathFinder::Node::open(float cost, Node * pPredecessor, Node const * goal)
{
    assert(status_ == Status::NOT_VISITED);
    status_ = Status::OPEN;
    cachedH_ = h(goal);
    update(cost, pPredecessor);
}

void PathFinder::Node::close()
{
    assert(status_ == Status::OPEN);
    status_ = Status::CLOSED;
}

void PathFinder::Node::reset()
{
    f_              = 0.0f;
    g_              = 0.0f;
    predecessor_    = nullptr;
    cachedH_        = 0.0f;
    status_         = Status::NOT_VISITED;
}

void PathFinder::Node::update(float cost, Node * pred)
{
    assert(status_ == Status::OPEN);
    g_              = cost;
    f_              = g_ + cachedH_;
    predecessor_    = pred;
}
