 #include "markdown_request_handler.h"
 #include <iostream>
 #include <fstream>
 //#include <iterator>
 #include <boost/filesystem.hpp>
 //#include <boost/range/iterator_range.hpp>
 #include "markdown.h"
 #include "html_assets.h"

http::status MarkdownRequestHandler::serve(http::request<http::string_body> req, 
    http::response<http::string_body>& res) {
// TODO: parse the request URL and call the appropriate function
    bool error_flag = false;
    std::string method = std::string(req.method_string());
    std::string target = std::string(req.target());
    std::string entity_id;
    std::string entity;
    std::string id;
    
    // Do parsing based on URL
    size_t action_pos = target.find("/markdown/");
    size_t query_pos = target.find('?');
    BOOST_LOG_TRIVIAL(info) << "query_pos: " << query_pos;
    BOOST_LOG_TRIVIAL(info) << "target: " << target;
    std::string action;

    if (query_pos == std::string::npos) {
        action = target.substr(action_pos+10);
    }
    else {
        action = target.substr(action_pos+10, query_pos-10);
    }

    if (method == "GET") {
        if(action == "list") {
            BOOST_LOG_TRIVIAL(debug) << "recognized it's list";
            return list(req, res);
        }
        else if(action == "view") {
            return view(req, res);
        }
        else if(action == "convert") {
            return convert(req, res);
        }
        else if(action == "create") {
            return create(req, res);
        }
        else {
          res.result(http::status::not_implemented);
        }
    }
    else if (method == "POST") {
        if(action == "edit") {
	        BOOST_LOG_TRIVIAL(debug) << "action is edit";
            return edit(req, res);
        } 
        else if (action == "save") {
	        BOOST_LOG_TRIVIAL(debug) << "action is save";
	        return save(req, res);
        }
        else if (action == "delete")
            return md_delete(req, res);
        else {
          res.result(http::status::not_implemented);
        }
        if (action == "save") {
            return save(req, res);
        }
    }
    else {
        res.result(http::status::not_implemented);
    }
    return res.result();
}

