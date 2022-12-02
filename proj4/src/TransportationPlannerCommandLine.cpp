#include "TransportationPlannerCommandLine.h"
#include "StringUtils.h"
#include "GeographicUtils.h"
#include <math.h>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <sstream>

int g_lastChecked = -1; // -1 = nothing, 0 = shortest, 1 = fastest
CTransportationPlanner::TNodeID g_lastSrcID;
CTransportationPlanner::TNodeID g_lastDestID;
std::vector<CTransportationPlanner::TTripStep> g_fastestPath;
std::vector<CTransportationPlanner::TNodeID> g_shortestPath;
double g_distShortest;
double g_timeFastest;

struct CTransportationPlannerCommandLine::SImplementation {

    public: 
        std::shared_ptr<CDataSource> m_cmdsrc;
        std::shared_ptr<CDataSink> m_outsink;
        std::shared_ptr<CDataSink> m_errsink;
        std::shared_ptr<CDataFactory> m_results;
        std::shared_ptr<CTransportationPlanner> m_planner;
        
        std::vector<std::string> m_cmds;
        std::unordered_map<std::string, int> m_cmdMap = {
            // key = command, value = minimum number of args (rest get cut off)
            {"help", 0}, // args dont matter
            {"exit", 0}, // args dont matter
            {"count", 0}, // args dont matter
            {"node", 1}, // node 123
            {"fastest", 2}, // fastest 123 456
            {"shortest", 2}, // shortest 123 456
            {"save", 0},
            {"print", 0}
        };
        
        std::unordered_map<CTransportationPlanner::ETransportationMode, std::string> m_transMap = {
            {CTransportationPlanner::ETransportationMode::Bus, "Bus"},
            {CTransportationPlanner::ETransportationMode::Walk, "Walk"},
            {CTransportationPlanner::ETransportationMode::Bike, "Bike"}
        };


        SImplementation(
            std::shared_ptr<CDataSource> cmdsrc, 
            std::shared_ptr<CDataSink> outsink, 
            std::shared_ptr<CDataSink> errsink, 
            std::shared_ptr<CDataFactory> results, 
            std::shared_ptr<CTransportationPlanner> planner) {

                m_cmdsrc = cmdsrc;
                m_outsink = outsink;
                m_errsink = errsink;
                m_results = results;
                m_planner = planner;
            
            };
            
};

CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(
    std::shared_ptr<CDataSource> cmdsrc, 
    std::shared_ptr<CDataSink> outsink, 
    std::shared_ptr<CDataSink> errsink, 
    std::shared_ptr<CDataFactory> results, 
    std::shared_ptr<CTransportationPlanner> planner) {

        DImplementation = std::make_unique<SImplementation>(
            SImplementation(cmdsrc, outsink, errsink, results, planner));

        std::string tmp_cmd;
        char tmp_char;
        while (!DImplementation -> m_cmdsrc -> End()) {
            DImplementation -> m_cmdsrc -> Get(tmp_char);
            
            if (tmp_char == '\n') {
                DImplementation -> m_cmds.push_back(tmp_cmd);
                tmp_cmd.clear();
                continue;
            }
            
            tmp_cmd += tmp_char;
        }
}

CTransportationPlannerCommandLine::~CTransportationPlannerCommandLine() {};

