
/*! \mainpage Decomposing of Arbitrary Polygon into Convex Polygon
 *
 * ABHINAV TYAGI 2020A7PS2043H
 *
 * DHRUV MERCHANT 2020A7PS2063H
 *
 * RITVIK 2020A7PS1723H
 *
 * UDAY SINGH THAKUR 2020A7PS2050H
 *
 */

#include <bits/stdc++.h>
using namespace std;

class Vertex;
class Edge;
class Face;

/// A class representing a vertex in a graph
class Vertex
{
public:
    int id;                           /// The id of the vertex
    pair<double, double> coordinates; /// The coordinates of the vertex
    Edge *incident_edge;              /// A pointer to the incident edge of the vertex

    /// Default constructor for the Vertex class

    Vertex() {}

    /**
     * @brief Constructor for the Vertex class
     * @param x The x-coordinate of the vertex
     * @param y The y-coordinate of the vertex
     * @param id The id of the vertex
     */
    Vertex(double x, double y, int id)
    {
        coordinates.first = x;
        coordinates.second = y;
        this->id = id;
        incident_edge = NULL;
    }

    /**
     * @brief Overloaded equality operator
     * @return True if the vertices have the same id, false otherwise
     */
    bool operator==(const Vertex &vertex)
    {
        return (this->id == vertex.id);
    }

    /**
     * @brief Overloaded inequality operator
     * @return True if the vertices have different ids, false otherwise
     */
    bool operator!=(const Vertex &vertex)
    {
        return (this->id != vertex.id);
    }
};

/// A class representing an edge in a graph
class Edge
{
public:
    Vertex *origin;      /// A pointer to the origin vertex of the edge
    Edge *twin_edge;     /// A pointer to the twin edge of the edge
    Face *incident_face; /// A pointer to the incident face of the edge
    Edge *next_edge;     /// A pointer to the next edge in a face's boundary
    Edge *prev_edge;     /// A pointer to the previous edge in a face's boundary

    /// Default constructor for the Edge class
    Edge() {}

    /**
     * @brief Constructor for the Edge class
     * @param source A pointer to the origin vertex of the edge
     * @param destination A pointer to the origin vertex of the edge
     */

    Edge(Vertex *source, Vertex *destination)
    {
        origin = source;

        twin_edge = new Edge();
        twin_edge->origin = destination;
        twin_edge->twin_edge = this;

        incident_face = NULL;
        next_edge = NULL;
        prev_edge = NULL;
    }

    /**
     * @brief Overloaded equality operator
     * @return True if the edges have the same origin, twin edge origin, next edge origin, and previous edge origin, false otherwise
     */

    bool operator==(const Edge &edge)
    {
        if (*(this->origin) != *(edge.origin))
            return false;
        if (*(this->twin_edge->origin) != *(edge.twin_edge->origin))
            return false;
        if (*(this->next_edge->origin) != *(edge.next_edge->origin))
            return false;
        if (*(this->prev_edge->origin) != *(edge.prev_edge->origin))
            return false;

        return true;
    }
};

/// A class representing a face in a graph
class Face
{
public:
    Edge *incident_edge; /// A pointer to the next edge in a face's boundary

    /// Default constructor for the Face class
    Face() {}

    /**
     * @brief Constructor for the Face class
     * @param edge A pointer to the edge
     */
    Face(Edge *edge)
    {
        incident_edge = edge;
        edge->incident_face = this;
    }

    /**
     * @brief Overloaded equality operator
     * @return True if the incident edges are same , false otherwise
     */
    bool operator==(const Face &face)
    {
        return (*(this->incident_edge) == *(face.incident_edge));
    }
};

/// A class representing a rectangular bounding box in 2D space

class Rectangle
{
public:
    double x_min, x_max, y_min, y_max; /// The minimum and maximum x and y coordinates of the rectangle

    /// Default constructor for the Rectangle class
    Rectangle() {}