http::status MarkdownRequestHandler::create(http::request<http::string_body> req, http::response<http::string_body>& res) {
  std::string abs_path = map_request_path_to_file_path(std::string(req.target())); 
  std::string file_list;

  filesystem_->listFiles(abs_path, file_list);

  int max_id = 0;
  int postid = 0;

  std::vector<std::string> files_vect;
  std::stringstream files_ss(file_list);

  while(files_ss.good()) {
    std::string substr;
    getline(files_ss, substr, ',');
    files_vect.push_back(substr);
  }

  if (file_list.size() == 0) {
    postid = 0;
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

  postid = max_id + 1;
  abs_path += to_string(postid);
  if (!filesystem_->writeFile(abs_path, "")) {
    res.result(http::status::internal_server_error);
    return http::status::internal_server_error;
  }

  req.target("/markdown/edit");
  req.body() = "postid=" + to_string(postid);
  return edit(req, res);
}

http::status MarkdownRequestHandler::convert(http::request<http::string_body> req, 
        http::response<http::string_body>& res) {

    // Generate response
    res.result(http::status::ok);
    res.version(11);
    res.set(http::field::content_type, "text/html");
    res.body() = convert_(std::string(req.body()));
    res.prepare_payload();
    return res.result();
}

http::status MarkdownRequestHandler::view( 
        http::request<http::string_body> req, 
        http::response<http::string_body>& res) {
    std::string path_str = std::string(req.target());
    std::string post_id;

    //Generate res version
    res.version(11);

    // Find postid
    post_id = extract_file_name(path_str);
    if (post_id == "") { 
        res.result(http::status::not_found);
        res.prepare_payload();
        return res.result();
    }
    BOOST_LOG_TRIVIAL(debug) << "post_id: " << post_id;
    
    std::string file_path = construct_file_path(post_id);
    BOOST_LOG_TRIVIAL(debug) << "file_path: " << file_path;

    std::string file_content = ""; 
    // Read the markdown text and convert to HTML
    if (filesystem_->readFile(file_path, file_content) == true) {
        // Generate response
        res.result(http::status::ok);
        res.version(11);
        res.set(http::field::content_type, "text/html");

        size_t sffs_pos = html_assets::edit_button.find("sffs");
	      std::string button = (html_assets::edit_button);
	      std::string filled_button = button.replace(sffs_pos, 4, post_id);
	      std::string response = convert_(file_content);
 	      response += "<br>";
 	      response += filled_button;
 	      std::string list_button = html_assets::list_button;
 	      response += list_button;
	
        res.body() = response;
    }
    else {
        res.result(http::status::not_found); 
    }
    res.prepare_payload();
    return res.result();
}

http::status MarkdownRequestHandler::list(http::request<http::string_body> req, 
        http::response<http::string_body>& res) {
    // TODO: parse items in Markdown folder
    // returns a simple html response that lists all the files
    // NOTE: similar to CRUD::list()
  BOOST_LOG_TRIVIAL(debug) << "in the list function" ;
    std::string file_path = map_request_path_to_file_path(std::string(req.target())+"/");
    std::string file_list = "";
    std::string response = "";
    std::string postid = "";

    BOOST_LOG_TRIVIAL(debug) << "path obtained, setup done";
    res.version(11);

    if (filesystem_->listFiles(file_path, file_list) && file_list.size() > 0)
    {
      response += html_assets::list_table_start;

      for (int i = 0; i < file_list.size(); i++) {
	if (file_list[i] != ',') {
	  postid += file_list[i];
	} else {
	  response += html_assets::list_row_start;
	  //want response of the form <postid> <button>
	  response += postid;
	  response += " ";
	  response += html_assets::list_row_mid;
	  response += html_assets::list_line_form_a + postid + html_assets::list_line_form_b;
	  response += html_assets::list_row_end;
	  postid="";
	}
      }

      if (file_list[file_list.size()-1] != ',') {
	  response += html_assets::list_row_start;
	  //want response of the form <postid> <button>
	  response += postid;
	  response += " ";
	  response += html_assets::list_row_mid;
	  response += html_assets::list_line_form_a + postid + html_assets::list_line_form_b;
	  response += html_assets::list_row_end;
      }
		
      response += html_assets::list_table_end;
      
      res.body() = response;
    }
    else
    {
      BOOST_LOG_TRIVIAL(info) << "didn't find the folder";
        res.body() = "Nothing to see here"; 
    }

    res.set(http::field::content_type, "text/html");
    res.result(http::status::ok); 
    res.prepare_payload();

    return res.result();
}

http::status MarkdownRequestHandler::save(http::request<http::string_body> req, 
        http::response<http::string_body>& res) {
    // TODO: read body of request, which is Markdown
    BOOST_LOG_TRIVIAL(debug) << "reached save";
  
    res.version(11);
    std::string body = std::string(req.body());
    
    // extract file name
    std::string file_name = extract_file_name(body);
    if (file_name == "") {
        res.result(http::status::not_found);
        res.prepare_payload();
        return res.result();
    }
    BOOST_LOG_TRIVIAL(info) << "save file name: " << file_name;
    std::string file_path = construct_file_path(file_name);
    BOOST_LOG_TRIVIAL(info) << "save file path: " << file_path;

    // extract file name
    std::string file_content = extract_text(body);
    BOOST_LOG_TRIVIAL(debug) << "save text: " << file_content;
  
    // now, find the file to write to or the next index to create
    // std::string abs_path = map_request_path_to_file_path(std::string(req.target())+"/"); 
    // std::string file_list;

    // filesystem_->listFiles(abs_path, file_list);

    // int max_id = 0;
    // int curr_id = 0;

    // std::vector<std::string> files_vect;
    // std::stringstream files_ss(file_list);

    // while(files_ss.good()) {
    //   std::string substr;
    //   getline(files_ss, substr, ',');
    //   files_vect.push_back(substr);
    // }

    // if (file_list.size() == 0) {
    //   curr_id = 0;
    // }
    // else {
    //   for(int i = 0; i < files_vect.size(); i++) {
    // 	std::string entry = files_vect[i];
    // 	BOOST_LOG_TRIVIAL(info) << "file entry found: " << entry;
    // 	int entry_as_int;
    // 	try {
    // 	  entry_as_int = stoi(entry);
    // 	}
    // 	catch (...) {
    // 	  res.result(http::status::internal_server_error);
    // 	  res.prepare_payload();
    // 	  return res.result();
    // 	}
    // 	if (entry_as_int > max_id)
    // 	  max_id = entry_as_int;
    //   }
    // }

    // curr_id = max_id + 1;

    // abs_path += "/" + to_string(curr_id);

    if (!filesystem_->writeFile(file_path, file_content)) {
        res.result(http::status::internal_server_error);
        res.prepare_payload();
        return res.result();
    }

    req.target("/markdown/view?postid=" + file_name);
    return view(req, res);
}


http::status MarkdownRequestHandler::md_delete(http::request<http::string_body> req, 
        http::response<http::string_body>& res) {
    // maintain the tab

    BOOST_LOG_TRIVIAL(debug) << "entered edit";
    
    std::string body = std::string(req.body());
    BOOST_LOG_TRIVIAL(info) << "body received: " << body;
    std::string file_name = extract_file_name(body);
    BOOST_LOG_TRIVIAL(info) << "MARKDOWN file name: " << file_name;
    std::string file_path = construct_file_path(file_name);
    BOOST_LOG_TRIVIAL(info) << "MARKDOWN file path: " << file_path;

    if(filesystem_->deleteFile(file_path)) {
        res.result(http::status::ok);
        res.set(http::field::content_type, "application/html");
        res.body() = "";
    }
    else
        res.result(http::status::not_found);
    
    res.prepare_payload();
    req.target("markdown/list");
    return list(req, res);
}

http::status MarkdownRequestHandler::edit(http::request<http::string_body> req,
        http::response<http::string_body>& res) {
    // maintain the tab

    BOOST_LOG_TRIVIAL(debug) << "entered edit";
    
    std::string body = std::string(req.body());
    std::string file_name = extract_file_name(body);
    BOOST_LOG_TRIVIAL(info) << "MARKDOWN file name: " << file_name;
    std::string file_path = construct_file_path(file_name);
    BOOST_LOG_TRIVIAL(info) << "MARKDOWN file path: " << file_path;
    
    std::string file_content = "";
    res.version(11);
    if (filesystem_->readFile(file_path, file_content))
    {
        res.result(http::status::ok);
        res.set(http::field::content_type, "text/html");

        // replace placeholder text "sffs" with contents of file
        std::string body = html_assets::edit_form;
        size_t sffs_pos = body.find("sffs");
        body = body.replace(sffs_pos, 4, file_content);
        // replace post id with the file name
        size_t pid_pos = body.find("editsPID");
        body = body.replace(pid_pos, 8, file_name);
        pid_pos = body.find("editsPID");
        body = body.replace(pid_pos, 8, file_name);
        res.body() = body;
    }
    else
    {
        res.result(http::status::not_found);
    }
    res.prepare_payload();
    return res.result();
}

std::string MarkdownRequestHandler::extract_file_name(std::string str) {
  size_t postid_pos = str.find("postid="); // 7 characters long
  if (postid_pos == std::string::npos) {
    return "";
  } else {
    str = str.substr(postid_pos+7);
    // trim off line feeds and carriage returns
    size_t lf_pos = str.find("\n");
    if (lf_pos != std::string::npos) {
      str = str.substr(0, lf_pos);
    }
    size_t cr_pos = str.find("\r");
    if (cr_pos != std::string::npos) {
      str = str.substr(0, cr_pos);
    }

    return str;
  }
}

std::string MarkdownRequestHandler::extract_text(std::string str) {
  size_t text_pos = str.find("text="); // 5 characters long
  if (text_pos == std::string::npos) {
    return "";
  } else {
    str = str.substr(text_pos+5);
    
    // trim off extra crlf
    while (str.back() == '\n' || str.back() == '\r') {
        str.pop_back();
    }
    return str;
  }
}

std::string MarkdownRequestHandler::construct_file_path(std::string filename) {
  std::string temp; 

  if (data_path_[(data_path_.size())-1] == '/')
    temp= data_path_ + filename;
  else
    temp= data_path_ + "/" + filename;
  return temp;   
}

std::string MarkdownRequestHandler::map_request_path_to_file_path(std::string request_uri)
{
  std::string filename = extract_file_name(request_uri);

  return construct_file_path(filename);
} 

std::string MarkdownRequestHandler::convert_(std::string request_markdown) {
    // Generate HTML conversion text
    std::ostringstream os;
    markdown::Document doc;
    doc.read(request_markdown);
    doc.write(os);

    return os.str();
}
