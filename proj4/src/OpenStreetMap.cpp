#include "OpenStreetMap.h"
#include "XMLReader.h"
#include <iostream>
#include <unordered_map>

struct COpenStreetMap::SImplementation {
    public:
        std::shared_ptr<CXMLReader> m_src;
        
        // Holds pointers to nodes & ways
        std::vector<std::shared_ptr<SNode>> t_nodes;
        std::unordered_map<TNodeID, std::shared_ptr<SNode>> t_nodes_map;
        std::vector<std::shared_ptr<SWay>> t_ways;
        std::unordered_map<TWayID, std::shared_ptr<SWay>> t_ways_map;


        // Create our own Node by inheriting from SNode
        struct S_Node: public CStreetMap::SNode {
            // Node id, node location, & node attributes
            TNodeID n_id;
            TLocation n_loc;
            std::vector<SXMLEntity> n_atts;

            // Constructor: Set node_id, node_location, & node attributes
            S_Node(const TNodeID &id, const TLocation &loc, const std::vector<SXMLEntity> &atts) {
                n_id = id;
                n_loc = loc;
                n_atts = atts;
            }

            TNodeID ID() const noexcept override {
                // Return node_id
                return n_id;
            }

            TLocation Location() const noexcept override {
                // Return pair<lat, lon> (node location)
                return n_loc;
            }

            std::size_t AttributeCount() const noexcept override {
                // Return num of node atts
                return (std::size_t) n_atts.size();
            }

            std::string GetAttributeKey(std::size_t index) const noexcept override {
                // Index greater than or equal to number of atts
                if (index >= AttributeCount()) {
                    return "";
                }
                return n_atts.at(index).DAttributes.at(0).second;
            }

            bool HasAttribute(const std::string &key) const noexcept override {
                // Checks to look for key in atts --> true if found else false
                for (int i = 0; i < n_atts.size(); i++) {
                    if (n_atts.at(i).DAttributes.at(0).second == key) {
                        return true;
                    }
                }
                return false;
            }

            std::string GetAttribute(const std::string &key) const noexcept override {
                // Looks for key in atts --> Returns val if found else ""
                for (int j = 0; j < n_atts.size(); j++) {
                    // Grabs jth att from node --> First att inside jth at --> Grabs second (value)
                    if (n_atts.at(j).DAttributes.at(0).second == key) {
                        return n_atts.at(j).DAttributes.at(1).second;
                    }
                }
                return "";
            }

            ~S_Node() {};
        };

        // Create our own Way by inheriting from SWay
        struct S_Way: public CStreetMap::SWay {
            // Way nodes, way ids, & way_atts
            TWayID w_id;
            std::vector<S_Node> w_nodes;
            std::vector<SXMLEntity> w_atts;
            
            // Constructor: Set way_id, way_nodes, & way_atts
            S_Way(const TWayID &id, const std::vector<S_Node> &nodes, const std::vector<SXMLEntity> &atts) {
                w_id = id;
                w_nodes = nodes;
                w_atts = atts;
            }

            TWayID ID() const noexcept override {
                // Return way id
                return w_id;
            }

            std::size_t NodeCount() const noexcept override {
                // Return nodes in way
                return (std::size_t) w_nodes.size();
            }

            TNodeID GetNodeID(std::size_t index) const noexcept override {
                // Index is greater than or equal to node count --> return InvalidNodeID
                // Else return ID of node at index
                if (index >= NodeCount()) {
                    return InvalidNodeID;
                }
                return w_nodes[index].ID();
            }

            // Returns num of atts stored inside the way
            std::size_t AttributeCount() const noexcept override {
                return (std::size_t) w_atts.size();
            }

            std::string GetAttributeKey(std::size_t index) const noexcept override {
                // Index greater than or equal to num of atts --> return ""
                // Else return indexed att key
                if (index >= AttributeCount()) {
                    return "";
                }
                // Get way att at index --> Go into first entity of SXMLEntity
                // Check first attribute & grab second (key val)
                return w_atts[index].DAttributes.at(0).second;
            }

            bool HasAttribute(const std::string &key) const noexcept override {
                // Checks atts in way for key --> true if found else false
                for (int j = 0; j < w_atts.size(); j++) {
                    // Gets jth att --> Gets first att within jth att --> Grabs second (key val)
                    if (w_atts[j].DAttributes.at(0).second == key) {
                        return true;
                    }
                }
                return false;
            }

            std::string GetAttribute(const std::string &key) const noexcept override {
                // Checks atts in way for key --> return val of att at key if found else ""
                for (int k = 0; k < w_atts.size(); k++) {
                    // Grabs kth att --> Grabs first att within kth att --> Grabs second (key val)
                    if (w_atts[k].DAttributes.at(0).second == key) {
                        return w_atts[k].DAttributes.at(1).second;
                    }
                }
                return "";
            }

            ~S_Way() {};  
        };