    /**
     * @brief Constructor for the Rectangle class
     * @param vertex_list A vector of vertices defining the rectangle
     */
    Rectangle(vector<Vertex *> vertex_list)
    {
        /// Find the minimum and maximum x and y coordinates of the rectangle
        x_min = x_max = vertex_list[0]->coordinates.first;
        y_min = y_max = vertex_list[0]->coordinates.second;

        for (int i = 1; i < vertex_list.size(); i++)
        {
            x_min = min(x_min, vertex_list[i]->coordinates.first);
            x_max = max(x_max, vertex_list[i]->coordinates.first);

            y_min = min(y_min, vertex_list[i]->coordinates.second);
            y_max = max(y_max, vertex_list[i]->coordinates.second);
        }
    }

    /**
     * @brief Check if a vertex is inside the rectangle
     * @param vertex A pointer to the vertex to check
     * @return True if the vertex is inside the rectangle, false otherwise
     */

    bool is_in_rectangle(Vertex *vertex)
    {
        bool in_rectangle = true;

        double x_coordinate = vertex->coordinates.first;
        double y_coordinate = vertex->coordinates.second;

        if (x_coordinate < x_min || x_coordinate > x_max)
        {
            in_rectangle = false;
        }

        if (y_coordinate < y_min || y_coordinate > y_max)
        {
            in_rectangle = false;
        }

        return in_rectangle;
    }
};

/// a class representing DCEL data structure (Doubly-connected Edge List) which is a common edge-based representation
class DCEL
{
public:
    vector<Vertex *> vertices; /// vector of vertices in the DCEL
    vector<Edge *> edges;      /// vector of edges in the DCEL
    vector<Face *> faces;      /// vector of faces in the DCEL

    void create_dcel(vector<pair<double, double>> &points);
    void enumerate_vertices(Face *face);
    Edge *split(Edge *edge, double x, double y);
    void splice(Edge *a1, Edge *a2, Edge *b1, Edge *b2);
    void merge(Edge *a1, Edge *b1, double x, double y);

    Vertex *get_next_vertex(Vertex *vertex);
    Vertex *get_prev_vertex(Vertex *vertex);

    void add_diagonal(vector<Vertex *> &L);
};

/**

*@brief create_dcel function creates a DCEL from a set of points
*@param points A vector of points represented as a pair of doubles
*/
void DCEL::create_dcel(vector<pair<double, double>> &points)
{
    Vertex *origin = new Vertex(points.front().first, points.front().second, 0);

    vertices.push_back(origin);

    for (int i = 1; i < points.size(); i++)
    {
        Vertex *vertex = new Vertex(points[i].first, points[i].second, i);
        vertices.push_back(vertex);

        Edge *edge = new Edge(origin, vertex);
        edges.push_back(edge);
        origin->incident_edge = edge;

        origin = vertex;
    }

    Edge *edge = new Edge(origin, vertices.front());
    edges.push_back(edge);
    origin->incident_edge = edge;

    int num_edges = edges.size();

    for (int i = 0; i < num_edges; i++)
    {
        edges[i]->prev_edge = edges[(num_edges + i - 1) % num_edges];
        edges[i]->next_edge = edges[(i + 1) % num_edges];

        edges[i]->twin_edge->prev_edge = edges[(i + 1) % num_edges]->twin_edge;
        edges[i]->twin_edge->next_edge = edges[(num_edges + i - 1) % num_edges]->twin_edge;
    }

    Face *face = new Face(vertices.front()->incident_edge);
    faces.push_back(face);
}

/**
 *@brief enumerate_vertices function assigns IDs to all vertices in a face
 *@param face A pointer to a face in the DCEL
 */
void DCEL::enumerate_vertices(Face *face)
{
    Edge *start = face->incident_edge;
    Edge *e = start;

    do
    {
        cout << e->origin->coordinates.first << " " << e->origin->coordinates.second << endl;
        e = e->next_edge;
    } while (e != start);
}

/**
 *@brief get_next_vertex function returns the next vertex in the cyclic order around a vertex
 *@param vertex A pointer to the vertex whose next vertex is to be found
 *@return A pointer to the next vertex in the cyclic order around the given vertex
 */
