#include "Graph.hpp"
#include "agents/Car.hpp"

Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::addNode(Vector3 pos)
{
    _nodes.insert(new Node(pos));
}

void Graph::removeNode(Node *node)
{
    for (auto n : _nodes)
    {
        if (n != node)
            n->unlink(node);
    }

    _nodes.erase(node);
    delete node;
}

std::set<Node *> &Graph::getNodes()
{
    return _nodes;
}

std::set<Vehicle *> &Graph::getVehicles()
{
    return _vehicles;
}

unsigned int Graph::nodeCount()
{
    return _nodes.size();
}

unsigned int Graph::vehicleCount()
{
    return _vehicles.size();
}

void Graph::spawnVehicle()
{
    // Find a start node at random
    std::vector<Node *> startNodes, endNodes;

    for (auto n : _nodes)
    {
        if (n->isStart())
            startNodes.push_back(n);
        if (n->isEnd())
            endNodes.push_back(n);
    }

    if(startNodes.size() == 0 || endNodes.size() == 0) return;

    auto start = startNodes[(rand() % startNodes.size())];
    auto end = start;

    while (end == start)
        end = endNodes[(rand() % endNodes.size())];

    while (startNodes.size() > 1 && end == start)
        start = startNodes[(rand() % startNodes.size())];

    auto path = findPath(start, end, Util::Car);

    Vehicle* v = new Car(0, 0, 0, path);

    TraceLog(LOG_WARNING, "Spawning vehicle with path of length %lu, start=%x, end=%x", path.size(), start, end);

    _vehicles.insert(v);
}

void Graph::despawnVehicle(Vehicle *vehicle)
{
    if(!_vehicles.contains(vehicle)) return;
    _vehicles.erase(vehicle);
    delete vehicle;
}

std::vector<Node *> Graph::findPath(Node *start, Node *destination, const char vehicleType) const
{
    std::vector<Node *> path;
    // Implement Djikstra's algorithm

    std::map<Node *, Node *> from;
    std::map<Node *, float> costs;

    std::priority_queue<PathNode, std::vector<PathNode>, PathComparator> frontier;

    frontier.push((PathNode){start, 0});
    costs.insert(std::pair(start, 0));

    while (!frontier.empty())
    {
        auto current = frontier.top();
        frontier.pop();

        if (current.inner == destination)
        {
            auto back = current.inner;
            while (back != start)
            {
                path.push_back(back);
                back = from[back];
            }

            path.push_back(start);

            break;
        }

        for (auto n : current.inner->getAllLinks())
        {
            float n_cost = costs[current.inner] + 1;
            bool check = !costs.contains(n);

            if (check || n_cost < costs[n])
            {
                if (check)
                    costs.insert(std::pair(n, n_cost));
                else
                    costs[n] = n_cost;

                frontier.push((PathNode){n, n_cost + Vector3Distance(destination->getPos(), n->getPos())});

                if (from.contains(n))
                    from[n] = current.inner;
                else
                    from.insert(std::pair(n, current.inner));
            }
        }
    }

    return path;
}