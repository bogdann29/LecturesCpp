#include "serialization.h"

uint32_t arr_to_num(const uint8_t id[], int num){
    uint32_t res = 0;
    for(int i = num-1; i >= 0; --i)
        res = (res << 8) | id[i];
    return res;
}


auto Serializer::readData(uint64_t& max_deg){
    graph edges;

    std::ifstream in(inputFile);
    std::string line;
    uint32_t id1, id2;
    uint8_t weight;
    uint32_t tmp;

    max_deg = 0;

    while(std::getline(in, line)){
        std::istringstream s(line);
        s >> id1 >> id2 >> tmp;
        weight = static_cast<uint8_t>(tmp);
        if(id1 != id2){
            edges[id1].push_back({id2, weight});
            edges[id2].push_back({id1, weight});
        }
        else{
            edges[id1].push_back({id2, weight});
        }
        max_deg = std::max(edges[id1].size(), max_deg);
        max_deg = std::max(edges[id2].size(), max_deg);
    }
    in.close();
    
    return std::move(edges);
}


void Serializer::writeData(){
    std::ofstream out(outputFile, std::ios_base::binary);
    uint32_t len = static_cast<uint32_t>(gr.size());

    out.write(reinterpret_cast<char*>(&len), sizeof(len));

    for(auto it = gr.begin(); it != gr.end(); ++it){
        auto id = it->first;
        auto size = it->second.size();
        out.write(reinterpret_cast<char*>(&id), sizeof(id));

        uint8_t type = 0;
        if(size < 256)
            type = static_cast<uint8_t>(size);
        out.write(reinterpret_cast<char*>(&type), sizeof(type));
        if(!type){
            uint8_t ar_size[3];
            for(auto i = 0; i < 3; ++i){
                ar_size[i] = size & 0xff;
                size = size >> 8;
            }
            out.write(reinterpret_cast<char*>(ar_size), 3);
        }

        for (auto& el: it->second) {
            auto id2 = el.first;
            auto w = el.second;
            out.write(reinterpret_cast<char*>(&id2), sizeof(id2));
            out.write(reinterpret_cast<char*>(&w), sizeof(w));
        }
    }
    out.close();
}


void Serializer::Serialization(){
    uint64_t max_deg;
    auto edges = readData(max_deg);

    // transform data
    std::vector<std::vector<uint32_t>> vertex_by_degree(max_deg+1);

    for(const auto& [key, value]: edges){
        vertex_by_degree[value.size()].push_back(key);
    }

    std::unordered_set<uint32_t> visited;
    for(auto it = vertex_by_degree.rbegin(); it != vertex_by_degree.rend(); ++it){
        for(auto id: *it){
            std::vector<std::pair<uint32_t, uint8_t>> connected;
            for(auto vertex: edges[id]){
                if(!visited.count(vertex.first)){
                    connected.push_back(vertex);
                }
            }
            if(connected.empty())
                continue;
            
            visited.insert(id);
            gr[id] = connected;
        }
    }

    writeData();
}


void Deserializer::Deserialization(){
    std::ifstream in(inputFile, std::ios_base::binary);
    std::ofstream out(outputFile);

    uint32_t id_1, id_2;
    uint8_t type, weight;
    uint8_t size_ar[3];
    uint32_t size, len, iter = 0;
    in.read(reinterpret_cast<char*>(&len), sizeof(len));

    while(iter++ < len){
        in.read(reinterpret_cast<char*>(&id_1), sizeof(id_1));
        in.read(reinterpret_cast<char*>(&type), sizeof(type));
        size = static_cast<uint32_t>(type);
        if(!size){
            in.read(reinterpret_cast<char*>(size_ar), 3);
            size = arr_to_num(size_ar, 3);
        }
        for(int i = 0; i < size; ++i){
            in.read(reinterpret_cast<char*>(&id_2), sizeof(id_2));
            in.read(reinterpret_cast<char*>(&weight), sizeof(weight));
            out << id_1 << '\t' << id_2 << '\t' << (int)weight << '\n';
        }
    }

    out.close();
    in.close();
}