Vertex *DCEL::get_next_vertex(Vertex *vertex)
{
    return vertex->incident_edge->next_edge->origin;
}

/**
 *@brief get_prev_vertex function returns the previous vertex in the cyclic order around a vertex
 *@param vertex A pointer to the vertex whose previous vertex is to be found
 *@return A pointer to the previous vertex in the cyclic order around the given vertex
 */
Vertex *DCEL::get_prev_vertex(Vertex *vertex)
{
    return vertex->incident_edge->prev_edge->origin;
}

/**
 *@brief split function splits an edge in the DCEL by adding a new vertex
 *@param edge A pointer to an edge in the DCEL to be split
 *@param x The x-coordinate of the new vertex
 *@param y The y-coordinate of the new vertex
 *@return A pointer to the newly created edge
 */
Edge *DCEL::split(Edge *edge, double x, double y)
{
    Vertex *v = new Vertex(x, y, vertices.size());
    Edge *new_edge = new Edge(v, edge->twin_edge->origin);

    new_edge->next_edge = edge->next_edge;
    new_edge->prev_edge = edge;
    edge->next_edge = new_edge;
    new_edge->next_edge->prev_edge = new_edge;
    v->incident_edge = new_edge;

    edge->twin_edge->origin = v;
    edge->twin_edge->next_edge = edge->prev_edge->twin_edge;
    edge->twin_edge->prev_edge = new_edge->twin_edge;

    vertices.push_back(v);
    edges.push_back(new_edge);
    return new_edge;
}

/**
 *@brief splice function splices two edges in the DCEL by changing their next and previous pointers
 *@param a1 A pointer to the first edge in the first pair
 *@param a2 A pointer to the second edge in the first pair
 *@param b1 A pointer to the first edge in the second pair
 *@param b2 A pointer to the second edge in the second pair
 */
void DCEL::splice(Edge *a1, Edge *a2, Edge *b1, Edge *b2)
{
    a1->next_edge = b2;
    b2->prev_edge = a1;

    b2->twin_edge->next_edge = a2;
    a2->prev_edge = b2->twin_edge;

    a2->twin_edge->next_edge = b1->twin_edge;
    b1->twin_edge->prev_edge = a2->twin_edge;

    b1->next_edge = a1->twin_edge;
    a1->twin_edge->prev_edge = b1;
}

/**
 *@brief merge function merges two edges in the DCEL by removing the diagonal between them
 *@param a1 A pointer to the first edge to be merged
 *@param b1 A pointer to the second edge to be merged
 *@param x The x-coordinate of the common endpoint of the two edges
 *@param y The y-coordinate of the common endpoint of the two edges
 */
void DCEL::merge(Edge *a1, Edge *b1, double x, double y)
{
    Edge *a2 = split(a1, x, y);
    Edge *b2 = split(b1, x, y);

    splice(a1, a2, b1, b2);
}

/**
 *@brief add_diagonal function adds a diagonal to a convex polygon
 *@param L A vector of vertices representing a convex polygon
 */
void DCEL::add_diagonal(vector<Vertex *> &L)
{
    Vertex *source = L.front();
    Vertex *destination = L.back();

    Edge *diagonal = new Edge(source, destination);

    diagonal->next_edge = destination->incident_edge;
    diagonal->prev_edge = source->incident_edge->prev_edge;
    diagonal->twin_edge->next_edge = source->incident_edge;
    diagonal->twin_edge->prev_edge = destination->incident_edge->prev_edge;

    destination->incident_edge->prev_edge->next_edge = diagonal->twin_edge;
    source->incident_edge->prev_edge->next_edge = diagonal;
    source->incident_edge->prev_edge = diagonal->twin_edge;
    destination->incident_edge->prev_edge = diagonal;

    source->incident_edge = diagonal;

    // vector<Edge *> top_face_diagonals = faces.back()->diagonals;
    faces.pop_back();

    Face *face1 = new Face(diagonal->twin_edge);
    Face *face2 = new Face(diagonal);

    // Face *face1 = new Face(diagonal->twin_edge, {});
    // Face *face2 = new Face(diagonal, top_face_diagonals);

    faces.push_back(face1);
    faces.push_back(face2);

    edges.push_back(diagonal);
}

