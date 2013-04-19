#ifndef GRAPH_H
#define GRAPH_H

#include "matrix.h"

template <typename V, typename E>
class Graph {
public:
    Graph(size_t n, const E& defaultEdge);
    Graph(const Graph<V,E>& other);
    ~Graph();
    V& Vertex(size_t n);
    E& Edge(size_t from, size_t to);
    size_t Vertices() const;
    Graph<V,E>& operator=(const Graph<V,E>& other);
private:
    void copy(const Graph<V,E>& other);
    std::vector<V> vertices;
    Matrix<E>* edges;
};

template <typename V, typename E>
Graph<V,E>::Graph(size_t n, const E& defaultEdge) {
    V temp;
    for(size_t i = 0; i < n; i++)
        vertices.push_back(temp);
    edges = new Matrix<E>(n,n,defaultEdge);
}

template <typename V, typename E>
Graph<V,E>::Graph(const Graph<V,E>& other) {
    copy(other);
}

template <typename V, typename E>
Graph<V,E>::~Graph() {
    delete edges;
}

template <typename V, typename E>
V& Graph<V,E>::Vertex(size_t n) {
    if(n > vertices.size()-1)
        throw INDEX_OOB;
    return vertices[n];
}

template <typename V, typename E>
E& Graph<V,E>::Edge(size_t from, size_t to) {
    return edges->Item(from,to);
}

template <typename V, typename E>
size_t Graph<V,E>::Vertices() const {
    return vertices.size();
}

template <typename V, typename E>
Graph<V,E>& Graph<V,E>::operator=(const Graph<V,E>& other) {
    if(this != &other) {
        delete edges;
        vertices.clear();
        copy(other);
    }
    return *this;
}

template <typename V, typename E>
void Graph<V,E>::copy(const Graph<V,E>& other) {
    edges = new Matrix<E>(*other.edges);
    vertices = other.vertices;
}


#endif // GRAPH_H
