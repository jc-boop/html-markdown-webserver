 #include "crud_request_handler.h"
 #include <iostream>
 #include <fstream>
 //#include <iterator>
 #include <boost/filesystem.hpp>
 //#include <boost/range/iterator_range.hpp>

// Using so that I can iterate over files in a directory
//using namespace boost::filesystem;

std::stringbuf CRUDRequestHandler::get_JSON_id(int id) {
    std::stringbuf str_buffer;
    std::ostream os (&str_buffer);

    // should there be a newline here?
    os << "{ \"id\": " << id << " }\n";
    return str_buffer;
}

http::status CRUDRequestHandler::serve(http::request<http::string_body> req, 
                                       http::response<http::string_body>& res) {
    bool error_flag = false;
    std::string method = req.method_string().data();
    std::string target = req.target().data();
    std::string entity_id;
    std::string entity;
    std::string id;

    // Request URI processing 
    try {
        if (target.substr(0,location_.length()) != location_) { 
            error_flag = true; // rename to not found flag later
        }

        if (location_[location_.length() - 1] != '/') {
            entity_id = target.substr(location_.length() + 1);
        }
        else {
            entity_id = target.substr(location_.length());
        }
        std::size_t pos = entity_id.find("/");

        // "/" not found
        if (pos == std::string::npos) {
            entity = entity_id;
            id = "";
        }
        // "/" found
        else {
            entity = entity_id.substr(0, pos);
            id = (pos >= entity_id.length()) ? "" : entity_id.substr(pos+1);
        }
    } 
    catch (...) { }
    
    if (method == "POST") {
        return create(entity, req, res);
    }
    else if (method == "GET") {
        if (id != "") {
            return retrieve(entity_id, req, res);
        }
        else {
            return list(entity, req, res);
        }
    }
    else if (method == "PUT") {
        return update(entity_id, req, res);
    }
    else if (method == "DELETE") {
        return crud_delete(entity_id, req, res);
    }
    else {
        res.result(http::status::not_implemented);
        return res.result();
    }
}

http::status CRUDRequestHandler::create(std::string entity, http::request<http::string_body> req, http::response<http::string_body>& res) {
    std::string abs_path = map_request_path_to_file_path(std::string(req.target())); 
    std::string file_list;

    filesystem_->listFiles(abs_path, file_list);

    int max_id = 0;
    int curr_id = 0;

    std::vector<std::string> files_vect;
    std::stringstream files_ss(file_list);

    while(files_ss.good()) {
        std::string substr;
        getline(files_ss, substr, ',');
        files_vect.push_back(substr);
    }

    if (file_list.size() == 0) {
        curr_id = 0;
    }
    else {
        for(int i = 0; i < files_vect.size(); i++) {
            std::string entry = files_vect[i];
            std::cout << entry << "\n";
            int entry_as_int;
            try {
                entry_as_int = stoi(entry);
            }
            catch (...) { }
            if (entry_as_int > max_id)
                max_id = entry_as_int;
        }
    }

    curr_id = max_id + 1;

    abs_path += "/" + to_string(curr_id);

    if (!filesystem_->writeFile(abs_path, req.body())) {
        res.result(http::status::internal_server_error);
        return http::status::internal_server_error;
    }

    std::stringbuf body = get_JSON_id(curr_id);
    std::string body_as_str = body.str();

    res.result(http::status::created);
    res.version(11);
    res.set(http::field::content_type, "application/json");
    res.body() = body_as_str;
    res.prepare_payload();
    return http::status::created;
}

http::status CRUDRequestHandler::retrieve(std::string entity_id, http::request<http::string_body> req, http::response<http::string_body>& res) {
    std::string file_path = map_request_path_to_file_path(std::string(req.target()));
    std::string file_content = ""; 
    res.version(11);
    if (filesystem_->readFile(file_path, file_content) == true)
    {
        res.result(http::status::ok);
        res.set(http::field::content_type, "application/json");
        res.body() = file_content;
    }
    else
    {
        res.result(http::status::not_found); 
    }
    res.prepare_payload();
    return res.result(); 
}

http::status CRUDRequestHandler::update(std::string entity_id, http::request<http::string_body> req, http::response<http::string_body>& res) {
    std::string abs_path = map_request_path_to_file_path(std::string(req.target()));

    if (!filesystem_->writeFile(abs_path, req.body())) {
        res.result(http::status::internal_server_error);
        return http::status::internal_server_error;
    }

    res.result(http::status::ok);
    res.version(11);
    res.prepare_payload();

    return http::status::ok;
}

http::status CRUDRequestHandler::crud_delete(std::string entity_id, http::request<http::string_body> req, http::response<http::string_body>& res) {
    std::string file_path = map_request_path_to_file_path(std::string(req.target()));
    res.version(11); 
    filesystem_->deleteFile(file_path);
    res.result(http::status::ok);
    res.prepare_payload();
    return res.result(); 
}

http::status CRUDRequestHandler::list(std::string entity, http::request<http::string_body> req, http::response<http::string_body>& res) {
    std::string file_path = map_request_path_to_file_path(std::string(req.target()));
    std::vector<std::string> filesList = {};
    std::string file_list = ""; 
    
    if (filesystem_->listFiles(file_path, file_list) == true)
    {
        std::string file_list_json = "[" + file_list.substr(0, file_list.size()-1) + "]";
        res.body() = file_list_json; 
    }
    else
    {
        std::string file_list_json = "[]";
        res.body() = file_list_json; 
    }
    res.set(http::field::content_type, "application/json");
    res.result(http::status::ok); 
    res.prepare_payload();
    return res.result(); 
}

std::string CRUDRequestHandler::map_request_path_to_file_path(std::string request_uri)
{
    int prefix_size = location_.size(); 
    if (location_[prefix_size-1] != '/')
    {
        prefix_size += 1; 
    }
    std::string temp0 = request_uri.substr(prefix_size); 
    std::string temp; 
    if (data_path_[(data_path_.size())-1] == '/')
        temp= data_path_ + temp0;
    else
        temp= data_path_ + "/" + temp0;
    return temp;   
} 