/**
 *@brief cross_product function calculates the cross product of the three vertices
 *@param v1 pointer to the vertex v1
 *@param v2 pointer to the vertex v2
 *@param v3 pointer to the vertex v3
 *@return result of the cross product
 */
double cross_product(Vertex *v1, Vertex *v2, Vertex *v3)
{
    double ax = v1->coordinates.first - v2->coordinates.first;
    double ay = v1->coordinates.second - v2->coordinates.second;
    double bx = v3->coordinates.first - v2->coordinates.first;
    double by = v3->coordinates.second - v2->coordinates.second;
    double result = ax * by - ay * bx;

    return result;
}

/**
 *@brief is_reflex function checks if the cross product of the vertices is negative
 *@param v1 pointer to the vertex v1
 *@param v2 pointer to the vertex v2
 *@param v3 pointer to the vertex v3
 *@return true if the result is negative else false
 */
bool is_reflex(Vertex *v1, Vertex *v2, Vertex *v3)
{
    return (cross_product(v1, v2, v3) < 0);
}

/**
 *@brief is_type gives values 0 , 1 , -1 if the cross_prod is 0 , positive and negative
 *@param v1 pointer to the vertex v1
 *@param v2 pointer to the vertex v2
 *@param v3 pointer to the vertex v3
 *@return true if the result is negative else false
 */
int is_type(Vertex *v1, Vertex *v2, Vertex *v3)
{
    double cross_prod = cross_product(v1, v2, v3);

    if (cross_prod == 0.0)
        return 0;
    else if (cross_prod > 0.0)
        return 1;
    else if (cross_prod < 0.0)
        return -1;

    return 0;
}

/**
 *@brief is_in_polygon function determines whether a given vertex is inside a polygon
 *@param L A vector of vertices representing a convex polygon
 *@param vertex pointer to the vertex
 *@return true if the vertex is inside the polygon, false otherwise
 */
bool is_in_polygon(vector<Vertex *> &L, Vertex *vertex)
{
    Vertex *src_vertex = L.back();
    Vertex *dest_vertex = L.front();

    int direction = is_type(src_vertex, vertex, dest_vertex);

    for (int i = 0; i < L.size() - 1; i++)
    {
        src_vertex = L[i];
        dest_vertex = L[i + 1];

        if (direction != is_type(src_vertex, vertex, dest_vertex))
        {
            return false;
        }
    }

    return true;
}

/**
 *@brief remove_common_vertex function removes the common vertices between two vertex vectors
 *@param from_vector the original vector of vertices to remove common vertices from
 *@param common_vector the vector of common vertices to remove from from_vector
 *@return a new vector of vertices that contains all vertices from from_vector except those that are present in common_vector
 */
vector<Vertex *> remove_common_vertex(vector<Vertex *> &from_vector, vector<Vertex *> &common_vector)
{
    vector<Vertex *> result;

    int j = 0;

    for (int i = 0; i < from_vector.size(); i++)
    {
        if (from_vector[i] != common_vector[j])
        {
            result.push_back(from_vector[i]);
        }
        else
        {
            while (i < from_vector.size() && j < common_vector.size() && from_vector[i] == common_vector[j])
            {
                i++;
                j++;
            }

            i = i - 1;
        }
    }

    return result;
}

vector<Vertex *> get_LPVS(DCEL &polygon, vector<Vertex *> &L)
{
    vector<Vertex *> LPVS;

    int L_index = 0;

    Vertex *vertex = polygon.get_next_vertex(L.back());
    Vertex *prev_vertex = polygon.get_prev_vertex(vertex);
    Vertex *next_vertex = polygon.get_next_vertex(vertex);

    while (vertex != L.front())
    {
        if (is_reflex(prev_vertex, vertex, next_vertex))
        {
            LPVS.push_back(vertex);
        }

        vertex = next_vertex;
        next_vertex = polygon.get_next_vertex(vertex);
        prev_vertex = polygon.get_prev_vertex(vertex);
    }

    return LPVS;
}