bool CTransportationPlannerCommandLine::ProcessCommands() {
    std::vector<char> prompt = {'>', ' '};

    for (int i = 0; i < DImplementation -> m_cmds.size(); i++) {
        
        std::vector<std::string> args = StringUtils::Split(DImplementation -> m_cmds[i], " ");

        std::string cmd = args[0];

        DImplementation -> m_outsink -> Write(prompt);

        // invalid command 😡
        if (DImplementation->m_cmdMap.find(cmd) == DImplementation->m_cmdMap.end()) {
            std::string err_msg = "Unknown command \"" + cmd + "\" type help for help.\n"; 
            std::cerr << err_msg;
            std::vector<char> err_msg_buf(err_msg.begin(), err_msg.end());
            DImplementation -> m_errsink -> Write(err_msg_buf);
        }

        if (cmd == "help") {
            std::string help_msg = "------------------------------------------------------------------------\n"
                                    "help     Display this help menu\n"
                                    "exit     Exit the program\n"
                                    "count    Output the number of nodes in the map\n"
                                    "node     Syntax \"node [0, count)\" \n"
                                    "         Will output node ID and Lat/Lon for node\n"
                                    "fastest  Syntax \"fastest start end\" \n"
                                    "         Calculates the time for fastest path from start to end\n"
                                    "shortest Syntax \"shortest start end\" \n"
                                    "         Calculates the distance for the shortest path from start to end\n"
                                    "save     Saves the last calculated path to file\n"
                                    "print    Prints the steps for the last calculated path\n";
            std::cout << help_msg;
            std::vector<char> help_msg_buf(help_msg.begin(), help_msg.end());
            DImplementation -> m_outsink -> Write(help_msg_buf);
        }
        
        else if (cmd == "exit") {
            return true; // true lets transplanner know to exit loop
        }

        else if (cmd == "count") {
            size_t count = DImplementation -> m_planner -> NodeCount();
            std::string count_msg = std::to_string(count) + " nodes\n";
            std::cout << count_msg;
            std::vector<char> count_msg_buf(count_msg.begin(), count_msg.end());
            DImplementation -> m_outsink -> Write(count_msg_buf);
            
        }

        else if (cmd == "node") {
            std::string invalid_node_msg = "Invalid node ";
            bool invalid_cmd = false;
            size_t nodeIndex;
            
            if (args.size() < 2) {
                invalid_node_msg += "command, see help.\n";
                invalid_cmd = true;
            }

            else {
                // try converting to long
                try {
                    // Is this bigger than our node count
                    if (std::stol(args[1]) < 0 || std::stol(args[1]) >= 
                        DImplementation -> m_planner -> NodeCount()) {
                            invalid_node_msg += "parameter, see help.\n";
                            invalid_cmd = true;
                    }
                    else {
                        nodeIndex = (size_t) std::stol(args[1]);
                    }
                }

                catch(std::invalid_argument) {
                    invalid_node_msg += "parameter, see help.\n";
                    invalid_cmd = true;
                }
            }

            if (invalid_cmd) {
                std::cerr << invalid_node_msg;
                std::vector<char> node_msg_buf(invalid_node_msg.begin(), invalid_node_msg.end());
                DImplementation -> m_errsink -> Write(node_msg_buf);
            }
            // 1) Take the arg[1] & grab a corresponding Node
            // 2) Store the id
            // 3) Use GeographicUtils
            // CalculateBearing(TLocation src, TLocation dest) --> BearingToDirection(bearing)
            // ConvertLLToDMS(TLocation loc)
            else {
                std::shared_ptr<CStreetMap::SNode> node = 
                    DImplementation -> m_planner -> SortedNodeByIndex(nodeIndex);
                CTransportationPlanner::TNodeID nodeID = node -> ID();
                CStreetMap::TLocation loc = node -> Location();

                std::string direction = SGeographicUtils::ConvertLLToDMS(loc);
                
                std::string node_msg = "Node " + std::to_string(nodeIndex) + 
                                       ": id = " + std::to_string(nodeID) + " is at " +
                                       direction + "\n"; 
                std::cout << node_msg;
                std::vector<char> node_msg_buf(node_msg.begin(), node_msg.end());
                DImplementation -> m_outsink -> Write(node_msg_buf);
            }
            

        }

        else if (cmd == "fastest") {
            std::string invalid_fastest_msg = "Invalid fastest ";
            bool invalid_cmd = false;

            CTransportationPlanner::TNodeID srcID, destID;

            // Less than 3 args
            if (args.size() < 3) {
                invalid_fastest_msg += "command, see help.\n";
                invalid_cmd = true;
            }

            else {
                // 1) Check arg[1] & arg[2] --> Try to convert to longs (TNodeID)
                // 2) Check if valid TNodeIDs
                // 3) Find fastest path & store time
                try {
                    srcID = (CTransportationPlanner::TNodeID) std::stol(args[1]);
                    destID = (CTransportationPlanner::TNodeID) std::stol(args[2]);
                }
                
                catch (std::invalid_argument) {
                    invalid_fastest_msg += "parameter, see help.\n";
                    invalid_cmd = true;
                }
            }

            if (invalid_cmd) {
                std::cerr << invalid_fastest_msg;
                std::vector<char> invalid_fastest_msg_buf(invalid_fastest_msg.begin(), invalid_fastest_msg.end());
                DImplementation -> m_errsink -> Write(invalid_fastest_msg_buf);
                g_lastChecked = -1;
            }

            else {
                // time in seconds
                // whole hrs - whole mins - seconds
                // time = 0 --> No path found
                double time = DImplementation -> m_planner -> FindFastestPath(srcID, destID, 
                                                                              g_fastestPath);
                g_timeFastest = time;
                if (time == 0) {
                    std::string no_path_msg = "Unable to find fastest path from " + std::to_string(srcID) + 
                                              " to " + std::to_string(destID) + ".\n";
                    std::cerr << no_path_msg;
                    std::vector<char> no_path_msg_buf(no_path_msg.begin(), no_path_msg.end());
                    DImplementation -> m_errsink -> Write(no_path_msg_buf);
                    g_lastChecked = -1;
                }
                else {
                    std::string hrs = floor(time) != 0 ? std::to_string((int) floor(time)) + " hr" : "";
                    std::string mins = floor((time - floor(time)) * 60) != 0 ? std::to_string((int) floor(floor((time - floor(time)) * 60))) + " min" : "";
                    std::string secs = floor(((time - floor(time)) * 60 - floor((time - floor(time)) * 60)) * 60) != 0 ? std::to_string((int) floor(((time - floor(time)) * 60 - floor((time - floor(time)) * 60)) * 60)) + " sec" : "";

                    std::string fastest_msg = "Fastest path takes ";
                    int time_vars = 0;

                    if (!hrs.empty()) {
                        fastest_msg += hrs;
                        time_vars++;
                    }

                    if (!mins.empty()) {
                        if (time_vars > 0) {
                            fastest_msg += " ";
                        }
                        fastest_msg += mins;
                        time_vars++;
                    }

                    if (!secs.empty()) {
                        if (time_vars > 0) {
                            fastest_msg += " ";
                        }
                        fastest_msg += secs;
                    }
                    
                    fastest_msg += ".\n";
                    std::cout << fastest_msg;
                    std::vector<char> fastest_msg_buf(fastest_msg.begin(), fastest_msg.end());
                    DImplementation -> m_outsink -> Write(fastest_msg_buf);
                    g_lastChecked = 1;
                    g_lastSrcID = srcID;
                    g_lastDestID = destID;
                }
            }
            
        }

        else if (cmd == "shortest") {
            std::string invalid_shortest_msg = "Invalid shortest ";
            bool invalid_cmd = false;

            CTransportationPlanner::TNodeID srcID, destID;

            // Less than 3 args
            if (args.size() < 3) {
                invalid_shortest_msg += "command, see help.\n";
                invalid_cmd = true;
            }

            else {
                // 1) Check arg[1] & arg[2] --> Try to convert to longs (TNodeID)
                // 2) Check if valid TNodeIDs
                // 3) Find shortest path & store dist
                try {
                    srcID = (CTransportationPlanner::TNodeID) std::stol(args[1]);
                    destID = (CTransportationPlanner::TNodeID) std::stol(args[2]);
                }
                
                catch (std::invalid_argument) {
                    invalid_shortest_msg += "parameter, see help.\n";
                    invalid_cmd = true;
                }
            }

            if (invalid_cmd) {
                std::cerr << invalid_shortest_msg;
                std::vector<char> invalid_shortest_msg_buf(invalid_shortest_msg.begin(), invalid_shortest_msg.end());
                DImplementation -> m_errsink -> Write(invalid_shortest_msg_buf);
                g_lastChecked = -1;
            }

            else {
                // time in seconds
                // whole hrs - whole mins - seconds
                // time = 0 --> No path found
                double dist = DImplementation -> m_planner -> FindShortestPath(srcID, destID, 
                                                                               g_shortestPath);
                g_distShortest = dist;
                // std::cout << "Distance: " << dist << std::endl;
                if (dist == 0) {
                    std::string no_path_msg = "Unable to find shortest path from " + std::to_string(srcID) + 
                                              " to " + std::to_string(destID) + ".\n";
                    std::cerr << no_path_msg;
                    std::vector<char> no_path_msg_buf(no_path_msg.begin(), no_path_msg.end());
                    DImplementation -> m_errsink -> Write(no_path_msg_buf);
                    g_lastChecked = -1;
                }
                else {
                    std::string shortest_msg = "Shortest path is ";
                    
                    if (dist >= 1) {
                        double rawMile =std::ceil(dist * 100.0) / 100.0;
                        std::stringstream tmpStream;
                        tmpStream << std::fixed << std::setprecision(1) << rawMile;
                        shortest_msg += tmpStream.str() + " mi";
                    }
                    else {
                        std::string ft = std::to_string((int) floor((dist - floor(dist)) * 5280)) + " ft";
                        shortest_msg += ft;
                    }

                    shortest_msg += ".\n";
                    std::cout << shortest_msg;
                    std::vector<char> shortest_msg_buf(shortest_msg.begin(), shortest_msg.end());
                    DImplementation -> m_outsink -> Write(shortest_msg_buf);
                    g_lastChecked = 0;
                    g_lastSrcID = srcID;
                    g_lastDestID = destID;
                }
            }
        }
        // Shortest: srcNodeID_destNodeID_shortestDistmi.csv
        // Fastest: srcNodeID_destNodeID_fastestTimehr.csv
        else if (cmd == "save") {
        
            std::string header = "mode,node_id\n";
            std::vector<char> header_buf(header.begin(), header.end());

                if (g_lastChecked == -1) { //nothing checked
                    std::string invalid_save_msg = "No valid path to save, see help.\n";
                    std::cerr << invalid_save_msg;
                    std::vector<char> invalid_save_msg_buf(invalid_save_msg.begin(), invalid_save_msg.end());
                    DImplementation -> m_errsink -> Write(invalid_save_msg_buf);
                }
                
                if (g_lastChecked == 0) { // shortest
                    std::string fileName = std::to_string(g_shortestPath.front()) + "_" +
                        std::to_string(g_shortestPath.back()) + "_" + 
                        std::to_string(g_distShortest) + "mi.csv";
                    auto resultsSink = DImplementation -> m_results -> CreateSink(fileName);

                    resultsSink -> Write(header_buf);
                    for (auto id : g_shortestPath) {
                        std::string tmpLine = "Walk," + std::to_string(id) + "\n";
                        std::vector<char> tmpLineBuf(tmpLine.begin(), tmpLine.end());
                        resultsSink -> Write(tmpLineBuf);
                    }
                    
                    std::string save_msg = "\"Path saved to <results>/" + fileName + "\n\"";
                    std::cout << save_msg;
                    std::vector<char> save_msg_buf(save_msg.begin(), save_msg.end());
                    DImplementation -> m_outsink -> Write(save_msg_buf);
                }
                
                if (g_lastChecked == 1) { // fastest
                    std::string fileName = std::to_string(g_lastSrcID) + "_" +
                        std::to_string(g_lastDestID) + "_" + 
                        std::to_string(g_timeFastest) + "hr.csv";
                    auto resultsSink = DImplementation -> m_results -> CreateSink(fileName);
                    
                    resultsSink -> Write(header_buf);
                    for (int i = 0; i < g_fastestPath.size(); i++) {
                        std::string tmpLine = DImplementation->m_transMap[g_fastestPath[i].first] + "," + 
                        std::to_string(g_fastestPath[i].second);
                        if (i != g_fastestPath.size() - 1) {
                            tmpLine += "\n";
                        }
                        std::vector<char> tmpLineBuf(tmpLine.begin(), tmpLine.end());
                        resultsSink -> Write(tmpLineBuf);
                    }

                    std::string save_msg = "Path saved to <results>/" + fileName + "\n";
                    std::cout << save_msg;
                    std::vector<char> save_msg_buf(save_msg.begin(), save_msg.end());
                    DImplementation -> m_outsink -> Write(save_msg_buf);
                }

        }

        else if (cmd == "print") {
            // Shortest Path: Use adjacent TNodeIDs & grab nodes --> Grab location
            // Use CalculateBearing(TLocation src, TLocation, dest)
            // Use BearingToDirection(double bearing) to get direction
            // ConvertLLToDMS(TLocation loc) for start & end
            
            // Invalid
            if (g_lastChecked == -1) {
                std::string invalid_print_msg = "No valid path to print, see help.\n";
                std::cerr << invalid_print_msg;
                std::vector<char> invalid_print_msg_buf(invalid_print_msg.begin(), invalid_print_msg.end());
                DImplementation -> m_errsink -> Write(invalid_print_msg_buf);
            }

            // Shortest
            if (g_lastChecked == 0) {
                std::vector< CTransportationPlanner::TTripStep > path;

                for (int i = 0; i < g_shortestPath.size(); i++) {
                    path.push_back(CTransportationPlanner::TTripStep(
                        CTransportationPlanner::ETransportationMode::Walk, 
                        g_shortestPath[i]));
                }
                std::vector< std::string > shortestDesc;

                DImplementation -> m_planner -> GetPathDescription(path, shortestDesc);

                std::string print_msg;
                for (auto s : shortestDesc) {
                    print_msg += s + '\n';
                }
                std::cout << print_msg;
                std::vector<char> print_msg_buf(print_msg.begin(), print_msg.end());
                DImplementation -> m_outsink -> Write(print_msg_buf);
            }

            // Fastest
            if (g_lastChecked == 1) {
                std::vector< CTransportationPlanner::TTripStep > path = g_fastestPath;

                std::vector< std::string > fastestDesc;
                
                DImplementation -> m_planner -> GetPathDescription(path, fastestDesc);
                
                std::string print_msg;
                for (auto s : fastestDesc) {
                    print_msg += s + '\n';
                }
                std::cout << print_msg;
                std::vector<char> print_msg_buf(print_msg.begin(), print_msg.end());
                DImplementation -> m_outsink -> Write(print_msg_buf);
            }
            
        }

    }

    DImplementation -> m_outsink -> Write(prompt);
    return false;
}