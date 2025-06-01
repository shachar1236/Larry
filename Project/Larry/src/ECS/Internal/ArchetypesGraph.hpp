#pragma once

#include "common.h"
#include "ECS/Internal/TypeManager.hpp"
#include "ECS/Internal/Archetype.hpp"
#include "ECS/Internal/TypesBitmap.hpp"
#include <cassert>
#include <strings.h>
#include <vector>
namespace Larry::ECS::Internal {

    // TODO: need to change this to be a every node points to other node and the dge value between them is what type the other node have that the first node dosent
    class ArchetypesGraph {
        private:
            struct Edge {
                int index;
                TypesBitmap value;
            };

            struct Node {
                Archetype archetype;
                std::vector<Edge> edges;
                bool visited = false;
                int next_visited = -1;
            };

            std::vector<Node> archetypes;

            // stores the index of the archetypes with the most types
            std::vector<int> archetype_with_most_types;
            int most_types_count = -1;

            void ClearVisited(int start_index) {
                int i = start_index;
                while (i != -1) {
                    archetypes[i].visited = false;
                    int tmp = archetypes[i].next_visited;
                    archetypes[i].next_visited = -1;
                    i = tmp;
                }
            }

            void AddArchetypeAsEdgeToNodes(int node_index, int new_archetype_index, const TypesBitmap& archetype_types, int type_count, int* last_visited) {
                if (archetypes[node_index].visited) {
                    return;
                }
                archetypes[node_index].visited = true;
                archetypes[node_index].next_visited = *last_visited;
                *last_visited = node_index;

                TypesBitmap node_types_bitmap = archetypes[node_index].archetype.GetTypesBitmap();

                if (node_types_bitmap.Intersect(archetype_types)) {
                    int node_types_count = node_types_bitmap.GetTypesCount();
                    if (node_types_count == type_count + 1) {
                        TypesBitmap edge_type = node_types_bitmap & ~archetype_types;
                        archetypes[node_index].edges.push_back(Edge{ new_archetype_index, edge_type });
                    } else if (type_count == node_types_count + 1) {
                        // add to my edges
                        TypesBitmap edge_type = archetype_types & ~node_types_bitmap;
                        archetypes[new_archetype_index].edges.push_back(Edge{node_index, edge_type});
                    } else if (node_types_count + 1 < type_count) {
                        return;
                    } else {
                        // he is bigger than me
                        for (int i = 0; i < archetypes[node_index].edges.size(); i++) {
                            Edge& edge = archetypes[node_index].edges[i];
                            AddArchetypeAsEdgeToNodes(i, new_archetype_index, archetype_types, type_count, last_visited);
                        }
                    }
                }
            }

            // returns index of the archetype
            int GetArchetypeRecursive(int node_index, const TypesBitmap& types, int* last_visited) {
                if (archetypes[node_index].visited) {
                    return -1;
                }
                archetypes[node_index].visited = true;
                archetypes[node_index].next_visited = *last_visited;
                *last_visited = node_index;

                TypesBitmap node_types_bitmap = archetypes[node_index].archetype.GetTypesBitmap();

                if (types == node_types_bitmap) {
                    return node_index;
                }

                bool types_inside_node = (node_types_bitmap & types) == types;
                if (types_inside_node) {
                    for (auto& edge : archetypes[node_index].edges) {
                        bool edge_type_not_in_types = (types & edge.value) == types;
                        if (edge_type_not_in_types) {
                            int a = GetArchetypeRecursive(edge.index, types, last_visited);
                            if (a != -1) {
                                return a;
                            }
                        }
                    }
                }
                return -1;
            }

            template<typename F>
            void CallWithIntersectingArchetypesRecursive(int node_index, const TypesBitmap& types, int* last_visited, const F& callback, bool* stop) {
                if (archetypes[node_index].visited) {
                    return;
                }
                archetypes[node_index].visited = true;
                archetypes[node_index].next_visited = *last_visited;
                *last_visited = node_index;

                TypesBitmap node_types_bitmap = archetypes[node_index].archetype.GetTypesBitmap();

                bool types_inside_node = (node_types_bitmap & types) == types;
                if (types_inside_node) {
                    callback(&(archetypes[node_index].archetype), stop);

                    for (auto& edge : archetypes[node_index].edges) {
                        if (!stop) {
                            bool edge_type_not_in_types = (types & edge.value) == types;
                            if (edge_type_not_in_types) {
                                CallWithIntersectingArchetypesRecursive(edge.index, types, last_visited, callback, stop);
                            }
                        }
                    }
                }
            }
        public:

            ArchetypesGraph() { }
            ~ArchetypesGraph() { }

            // create a new archetype with specified types and returns the index of the new archetype
            int CreateArchetype(const TypesBitmap& types, TypeManager* type_manager) {
                int index = archetypes.size();
                archetypes.push_back({ Archetype(types, type_manager) , std::vector<Edge>() });
                int last_visited = -1;

                int type_count = types.GetTypesCount();
                for (int i = 0; i < archetype_with_most_types.size(); i++) {
                    AddArchetypeAsEdgeToNodes(archetype_with_most_types[i], index, types, type_count, &last_visited);
                }
                ClearVisited(last_visited);

                if (type_count > most_types_count) {
                    archetype_with_most_types.clear();
                    archetype_with_most_types.push_back(index);
                } else if (type_count == most_types_count) {
                    archetype_with_most_types.push_back(index);
                }

                return index;
            }

            int GetArchetype(const TypesBitmap& types) {
                int last_visited = -1;
                int res = -1;
                for (int i = 0; i < archetype_with_most_types.size(); i++) {
                    res = GetArchetypeRecursive(archetype_with_most_types[i], types, &last_visited);
                    if (res != -1) {
                        break;
                    }
                }
                ClearVisited(last_visited);

                return res;
            }

            Archetype& GetArchetypeByIndex(int i) {
                return archetypes[i].archetype;
            }

            template<typename F>
            void CallWithIntersectingArchetypes(const TypesBitmap& types, const F& callback) {
                int last_visited = -1;
                bool stop = false;
                for (int i = 0; i < archetype_with_most_types.size(); i++) {
                    if (!stop) {
                        CallWithIntersectingArchetypesRecursive(archetype_with_most_types[i], types, &last_visited, callback, &stop);
                    }
                }
                ClearVisited(last_visited);
            }
    };
}