vector<Vertex *> get_VTR(Vertex *v1, Vertex *v, vector<Vertex *> &L)
{
    vector<Vertex *> VTR;

    for (int i = 0; i < L.size(); i++)
    {
        if (is_reflex(v1, L.back(), v) == is_reflex(v1, L[i], v))
        {
            VTR.push_back(L[i]);
        }
    }

    return VTR;
}

/**
 *@brief decompose_polygon function decomposes a simple polygon into monotone polygons by adding diagonals between vertices
 *@param polygon the DCEL representing the polygon to decompose
 *@param start_index the index of the vertex to start the decomposition from
 *@param for_first_polygon a flag indicating whether this decomposition is for the first polygon (default is false)
 */
void decompose_polygon(DCEL &polygon, int start_index, bool for_first_polygon = false)
{
    vector<vector<Vertex *>> L;

    int n = polygon.vertices.size();
    int num_vertices = n;

    L.push_back({polygon.vertices[start_index % num_vertices]});

    int m = 1;

    while (n > 3)
    {
        vector<Vertex *> v(2);
        v[0] = L[m - 1].back();
        v[1] = polygon.get_next_vertex(v[0]);

        L.push_back({v[0], v[1]});
        int index = 1;

        v.push_back(polygon.get_next_vertex(v[index]));

        while (!is_reflex(v[index - 1], v[index], v[index + 1]) && !is_reflex(v[index], v[index + 1], v[0]) && !is_reflex(v[index + 1], v[0], v[1]) && L[m].size() < n)
        {
            L[m].push_back(v[index + 1]);
            index += 1;
            v.push_back(polygon.get_next_vertex(v[index]));
        }

        if (L[m].size() != n)
        {
            vector<Vertex *> LPVS = get_LPVS(polygon, L[m]);

            while (LPVS.size() > 0)
            {
                Rectangle rectangle(L[m]);

                bool backward = false;

                while (!backward && LPVS.size() > 0)
                {
                    Vertex *vertex = LPVS.front();

                    while (true)
                    {
                        vertex = LPVS.front();

                        if (!rectangle.is_in_rectangle(vertex))
                        {
                            LPVS.erase(LPVS.begin());
                        }

                        if ((LPVS.size() == 0) || rectangle.is_in_rectangle(vertex))
                        {
                            break;
                        }
                    }

                    if (LPVS.size() > 0)
                    {
                        if (is_in_polygon(L[m], vertex))
                        {
                            vector<Vertex *> VTR = get_VTR(v[0], vertex, L[m]);
                            L[m] = remove_common_vertex(L[m], VTR);
                            backward = true;
                        }
                        LPVS.erase(LPVS.begin());
                    }
                }
            }
        }
        if (L[m].back() != v[1])
        {
            polygon.add_diagonal(L[m]);
            n = n - L[m].size() + 2;
        }

        m += 1;

        if (for_first_polygon)
            break;
    }

    if (n == 2)
        polygon.faces.pop_back();
}

/**
 *@brief vertex_dependecy_process function decomposes
 *@param polygon polygon The DCEL representing the simple polygon
 *@param points vector of pair of points
 *@return vector with the minimial decompostions obtained by the Algorithm
 */
vector<DCEL> vertex_dependency_process(DCEL &polygon, vector<pair<double, double>> &points)
{
    vector<DCEL> OS;
    int cardinality = points.size();
    int s = 0;
    int n = polygon.vertices.size();

    while (s < n)
    {
        DCEL temp_polygon;
        temp_polygon.create_dcel(points);

        decompose_polygon(temp_polygon, s);

        if (temp_polygon.faces.size() == cardinality)
        {
            OS.push_back(temp_polygon);
        }
        else if (temp_polygon.faces.size() < cardinality)
        {
            OS = {temp_polygon};
            cardinality = temp_polygon.faces.size();
        }

        s += 1;

        bool new_first_polygon = false;

        while (!new_first_polygon && s < n)
        {
            DCEL P;
            P.create_dcel(points);

            decompose_polygon(P, s, true);

            if (*(temp_polygon.faces.front()) == *(P.faces.front()))
            {
                s += 1;
            }
            else
            {
                new_first_polygon = true;
            }
        }
    }

    polygon = OS.front();

    return OS;
}

