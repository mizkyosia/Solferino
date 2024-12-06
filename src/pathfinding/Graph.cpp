#include "Graph.hpp"
#include "agents/Car.hpp"

Graph::Graph()
{
    // End nodes
    Node *ends[6] = {
        addNode(Vector3(22.5f, 0, 0), false, true, Util::VehicleType::Car, RED),
        addNode(Vector3(17.5f, 0, 40), false, true, Util::VehicleType::Car, RED),
        addNode(Vector3(40, 0, 29.5f), false, true, Util::VehicleType::Bus, RED),
        addNode(Vector3(40, 0, 24.5f), false, true, Util::VehicleType::Car, RED),
        addNode(Vector3(0, 0, 10.5f), false, true, Util::VehicleType::Bus, RED),
        addNode(Vector3(0, 0, 15.5f), false, true, Util::VehicleType::Car, RED),
    };

    // Start nodes
    Node *starts[6] = {
        addNode(Vector3(0, 0, 24.5f), true, false, Util::VehicleType::Car, BLUE),
        addNode(Vector3(0, 0, 29.5f), true, false, Util::VehicleType::Bus, BLUE),
        addNode(Vector3(40, 0, 15.5f), true, false, Util::VehicleType::Car, BLUE),
        addNode(Vector3(40, 0, 10.5f), true, false, Util::VehicleType::Bus, BLUE),
        addNode(Vector3(17.5f, 0, 0), true, false, Util::VehicleType::Car, BLUE),
        addNode(Vector3(22.5f, 0, 40), true, false, Util::VehicleType::Car, BLUE),
    };

    // First intersection
    Node *inter1[6] = {
        addNode(Vector3(22.5f, 0, 8.5f)),
        addNode(Vector3(17.5f, 0, 8.5f)),
        addNode(Vector3(24.5f, 0, 15.5f)),
        addNode(Vector3(24.5f, 0, 10.5f)),
        addNode(Vector3(15.5f, 0, 10.5f)),
        addNode(Vector3(15.5f, 0, 15.5f)),
    };

    // Between the two
    Node *pass[2] = {
        addNode(Vector3(17.5f, 0, 20), false, false, Util::VehicleType::Car, PURPLE),
        addNode(Vector3(22.5f, 0, 20), false, false, Util::VehicleType::Car, PURPLE),
    };

    // Second intersection
    Node *inter2[6] = {
        addNode(Vector3(22.5f, 0, 31.5f)),
        addNode(Vector3(17.5f, 0, 31.5f)),
        addNode(Vector3(24.5f, 0, 24.5f)),
        addNode(Vector3(24.5f, 0, 29.5f)),
        addNode(Vector3(15.5f, 0, 29.5f)),
        addNode(Vector3(15.5f, 0, 24.5f)),
    };

    for (int i = 0; i < 6; i++)
    {
        Node *endTarget = inter1[0], *startTarget = inter1[0];
        for (int j = 0; j < 6; j++)
        {
            if (Vector3Distance(inter1[j]->getPos(), starts[i]->getPos()) < Vector3Distance(startTarget->getPos(), starts[i]->getPos()))
                startTarget = inter1[j];
            if (Vector3Distance(inter2[j]->getPos(), starts[i]->getPos()) < Vector3Distance(startTarget->getPos(), starts[i]->getPos()))
                startTarget = inter2[j];

            if (Vector3Distance(inter1[j]->getPos(), ends[i]->getPos()) < Vector3Distance(endTarget->getPos(), ends[i]->getPos()))
                endTarget = inter1[j];
            if (Vector3Distance(inter2[j]->getPos(), ends[i]->getPos()) < Vector3Distance(endTarget->getPos(), ends[i]->getPos()))
                endTarget = inter2[j];
        }

        starts[i]->link(startTarget);
        endTarget->link(ends[i]);
    }

    inter2[5]->link(inter2[1]);
    inter2[5]->link(inter2[2]);
    inter2[5]->link(pass[1]);

    inter2[0]->link(inter2[2]);
    inter2[0]->link(pass[1]);

    pass[0]->link(inter2[1]);
    pass[0]->link(inter2[2]);

    inter1[2]->link(inter1[5]);
    inter1[2]->link(inter1[0]);
    inter1[2]->link(pass[0]);

    inter1[1]->link(inter1[5]);
    inter1[1]->link(pass[0]);

    pass[1]->link(inter1[5]);
    pass[1]->link(inter1[0]);
}

Graph::~Graph()
{
}

Node *Graph::addNode(Vector3 pos, bool start, bool end, Util::VehicleType allowedVehicles, Color color)
{
    auto n = new Node(pos, start, end, allowedVehicles, color);
    _nodes.insert(n);
    return n;
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

void Graph::spawnVehicle(Util::VehicleType kind)
{
    // Find a start node at random
    std::vector<Node *> startNodes, endNodes;

    for (auto n : _nodes)
    {
        if (n->isStart() && n->allowsVehicle(kind))
            startNodes.push_back(n);
        if (n->isEnd() && n->allowsVehicle(kind))
            endNodes.push_back(n);
    }

    if (startNodes.size() == 0 || endNodes.size() == 0)
        return;

    auto start = startNodes[(rand() % startNodes.size())];
    auto end = start;

    while (end == start || abs(end->getPos().x - start->getPos().x) < 5 || abs(end->getPos().z - start->getPos().z) < 5)
        end = endNodes[(rand() % endNodes.size())];

    while (startNodes.size() > 1 && (end == start || abs(end->getPos().x - start->getPos().x) < 5 || abs(end->getPos().z - start->getPos().z) < 5))
        start = startNodes[(rand() % startNodes.size())];

    auto path = findPath(start, end, kind);

    Vehicle *v = new Car(0, 0, 0, path);

    TraceLog(LOG_WARNING, "Spawning vehicle with path of length %lu, start=%x, end=%x", path.size(), start, end);

    _vehicles.insert(v);
}

void Graph::despawnVehicle(Vehicle *vehicle)
{
    if (!_vehicles.contains(vehicle))
        return;
    _vehicles.erase(vehicle);
    delete vehicle;
}

std::vector<Node *> Graph::findPath(Node *start, Node *destination, const Util::VehicleType vehicleType) const
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
            if (!n->allowsVehicle(vehicleType))
                continue;

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