        // IMPORTANT: Need to split data into nodes & ways here from src
        SImplementation(std::shared_ptr<CXMLReader> src) {
            m_src = src;
            
            SXMLEntity data;

            // Node Contents: Node_id, Node_loc, Node_atts
            // nodes_map: Used to keep track of node instances that we create for Way later
            // bool node: Used to keep track of whether a node is read or not
            TNodeID node_id;
            TLocation node_loc;
            std::vector<SXMLEntity> node_atts;
            std::unordered_map<TNodeID, S_Node> nodes_map;
            bool node = false;
            
            // Way Contents: Way_id, Way_nodes (use nodes_map to grab nodes), & Way_atts (tags)
            // bool way: Used to keep track of whether a way is read or not
            TWayID way_id;
            std::vector<S_Node> way_nodes;
            std::vector<SXMLEntity> way_atts;
            bool way = false;


            // While SXMLEntities are still being read, parse the data
            // Note: Nodes - id, node atts (id, lat, lon), tags (key : values)
            // Ways - id, ref (nodes in ways), tags (way atts: key : values)
            while (m_src -> ReadEntity(data, true)) {
                // Node --> Store node_id, node_loc & set node = true to indicate node found
                if (data.DNameData == "node" && data.DType == SXMLEntity::EType::StartElement) {
                    node_id = (TNodeID) std::stoi(data.DAttributes.at(0).second);
                    node_loc = (TLocation) {std::stod(data.DAttributes.at(1).second), 
                                            std::stod(data.DAttributes.at(2).second)};
                    node = true;
                }

                // Way --> Store way_id & set way = true to indicate way found
                else if (data.DNameData == "way" && data.DType == SXMLEntity::EType::StartElement) {
                    way_id = (TWayID) std::stoi(data.DAttributes.at(0).second);
                    way = true;
                }
                
                // Nodes in way --> Use nodes_map to find corresponding S_Node instance to store for that way
                else if (data.DNameData == "nd" && data.DType == SXMLEntity::EType::StartElement) {
                    way_nodes.push_back(nodes_map.at(std::stoi(data.DAttributes.at(0).second)));
                }

                // Tags for nodes: Store the SXMLEntity for node
                else if (data.DNameData == "tag" && node && data.DType == SXMLEntity::EType::StartElement) {
                    node_atts.push_back(data);
                }

                // Tags for way: Store the SXMLEntity for way
                else if (data.DNameData == "tag" && way && data.DType == SXMLEntity::EType::StartElement) {
                    way_atts.push_back(data);
                }
                
                // If end tag of node is read, push the node into t_nodes + insert into nodes_map for ways + clear node_atts + set node = false
                if (node && data.DNameData == "node" && data.DType == SXMLEntity::EType::EndElement) {
                    S_Node temp_node = S_Node(node_id, node_loc, node_atts);
                    auto temp_node_ptr = std::make_shared<S_Node>(temp_node);
                    t_nodes.push_back(temp_node_ptr);
                    t_nodes_map[(TNodeID) node_id] = temp_node_ptr;
                    nodes_map.insert(std::pair<TNodeID, S_Node>(node_id, temp_node));
                    node_atts.clear();
                    node = false;
                }

                // If end tag of way is read, push the way into t_ways + clear way_atts + set way = false
                if (way && data.DNameData == "way" && data.DType == SXMLEntity::EType::EndElement) {
                    S_Way temp_way = S_Way(way_id, way_nodes, way_atts);
                    auto temp_way_ptr = std::make_shared<S_Way>(temp_way);
                    t_ways.push_back(temp_way_ptr);
                    t_ways_map[(TWayID) way_id] = temp_way_ptr;
                    way_nodes.clear();
                    way_atts.clear();
                    way = false;
                }

            };

        };

        ~SImplementation() {};
};

COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src) {
    DImplementation = std::make_unique<SImplementation>(SImplementation(src));
}

COpenStreetMap::~COpenStreetMap() {};

std::size_t COpenStreetMap::NodeCount() const noexcept {
    // Return size of t_nodes (total nodes)
    return (std::size_t) DImplementation -> t_nodes.size();
}

std::size_t COpenStreetMap::WayCount() const noexcept {
    // Return size of t_ways (total ways)
    return (std::size_t) DImplementation -> t_ways.size();
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept {
    // Index greater than or equal to num of nodes --> Return nullptr else node at queried index
    if (index >= NodeCount()) {
        return nullptr;
    }
    return DImplementation -> t_nodes.at(index);
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(CStreetMap::TNodeID id) const noexcept {
    if(DImplementation->t_nodes_map.find(id) == DImplementation->t_nodes_map.end()) {
        return nullptr;
    }
    return DImplementation->t_nodes_map[id];
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept {
    // Index greater than or equal to num of ways --> Return nullptr
    // Else way at queried index
    if (index >= WayCount()) {
        return nullptr;
    }
    return DImplementation -> t_ways.at(index);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(CStreetMap::TWayID id) const noexcept {
    if(DImplementation->t_ways_map.find(id) == DImplementation->t_ways_map.end()) {
        return nullptr;
    }
    return DImplementation->t_ways_map[id];
}