/**
 *@brief get_diagonals adds the edges to the LLE vector
 *@param polygon the DCEL representing the polygon
 *@param LLE vector of Edges
 */

void get_diagonals(DCEL polygon, vector<Edge *> &LLE)
{
    for (int i = 0; i < polygon.faces.size(); i++)
    {
        LLE.push_back(polygon.faces[i]->incident_edge);
    }
    LLE.pop_back();
    for (int i = 0; i < polygon.faces.size(); i++)
    {
        LLE.push_back(polygon.faces[i]->incident_edge->twin_edge);
    }
    LLE.pop_back();
}

/**
 *@brief vertex_dependecy_process function decomposes
 *@param polygon a DCEL object representing the polygon for which to construct the LP
 *@param LLE vector of Edges
 *@param LP a reference to a map that will store the LP for each vertex
 */

void construct_LP(DCEL polygon, vector<Edge *> LLE, map<Vertex *, vector<pair<int, Vertex *>>> &LP)
{
    for (int i = 0; i < polygon.faces.size(); i++)
    {
        Edge *start = polygon.faces[i]->incident_edge;
        Edge *e = start;
        do
        {
            if (find(LLE.begin(), LLE.end(), e) != LLE.end())
            {
                LP[e->origin].push_back({i, e->twin_edge->origin});
            }
            e = e->next_edge;
        } while (e != start);
    }
}

/**
 *@brief merging_process function removes the diagonal if possible from the two faces possible and it still remains convex
 *@param polygon a DCEL object
 *@param is_vertex_notch map indicating which vertices are notches
 *@return a vector indicating whether each pair of adjacent notches can be merged
 */
vector<bool> merging_process(DCEL &polygon, map<int, bool> is_vertex_notch)
{
    int faces = 0;
    int NP = polygon.faces.size();
    int m = NP - 1;

    vector<Edge *> LLE;
    get_diagonals(polygon, LLE);

    map<Vertex *, vector<pair<int, Vertex *>>> LP;
    construct_LP(polygon, LLE, LP);

    vector<bool> LDP(NP, true);

    vector<int> LUP(NP);
    for (int i = 0; i < NP; i++)
    {
        LUP[i] = i;
    }

    for (int j = 0; j < m; j++)
    {
        Vertex *vs, *vt;
        vs = LLE[j]->origin;
        vt = LLE[j]->twin_edge->origin;

        int LPvs_size = LP[vs].size();

        int LPvt_size = LP[vt].size();

        bool is_vs_notch = is_vertex_notch[vs->id];
        bool is_vt_notch = is_vertex_notch[vt->id];

        if ((LPvs_size >= 2 && LPvt_size >= 2) || (LPvs_size >= 2 && !is_vt_notch) || (!is_vs_notch && LPvt_size >= 2) || (!is_vs_notch && !is_vt_notch))
        {
            Vertex *j2 = vt;
            Vertex *i2 = vs;
            Vertex *j3 = LLE[j]->next_edge->twin_edge->origin;
            Vertex *i1 = LLE[j]->prev_edge->origin;
            int u;
            for (auto p : LP[vt])
            {
                if (p.second == vs)
                {
                    u = p.first;
                }
            }
            // Edge *face2_incident = polygon.faces[face]->incident_edge;
            Vertex *j1 = LLE[j]->twin_edge->prev_edge->origin;
            Vertex *i3 = LLE[j]->twin_edge->next_edge->twin_edge->origin;

            if ((LDP[j] && LDP[u]) && (!is_reflex(i1, i2, i3) && !is_reflex(j1, j2, j3)))
            {
                NP++;
                LLE[j]->prev_edge->next_edge = LLE[j]->twin_edge->next_edge;
                LLE[j]->twin_edge->next_edge->prev_edge = LLE[j]->prev_edge;
                LLE[j]->next_edge->prev_edge = LLE[j]->twin_edge->prev_edge;
                LLE[j]->twin_edge->prev_edge->next_edge = LLE[j]->next_edge;
                vs->incident_edge = LLE[j]->prev_edge->next_edge;

                Face *face = new Face(vs->incident_edge);
                polygon.faces.push_back(face);
                remove(polygon.edges.begin(), polygon.edges.end(), LLE[j]);

                LDP[j] = false;
                LDP[u] = false;
                LDP.push_back(true);
                LUP[j] = NP;
                LUP[u] = NP;
                for (auto p : LP[vs])
                {
                    if (p.second == vt)
                    {
                        remove(LP[vt].begin(), LP[vt].end(), p);
                    }
                    else if (p.first == j)
                    {
                        p.first = NP;
                    }
                }
                for (auto p : LP[vt])
                {
                    if (p.second == vs)
                    {
                        remove(LP[vs].begin(), LP[vs].end(), p);
                    }
                    else if (p.first == u)
                    {
                        p.first = NP;
                    }
                }
                for (int h = 0; h < NP - 1; h++)
                {
                    if (LUP[h] == j || LUP[h] == u)
                    {
                        LUP[h] = NP;
                    }
                }
            }
        }
    }
    return LDP;
}

/**
 *@brief check_notch function checks if a vertex is a "notch" on the polygon.
 *@param polygon a DCEL object
 *@param is_vertex_notch a map indicating if a vertex is a notch or not
 */

void check_notch(DCEL polygon, map<int, bool> &is_vertex_notch)
{
    for (int i = 0; i < polygon.vertices.size(); i++)
    {
        Vertex *v = polygon.vertices[i];
        Vertex *v1 = polygon.vertices[i]->incident_edge->prev_edge->origin;
        Vertex *v2 = polygon.vertices[i]->incident_edge->twin_edge->origin;
        if (is_reflex(v1, v, v2))
        {
            is_vertex_notch[i] = true;
        }
        else
        {
            is_vertex_notch[i] = false;
        }
    }
}

int main(int argc, char *argv[])
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    FILE *time_data_file = fopen("time_data.txt", "w");

    int num_vertices;
    cin >> num_vertices; /// no of vertices as input

    vector<pair<double, double>> vertex_list(num_vertices);

    for (int i = 0; i < num_vertices; i++)
    {
        cin >> vertex_list[i].first >> vertex_list[i].second;
    }

    clock_t start, end;

    start = clock();

    DCEL polygon;
    polygon.create_dcel(vertex_list);

    map<int, bool> is_vertex_notch;
    check_notch(polygon, is_vertex_notch);

    vector<DCEL> OS = vertex_dependency_process(polygon, vertex_list);

    int min_card_ind = 0;
    int min_card = polygon.faces.size();

    vector<bool> min_LDP;

    for (int i = 0; i < OS.size(); i++)
    {
        vector<bool> LDP = merging_process(OS[i], is_vertex_notch);

        int faces = 0;

        for (int i = 0; i < LDP.size(); i++)
        {
            if (LDP[i])
            {
                faces++;
            }
        }

        if (faces <= min_card)
        {
            min_card_ind = i;
            min_LDP = LDP;
        }
    }

    DCEL min_polygon = OS[min_card_ind];

    int k = 0;

    for (int i = 0; i < min_LDP.size(); i++)
    {
        if (min_LDP[i])
        {
            k++;
        }
    }

    end = clock();

    double time_interval = double(end - start) / CLOCKS_PER_SEC;

    fprintf(time_data_file, "%lf", time_interval);
    fclose(time_data_file);

    for (int i = 0; i < min_polygon.faces.size(); i++)
    {
        if (min_LDP[i])
        {
            min_polygon.enumerate_vertices(min_polygon.faces[i]);
            cout << endl;
        }
    }

    return 0;
}